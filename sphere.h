#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

/*
 * Ray-Sphere Intersection
 * (๐ฅโ๐ถ๐ฅ)^2+(๐ฆโ๐ถ๐ฆ)^2+(๐งโ๐ถ๐ง)^2=๐2
 * (๐โ๐)โ(๐โ๐)=(๐ฅโ๐ถ๐ฅ)^2+(๐ฆโ๐ถ๐ฆ)^2+(๐งโ๐ถ๐ง)^2
 * (๐โ๐)โ(๐โ๐)=๐^2
 * (๐(๐ก)โ๐)โ(๐(๐ก)โ๐)=๐^2
 * (๐+๐ก๐โ๐)โ(๐+๐ก๐โ๐)=๐^2
 * ๐ก^2๐โ๐+2๐ก๐โ(๐โ๐)+(๐โ๐)โ(๐โ๐)โ๐^2=0
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
	sphere(point3 cen, double r, shared_ptr<material> m)
			: center(cen), radius(r), mat_ptr(m) {};

	virtual bool hit(
			const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
	point3 center;
	double radius{};
	shared_ptr<material> mat_ptr;
};

/* ็ใฎไบคๅทฎๅคๅฎ */
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	// ๅ็นใใ็ใฎไธญๅฟใพใงใฎใใฏใใซ(P-C)
	vec3 oc = r.origin() - center;
	// ่งฃใฎๅฌๅผๅถๆฐver(bใฏๅฟใๅถๆฐใ ใใ)
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius*radius;

	auto discriminant = half_b*half_b - a*c;
	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	// ่งฃใฎๅฌๅผๅถๆฐverใงๅนณๆนๆ นใๅบใ
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	// atใฏPt=A+tb
	rec.p = r.at(rec.t);
	// outward_normalใจใฏ๏ผ
	vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);
	// ๆๅพใซ
	rec.mat_ptr = mat_ptr;

	return true;
}

#endif