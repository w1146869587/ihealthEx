#include "control_card.h"

ControlCard::ControlCard() :
	card_name_(0), board_id_(0), total_axis_(0), 
	is_card_initialed_(0), axis_status_(OFF), clutch_status_(OFF) {

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
	I32 di_ch[__MAX_DI_CH] { 0 };
	I32 ret = 0;
	ret = APS_read_d_input(board_id_, 0, &di_data);
	for (int i = 0; i < __MAX_DI_CH; i++) {
		di_ch[i] = (di_data >> i) & 0x1;
	}
	/*注意，这里的光电开关信号和具体的连接有关，需要使用运动控制卡的软件经过测试之后才能得出正确的结论*/
	limit_switch_status[0][0] = di_ch[16]; //0号电机零位信号
	limit_switch_status[0][1] = di_ch[17]; //0号电机极限位置信号
	limit_switch_status[1][0] = di_ch[18]; //1号电机零位信号
	limit_switch_status[1][1] = di_ch[19]; //1号电机极限位置信号
}