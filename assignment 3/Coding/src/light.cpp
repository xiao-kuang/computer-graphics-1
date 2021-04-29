#include "light.hpp"
#include "geometry.hpp"
#include "utils.hpp"


/**
 * Light class
 */

Light::Light(Eigen::Vector3f pos, Eigen::Vector3f rgb)
    : position(pos)
    , color(rgb)
{
}

Eigen::Vector3f Light::getPosition() const
{
    return position;
}

Eigen::Vector3f Light::getColor() const
{
    return color;
}



/**
 * PointLight class
 */

PointLight::PointLight(Eigen::Vector3f pos, Eigen::Vector3f rgb)
    : Light(pos, rgb)
{
}

std::vector<LightSamplePair> PointLight::samples() const
{
    std::vector<LightSamplePair> ls;
    ls.push_back(LightSamplePair(position, color));
    return ls;
}

bool PointLight::rayIntersection(Interaction& interaction, const Ray& ray) const
{
    Eigen::Vector3f dist = position - ray.origin;
    float t = dist.norm();
    if (ray.direction == dist.normalized() && t >= ray.range_min && t <= ray.range_max)
    {
        interaction.entry_dist = t;
        interaction.exit_dist = t;
        interaction.entry_point = ray.getPoint(t);
        interaction.type = Interaction::Type::LIGHT;
        return true;
    }
    return false;
}


/**
 * AreaLight class
 */
AreaLight::AreaLight(Eigen::Vector3f pos, Eigen::Vector3f rgb, Eigen::Vector2f size)
    : Light(pos, rgb)
    , area_size(size)
    , geometry_delegation(
        pos - Eigen::Vector3f(size[0], 0, size[1]) / 2,
        Eigen::Vector3f(size[0], 0, 0),
        Eigen::Vector3f(0, 0, size[1]),
        Eigen::Vector3f(0, 1, 0), nullptr)
{
}

std::vector<LightSamplePair> AreaLight::samples() const
{
    std::vector<LightSamplePair> ls;
    Eigen::Vector3f pos_initial = this -> position ;
    pos_initial[0] -= area_size[0] / 2; 
    pos_initial[2] -= area_size[1] / 2; 
    for(int i = 0 ; i < 6 ; i++){
        for(int j = 0; j < 6 ; j++){
            
        
          // std::cout << float(i) / 6 * area_size[0] <<"    "<< float(j) / 6 * area_size[1]<<"s-------";
            Eigen::Vector3f pos = pos_initial + Eigen::Vector3f(float(i) / 6 * area_size[0] , 0,float(j) / 6 * area_size[1]);
            ls.push_back(LightSamplePair(pos, color));
        }
    }    
    //std::cout << area_size[0]<<"  " << area_size[1]<<"    ";
    return ls; 
}

bool AreaLight::rayIntersection(Interaction& interaction, const Ray& ray) const
{
    interaction.type = Interaction::Type::LIGHT;

    bool intersection = geometry_delegation.rayIntersection(interaction, ray);
    interaction.type = Interaction::Type::LIGHT;
    return intersection;
}


/**
 * CubeLight class
 */
CubeLight::CubeLight(Eigen::Vector3f pos, Eigen::Vector3f rgb, Eigen::Vector3f size,
	Material* tex_front ,
	Material* tex_back ,
	Material* tex_left ,
	Material* tex_right ,
	Material* tex_top ,
	Material* tex_bottom  )
    : Light(pos, rgb )
    , area_size(size),
    geometry_delegation1 (
        pos + Eigen::Vector3f(size[0], size[1], size[2]) / 2,
        Eigen::Vector3f(-size[0], 0, 0),
        Eigen::Vector3f(0, 0, -size[2]),
        Eigen::Vector3f(0, -1, 0), tex_front)

{
}


	

std::vector<LightSamplePair> CubeLight::samples() const
{
    

    std::vector<LightSamplePair> ls;
    Eigen::Vector3f pos_initial = this -> position ;
    pos_initial[0] -= area_size[0] / 2;
    pos_initial[1] -= area_size[1] / 2; 
    pos_initial[2] -= area_size[2] / 2; 
    for(int i = 0 ; i < 6 ; i++){
        for(int j = 0; j < 6 ; j++){
           
          // std::cout << float(i) / 6 * area_size[0] <<"    "<< float(j) / 6 * area_size[1]<<"s-------";
            Eigen::Vector3f pos = pos_initial + Eigen::Vector3f(float(i) / 6 * area_size[0] ,-2 ,float(j) / 6 * area_size[2]);
            ls.push_back(LightSamplePair(pos, color));
              
        }
    }    
    
    //std::cout << area_size[0]<<"  " << area_size[1]<<"    ";
    return ls; 
}


bool CubeLight::rayIntersection(Interaction& interaction, const Ray& ray) const
{
    interaction.type = Interaction::Type::LIGHT;
    
 
    bool intersection = false;
    
 //interaction.lighting_model.
    interaction.type = Interaction::Type::LIGHT;
    return intersection;
}


