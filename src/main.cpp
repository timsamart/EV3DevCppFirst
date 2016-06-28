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

#include "ev3dev.h"
#include <iostream>

#include "Control.h"

using namespace ev3dev;

int
main()
{
  Control c;
  sound s;
  if (c.initialized(0))
    {
	  s.beep();
	  s.speak("Hello My name is Brick, Lego Brick!",true);
	  std::cout << "Motor Control initialized" << std::endl;
      //c.terminate_on_key(); // we terminate if a button is pressed
	  c.driveprimitive();
	  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	  c.sensorRead();
    }


  std::cout << "Hello ARM World!" << std::endl;
  return 0;
}


