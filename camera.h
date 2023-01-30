#ifndef CAMERA_H
#define CAMERA_H

#include "rt.h"

class camera {
public:
	camera() {
		// 画面の比率を16:9に
		auto aspect_ratio = 16.0 / 9.0;
		// viewportがスクリーン
		auto viewport_height = 2.0;
		auto viewport_width = aspect_ratio * viewport_height;
		// 視点からviewportまでの法線の大きさ
		auto focal_length = 1.0;

		origin = point3(0, 0, 0);
		horizontal = vec3(viewport_width, 0.0, 0.0);
		vertical = vec3(0.0, viewport_height, 0.0);
		lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
	}

	ray get_ray(double u, double v) const {
		return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
	}

private:
	// 原点
	point3 origin;
	// スクリーン一番左下の座標
	point3 lower_left_corner;
	// 縦と横？
	vec3 horizontal;
	vec3 vertical;
};
#endif