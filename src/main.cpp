//============================================================================
// Name        : main.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++
//============================================================================
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
	  s.speak("Baby I like it! gimme more!",true);
	  std::cout << "Motor Control initialized" << std::endl;
      //c.terminate_on_key(); // we terminate if a button is pressed
      //c.drive(80,0);
	  c.sensorRead();
	  std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	  c.driveprimitive();
    }


  std::cout << "Hello ARM World!" << std::endl;
  return 0;
}
