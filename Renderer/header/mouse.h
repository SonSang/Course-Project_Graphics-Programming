#pragma once
#include "camera.h"

namespace RENDERER {
	class mouse {
	private:
		double	xpos;
		double	ypos;
		bool	first_mouse;

		bool	lpressed;
		bool	rpressed;
		bool	mpressed;

		double	sensitivity;

		void	update_camera_angle(camera &cam, double xpos, double ypos) const noexcept;
	public:
		mouse();

		void set_first_mouse(bool fm);
		void set_position(double x, double y);
		void set_sensitivity(double sensitivity);
		void set_lpressed(bool p);
		void set_rpressed(bool p);
		void set_mpressed(bool p);
		
		double get_xpos() const noexcept;
		double get_ypos() const noexcept;
		bool get_first_mouse() const noexcept;
		bool get_lpressed() const noexcept;
		bool get_rpressed() const noexcept;
		bool get_mpressed() const noexcept;
		double get_sensitivity() const noexcept;

		void move_camera(camera &cam, double xpos, double ypos);
	};
}