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
			double aspect_ratio, // スクリーンの縦横比
			double aperture, // 口径(視点レンスの大きさ)
			double focus_dist //集束距離()
	) {
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta/2);
		// viewpointがスクリーン
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;


		w = unit_vector(lookfrom - lookat); // スクリーンから視点までの法線ベクトル
		u = unit_vector(cross(vup, w)); // 外積計算して正規化
		v = cross(w, u); // lookatとlookfromの外積

		origin = lookfrom;
		// focus_lengthをかけてぼかす
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

		lens_radius = aperture / 2; // 口径/2 でレンズの半径
	}

	ray get_ray(double s, double t) const {
		vec3 rd = lens_radius * random_in_unit_disk();
		// originからどれだけずれたかを計算
		vec3 offset = u * rd.x() + v * rd.y();

		return ray(
				// originからのずれを出す
				origin + offset,
				// ずれた視点からスクリーンに向けた方向ベクトル
				lower_left_corner + s*horizontal + t*vertical - origin - offset
		);}

private:
	point3 origin; // 原点
	point3 lower_left_corner; // スクリーン一番左下の座標
	vec3 horizontal; // 横
	vec3 vertical; // 縦
	vec3 u, v, w;
	double lens_radius;
};
#endif