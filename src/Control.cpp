/*
 * Control.cpp
 *
 *  Created on: 16.04.2016
 *      Author: Chrysanth
 */
#include "Control.h"

namespace ev3dev {

Control::Control() :
	// TODO Auto-generated constructor stub
  _motor_A(OUTPUT_A),
  _motor_B(OUTPUT_B),
  _motor_C(OUTPUT_C),
  _motor_D(OUTPUT_D),
  _state(state_idle),
  _terminate(false)
{
}

Control::~Control() {
	// TODO Auto-generated destructor stub
	reset();
}

void Control::driveprimitive()
{
	_motor_A.reset();
	_motor_A.run_direct();
	while (_motor_A.state().count("running")) {
	    	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	    }
}

void Control::drive(int speed, int time)
{
  _motor_A.set_duty_cycle_sp(-speed);

  _state = state_driving;

  if (time > 0)
  {
    _motor_A.set_time_sp(time).run_timed();

    while (_motor_A.state().count("running")) {
    	std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    _state = state_idle;
  }
  else
  {
    _motor_A.run_forever();
    std::cout << "Motor Runs now";
  }
}

void Control::stop()
{
  _motor_A.stop();

  _state = state_idle;
}

void Control::reset()
{
  if (_motor_A.connected())
    _motor_A.reset();

  _state = state_idle;
}

bool Control::initialized() const
{
  return (_motor_A.connected());
}

} /* namespace ev3dev */
