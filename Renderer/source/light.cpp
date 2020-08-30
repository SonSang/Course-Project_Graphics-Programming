#include "../header/light.h"

namespace RENDERER {
	// light
	void light::set_ambient(double r, double g, double b) {
		this->ambient.set(r, g, b);
	}
	void light::set_diffuse(double r, double g, double b) {
		this->diffuse.set(r, g, b);
	}
	void light::set_specular(double r, double g, double b) {
		this->specular.set(r, g, b);
	}

	void light::set_ambient(const vec3d &v) {
		this->ambient = v;
	}
	void light::set_diffuse(const vec3d &v) {
		this->diffuse = v;
	}
	void light::set_specular(const vec3d &v) {
		this->specular = v;
	}

	vec3d light::get_ambient() const noexcept {
		return this->ambient;
	}
	vec3d light::get_diffuse() const noexcept {
		return this->diffuse;
	}
	vec3d light::get_specular() const noexcept {
		return this->specular;
	}

	// light_point
	void light_point::set_position(double x, double y, double z) {
		this->position.set(x, y, z);
	}
	void light_point::set_position(const vec3d &v) {
		this->position = v;
	}
	vec3d light_point::get_position() const noexcept {
		return this->position;
	}

	// light_tube
	void light_tube::set_beg(double x, double y, double z) {
		this->beg.set(x, y, z);
	}
	void light_tube::set_end(double x, double y, double z) {
		this->end.set(x, y, z);
	}

	void light_tube::set_beg(const vec3d &v) {
		this->beg = v;
	}
	void light_tube::set_end(const vec3d &v) {
		this->end = v;
	}

	vec3d light_tube::get_beg() const noexcept {
		return this->beg;
	}
	vec3d light_tube::get_end() const noexcept {
		return this->end;
	}

	void light_tube::get_diffuse_coef(double coef[3]) {
		coef[0] = get_beg().dot(get_beg());
		coef[1] = 2 * (get_end() - get_beg()).dot(get_beg());
		coef[2] = (get_end() - get_beg()).dot(get_end() - get_beg());
	}
}