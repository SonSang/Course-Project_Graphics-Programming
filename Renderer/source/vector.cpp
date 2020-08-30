#include "../header/vector.h"
#include "../header/exception.h"
#include "opengl/freeglut/freeglut.h"

namespace RENDERER {
	vec3d::vec3d(double x, double y, double z) : element{ x, y, z } {}

	void vec3d::set(double x, double y, double z) {
		element[0] = x; element[1] = y; element[2] = z;
	}

	vec3d vec3d::operator+(const vec3d &v) { 
		return vec3d(element[0] + v[0], element[1] + v[1], element[2] + v[2]); 
	}
	vec3d vec3d::operator-(const vec3d &v) {
		return vec3d(element[0] + v[0], element[1] + v[1], element[2] + v[2]);
	}
	vec3d vec3d::operator*(double c) {
		return vec3d(element[0] * c, element[1] * c, element[2] * c);
	}
	vec3d vec3d::operator/(double c) {
		return vec3d(element[0] / c, element[1] / c, element[2] / c);
	}
	void vec3d::operator+=(const vec3d &v) {
		element[0] += v[0];
		element[1] += v[1];
		element[2] += v[2];
	}
	void vec3d::operator-=(const vec3d &v) {
		element[0] -= v[0];
		element[1] -= v[1];
		element[2] -= v[2];
	}
	void vec3d::operator*=(double c) {
		element[0] *= c;
		element[1] *= c;
		element[2] *= c;
	}
	void vec3d::operator/=(double c) {
		element[0] /= c;
		element[1] /= c;
		element[2] /= c;
	}
	double vec3d::operator[](int i) const {
		if (i < 0 || i > 2)
		{
			print_message("Invalid index for vec3d.");
			exit(-1);
		}
		return element[i];
	}

	vec3d vec3d::cross(const vec3d &v) const {
		return vec3d(
			element[1] * v[2] - element[2] * v[1],
			element[2] * v[0] - element[0] * v[2],
			element[0] * v[1] - element[1] * v[0]
		);
	}
	double vec3d::dot(const vec3d &v) const noexcept {
		return element[0] * v[0] + element[1] * v[1] + element[2] * v[2];
	}

	double vec3d::norm() const noexcept {
		return sqrt(element[0] * element[0] + element[1] * element[1] + element[2] * element[2]);
	}
	void vec3d::normalize() {
		double
			n = norm();
		if (n == 0.0) {
			print_message("Zero vector cannot be normalized.");
			exit(-1);
		}
		*this /= n;
	}

	void vec3d::to_float_array(float arr[3]) const noexcept {
		arr[0] = (float)element[0];
		arr[1] = (float)element[1];
		arr[2] = (float)element[2];
	}

	void vec3d::gl_draw_vertex() const noexcept {
		glVertex3d(element[0], element[1], element[2]);
	}
	void vec3d::gl_draw_normal() const noexcept {
		glNormal3d(element[0], element[1], element[2]);
	}

	vec2d::vec2d(double x, double y) : element{ x, y } {}
	void vec2d::set(double x, double y) {
		element.at(0) = x;
		element.at(1) = y;
	}
	double vec2d::operator[](int i) const {
		return element.at(i);
	}
}