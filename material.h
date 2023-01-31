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

#endif