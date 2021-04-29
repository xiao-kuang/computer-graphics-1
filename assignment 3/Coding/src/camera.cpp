#include <utility>
#include "camera.hpp"
#include "constant.hpp"
#include "utils.hpp"


Camera::Camera(const Eigen::Vector3f& pos, float focal_len, float v_fov, const Eigen::Vector2i& film_res)
    : film(film_res)
    , vertical_fov(v_fov)
    , position(pos)
    , focal_length(focal_len)
    , forward(0, 0, 1)
    , right(1, 0, 0)
    , up(0, 1, 0)   
{
}

void Camera::lookAt(const Eigen::Vector3f& look_at, const Eigen::Vector3f& ref_up)
{
    //// TODO;cd
    std::cout << position<<"            sdsdsd";
    forward = -(look_at - position).normalized();
    right = forward.cross(ref_up).normalized();
    up = forward.cross(right).normalized();


std::cout << forward<<"   \n\n";
std::cout << right<<"   \n\n";
std::cout << up<<"   \n\n";

    return;
}

Ray Camera::generateRay(float dx, float dy) const
{
    
    Eigen::Vector3f ori;
    Eigen::Vector3f dir;
    
    float camera_x = -dx  * tan(vertical_fov / 2 * PI / 180) * film.getAspectRatio();
    float camera_y =  dy * tan(vertical_fov / 2 * PI / 180);
    float camera_z = -focal_length; 
  //  std::cout << tan(vertical_fov / 2 * PI / 180)<<"\n";
    Eigen::Vector4f film_coordinate(camera_x, camera_y, camera_z, 1.0f);
    Eigen::Matrix4f lookat, lookAt1, lookAt2;

    lookat.col(0) = Eigen::Vector4f(right[0], right[1], right[2], 0);
    lookat.col(1) = Eigen::Vector4f(up[0], up[1], up[2], 0);
    lookat.col(2) = Eigen::Vector4f(forward[0], forward[1], forward[2], 0);
    lookat.col(3) = Eigen::Vector4f(position[0], position[1], position[2], 1.0f);

    Eigen::Vector4f world_coordinate = lookat * film_coordinate;
    Eigen::Vector3f final_coordinate = Eigen::Vector3f(world_coordinate[0], world_coordinate[1], world_coordinate[2]);
    ori = position;
    dir = (final_coordinate - position).normalized();
    
    Ray ray = Ray(ori, dir);
    return ray;
}

void Camera::setPixel(int dx, int dy, Eigen::Vector3f value)
{
    film.pixel_array[dy * film.resolution.x() + dx] = value;
}

const Film& Camera::getFilm() const
{
    return film;
}