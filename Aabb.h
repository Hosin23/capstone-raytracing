#ifndef AABB_H
#define AABB_H

#include "rtcapstone.h"

class Aabb {
    public:
        Aabb() {
            //minimum = vec3(.0f, .0f, .0f);
            //maximum = vec3(.0f, .0f, .0f);
        }
        Aabb(const vec3& a, const vec3& b) {
            minimum = vec3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
            maximum = vec3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));
        }

        vec3 min() const {return minimum;}
        vec3 max() const {return maximum;}

        vec3 centroid() {
            return (maximum + minimum) / 2.0f;
        }

        bool hit(const Ray& r, float t_min, float t_max) const {
            for (int a = 0; a < 3; a++) {
                float bmin, bmax, ro, rd;
                if(a == 0) { bmin = min().x; bmax = max().x; ro = r.getOrigin().x; rd = r.getDirection().x;}
                if(a == 1) { bmin = min().y; bmax = max().y; ro = r.getOrigin().y; rd = r.getDirection().y;}
                if(a == 2) { bmin = min().z; bmax = max().z; ro = r.getOrigin().z; rd = r.getDirection().z;}

                auto invD = 1.0f / rd;
                auto t0 = (bmin - ro) * invD;
                auto t1 = (bmax - ro) * invD;
                if (invD < 0.0f)
                    swap(t0, t1);
                t_min = t0 > t_min ? t0 : t_min;
                t_max = t1 < t_max ? t1 : t_max;
                if (t_max <= t_min)
                    return false;
            }
            return true;
        }

        vec3 minimum;
        vec3 maximum;
};

Aabb surrounding_box(Aabb box0, Aabb box1) {
    vec3 small(fmin(box0.min().x, box1.min().x),
            fmin(box0.min().y, box1.min().y),
            fmin(box0.min().z, box1.min().z));

    vec3 big(fmax(box0.max().x, box1.max().x),
            fmax(box0.max().y, box1.max().y),
            fmax(box0.max().z, box1.max().z));

    return Aabb(small,big);
}

Aabb expand(const Aabb& box, const vec3 p){
    vec3 small(fmin(box.min().x, p.x),
            fmin(box.min().y, p.y),
            fmin(box.min().z, p.z));

    vec3 big(fmax(box.max().x, p.x),
            fmax(box.max().y, p.y),
            fmax(box.max().z, p.z));


    return Aabb(small, big);
}

#endif