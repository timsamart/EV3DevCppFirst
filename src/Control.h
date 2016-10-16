/*============================================================================
 * Name        : Control.h
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

#include <thread>
#include <mutex>

#include "ev3dev.h"
#include "events.h"
#include "ev3devextras.h"

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

	large_motor* motors[4]; // motor array populated in init function Control()
	sensor* sensors[4]; // sensor array populated in init function Control()

	events e; // events interface class for all events and input outputs of sensors and motors

	std::thread t_buttons; // thread for getting button inputs
	std::thread t_sensors; // thread for reading value of sensor
	std::thread t_motors; // thread for reading motors values

	void initialize(bool verbose = false, int ms = 1000);

	void drive(int ID, int speed = 1050, int timeinms=0, int rampupdown=0); // drive function multipurpose
	void drive_ToPositionRelative(int ID, int position);
	void drive_ToPositionAbsolute(int ID, int position);
	bool stop(int ID); // stop the motor
	void stopAll(); // stops all motors
	bool reset(int ID); // reset the motor
	void resetAll(); // resets all motors

	//Threads
	int sensorGetValue(int ID);

	void checkDevices(bool verbose=false); // asses current connected devices

	void sleep(int ms = 1000); // sleep main thread for so many milliseconds
	void terminate() { _terminate = true; }; // Terminates the program
	void exit(); // exit threads and cleanup

private:
	void th_initMotorStatus(int ms = 500);
	void th_initButtonInput(int ms = 500); // thread to get button inputs
	void th_initSensorInput(int ms = 500); // thread to get sensor inputs
	void th_motorReadStatus(int ID); // reads motor Status to events
	void th_sensorRead(int ID); // couts sensor readings from another sensor


protected:
  large_motor	_motor_A;
  large_motor	_motor_B;
  large_motor	_motor_C;
  large_motor	_motor_D;

  sensor		_sensor_A;
  sensor		_sensor_B;
  sensor		_sensor_C;
  sensor		_sensor_D;

  bool  _terminate;
};

} /* namespace ev3dev */

#endif /* CONTROL_H_ */
