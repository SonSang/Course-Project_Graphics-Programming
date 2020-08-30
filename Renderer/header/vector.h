#pragma once
#include <array>

namespace RENDERER {
	class vec3d {
	private:
		double element[3];
	public:
		vec3d() = default;
		vec3d(double x, double y, double z);

		void set(double x, double y, double z);

		vec3d operator+(const vec3d &v);
		vec3d operator-(const vec3d &v);
		vec3d operator*(double c);
		vec3d operator/(double c);
		void operator+=(const vec3d &v);
		void operator-=(const vec3d &v);
		void operator*=(double c);
		void operator/=(double c);
		double operator[](int i) const;

		vec3d cross(const vec3d &v) const;
		double dot(const vec3d &v) const noexcept;

		double norm() const noexcept;
		void normalize();

		void to_float_array(float arr[3]) const noexcept;

		void gl_draw_vertex() const noexcept;
		void gl_draw_normal() const noexcept;
	};

	class vec2d {
	private:
		std::array<double, 2> element;
	public:
		vec2d() = default;
		vec2d(double x, double y);

		void set(double x, double y);

		double operator[](int i) const;
	};
}