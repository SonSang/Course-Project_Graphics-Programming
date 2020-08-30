#pragma once
#include <string>
#include <vector>

namespace RENDERER {
	std::string					read_text_file(const std::string &filename);
	std::vector<std::string>	read_text_file_multiline(const std::string &filename);

	// read image at [filename] to [bits] and set [width], [height] of it.
	void read_image(const std::string &filename, unsigned char **bits, unsigned int &width, unsigned int &height);
}