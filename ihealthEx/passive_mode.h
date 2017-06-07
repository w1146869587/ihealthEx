#pragma once
#include <memory>
#include "control_card.h"

class PassiveMode {
public:
	PassiveMode();
	~PassiveMode() = default;
	void StartMove();
private:
	std::shared_ptr<ControlCard> p_control_card_;
};