#pragma once
#include "vector.h"

#include <array>
#include <vector>
#include <string>

namespace RENDERER {
	class parser_obj {
	public:
		class vertex {
		private:
			vec3d position;
			vec3d normal;
			vec2d texture;
		public:
			void set_position(const vec3d &v);
			void set_normal(const vec3d &n);
			void set_texture(const vec2d &t);

			vec3d& get_position() noexcept;
			vec3d& get_normal() noexcept;
			vec2d& get_texture() noexcept;
		};

		class face {
		private:
			std::vector<vertex> vertexlist;
		public:
			void add_vertex(const vertex &v);
			vertex& get_vertex(size_t i);
			bool is_empty() const noexcept;
			size_t size() const noexcept;

			void set_normal();
			void gl_draw();

			void scale(double scale_factor);
			void translate(double x, double y, double z);
		};

		class group {
		private:
			std::string name;
			std::vector<face> facelist;
		public:
			void set_name(const std::string &name);
			const std::string& get_name() const noexcept;

			void add_face(const face &f);
			face& get_face(size_t i);
			bool is_empty() const noexcept;
			size_t size() const noexcept;
		};

		class object {
		private:
			std::string name;
			std::vector<group> grouplist;
		public:
			void set_name(const std::string &name);
			const std::string& get_name() const noexcept;

			void add_group(const group &g);
			group& get_group(size_t i);
			bool is_empty() const noexcept;
			size_t size() const noexcept;
		};

	private:
		std::vector<object> objectlist;

		void start_object(std::string &line);
		void start_group(std::string &line);

		void parse_vertex(std::string &line, std::vector<vec3d> &vlist);
		void parse_normal(std::string &line, std::vector<vec3d> &nlist);
		void parse_texture(std::string &line, std::vector<vec2d> &tlist);

		void parse_face(std::string &line,
			const std::vector<vec3d> &vlist,
			const std::vector<vec3d> &nlist,
			const std::vector<vec2d> &tlist);
	public:
		parser_obj() = default;
		parser_obj(const std::string &filename);

		void parse(const std::string &filename);

		void add_object(const object &o);
		object& get_object(size_t i);

		bool is_empty() const noexcept;
		size_t size() const noexcept;

		void clear();

		void set_normal();	// set normal vector of each face.
		void gl_draw();

		void scale(double scale_factor);
		void translate(double x, double y, double z);
	};
}