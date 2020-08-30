#include "../header/mouse.h"

#define DEFAULT_FIRST_MOUSE true
#define DEFAULT_X_POS		0
#define DEFAULT_Y_POS		0
#define DEFAULT_SENSITIVITY	0.1

namespace RENDERER {
	mouse::mouse() : first_mouse(DEFAULT_FIRST_MOUSE), xpos(DEFAULT_X_POS), ypos(DEFAULT_Y_POS), sensitivity(DEFAULT_SENSITIVITY) {

	}

	void mouse::set_first_mouse(bool fm) {
		this->first_mouse = fm;
	}
	void mouse::set_position(double x, double y) {
		this->xpos = x;
		this->ypos = y;
	}
	void mouse::set_sensitivity(double sensitivity) {
		this->sensitivity = sensitivity;
	}
	void mouse::set_lpressed(bool p) {
		this->lpressed = p;
	}
	void mouse::set_rpressed(bool p) {
		this->rpressed = p;
	}
	void mouse::set_mpressed(bool p) {
		this->mpressed = p;
	}

	double mouse::get_xpos() const noexcept {
		return this->xpos;
	}
	double mouse::get_ypos() const noexcept {
		return this->ypos;
	}
	bool mouse::get_first_mouse() const noexcept {
		return first_mouse;
	}
	bool mouse::get_lpressed() const noexcept {
		return lpressed;
	}
	bool mouse::get_rpressed() const noexcept {
		return rpressed;
	}
	bool mouse::get_mpressed() const noexcept {
		return mpressed;
	}
	double mouse::get_sensitivity() const noexcept {
		return sensitivity;
	}

	void mouse::move_camera(camera &cam, double xpos, double ypos) {
		if (get_first_mouse()) {
			set_position(xpos, ypos);
			set_first_mouse(false);
		}
		update_camera_angle(cam, xpos, ypos);
		set_position(xpos, ypos);
	}

	void mouse::update_camera_angle(camera &cam, double xpos, double ypos) const noexcept {
		double
			xoffset = (xpos - get_xpos()) * get_sensitivity(),
			yoffset = (get_ypos() - ypos) * get_sensitivity(); // inverted.
		cam.update_angle(xoffset, yoffset);
	}
}