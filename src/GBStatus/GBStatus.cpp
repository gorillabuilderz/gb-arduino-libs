#include <Arduino.h>
#include "GBStatus.h"

// Global instance
GBStatus Status;

const State GBStatus::COMMAND = {50, 6};
const State GBStatus::SUCCESS = {100,6};
const State GBStatus::ERROR = {200,10};
const State GBStatus::FAILURE = {300,32767};	// Max int

GBStatus::GBStatus() {
    off();
    pinMode(13, OUTPUT);
}

void GBStatus::on() {
	digitalWrite(13, HIGH);
}

void GBStatus::off() {
	digitalWrite(13, LOW);
}

void GBStatus::idle() {
	if(_state != NULL) {
		if(_changeStateMillis < millis() && _blinkCount++ < _state->blinkCount) {
			if(_on) {
				off();
				_on = false;
			}
			else {
				on();
				_on = true;
			}
			_changeStateMillis = millis() + _state->delay;
		}
		else if(_blinkCount > _state->blinkCount) {
			off();
			_state = NULL;
			_blinkCount = 0;
		}
	}
}

void GBStatus::error() {
	setState(&ERROR);
}
void GBStatus::failure() {
	setState(&FAILURE);
}
void GBStatus::success() {
	setState(&SUCCESS);
}    

void GBStatus::command() {
	setState(&COMMAND);
}

void GBStatus::setState(const State *state) {
	_state = state;
	_blinkCount = 0;
	_changeStateMillis = _state->delay + millis();
}

bool GBStatus::isState() {
	return _state != NULL;
}
