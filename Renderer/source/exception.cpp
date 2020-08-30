#include "../header/exception.h"
#include <iostream>

namespace RENDERER {
	void print_stack_trace() {
		std::cout << "Stack trace : " << std::endl;
		// no using boost?
	}
	void print_message(const std::string &msg) {
		std::cout << "Message : " << std::endl;
		std::cout << msg << std::endl;
	}
	void print_message_exit(const std::string &msg) {
		print_message(msg);
		exit(-1);
	}
	void print_stack_trace_message(const std::string &msg) {

	}
}