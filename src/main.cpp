/*============================================================================
 * Name        : main.cpp
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
// Description : Hello World in C++ for EV3 Brick
//============================================================================
// Run Programm in ssh
//  upload to "Home" or "My Home" Folder by drag and drop the ".elf" file from
//  "Binaries" in the Project folder to the "Home" folder.
//  right click "Ssh Shells"->"Launch Shell"
//  in the shell set execution rights to your uploaded file:
//    chmod -R ugo+x test.elf
//  remember to navigate to any subfolder your file is in first! by doing:
//    cd *pathname of subfolder*
//  now you can run the program by typing the following:
//    ./test.elf
//  you should see the motor running and after a few seconds you should see the
//  readouts of the sonic Sensor in cm
//    Sonic Sensor Distance: = 9
//------------------------------------------------------------
// usefull linux commands:
// #ps -All //shows all processes
// # kill [signal or option] PID(s)
// where signal is:
// Signal Name			Signal			Behaviour
// SIGHUP			      1				Hangup
// SIGKILL			      9				Kill Signal
// SIGTERM			      15			Terminate

#include "ev3dev.h"
#include <iostream>

#include "Control.h"

using namespace ev3dev;

int main()
{
	Control c;
	sound s;
	s.beep();
	s.speak("Hello My name is Brick, Lego Brick!",true);
	c.initialize();
	std::cout << "Hello ARM World!" << std::endl;
	//c.drive(0,1050,0,5000);
	//c.drive(1,1050,0,5000);
	bool x = true;
	/*std::thread thj = std::thread([&] {
		while(x){
			intercom::msg(c.e.getMotorStatus(0).c_str());
			intercom::msg("speed ", c.motors[0]->duty_cycle());
			c.sleep(500);
		};
	});*/
	//while(!c.e.buttonPressed(ESCAPE)){};
	//c.drive(0,300,2000);
	while(!c.e.buttonPressed(ESCAPE)){
		if(c.e.buttonPressed(LEFT)){
			c.drive_ToPositionAbsolute(0, 50);
		}
		if(c.e.buttonPressed(RIGHT)){
			c.drive_ToPositionRelative(0, 50);
		}
	};

	x=false;
	//std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	//thj.join();
	c.exit();
	s.speak("Bye, Bye, Jacky!");
	//c.terminate_on_key(); // we terminate if a button is pressed
	return 0;
}


