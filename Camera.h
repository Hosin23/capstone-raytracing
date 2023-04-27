#ifndef CAMERA_H
#define CAMERA_H


class Camera 
{
    public:
        Camera(
            
        ) {
            auto aspect_ratio = 16.0 / 9.0;
            auto viewport_height = 2.0;
            auto viewport_width = aspect_ratio * viewport_height;
            auto focal_length = 1.0;

            origin = vec3(0, 0, 0);
            horizontal = vec3(viewport_width, 0.0, 0.0);
            vertical = vec3(0.0, viewport_height, 0.0);
            lower_left_corner = origin - horizontal/2.0f - vertical/2.0f - vec3(0, 0, focal_length);
        }

        Ray get_ray(float u, float v) const {
            return Ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
        }

    private:
        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};

#endif