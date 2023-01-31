#ifndef HITTABLE_H
#define HITTABLE_H

#include "rt.h"

// hit_recordにmaterialの存在を示さなければいけない
class material;

struct hit_record {
	point3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;
	double t;
	bool front_face;

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		/* 法線の向きを計算 */
		// rayが物体の内か外かを調べる
		// 内積がマイナスのときに1が入る
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal :-outward_normal;
	}
};

class hittable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif