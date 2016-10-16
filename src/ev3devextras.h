/*
 * ev3devextras.h
 *
 *  Created on: 16.10.2016
 *      Author: Chrysanth
 */

#ifndef EV3DEVEXTRAS_H_
#define EV3DEVEXTRAS_H_

#include <thread>
#include <mutex>
#include <iostream>

namespace ev3dev {

static std::mutex m_screen; // ENSURE THREAD SAFETY
static std::mutex m_sensors[4];
static std::mutex m_motors[4];
static bool verbose = false;

class intercom {
	public:
		//static void msg(char const * const message); // Thread safe cout replacement
		//static void msg(char const * const message, int const &value); // Thread safe cout replacement for numericals
		//static void msg(char const * const message, std::string &value); // Thread safe cout replacement for strings
		static void msg(char const * const message)
		{
		  m_screen.lock();
		  std::cout << message << std::endl;
		  m_screen.unlock();
		};
		static void msg(char const * const message, int const &value)
		{
		  m_screen.lock();
		  std::cout << message << " = " << value << std::endl;
		  m_screen.unlock();
		};
		static void msg(char const * const message, std::string &value)
		{
		  m_screen.lock();
		  std::cout << message << " = " << value << std::endl;
		  m_screen.unlock();
		};
	};
} /* namespace ev3dev */

#endif /* EV3DEVEXTRAS_H_ */
