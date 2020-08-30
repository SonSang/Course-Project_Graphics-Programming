#pragma once
#include <string>

namespace RENDERER {
	class timer {
	public:
		static void start(const std::string &n);
		static void end();
		static void print();
		static void end_print();

		static double get_time();
	};
}