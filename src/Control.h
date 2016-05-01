/*
 * Control.h
 *
 *  Created on: 16.04.2016
 *      Author: Chrysanth
 */

/*
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

#include "ev3dev.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

#ifndef NO_LINUX_HEADERS
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#define KEY_RELEASE 0
#define KEY_PRESS   1
#define KEY_REPEAT  2
#endif

#ifndef CONTROL_H_
#define CONTROL_H_

namespace ev3dev {

class Control {
public:
	Control();
	virtual ~Control();

	void driveprimitive();
	void drive(int speed, int time);
	void stop();
	void reset();

	bool initialized() const;
	void terminate() { _terminate = true; };

protected:
  large_motor     _motor_A;
  large_motor     _motor_B;
  large_motor     _motor_C;
  large_motor     _motor_D;

  enum state
  {
    state_idle,
    state_driving,
    state_turning
  };

  state _state;
  bool  _terminate;
};

} /* namespace ev3dev */

#endif /* CONTROL_H_ */
