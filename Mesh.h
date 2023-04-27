#ifndef MESH_H
#define MESH_H

#include "rtcapstone.h"
#include "hittable.h"
#include "Triangle.h"


class Mesh : public Hittable {
    public:
        vector<vec3> vertices;
        vector<int> indices;
        vector<vec3> normals;
        vector<shared_ptr<Triangle>> triangles;
        Mesh() {};
        Mesh(vector<vec3>& v, vector<int>& i, vector<vec3>& n) {
            numP = v.size();
            numT = (i.size()) / 3;
            vertices = v;
            indices = i;
            normals = n;
        };
        Mesh(string s){
            readObj(s);
        };
        Mesh(mat4x4 t, string s){
            readObj(t, s);
        }

        virtual bool hit(
            const Ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(
            Aabb& output_box) const override;

    private:
        int numT, numP, numN;

        void readObj(string& s){
            
            ifstream oFile(s);
            string sline;
            int numVn, numV, numF;
            numV = numVn = numF = 0;

            //get line number
            getline(oFile, sline);
            while(getline(oFile, sline)){
                if(sline[0] == 'v'){
                    if(sline[1] == 'n'){
                        numVn++;
                    }else{
                        numV++;
                    }
                } 

                // triangles only
                if(sline[0] == 'f'){
                    numF++;
                }
            }
            oFile.close();
            numT = numF;
            numP = numV;
            numN = numVn;

            ifstream oFile1(s);
            sline.clear();

            int fCount = 0;
            auto tm = make_shared<TriangleMesh>(&vertices[0], &indices[0], &normals[0], numP, numT);

            while(getline(oFile1, sline)){
                string type;
                if(sline[0] == 'v'){
                    if(sline[1] == 'n'){
                        istringstream ins(sline);
                        float x, y, z;
                        ins >> type >> x >> y >> z;
                        normals.push_back(vec3(x, y, z));
                    }
                    else{
                        istringstream ins(sline);
                        float x, y, z;
                        ins >> type >> x >> y >> z;
                        vertices.push_back(vec3(x, -1.0f*y, z - 600.0f));
                    }
                }

                int l = 1;
                if(sline[0] == 'f'){
                    istringstream ins(sline);
                    ins >> type;

                    int v1, v2, v3;
                    ins >> v1 >> v2 >> v3;
                    indices.push_back(v1 - 1);
                    indices.push_back(v2 - 1);
                    indices.push_back(v3 - 1);

                    int a;
                    string p;
                    /*
                    for(int i = 0; i < 3; i++){
                        a = 0;
                        ins >> p;
                        int k;
                        for(k = 0; p[k] != '/' || l == 0 ; k++){
                            a = a * 10 + (p[k] - 48);
                            l--;
                        }
                        cout << a;
                        indices.push_back(a);

                        
                        a = 0;
                        for(k = k + 1; p[k] != '/'; k++){
                            a = a * 10 + (p[k] - 48);
                        }

                        indices.push_back(a);

                        a = 0;
                        for(k = k + 1; p[k] != '/'; k++){
                            a = a * 10 + (p[k] - 48);
                        }
                        
                        indices.push_back(a);
                        cout << "hi";
                    
                    }
                    */


                    //triangles.push_back(new Triangle(tm, fCount));
                    fCount++;
                }
            }
            oFile1.close();
            
            tm -> vertices = vertices;
            
            tm -> indices = indices;
            

            for(int i = 0; i < fCount; i++){
                triangles.push_back(make_shared<Triangle>(tm, i));
            }
            
            generateNormals();

            tm -> normals = normals;
            
        };

        void readObj(mat4x4 t, string& s){
            
            ifstream oFile(s);
            string sline;
            int numVn, numV, numF;
            numV = numVn = numF = 0;

            //get line number
            getline(oFile, sline);
            while(getline(oFile, sline)){
                if(sline[0] == 'v'){
                    if(sline[1] == 'n'){
                        numVn++;
                    }else{
                        numV++;
                    }
                } 

                // triangles only
                if(sline[0] == 'f'){
                    numF++;
                }
            }
            oFile.close();
            numT = numF;
            numP = numV;
            numN = numVn;

            ifstream oFile1(s);
            sline.clear();

            int fCount = 0;
            auto tm = make_shared<TriangleMesh>(&vertices[0], &indices[0], &normals[0], numP, numT);

            while(getline(oFile1, sline)){
                string type;
                if(sline[0] == 'v'){
                    if(sline[1] == 'n'){
                        istringstream ins(sline);
                        float x, y, z;
                        ins >> type >> x >> y >> z;
                        normals.push_back(vec3(x, y, z));
                    }
                    else{
                        istringstream ins(sline);
                        float x, y, z;
                        ins >> type >> x >> y >> z;
                        vertices.push_back(vec3(vec4(x, y, z, 1.0f) * t));
                    }
                }

                int l = 1;
                if(sline[0] == 'f'){
                    istringstream ins(sline);
                    ins >> type;

                    int v1, v2, v3;
                    ins >> v1 >> v2 >> v3;
                    indices.push_back(v1 - 1);
                    indices.push_back(v2 - 1);
                    indices.push_back(v3 - 1);

                    int a;
                    string p;
                    
                    fCount++;
                }
            }
            oFile1.close();
            
            tm -> vertices = vertices;
            
            tm -> indices = indices;
            

            for(int i = 0; i < fCount; i++){
                triangles.push_back(make_shared<Triangle>(tm, i));
            }
            
            generateNormals();

            tm -> normals = normals;
            
        };

        void generateNormals(){
            normals = vector<vec3>(numP, vec3(0,0,0));

            //vector<int> fCount = vector<int>(numP, 0);
            for (const auto& tri : triangles){
                vec3 p0 = vertices[tri->v0()];
                vec3 p1 = vertices[tri->v1()];
                vec3 p2 = vertices[tri->v2()];

                vec3 e1 = p1 - p0;
                vec3 e2 = p2 - p0;

                vec3 n = normalize(cross(e1, e2));
                normals[tri->v0()] += n;
                normals[tri->v1()] += n;
                normals[tri->v2()] += n;
                //fCount[tri->v0()]++;
                //fCount[tri->v1()]++;
                //fCount[tri->v2()]++;
            }

            for (int i = 0; i < normals.size(); i++){
                normals[i] = normalize(normals[i]);
            }
        }


};

bool Mesh::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;
    //cout << triangles.size();
    for (const auto& tri : triangles) {
        if (tri->intersect(r, closest_so_far, t_min, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool Mesh::bounding_box(Aabb& output_box) const {
    
    if (triangles.size() == 0) return false;
    Aabb temp_box;
    bool first_box = true;

    for (const auto& tri : triangles) {
        if (!tri->bounding_box(temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

#endif