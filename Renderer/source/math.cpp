#include "../header/math.h"

namespace RENDERER {
	double deg2rad(double degree) {
		return degree * (PI / 180.0);
	}
	double rad2deg(double radian) {
		return 180.0 * (radian / PI);
	}
}