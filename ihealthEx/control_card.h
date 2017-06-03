#pragma once
#include "APS168.h"
#include "ErrorCodeDef.h"
#include "APS_define.h"
#include "type_def.h"

#define ON (1)
#define OFF (0)

#define YES (1)
#define NO (0) 

#define __MAX_DO_CH (24)
#define __MAX_DI_CH (24)

#define SHOULDER_AXIS_ID (0)
#define ELBOW_AXIS_ID (1)

class ControlCard {
public:
	ControlCard();
	~ControlCard();

	I32 Initial();
	void SetAxisParamZero();
	void SetClutch(bool on_or_off = TRUE);
	void SetMotor(bool on_or_off = TRUE);
	void GetLimitSwitchStatus();


private:
	bool axis_status_;
	bool clutch_status_;
	long card_name_;
	long board_id_;
	int total_axis_;
	bool is_card_initialed_;
	bool limit_switch_status[2][2];
	I32 FindSuitableControlCard(I32 borad_id_in_bits);
};