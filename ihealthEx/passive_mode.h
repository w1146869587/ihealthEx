#pragma once
#include <memory>
#include "control_card.h"

/*被动动作参数，记录被动运动的起点和终点的位置、速度和插值点数*/
struct PassiveActionParam {
	PassiveActionParam(double shoulder_start_pos, double shoulder_end_pos, double elbow_start_pos, double elbow_end_pos, double step) :
		shoulder_start_position(shoulder_start_pos), shoulder_end_position(shoulder_end_pos), elbow_start_position(elbow_start_pos),
		elbow_end_position(elbow_end_pos), total_step(step) {
	
	}

	double shoulder_start_position;
	double shoulder_end_position;
	double shoulder_start_velocity = 0;
	double shoulder_end_velocity = 0;

	double elbow_start_position;
	double elbow_end_position;
	double elbow_start_velocity = 0;
	double elbow_end_velocity = 0;

	double total_step;
};

/**/
struct PassivePosition {
	double shoulder_position = 0;
	double elbow_position = 0;
};

class PassiveMode {
public:
	PassiveMode();
	~PassiveMode() = default;
	void PositionReset();
	void StartMove(const PassiveActionParam &param);
	void StopMove();
private:
	std::shared_ptr<ControlCard> p_control_card_;
	void PositionMove(const  PassivePosition &position);
	PassivePosition CalculateAxisPosition(const PassiveActionParam &param, double step);
	void WaitTillMoveFinish();
};