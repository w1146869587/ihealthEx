#include "passive_mode.h"

#include<cmath>
#include <memory>

#include "control_card.h"

PassiveMode::PassiveMode() : p_control_card_(std::make_shared<ControlCard>()){
	p_control_card_->Initial();
}

void PassiveMode::StartMove(const PassiveActionParam &param) {
	int step_count = 0;
	PassivePosition position;
	p_control_card_->SetMotor(MOTOR_ON);
	p_control_card_->SetClutch(CLUTCH_ON);
	while (step_count <= param.total_step) {
		position = CalculateAxisPosition(param, step_count);
		PositionMove(position);
		WaitTillMoveFinish();
		++step_count;
	}
}

PassivePosition PassiveMode::CalculateAxisPosition(const PassiveActionParam &param, int step) {
	PassivePosition position;
	double a[2] = { 0 };
	double b[2] = { 0 };
	a[0] = (1 + 2 * step / param.total_step) * pow((step - param.total_step) / (-param.total_step), 2);
	a[1] = (1 + 2 * (step - param.total_step) / (-param.total_step)) * pow((step) / (param.total_step), 2);
	b[0] = step * pow((param.total_step - step) / (param.total_step), 2);
	b[1] = (step - param.total_step) * pow(step / param.total_step, 2);

	position.shoulder_position = a[0] * param.shoulder_start_position + a[1] * param.shoulder_end_position 
		+ b[0] * param.shoulder_start_velocity + b[1] * param.shoulder_end_velocity;
	position.elbow_position = a[0] * param.elbow_start_position + a[1] * param.elbow_end_position
		+ b[0] * param.elbow_start_velocity + b[1] * param.elbow_end_velocity;

	return position;
}

void PassiveMode::PositionMove(const PassivePosition &position) {
	p_control_card_->MotorAbsoluteMove(SHOULDER_AXIS_ID, position.shoulder_position, 4);
	p_control_card_->MotorAbsoluteMove(ELBOW_AXIS_ID, position.elbow_position, 4);
}

void PassiveMode::WaitTillMoveFinish() {
	p_control_card_->WaitTillMoveFinish();
}

void PassiveMode::PositionReset() {
	p_control_card_->PositionReset();
}

void PassiveMode::StopMove() {
	p_control_card_->StopMove();
}