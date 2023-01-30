#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

/*
 * Ray-Sphere Intersection
 * (𝑥−𝐶𝑥)^2+(𝑦−𝐶𝑦)^2+(𝑧−𝐶𝑧)^2=𝑟2
 * (𝐏−𝐂)⋅(𝐏−𝐂)=(𝑥−𝐶𝑥)^2+(𝑦−𝐶𝑦)^2+(𝑧−𝐶𝑧)^2
 * (𝐏−𝐂)⋅(𝐏−𝐂)=𝑟^2
 * (𝐏(𝑡)−𝐂)⋅(𝐏(𝑡)−𝐂)=𝑟^2
 * (𝐀+𝑡𝐛−𝐂)⋅(𝐀+𝑡𝐛−𝐂)=𝑟^2
 * 𝑡^2𝐛⋅𝐛+2𝑡𝐛⋅(𝐀−𝐂)+(𝐀−𝐂)⋅(𝐀−𝐂)−𝑟^2=0
 */

/*
 * auto a = dot(r.direction(), r.direction());
 * auto b = 2.0 * dot(oc, r.direction());
 * auto c = dot(oc, oc) - radius*radius;
 * auto discriminant = b*b - 4*a*c;
 */

class sphere : public hittable {
public:
	sphere() {}
	sphere(point3 cen, double r) : center(cen), radius(r) {};

	virtual bool hit(
			const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
	point3 center;
	double radius;
};

/* 球の交差判定 */
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	// 原点から球の中心までのベクトル(P-C)
	vec3 oc = r.origin() - center;
	// 解の公式偶数ver(bは必ず偶数だから)
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius*radius;

	auto discriminant = half_b*half_b - a*c;
	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	// 解の公式偶数verで平方根を出す
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	// outward_normalとは？
	vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);

	return true;
}

#endif