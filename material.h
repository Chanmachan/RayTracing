#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt.h"

struct hit_record;

// scatter:散らばる attenuation:減衰
class material {
public:
	virtual bool scatter(
			const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const = 0;
};

// lambertian:拡散
class lambertian : public material {
public:
	// colorのクラスをそのままalbedo(反射光の比)として採用
	lambertian(const color& a) : albedo(a) {}

	// 純粋仮想関数のscatterをオーバーライド
	virtual bool scatter(
			const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		// randomに拡散する光の方向ベクトルを計算
		auto scatter_direction = rec.normal + random_unit_vector();

		// 生成するランダムな単位ベクトルが法線ベクトルと正反対のときを防ぐ関数
		// Catch degenerate scatter direction
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = ray(rec.p, scatter_direction);
		// attenuation:減衰率
		attenuation = albedo;
		return true;
	}

public:
	color albedo;
};

// 金属の場合のレイの反射
class metal : public material {
public:
	metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(
			const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

public:
	color albedo;
	double fuzz;
};

class dielectric : public material {
public:
	dielectric(double index_of_refraction) : ir(index_of_refraction) {}

	virtual bool scatter(
			const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		// 透明->背景と同じ
		attenuation = color(1.0, 1.0, 1.0);
		// front_faceでレイが内か外かを計算して屈折率を出す
		double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

		vec3 unit_direction = unit_vector(r_in.direction());
		// 屈折レイ
		vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

		scattered = ray(rec.p, refracted);
		return true;
	}

public:
	double ir; // Index of Refraction:屈折率
};

#endif