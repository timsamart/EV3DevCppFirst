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
  _terminate(false)
{
	motors[0] = &_motor_A;
	motors[1] = &_motor_B;
	motors[2] = &_motor_C;
	motors[3] = &_motor_D;
	sensors[0] = &_sensor_A;
	sensors[1] = &_sensor_B;
	sensors[2] = &_sensor_C;
	sensors[3] = &_sensor_D;
	e.reset();
}

Control::~Control() {
	// TODO Auto-generated destructor stub
	resetAll();
}

void Control::exit()
{
	intercom::msg("starting exit");
	e.terminate = true;
	intercom::msg("terminate launched");
	t_buttons.join();
	intercom::msg("button thread terminated!");
	t_sensors.join();
	intercom::msg("sensor thread terminated!");
	t_motors.join();
	intercom::msg("motor thread terminated!");
	t_network.join();
	intercom::msg("Network thread terminated!");
	stopAll();
	intercom::msg("motors stopped!");
}

void Control::sleep(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void Control::initialize(bool _verbose, int ms)
{
	verbose = _verbose;
	checkDevices(true);
	th_initButtonInput();
	th_initSensorInput(ms);
	th_initMotorStatus(ms);
	th_initNetwork();
}

void Control::drive(int ID, int speed, int timeinms, int rampupdownms)
{
	motors[ID]->reset();
	motors[ID]->set_speed_sp(speed);
	if(timeinms>0){
		motors[ID]->set_time_sp(timeinms);
		motors[ID]->run_timed();
	}else{
		if(rampupdownms>0){
			motors[ID]->set_ramp_up_sp(rampupdownms);
			motors[ID]->run_forever();
		}else if(rampupdownms<0){
			motors[ID]->set_ramp_down_sp(-rampupdownms);
			motors[ID]->run_forever();
		}else{
			motors[ID]->run_forever();
		}
	}
}

void Control::drive_ToPositionRelative(int ID, int position)
{
	intercom::msg("position in tachocounts ", motors[ID]->position());
	intercom::msg("tachocounts ", motors[ID]->count_per_rot());
	motors[ID]->set_position_sp(position);
	motors[ID]->run_to_rel_pos();
}

void Control::drive_ToPositionAbsolute(int ID, int position)
{
	intercom::msg("position in tachocounts ", motors[ID]->position());
	intercom::msg("tachocounts ", motors[ID]->count_per_rot());
	motors[ID]->set_position_sp(position);
	motors[ID]->run_to_abs_pos();
}

bool Control::stop(int ID)
{
	if (motors[ID]->connected())
	{
		motors[ID]->reset();
		th_motorReadStatus(ID);
		intercom::msg("Motor stopped ID ", ID);
		return true;
	}
	return false;
}

void Control::stopAll(){
	for(int i = 0; i < 4; i++)
	{
		stop(i);
	}
}

bool Control::reset(int ID)
{
	if (motors[ID]->connected())
	{
		motors[ID]->reset();
		th_motorReadStatus(ID);
		return true;
	}
	return false;
}

void Control::resetAll(){
	for(int i = 0; i < 4; i++)
	{
		reset(i);
	}
}

void Control::checkDevices(bool _verbose)
{
	verbose = _verbose;
	for(int i = 0; i < 4; i++)
	{
		if(motors[i]->connected())
		{
			if(verbose){
				m_screen.lock();
				std::cout << "Motor " << i << " ready!" << std::endl;
				m_screen.unlock();
			}
			e.motorsInitialized[i] = true;
		}
		if(sensors[i]->connected())
		{
			if(verbose){
				m_screen.lock();
				std::cout << "Sensor " << i << " ready!" << std::endl;
				m_screen.unlock();
			}
			e.sensorsInitialized[i] = true;
			e.sensorType[i] = sensors[i]->driver_name();
			intercom::msg("Sensor name: ", e.sensorType[i]);
			intercom::msg("Sensor ID: ", i);
		}
	}
}

int Control::sensorGetValue(int ID) // gives ID 0 to 3
{
	m_sensors[ID].lock();
	return(sensors[ID]->value(0));
	m_sensors[ID].unlock();
}
// private functions
void Control::th_motorReadStatus(int ID){
	m_motors[ID].lock();
	e.motorSpeed[ID] = motors[ID]->speed_sp();
	if(motors[ID]->state().count("running"))
		e.setMotorStatus(ID,"running");
	else if(motors[ID]->state().count("ramping"))
		e.setMotorStatus(ID,"ramping");
	else if(motors[ID]->state().count("holding"))
		e.setMotorStatus(ID,"holding");
	else if(motors[ID]->state().count("stalled"))
		e.setMotorStatus(ID,"stalled");
	m_motors[ID].unlock();
}

void Control::th_sensorRead(int ID)
{
	m_sensors[ID].lock();
	e.sensorsValues[ID] = sensors[ID]->value();
	m_sensors[ID].unlock();
}

void Control::th_initMotorStatus(int ms)
{
	t_motors = std::thread([&] {
		bool term = false;
		while(!term){
			term = e.getTerm(); // mutex lock to check if terminate initiated
			for(int i = 0; i<4; i++){
				if(e.motorsInitialized[i]){
					th_motorReadStatus(i);
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	});
}

void Control::th_initSensorInput(int ms)
{
	t_sensors = std::thread([&] {
		bool term = false;
		while(!term){
			term = e.getTerm(); // mutex lock to check if terminate initiated
			for(int i = 0; i<4; i++){
				if(e.sensorsInitialized[i]){
					th_sensorRead(i);
					//intercom::msg("sensor read", i);
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	});
}

void Control::th_initButtonInput(int ms)
{
	std::cout << "Start button input thread... " << std::endl;
	t_buttons = std::thread([&] {
		bool term = false;
		while (!term)
		{
			term = e.getTerm(); // mutex lock to check if terminate initiated
			e.m_buttons.lock(); //mutexlock
			if(!e.b_up_c){if(button::up.pressed() != e.b_up){e.b_up_p = e.b_up;	e.b_up = button::up.pressed(); e.b_up_c = true;}}
			if(!e.b_down_c){if(button::down.pressed() != e.b_down){e.b_down_p = e.b_down;	e.b_down = button::down.pressed(); e.b_down_c = true;}}
			if(!e.b_left_c){if(button::left.pressed() != e.b_left){e.b_left_p = e.b_left;	e.b_left = button::left.pressed(); e.b_left_c = true;}}
			if(!e.b_right_c){if(button::right.pressed() != e.b_right){e.b_right_p = e.b_right;	e.b_right = button::right.pressed(); e.b_right_c = true;}}
			if(!e.b_enter_c){if(button::enter.pressed() != e.b_enter){e.b_enter_p = e.b_enter;	e.b_enter = button::enter.pressed(); e.b_enter_c = true;}}
			if(!e.b_escape_c){if(button::back.pressed() != e.b_escape){e.b_escape_p = e.b_escape;	e.b_escape = button::back.pressed(); e.b_escape_c = true;}}
			e.m_buttons.unlock();
			// cout current buttons if pressed
			/*if(_verbose){
				intercom::msg("verbose is true");
				m_screen.lock();
				std::cout << "u " << e.b_up << "d " << e.b_down << "l " << e.b_left << "r " << e.b_right << "e " << e.b_enter << "p " << e.b_escape << std::endl;
				m_screen.unlock();
			}*/
			//printf ("up:%d down:%d left:%d right:%d enter:%d esc:%d\n", up, down, left, right, enter, escape);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	});
	//tbutton.join();
}

void Control::th_initNetwork()
{
	std::cout << "Start button input thread... " << std::endl;
	t_network = std::thread([&] {
		int listenfd = 0, connfd = 0;
	    struct sockaddr_in serv_addr;
	    char sendBuff[1025];
	    time_t ticks;
		listenfd = socket(AF_INET, SOCK_STREAM, 0);
		memset(&serv_addr, '0', sizeof(serv_addr));
		memset(sendBuff, '0', sizeof(sendBuff));

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(5000);
	    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	    listen(listenfd, 10);
	    while(1)
	    {
	    	connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

	    	//ticks = time(NULL);
		    //snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
	    	snprintf(sendBuff, sizeof(sendBuff), "Hello and Welcome!");
		    write(connfd, sendBuff, strlen(sendBuff));

		    close(connfd);
		    sleep(1);
	    }
	});
}
} /* namespace ev3dev */
