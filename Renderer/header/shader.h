#pragma once
#include <string>

namespace RENDERER {
	class shader {
	public:
		using uint = unsigned int;
	private:
		uint program;
		uint vertex_shader;
		uint fragment_shader;

	public:
		shader() = default;
		shader(const std::string &vert_filename, const std::string &frag_filename);
		~shader();

		static uint create_vertex_shader(const std::string &filename);
		static uint create_fragment_shader(const std::string &filename);

		void set_vertex_shader(const std::string &filename);
		void set_fragment_shader(const std::string &filename);
		void set_program(const std::string &vert_filename, const std::string &frag_filename);

		uint get_program() const noexcept;
		uint get_vertex_shader() const noexcept;
		uint get_fragment_shader() const noexcept;

		uint get_uniform_location(const std::string &varname) const;

		void use() const;
		void nouse() const;
	};
}