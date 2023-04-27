#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "rtcapstone.h"
#include "Mesh.h"
#include "Aabb.h"

struct TriangleMesh {
    TriangleMesh(const vec3*v, const int*i, const vec3*n, int numP, int numT);

    const int numP, numT;
    std::vector<vec3> vertices;
    std::vector<int> indices;
    std::vector<vec3> normals;
};

TriangleMesh::TriangleMesh(const vec3*v, const int*i, const vec3*n, int numP, int numT)
: numP(numP), numT(numT){
    vertices = std::vector<vec3>(v, v);
    indices = std::vector<int>(i, i);
    normals = std::vector<vec3>(n, n);
};

class Triangle : public Hittable {
    public:
        Triangle() {};
        Triangle(const shared_ptr<TriangleMesh> &mesh, int triNumber): mesh(mesh) {
            v = &mesh -> indices[3 * triNumber];
        };

        virtual bool hit(
            const Ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(
            Aabb& output_box) const override;

        bool intersect(const Ray &r, float t_max, float t_min, hit_record& rec) const {
            vec3 &p0 = mesh -> vertices[v[0]];
            vec3 &p1 = mesh -> vertices[v[1]];
            vec3 &p2 = mesh -> vertices[v[2]];

            vec3 e1 = p1 - p0;
            vec3 e2 = p2 - p0;

            vec3 s1 = cross(r.getDirection(), e2);
            float divisor = dot(s1, e1);
            if (divisor == 0.)
                return false;

            float indivisor = 1.f / divisor;

            vec3 s = r.getOrigin() - p0;
            float b1 = dot(s, s1) * indivisor;
            if (b1 < 0.f || b1 > 1.f)
                return false;

            vec3 s2 = cross(s, e1);
            float b2 = dot(r.getDirection(), s2) * indivisor;
            if (b2 < 0.f || b1 + b2 > 1.f)
                return false;
            
            float t = dot(e2, s2) * indivisor;
            if (t > t_max || t < t_min)
                return false;

            rec.t = t;
            rec.normal = normalize(cross(e1, e2));
            return true;
        };

        string to_string(){
            string s;
            s += std::to_string( mesh -> vertices[v[0]].x);
            s += "/";
            s += std::to_string( mesh -> vertices[v[0]].y);
            s += "/";
            s += std::to_string( mesh -> vertices[v[0]].z);
            s += " ";
            s += std::to_string( mesh -> vertices[v[1]].x);
            s += "/";
            s += std::to_string( mesh -> vertices[v[1]].y);
            s += "/";
            s += std::to_string( mesh -> vertices[v[1]].z);
            s += " ";
            s += std::to_string( mesh -> vertices[v[2]].x);
            s += "/";
            s += std::to_string( mesh -> vertices[v[2]].y);
            s += "/";
            s += std::to_string( mesh -> vertices[v[2]].z);
            return s;
        };

        int v0(){
            return v[0];
        };

        int v1(){
            return v[1];
        };

        int v2(){
            return v[2];
        };

        vec3 centroid(){
            vec3 &p0 = mesh -> vertices[v[0]];
            vec3 &p1 = mesh -> vertices[v[1]];
            vec3 &p2 = mesh -> vertices[v[2]];
            float x = (p0.x + p1.x + p2.x) / 3.0f;
            float y = (p0.y + p1.y + p2.y) / 3.0f;
            float z = (p0.z + p1.z + p2.z) / 3.0f;
            return vec3(x, y, z);
        }

    private:
        shared_ptr<TriangleMesh> mesh;
        const int *v; 
};

bool Triangle::hit(
    const Ray& r, float t_min, float t_max, hit_record& rec) const {

    return intersect(r, t_max, t_min, rec);

};

bool Triangle::bounding_box(
    Aabb& output_box) const {

    const vec3 &p0 = mesh -> vertices[v[0]];
    const vec3 &p1 = mesh -> vertices[v[1]];
    const vec3 &p2 = mesh -> vertices[v[2]];
            
    output_box = expand(Aabb(p0, p1), p2);
    return true;

};

#endif