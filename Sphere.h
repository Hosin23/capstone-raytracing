#ifndef SPHERE_H
#define SPHERE_H

#include "rtcapstone.h"
#include "hittable.h"

class Sphere : public Hittable {
    public:
        Sphere() {}
        Sphere(vec3 cen, float r) : center(cen), radius(r) {};

        virtual bool hit(
            const Ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(
            Aabb& output_box) const override;


    private:
        vec3 center;
        float radius;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
    
    vec3 oc = r.getOrigin() - center;
    auto a = length(r.getDirection()) * length(r.getDirection());
    auto half_b = dot(oc, r.getDirection());
    auto c = length(oc) * length(oc) - radius * radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}


bool Sphere::bounding_box(Aabb& output_box) const {
    output_box = Aabb(
        center - vec3(radius, radius, radius),
        center + vec3(radius, radius, radius));
    return true;
}

#endif