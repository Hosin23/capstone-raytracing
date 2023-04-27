#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "rtcapstone.h"
#include "aabb.h"
#include "hittable_list.h"
#include "Mesh.h"

class Bvh_node : public Hittable {
    public:
        Bvh_node();

        Bvh_node(const hittable_list& list)
            : Bvh_node(list.objects)
        {}

        Bvh_node(
            const vector<shared_ptr<Hittable>>& src_objects);

        virtual bool hit(
            const Ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(
            Aabb& output_box) const override;

        string toString(){
            string s;
            s += box.centroid().x;
            s += "/";
            s += box.centroid().y;
            s += "/";
            s += box.centroid().z;
            s += "\n";
            cout << s << endl;
            if(left_child != nullptr){
                Bvh_node* left = dynamic_cast<Bvh_node*>(&*left_child);
                if (left != nullptr) {
                    s += left->toString();
                }

                if (right != nullptr) {
                    s += left->toString();
                }
            }

            return s;
        }

        Aabb box;
        shared_ptr<Hittable> left_child = nullptr;
        shared_ptr<Hittable> right_child = nullptr;


};

bool Bvh_node::bounding_box(Aabb& output_box) const {
    output_box = box;
    return true;
}

bool Bvh_node::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
    if (!box.hit(r, t_min, t_max))
        return false;

    bool hit_left = left_child->hit(r, t_min, t_max, rec);
    bool hit_right = right_child->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

bool compX(shared_ptr<Hittable> o1, shared_ptr<Hittable> o2){
    Aabb b1, b2;
    o1->bounding_box(b1);
    o2->bounding_box(b2);
    return (b1.centroid().x < b2.centroid().x);
}

bool compY(shared_ptr<Hittable> o1, shared_ptr<Hittable> o2){
    Aabb b1, b2;
    o1->bounding_box(b1);
    o2->bounding_box(b2);
    return (b1.centroid().y < b2.centroid().y);
}

bool compZ(shared_ptr<Hittable> o1, shared_ptr<Hittable> o2){
    Aabb b1, b2;
    o1->bounding_box(b1);
    o2->bounding_box(b2);
    return (b1.centroid().z < b2.centroid().z);
}

Bvh_node::Bvh_node(const vector<shared_ptr<Hittable>>& src_objects){

    vector<shared_ptr<Hittable>> objects;

    float axis_length = 0.0f;
    box = Aabb();
    bool first_box = true;
    for(auto& obj : src_objects){
        const Mesh* mesh = dynamic_cast<const Mesh*>(&*obj);
        Aabb b;

        if(mesh != nullptr){
            for(auto& tri : mesh->triangles){
                objects.push_back(tri);
                tri->bounding_box(b);
            }
        }
        else{
            objects.push_back(obj);
            obj->bounding_box(b);
        }

        box = first_box ? b : surrounding_box(box, b);
        first_box = false;
        //cout << box.centroid().y << endl;
    }

    float x_length = box.max().x - box.min().x;
    float y_length = box.max().y - box.min().y;
    float z_length = box.max().z - box.min().z;
    axis_length = std::max(x_length, std::max(y_length, z_length));

    if(axis_length == x_length){
        std::nth_element(objects.begin(), objects.begin() + objects.size()/2, objects.end(), compX);
    }
    if(axis_length == y_length){
        std::nth_element(objects.begin(), objects.begin() + objects.size()/2, objects.end(), compY);
    }
    if(axis_length == z_length){
        std::nth_element(objects.begin(), objects.begin() + objects.size()/2, objects.end(), compZ);
    }

    auto beginning = objects.begin();
    auto middling = objects.begin() + objects.size()/2;
    auto ending = objects.end();

    auto leftshapes = vector<shared_ptr<Hittable>>(beginning, middling);
    auto rightshapes = vector<shared_ptr<Hittable>>(middling, ending);

    if(leftshapes.size() <= 10){
        auto left_list = make_shared<hittable_list>();
        for(auto& items : leftshapes){
            left_list->add(items);
        }
        left_child = left_list;
    }else{
        left_child = make_shared<Bvh_node>(leftshapes);
    }

    if(rightshapes.size() <= 10){
        auto right_list = make_shared<hittable_list>();
        for(auto& items : rightshapes){
            right_list->add(items);
        }
        right_child = right_list;
    }else{
        right_child = make_shared<Bvh_node>(rightshapes);
    }

    Aabb left_box, right_box;
    left_child->bounding_box(left_box);
    right_child->bounding_box(right_box);
    box = surrounding_box(left_box, right_box);
}

#endif