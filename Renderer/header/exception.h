#pragma once
#include <string>

namespace RENDERER {
	void print_stack_trace();
	void print_message(const std::string &msg);
	void print_message_exit(const std::string &msg);
	void print_stack_trace_message(const std::string &msg);
}