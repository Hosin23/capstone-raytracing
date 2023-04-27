#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "aabb.h"

struct hit_record {
    vec3 p;
    vec3 normal;
    float t;
    bool front_face;

    inline void set_face_normal(const Ray& r, const vec3& outward_normal) {
        front_face = dot(r.getDirection(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};

class Hittable {
    public:
        virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;

        virtual bool bounding_box(Aabb& output_box) const = 0;
};

#endif
