#include "passive_mode.h"

#include <memory>

#include "control_card.h"

PassiveMode::PassiveMode() : p_control_card_(std::make_shared<ControlCard>()){
	p_control_card_->Initial();
}

void PassiveMode::StartMove() {
	
}