#include "../header/parser.h"
#include "../header/io.h"
#include "opengl/freeglut/freeglut.h"

namespace RENDERER {

	int random_name = 0;

	void parser_obj::vertex::set_position(const vec3d &v) {
		position = v;
	}
	void parser_obj::vertex::set_normal(const vec3d &n) {
		normal = n;
	}
	void parser_obj::vertex::set_texture(const vec2d &t) {
		texture = t;
	}

	vec3d& parser_obj::vertex::get_position() noexcept {
		return position;
	}
	vec3d& parser_obj::vertex::get_normal() noexcept {
		return normal;
	}
	vec2d& parser_obj::vertex::get_texture() noexcept {
		return texture;
	}
	
	void parser_obj::face::add_vertex(const vertex &v) {
		vertexlist.push_back(v);
	}
	parser_obj::vertex& parser_obj::face::get_vertex(size_t i) {
		return vertexlist.at(i);
	}
	bool parser_obj::face::is_empty() const noexcept {
		return vertexlist.empty();
	}
	size_t parser_obj::face::size() const noexcept {
		return vertexlist.size();
	}
	void parser_obj::face::set_normal() {
		vec3d
			&pos0 = get_vertex(0).get_position(),
			&pos1 = get_vertex(1).get_position(),
			&pos2 = get_vertex(2).get_position(),
			sub0 = pos1 - pos0,
			sub1 = pos2 - pos1,
			normal = sub0.cross(sub1);
		normal.normalize();
		size_t
			vnum = size();
		for (int i = 0; i < vnum; i++)
			get_vertex(i).set_normal(normal);
	}
	void parser_obj::face::gl_draw() {
		auto
			vnum = size();
		glBegin(GL_POLYGON);
		for (int i = 0; i < vnum; i++) {
			get_vertex(i).get_normal().gl_draw_normal();
			get_vertex(i).get_position().gl_draw_vertex();
		}
		glEnd();
	}

	void parser_obj::face::scale(double scale_factor) {
		auto
			vnum = size();
		for (int i = 0; i < vnum; i++) {
			get_vertex(i).get_position() *= scale_factor;
		}
	}
	void parser_obj::face::translate(double x, double y, double z) {
		auto
			vnum = size();
		for (int i = 0; i < vnum; i++) {
			get_vertex(i).get_position() += vec3d(x, y, z);
		}
	}
	
	void parser_obj::group::set_name(const std::string &name) {
		this->name = name;
	}
	const std::string& parser_obj::group::get_name() const noexcept {
		return this->name;
	}
	void parser_obj::group::add_face(const face &f) {
		facelist.push_back(f);
	}
	parser_obj::face& parser_obj::group::get_face(size_t i) {
		return facelist.at(i);
	}
	bool parser_obj::group::is_empty() const noexcept {
		return facelist.empty();
	}
	size_t parser_obj::group::size() const noexcept {
		return facelist.size();
	}

	void parser_obj::object::set_name(const std::string &name) {
		this->name = name;
	}
	const std::string& parser_obj::object::get_name() const noexcept {
		return this->name;
	}
	void parser_obj::object::add_group(const group &g) {
		grouplist.push_back(g);
	}
	parser_obj::group& parser_obj::object::get_group(size_t i) {
		return grouplist.at(i);
	}
	bool parser_obj::object::is_empty() const noexcept {
		return grouplist.empty();
	}
	size_t parser_obj::object::size() const noexcept {
		return grouplist.size();
	}

	parser_obj::parser_obj(const std::string &filename) {
		parse(filename);
	}

	void parser_obj::start_object(std::string &line) {
		char
			*token = nullptr;
		char
			*copy = new char[line.length() + 1];
		strcpy(copy, line.c_str());
		token = strtok(copy, " "); // o
		token = strtok((char *)nullptr, " "); // name
		object
			no;
		no.set_name(token);
		add_object(no);
		delete[] copy;
	}
	void parser_obj::start_group(std::string &line) {
		char
			*token = nullptr;
		char
			*copy = new char[line.length() + 1];
		strcpy(copy, line.c_str());
		token = strtok(copy, " "); // g
		token = strtok((char *)nullptr, " "); // name
		group
			ng;
		ng.set_name(token);
		if (is_empty()) {
			object
				no;
			no.set_name(std::to_string(random_name++));
			add_object(no);
		}
		object
			&curobject = get_object(size() - 1);
		curobject.add_group(ng);
		delete[] copy;
	}

	void parser_obj::parse_vertex(std::string &line, std::vector<vec3d> &vlist) {
		vec3d
			val(0, 0, 0);
		double
			v0 = 0, v1 = 0, v2 = 0;
		char
			*token = (char *)nullptr;
		char
			*copy = new char[line.length() + 1];
		strcpy(copy, line.c_str());
		token = strtok(copy, " ");	// v
		token = strtok((char *)nullptr, " ");  // First
		v0 = atof(token);
		token = strtok((char *)nullptr, " ");  // Second
		v1 = atof(token);
		token = strtok((char *)nullptr, " ");  // Third
		v2 = atof(token);
		val.set(v0, v1, v2);
		vlist.push_back(val);
		delete[] copy;
	}
	void parser_obj::parse_normal(std::string &line, std::vector<vec3d> &nlist) {
		vec3d
			val(0, 0, 0);
		double
			v0 = 0, v1 = 0, v2 = 0;
		char
			*token = (char *)nullptr;
		char
			*copy = new char[line.length() + 1];
		strcpy(copy, line.c_str());
		token = strtok(copy, " ");	// vn
		token = strtok((char *)nullptr, " ");  // First
		v0 = atof(token);
		token = strtok((char *)nullptr, " ");  // Second
		v1 = atof(token);
		token = strtok((char *)nullptr, " ");  // Third
		v2 = atof(token);
		val.set(v0, v1, v2);
		nlist.push_back(val);
		delete[] copy;
	}
	void parser_obj::parse_texture(std::string &line, std::vector<vec2d> &tlist) {
		vec2d
			val(0, 0);
		double
			v0 = 0, v1 = 0;
		char
			*token = (char *)nullptr;
		char
			*copy = new char[line.length() + 1];
		strcpy(copy, line.c_str());
		token = strtok(copy, " ");	// vt
		token = strtok((char *)nullptr, " ");  // First
		v0 = atof(token);
		token = strtok((char *)nullptr, " ");  // Second
		v1 = atof(token);
		val.set(v0, v1);
		tlist.push_back(val);
		delete[] copy;
	}

	void parser_obj::parse_face(std::string &line,
		const std::vector<vec3d> &vlist,
		const std::vector<vec3d> &nlist,
		const std::vector<vec2d> &tlist) {
		face
			f;
		char
			*token = (char *)nullptr;
		char
			*copy = new char[line.length() + 1];
		strcpy(copy, line.c_str());
		token = strtok(copy, " ");	// f
		std::vector<std::string>
			tokenlist;
		while (true) {
			token = strtok((char *)nullptr, " ");
			if (token == (char *)nullptr)
				break;
			tokenlist.push_back(token);
		}
		size_t
			len = tokenlist.size();
		for (int i = 0; i < len; i++) {
			std::string
				str = tokenlist.at(i);
			size_t
				len = str.length();
			int
				delim[3];
			int
				idx = 0;
			vertex
				vert;
			for (int j = 0; j < len; j++) {
				if (str[j] == '/')
					delim[idx++] = j;
			}
			if (idx == 0) // only vertex info
			{
				size_t
					vertexindex = atoi(str.c_str());
				if (vertexindex < 0) {
					size_t
						curvertnum = vlist.size();
					vertexindex += curvertnum + 1;
				}
				vert.set_position(vlist.at(vertexindex - 1));
				vert.set_normal(vec3d(0, 0, 0));
				vert.set_texture(vec2d(0, 0));
				f.add_vertex(vert);
				continue;
			}
			else if (idx == 1) // vertex and texture info
			{
				str[delim[0]] = '\0';
				size_t
					vertexindex = atoi(str.c_str());
				if (vertexindex < 0) {
					size_t
						curvertnum = vlist.size();
					vertexindex += curvertnum + 1;
				}
				size_t
					textureindex = atoi(str.c_str() + delim[0] + 1);
				if (textureindex < 0) {
					size_t
						curtexnum = tlist.size();
					textureindex += curtexnum + 1;
				}
				vert.set_position(vlist.at(vertexindex - 1));
				vert.set_texture(tlist.at(textureindex - 1));
				vert.set_normal(vec3d(0, 0, 0));
				str[delim[0]] = '/';
				f.add_vertex(vert);
				continue;
			}
			else // all 3 info
			{
				str[delim[0]] = '\0';
				str[delim[1]] = '\0';
				bool
					has_texture_info = false;
				int
					vertexindex = 0,
					normalindex = 0,
					textureindex = 0;
				vertexindex = atoi(str.c_str());
				if (vertexindex < 0) {
					size_t
						curvertnum = vlist.size();
					vertexindex += (int)curvertnum + 1;
				}
				if (delim[1] == delim[0] + 1) // no texture info
					textureindex = -1;
				else {
					textureindex = atoi(str.c_str() + delim[0] + 1);
					has_texture_info = true;
				}
				if (has_texture_info && textureindex < 0) {
					size_t
						curtexnum = tlist.size();
					textureindex += (int)curtexnum + 1;
				}
				normalindex = atoi(str.c_str() + delim[1] + 1);
				if (normalindex < 0) {
					size_t
						curnormnum = nlist.size();
					normalindex += (int)curnormnum + 1;
				}
				vert.set_position(vlist.at(vertexindex - 1));
				vert.set_normal(nlist.at(normalindex - 1));
				vert.set_texture((textureindex < 0) ? vec2d(0, 0) : tlist.at(textureindex - 1));
				f.add_vertex(vert);
				str[delim[0]] = '/';
				str[delim[1]] = '/';
				continue;
			}
		}
		if (is_empty()) { // no prev object.
			object
				no;
			no.set_name(std::to_string(random_name++));
			add_object(no);
		}
		object
			&o = get_object(size() - 1);
		if (o.is_empty()) { // no prev group.
			group
				ng;
			ng.set_name(std::to_string(random_name++));
			o.add_group(ng);
		}
		group
			&g = o.get_group(o.size() - 1);
		g.add_face(f);
	}

	void parser_obj::parse(const std::string &filename) {
		std::vector<std::string>
			linelist = read_text_file_multiline(filename);
		std::vector<vec3d>
			vertexlist;
		std::vector<vec3d>
			normallist;
		std::vector<vec2d>
			texturelist;
		for (auto it = linelist.begin(); it != linelist.end(); it++) {
			std::string
				&line = *it;
			if (line[0] == 'o') {
				start_object(line);
			}
			else if (line[0] == 'g') {
				start_group(line);
			}
			else if (line[0] == 'v') {
				if (line[1] == ' ') {
					parse_vertex(line, vertexlist);
				}
				else if (line[1] == 'n') {
					parse_normal(line, normallist);
				}
				else if (line[1] == 't') {
					parse_texture(line, texturelist);
				}
			}
			else if (line[0] == 'f') {
				parse_face(line, vertexlist, normallist, texturelist);
			}
		}
	}

	void parser_obj::add_object(const object &o) {
		objectlist.push_back(o);
	}
	parser_obj::object& parser_obj::get_object(size_t i) {
		return objectlist.at(i);
	}

	bool parser_obj::is_empty() const noexcept {
		return objectlist.empty();
	}
	size_t parser_obj::size() const noexcept {
		return objectlist.size();
	}

	void parser_obj::clear() {
		objectlist.clear();
	}

	void parser_obj::set_normal() {
		for (auto o = objectlist.begin(); o != objectlist.end(); o++) {
			size_t
				gsize = o->size();
			for (int i = 0; i < gsize; i++) {
				group
					&g = o->get_group(i);
				size_t
					fsize = g.size();
				for (int j = 0; j < fsize; j++) {
					face
						&f = g.get_face(j);
					f.set_normal();
				}
			}
		}
	}
	void parser_obj::gl_draw() {
		for (auto o = objectlist.begin(); o != objectlist.end(); o++) {
			size_t
				gsize = o->size();
			for (int i = 0; i < gsize; i++) {
				group
					&g = o->get_group(i);
				size_t
					fsize = g.size();
				for (int j = 0; j < fsize; j++) {
					face
						&f = g.get_face(j);
					f.gl_draw();
				}
			}
		}
	}


	void parser_obj::scale(double scale_factor) {
		for (auto o = objectlist.begin(); o != objectlist.end(); o++) {
			size_t
				gsize = o->size();
			for (int i = 0; i < gsize; i++) {
				group
					&g = o->get_group(i);
				size_t
					fsize = g.size();
				for (int j = 0; j < fsize; j++) {
					face
						&f = g.get_face(j);
					f.scale(scale_factor);
				}
			}
		}
	}
	void parser_obj::translate(double x, double y, double z) {
		for (auto o = objectlist.begin(); o != objectlist.end(); o++) {
			size_t
				gsize = o->size();
			for (int i = 0; i < gsize; i++) {
				group
					&g = o->get_group(i);
				size_t
					fsize = g.size();
				for (int j = 0; j < fsize; j++) {
					face
						&f = g.get_face(j);
					f.translate(x, y, z);
				}
			}
		}
	}
}