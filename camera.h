#ifndef CAMERA_H
#define CAMERA_H

#include "rt.h"

class camera {
public:
	camera(
			point3 lookfrom, //カメラ位置
			point3 lookat, // 注視する点
			vec3   vup, // view_up:シーン全体の上方向
			double vfov, // vertical field-of-view in degrees(垂直方向の視野角 (弧度法))
			double aspect_ratio // スクリーンの縦横比
	) {
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta/2);
		// viewpointがスクリーン
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;


		auto w = unit_vector(lookfrom - lookat); // スクリーンから視点までの法線ベクトル
		auto u = unit_vector(cross(vup, w)); // 外積計算して正規化
		auto v = cross(w, u);

		origin = lookfrom;
		horizontal = viewport_width * u;
		vertical = viewport_height * v;
		lower_left_corner = origin - horizontal/2 - vertical/2 - w;
	}

	ray get_ray(double s, double t) const {
		return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
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