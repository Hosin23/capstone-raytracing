#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "rtcapstone.h"
#include "hittable.h"


class hittable_list : public Hittable {
    public:
        hittable_list() {}
        hittable_list(shared_ptr<Hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<Hittable> object) { objects.push_back(object); }

        virtual bool hit(
            const Ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(
            Aabb& output_box) const override;


    public:
        std::vector<shared_ptr<Hittable>> objects;
};

bool hittable_list::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool hittable_list::bounding_box(Aabb& output_box) const {
    if (objects.empty()) return false;

    Aabb temp_box;
    bool first_box = true;

    for (const auto& object : objects) {
        if (!object->bounding_box(temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

#endif