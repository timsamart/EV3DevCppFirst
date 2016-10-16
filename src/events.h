/*
 * events.h
 *
 *  Created on: 14.10.2016
 *      Author: Chrysanth
 */

#ifndef EVENTS_H_
#define EVENTS_H_

#include "ev3devextras.h"

namespace ev3dev {

enum Buttons {UP, DOWN, LEFT, RIGHT, ENTER, ESCAPE};

class events {
public:
	std::mutex m_buttons; // blocks rw-access to up,down,left,right,enter,escape:
	std::mutex m_term;
	std::mutex m_motorStatus;
	bool b_up = false;
	bool b_up_p = false; // previous value
	bool b_up_c = false; // value changed yes no
	bool b_down = false;
	bool b_down_p = false;
	bool b_down_c = false;
	bool b_left = false;
	bool b_left_p = false;
	bool b_left_c = false;
	bool b_right = false;
	bool b_right_p = false;
	bool b_right_c = false;
	bool b_escape = false;
	bool b_escape_p = false;
	bool b_escape_c = false;
	bool b_enter = false;
	bool b_enter_p = false;
	bool b_enter_c = false;
	bool b_back = false;
	bool b_back_p = false;
	bool b_back_c = false;
	bool terminate = false;
	bool sensorsInitialized[4];
	int sensorsValues[4];
	std::string sensorType[4];
	bool motorsInitialized[4];
	std::string motorStatus[4];
	int motorPosition[4];
	int motorSpeed[4];

	void reset(){
		for(int i = 0; i<4; i++){
			sensorsInitialized[i] = false;
			motorsInitialized[i] = false;
			sensorType[i] = "";
			motorStatus[i] = "";
			motorPosition[i] = 0;
			motorSpeed[i] = 0;
		}
	}

	bool buttonPressed(int b_name){ //up, down, left, right, enter, escape
		switch(b_name){
			case UP:{m_buttons.lock();if(b_up_c){intercom::msg("UP"); b_up_c = false; m_buttons.unlock(); return b_up;} m_buttons.unlock();	break;}
			case DOWN:{m_buttons.lock();if(b_down_c){intercom::msg("DOWN"); b_down_c = false; m_buttons.unlock(); return b_down;} m_buttons.unlock();	break;}
			case LEFT:{m_buttons.lock();if(b_left_c){intercom::msg("LEFT"); b_left_c = false; m_buttons.unlock(); return b_left;} m_buttons.unlock();	break;}
			case RIGHT:{m_buttons.lock();if(b_right_c){intercom::msg("RIGHT");b_right_c = false; m_buttons.unlock(); return b_right;} m_buttons.unlock();	break;}
			case ENTER:{m_buttons.lock();if(b_enter_c){intercom::msg("ENTER"); b_enter_c = false; m_buttons.unlock(); return b_enter;} m_buttons.unlock(); break;}
			case ESCAPE:{m_buttons.lock();if(b_escape_c){intercom::msg("ESCAPE");b_escape_c = false; m_buttons.unlock(); return b_escape;} m_buttons.unlock(); break;}
			default:{
				return false;
			}
		}
		return false;
	}

	bool getTerm(){
		bool term = false;
		m_term.lock();
		term = terminate;
		m_term.unlock();
		return term;
	}

	void setMotorStatus(int ID, std::string status){
		m_motorStatus.lock();
		motorStatus[ID] = status;
		m_motorStatus.unlock();
	}

	std::string getMotorStatus(int ID)
	{
		std::string _status;
		m_motorStatus.lock();
		_status = motorStatus[ID];
		m_motorStatus.unlock();
		return _status;
	}

	bool verbose = false;
};
}


#endif /* EVENTS_H_ */
