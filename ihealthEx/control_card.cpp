#include "control_card.h"
#include <cmath>

ControlCard::ControlCard() :
	card_name_(0), board_id_(0), total_axis_(0),
	is_card_initialed_(0), axis_status_(MOTOR_OFF), clutch_status_(CLUTCH_OFF) {

}

ControlCard::~ControlCard() {

}

I32 ControlCard::Initial() {
	I32 board_id_in_bits = 0;
	I32 mode = 0;
	I32 ret = 0;
	ret = APS_initial(&board_id_in_bits, mode);

	if (ret != ERR_NoError) {
		is_card_initialed_ = NO;
		return ret;
	}
	FindSuitableControlCard(board_id_in_bits);
	SetAxisParamZero();
	APS_load_parameter_from_flash(board_id_);
	return ret;
}

I32 ControlCard::FindSuitableControlCard(I32 board_id_in_bits) {
	I32 ret = 0;
	I32 temp = 0;
	I32 card_name = 0;
	I32 start_axis_id = 0;
	I32 total_axis_num = 0;
	for (int i = 0; i < 32; i++) {
		temp = (board_id_in_bits >> i) & 0x1;
		if (temp == 1) {
			ret = APS_get_card_name(i, &card_name);
			if (card_name == DEVICE_NAME_PCI_8258 || card_name == DEVICE_NAME_AMP_82548) {
				ret = APS_get_first_axisId(i, &start_axis_id, &total_axis_num);
				card_name_ = card_name;
				board_id_ = i;
				total_axis_ = total_axis_num;
				is_card_initialed_ = YES;
				break;
			}
		}
	}
	return ret;
}

void ControlCard::SetAxisParamZero() {
	for (I32 axis_id = 0; axis_id < total_axis_; axis_id++) {
		APS_set_command_f(axis_id, 0.0);
		APS_set_position_f(axis_id, 0.0);
	}
}

void ControlCard::SetClutch(bool on_or_off) {
	bool do_ch_status[__MAX_DO_CH] { 0 };
	I32 digit_output_value = 0;
	I32 ret = 0;
	do_ch_status[8] = on_or_off;
	do_ch_status[9] = on_or_off;
	for (int i = 0; i < __MAX_DO_CH; i++) {
		digit_output_value |= (do_ch_status[i] << i);
	}
	ret = APS_write_d_output(board_id_, 0, digit_output_value);
	if (ret == ERR_NoError) {
		clutch_status_ = on_or_off;
	}
}

void ControlCard::SetMotor(bool on_or_off) {
	for (int i = SHOULDER_AXIS_ID; i <= ELBOW_AXIS_ID; i++) {
		APS_set_axis_param_f(i, PRA_CURVE, 0.5);//Set acceleration rate
		APS_set_axis_param_f(i, PRA_ACC, 300000.0); //Set acceleration rate
		APS_set_axis_param_f(i, PRA_DEC, 300000.0); //Set deceleration rate
		APS_set_axis_param_f(i, PRA_STP_DEC, 10000.0);
		APS_set_servo_on(i, on_or_off);
	}
	axis_status_ = on_or_off;
}

void ControlCard::GetLimitSwitchStatus() {
	I32 di_data = 0;
	bool di_ch[__MAX_DI_CH] { 0 };
	I32 ret = 0;
	ret = APS_read_d_input(board_id_, 0, &di_data);
	for (int i = 0; i < __MAX_DI_CH; i++) {
		di_ch[i] = (di_data >> i) & 0x1;
	}
	/*注意，这里的光电开关信号和具体的连接有关，需要使用运动控制卡的软件经过测试之后才能得出正确的结论*/
	elbow_limit_switch_status_[0] = di_ch[16]; //肘部电机零位信号
	elbow_limit_switch_status_[1] = di_ch[17]; //肘部电机极限位置信号
	shoulder_limit_switch_status_[0] = di_ch[18]; //肩部电机零位信号
	shoulder_limit_switch_status_[1] = di_ch[19]; //肩部电机极限位置信号
}

//电机运动，在运动之前首先取得光电开关的值，用于限位
void ControlCard::MotorMove(I32 axis_id, double velocity) {
	GetLimitSwitchStatus();
	switch (axis_id) {
		case SHOULDER_AXIS_ID:
			ShoulderMotorMove(velocity);
			break;
		case ELBOW_AXIS_ID:
			ElbowMotorMove(velocity);
			break;
		default:
			break;
	}
}

//肩部电机运动，肩部光电开关进行限位
void ControlCard::ShoulderMotorMove(double velocity) {
	if (!shoulder_limit_switch_status_[0] && !shoulder_limit_switch_status_[1]) {
		MoveInVelocityMode(SHOULDER_AXIS_ID, velocity);
	} else if (shoulder_limit_switch_status_[0]) {
		if (velocity > 0) {
			MoveInVelocityMode(SHOULDER_AXIS_ID, velocity);
		} else {
			APS_stop_move(SHOULDER_AXIS_ID);
		}
	} else if (shoulder_limit_switch_status_[1]) {
		if (velocity < 0) {
			MoveInVelocityMode(SHOULDER_AXIS_ID, velocity);
		} else {
			APS_stop_move(SHOULDER_AXIS_ID);
		}
	}
}

//肘部电机运动，肘部光电开关进行限位
void ControlCard::ElbowMotorMove(double velocity) {
	if (!elbow_limit_switch_status_[0] && !elbow_limit_switch_status_[1]) {
		MoveInVelocityMode(ELBOW_AXIS_ID, velocity);
	} else if (elbow_limit_switch_status_[0]) {
		if (velocity > 0) {
			MoveInVelocityMode(ELBOW_AXIS_ID, velocity);
		} else {
			APS_stop_move(ELBOW_AXIS_ID);
		}
	} else if (shoulder_limit_switch_status_[1]) {
		if (velocity < 0) {
			MoveInVelocityMode(ELBOW_AXIS_ID, velocity);
		} else {
			APS_stop_move(ELBOW_AXIS_ID);
		}
	}
}


void ControlCard::MoveInVelocityMode(I32 axis_id, double velocity) {
	double max_velocity = (fabs(velocity) / VEL_TO_PALSE);

	if (!axis_status_) {
		SetMotor(MOTOR_ON);
	}
	if (!clutch_status_) {
		SetClutch(CLUTCH_ON);
	}

	if (velocity > 0) {
		APS_vel(axis_id, 0, max_velocity, 0);
	} else {
		APS_vel(axis_id, 1, max_velocity, 0);
	}
}

void ControlCard::PositionReset() {
	SetClutch(CLUTCH_ON);
	SetMotor(MOTOR_ON);
	while (!shoulder_limit_switch_status_[0] || !elbow_limit_switch_status_[0]) {
		ShoulderMotorMove(-2);
		ElbowMotorMove(-2);
		Sleep(100);
	}
	SetClutch(CLUTCH_OFF);
	SetMotor(MOTOR_OFF);
	SetAxisParamZero();
}


