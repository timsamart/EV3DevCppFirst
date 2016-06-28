/*============================================================================
 * Name        : Control.cpp
 * Author      : Timotheos Samartzidis
 * Version     : 1
 *
 * Copyright (c) 2016 - Timotheos Samartzidis
 *
 * Program for Motor Control loosely based on Franz Detro's Motor Control program
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "Control.h"

namespace ev3dev {

Control::Control() :
	// TODO Auto-generated constructor stub
  _motor_A(OUTPUT_A),
  _motor_B(OUTPUT_B),
  _motor_C(OUTPUT_C),
  _motor_D(OUTPUT_D),
  _sensor_A(INPUT_1),
  _sensor_B(INPUT_2),
  _sensor_C(INPUT_3),
  _sensor_D(INPUT_4),
  _state(state_idle),
  _terminate(false)
{
}

Control::~Control() {
	// TODO Auto-generated destructor stub
	reset();
}

void Control::msg(char const * const message)
{
  m_screen.lock();
  std::cout << message << std::endl;
  m_screen.unlock();
}

void Control::msg(char const * const message, int const &value)
{
  m_screen.lock();
  std::cout << message << " = " << value << std::endl;
  m_screen.unlock();
}

void Control::driveprimitive()
{
	_motor_A.reset();
	_motor_A.set_speed_sp(1050);
	_motor_A.set_time_sp(2000);
	_motor_A.run_timed();
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

bool Control::initialized(int ID) const
{
  switch(ID)
  {
  case 0:{
	  return (_motor_A.connected()); break;}
  case 1:{
  	  return (_motor_B.connected()); break;}
  case 2:{
  	  return (_motor_C.connected()); break;}
  case 3:{
  	  return (_motor_D.connected()); break;}
  case 4:{
  	  return (_sensor_A.connected()); break;}
  case 5:{
  	  return (_sensor_B.connected()); break;}
  case 6:{
  	  return (_sensor_C.connected()); break;}
  case 7:{
  	  return (_sensor_D.connected()); break;}
  default:
	  return false; break;
  }
}

void Control::sensorRead()
{
	std::cout << "first: " << _sensor_A.value(0) << std::endl;
	std::thread t;
	t = std::thread([&] {
		while (true) {
			msg("Sonic Sensor Distance: ", _sensor_A.value(0)/10);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
	}); // Lambda function for threading
	t.join();
}

} /* namespace ev3dev */
