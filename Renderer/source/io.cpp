#include "../header/io.h"
#include "../header/exception.h"
#include "FreeImage/x64/FreeImage.h"

#include <fstream>

namespace RENDERER {
	// check if [filename] is valid.
	void check_valid_filename(const std::string &filename) {
		std::ifstream
			ifs(filename);
		if (!ifs.is_open()) {
			print_message(std::string("Cannot find ") + filename);
			exit(-1);
		}
		ifs.close();
	}

	// Reference : https://modoocode.com/215
	std::string read_text_file(const std::string &filename) {
		check_valid_filename(filename);
		std::ifstream
			ifs(filename);
		std::string
			str;
		ifs.seekg(0, std::ios::end);
		auto
			size = ifs.tellg();
		str.resize(size);
		ifs.seekg(0, std::ios::beg);
		ifs.read(&str[0], size);
		ifs.close();
		return str;
	}
	std::vector<std::string>	read_text_file_multiline(const std::string &filename) {
		check_valid_filename(filename);
		std::ifstream
			ifs(filename);
		std::vector<std::string>
			ret;
		if (!ifs.is_open()) {
			print_message(std::string("Cannot find ") + filename);
			exit(-1);
		}
		while (!ifs.eof()) {
			std::string
				line;
			std::getline(ifs, line);
			ret.push_back(line);
		}
		ifs.close();
		return ret;
	}

	// Reference : FreeImage example - OpenGL Texture Manager
	void read_image(const std::string &filename, unsigned char **bits, unsigned int &width, unsigned int &height) {
		check_valid_filename(filename);
		FREE_IMAGE_FORMAT
			fif = FIF_UNKNOWN;
		FIBITMAP
			*dib = 0;
		
		fif = FreeImage_GetFileType(filename.c_str(), 0);
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename.c_str());
		if (fif == FIF_UNKNOWN) 
			print_message_exit(std::string("Cannot identify image file type of ") + filename);

		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename.c_str());
		if(!dib)
			print_message_exit(std::string("This type of image is not supported in FreeImage."));

		/*FREE_IMAGE_COLOR_TYPE
			ctype = FreeImage_GetColorType(dib);
		unsigned int
			bpp = FreeImage_GetBPP(dib);*/

		*bits = FreeImage_GetBits(dib);
		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);
	}
}