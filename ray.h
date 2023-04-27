#include "glm/glm.hpp"

using namespace glm;

class Ray 
{
    private:
        vec3 origin;
        vec3 direction;
        float tMax;

    public:
        Ray() {};
        Ray(const vec3& orig, const vec3& dir)
            : origin(orig), direction(dir)
        {}

        vec3 getOrigin() const { return origin; }
        vec3 getDirection() const { return direction; }

        vec3 at(float t) const {
            return origin + t*direction;
        };
};