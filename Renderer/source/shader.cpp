#include "../header/shader.h"
#include "../header/io.h"

#include "opengl/glew/glew.h"
#include "opengl/freeglut/freeglut.h"

namespace RENDERER {
	using uint = unsigned int;
	shader::shader(const std::string &vert_filename, const std::string &frag_filename) {
		set_program(vert_filename, frag_filename);
	}
	shader::~shader() {
		glDetachShader(get_program(), get_vertex_shader());
		glDetachShader(get_program(), get_fragment_shader());
		glDeleteShader(get_vertex_shader());
		glDeleteShader(get_fragment_shader());
		glDeleteProgram(get_program());
	}

	uint shader::create_vertex_shader(const std::string &filename) {
		int
			success;
		char
			infoLog[512];
		GLuint
			id;
		std::string
			shader = read_text_file(filename);
		const GLchar
			*glshader = shader.c_str();
		id = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(id, 1, &glshader, NULL);
		glCompileShader(id);

		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			fprintf(stderr, "[SHADER ERROR] : Shader compilation failed\n");
			fprintf(stderr, "[COMPILATION ERROR MESSAGE] : \n%s\n\n", infoLog);
			exit(-1);
		}
		return id;
	}
	uint shader::create_fragment_shader(const std::string &filename) {
		int
			success;
		char
			infoLog[512];
		GLuint
			id;
		std::string
			shader = read_text_file(filename);
		const GLchar
			*glshader = shader.c_str();
		id = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(id, 1, &glshader, NULL);
		glCompileShader(id);

		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			fprintf(stderr, "[SHADER ERROR] : Shader compilation failed\n");
			fprintf(stderr, "[COMPILATION ERROR MESSAGE] : \n%s\n\n", infoLog);
			exit(-1);
		}
		return id;
	}

	void shader::set_vertex_shader(const std::string &filename) {
		this->vertex_shader = create_vertex_shader(filename);
	}
	void shader::set_fragment_shader(const std::string &filename) {
		this->fragment_shader = create_fragment_shader(filename);
	}
	void shader::set_program(const std::string &vert_filename, const std::string &frag_filename) {
		set_vertex_shader(vert_filename);
		set_fragment_shader(frag_filename);
		GLuint
			p = glCreateProgram();
		glAttachShader(p, get_vertex_shader());
		glAttachShader(p, get_fragment_shader());
		glLinkProgram(p);
		int
			success;
		char
			infoLog[512];
		glGetProgramiv(p, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(p, 512, NULL, infoLog);
			fprintf(stderr, "[SHADER ERROR] : Shader Program linkage failed\n");
			fprintf(stderr, "[LINKAGE ERROR MESSAGE] : \n%s\n\n", infoLog);
			exit(-1);
		}
		fprintf(stdout, "Created shader successfully.\n");
		program = p;
	}

	uint shader::get_program() const noexcept {
		return this->program;
	}
	uint shader::get_vertex_shader() const noexcept {
		return this->vertex_shader;
	}
	uint shader::get_fragment_shader() const noexcept {
		return this->fragment_shader;
	}

	uint shader::get_uniform_location(const std::string &varname) const {
		return glGetUniformLocation(get_program(), varname.c_str());
	}

	void shader::use() const {
		glUseProgram(get_program());
	}
	void shader::nouse() const {
		glUseProgram(0);
	}


}