#pragma once
#include "../header/timer.h"
#include <omp.h>
#include <iostream>

std::string name;
double		start;
double		end;

namespace RENDERER {
	void timer::start(const std::string &n) {
		name = n;
		::start = get_time();
	}
	void timer::end() {
		::end = get_time();
	}
	void timer::print() {
		std::cout << "[" << name << "] : " << ::end - ::start << "(sec), " << (::end - ::start) * 1e+3 << "(msec)" << std::endl;
	}
	void timer::end_print() {
		end();
		print();
	}

	double timer::get_time() {
		return omp_get_wtime();
	}
}