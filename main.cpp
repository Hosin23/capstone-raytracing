#include "rtcapstone.h"

#include "image.h"
#include "hittable_list.h"
#include "sphere.h"
#include "Camera.h"
#include "Mesh.h"
#include "Bvh_node.h"

using namespace glm;

using std::shared_ptr;
using std::make_shared;


vec3 random_in_unit_sphere(){
    while (true) {
        auto p = random(-1,1);
        if ((length(p) * length(p)) >= 1) continue;
        return p;
    }
}

vec3 ray_color(const Ray& r, const Hittable& world, int depth) {
    hit_record rec;

    if (depth <= 0)
    return vec3(0,0,0);

    if( world.hit(r, 0, INFINITY, rec) ){

        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5f * ray_color(Ray(rec.p, target - rec.p), world, (depth - 1));

        //return 0.5f * (rec.normal + vec3(1, 1, 1));
    }

    vec3 unit_direction = normalize(r.getDirection());
    auto t = 0.5f*(unit_direction.y + 1.0f);
    return (1.0f-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

vec3 ray_color(const Ray& r, const Hittable& world) {
    hit_record rec;

    if( world.hit(r, 0, INFINITY, rec) ){
        return 0.5f * (rec.normal + vec3(1.0,1.0,1.0));
    }

    vec3 unit_direction = normalize(r.getDirection());
    auto t = 0.5f*(unit_direction.y + 1.0f);
    return (1.0f-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

float ambient_occlusion(const Hittable& world, const vec3& p, const vec3& normal){
    int num_samples = 16;
    int num_occluded = 0;
    hit_record rec;

    for(int i = 0; i < num_samples; i++) {
        vec3 random_dir = normalize(random_in_unit_sphere());
        if (dot(random_dir, normal) < 0){
            random_dir = -random_dir;
        }
        Ray random_ray(p + 0.001f * normal, random_dir);
        bool hit = world.hit(random_ray, 0, INFINITY, rec);
        if(hit){
            num_occluded++;
        } 
    }
    
    return 1.0f - static_cast<float>(num_occluded) / num_samples;
}

vec3 ray_color_ao(const Ray& r, const Hittable& world) {
    hit_record rec;

    if( world.hit(r, 0, INFINITY, rec) ){
        float ao = ambient_occlusion(world, rec.p, rec.normal);
        return vec3(1.0,1.0,1.0) * ao;
    }

    vec3 unit_direction = normalize(r.getDirection());
    auto t = 0.5f*(unit_direction.y + 1.0f);
    return (1.0f-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    const int max_depth = 50;

    Image img(image_height, image_width);

    // World
    hittable_list world;
    //world.add(make_shared<Sphere>(vec3(0,0,-1), 0.5f));
    //world.add(make_shared<Sphere>(vec3(0,-100.5,-1), 100.0f));
    //world.add(make_shared<Mesh>("data/box.obj"));
    //world.add(make_shared<Mesh>("data/KleinBox/KleinBox_triangulated.obj"));
    //world.add(make_shared<Mesh>("data/bunny_high.obj"));

    mat4x4 m = mat4x4(1.f, 0.f, 0.f, 0.f, 
                        0.f, cosf(pi/2.0), sinf(pi/2.0), 0.f, 
                        0.f, -1.f*sinf(pi/2.0), cosf(pi/2.0), -500.f, 
                        0.f, 0.f, 0.f, 1.f);
    mat4x4 m1 = mat4x4(1.f, 0.f, 0.f, 300.f, 
                        0.f, cosf(pi/2.0), sinf(pi/2.0), 0.f, 
                        0.f, -1.f*sinf(pi/2.0), cosf(pi/2.0), -600.f, 
                        0.f, 0.f, 0.f, 1.f);
    mat4x4 m2 = mat4x4(1.f, 0.f, 0.f, 600.f, 
                        0.f, cosf(pi/2.0), sinf(pi/2.0), 0.f, 
                        0.f, -1.f*sinf(pi/2.0), cosf(pi/2.0), -700.f, 
                        0.f, 0.f, 0.f, 1.f);
    mat4x4 m3 = mat4x4(1.f, 0.f, 0.f, -300.f, 
                        0.f, cosf(pi/2.0), sinf(pi/2.0), 0.f, 
                        0.f, -1.f*sinf(pi/2.0), cosf(pi/2.0), -600.f, 
                        0.f, 0.f, 0.f, 1.f);
    mat4x4 m4 = mat4x4(1.f, 0.f, 0.f, -600.f, 
                        0.f, cosf(pi/2.0), sinf(pi/2.0), 0.f, 
                        0.f, -1.f*sinf(pi/2.0), cosf(pi/2.0), -700.f, 
                        0.f, 0.f, 0.f, 1.f);
    //world.add(make_shared<Mesh>(m, "data/Nefertiti.obj"));
    //world.add(make_shared<Mesh>(m1, "data/Nefertiti.obj"));
    //world.add(make_shared<Mesh>(m2, "data/Nefertiti.obj"));
    //world.add(make_shared<Mesh>(m3, "data/Nefertiti.obj"));
    //world.add(make_shared<Mesh>(m4, "data/Nefertiti.obj"));

    world.add(make_shared<Mesh>(mat4x4(20.f, 0.f, 0.f, -13.f, 
                                        0.f, 20.f, 0.f, -3.f, 
                                        0.f, 0.f, 20.f, -5.f, 
                                        0.f, 0.f, 0.f, 1.f), "data/city/city_triangulated.obj"));
    world.add(make_shared<Mesh>(mat4x4(20.f, 0.f, 0.f, -13.f, 
                                        0.f, 20.f, 0.f, -3.f, 
                                        0.f, 0.f, 20.f, -5.f, 
                                        0.f, 0.f, 0.f, 1.f), "data/city/groundplane.obj"));                                

    hittable_list final_world;
    final_world.add(make_shared<Bvh_node>(world));
    cout << "Bvh constucted" << endl;


    // Camera

    Camera cam;

    for (int j = image_height-1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {

            vec3 color(0,0,0);

            for(int s = 0; s < samples_per_pixel; ++s){
                auto u = float(i + random_float()) / (image_width - 1);
                auto v = float(j + random_float()) / (image_height - 1);
                //auto u = float(i) / (image_width - 1);
                //auto v = float(j) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                color += ray_color_ao(r, final_world);
            }
            

            auto r = color.x;
            auto g = color.y;
            auto b = color.z;

            auto scale = 1.0f / samples_per_pixel;
            r *= scale;
            g *= scale;
            b *= scale;

            img.setData(i, j, 0, color.x);
            img.setData(i, j, 1, color.y);
            img.setData(i, j, 2, color.z);
            img.setData(i, j, 3, 1.0);
            //img.setData(i, j, 0, clamp(color.x, 0.0f, 0.999f));
            //img.setData(i, j, 1, clamp(color.y, 0.0f, 0.999f));
            //img.setData(i, j, 2, clamp(color.z, 0.0f, 0.999f));
            //img.setData(i, j, 3, 1.0);
        }
    }

    img.writeToHdr();
    return 0;
}

