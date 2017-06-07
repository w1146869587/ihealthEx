#pragma once
#include "APS168.h"
#include "ErrorCodeDef.h"
#include "APS_define.h"
#include "type_def.h"

#define MOTOR_ON (1)
#define MOTOR_OFF (0)

#define CLUTCH_ON (1)
#define CLUTCH_OFF (0)

#define YES (1)
#define NO (0) 

#define __MAX_DO_CH (24)
#define __MAX_DI_CH (24)

#define SHOULDER_AXIS_ID (0)
#define ELBOW_AXIS_ID (1)

#define POSITION_RESET_TIMER_ID (1)

#define VEL_TO_PALSE (0.009) //为了将速度转化为脉冲

class ControlCard {
public:
	ControlCard();
	~ControlCard() = default;

	I32 Initial();
	void SetAxisParamZero();
	void SetClutch(bool on_or_off = CLUTCH_ON);
	void SetMotor(bool on_or_off = MOTOR_ON);
	void GetLimitSwitchStatus();
	void MotorVelocityMove(I32 axis_id, double velocity);
	void MotorAbsoluteMove(I32 axis_id, double position);
	void PositionReset();

private:
	bool axis_status_;
	bool clutch_status_;
	long card_name_;
	long board_id_;
	int total_axis_;
	bool is_card_initialed_;
	bool shoulder_limit_switch_status_[2]{ 0 };
	bool elbow_limit_switch_status_[2]{ 0 };

private:
	I32 FindSuitableControlCard(I32 borad_id_in_bits);
	void ShoulderMotorVelocityMove(double velocity);
	void ElbowMotorVelocityMove(double velocity);
	void MoveInVelocityMode(I32 axis_id, double velocity);
	
};