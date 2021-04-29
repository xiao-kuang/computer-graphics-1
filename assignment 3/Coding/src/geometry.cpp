#include <math.h>
#include "geometry.hpp"
#include "constant.hpp"
#include "utils.hpp"


/**
 * Geometry class
 */

void Geometry::setMaterial(Material* new_mat)
{
    material = new_mat;
}


/**
 * Parallelogram class
 */

Parallelogram::Parallelogram(Eigen::Vector3f p0, Eigen::Vector3f s0, Eigen::Vector3f s1, Eigen::Vector3f normal, Material* mat)
    : p0(p0)
    , normal(normal.normalized())
{
    s0_len = s0.norm();
    s1_len = s1.norm();
    this->s0 = s0.normalized();
    this->s1 = s1.normalized();

    setMaterial(mat);
}

bool Parallelogram::rayIntersection(Interaction& interaction, const Ray& ray) const
{
    if (ray.direction.dot(normal) == 0)
    {
        return false;
    }
    
    float t = (p0 - ray.origin).dot(normal) / ray.direction.dot(normal);
    Eigen::Vector3f p0_p = ray.getPoint(t) - p0;
    float q0 = p0_p.dot(s0) / s0_len;
    float q1 = p0_p.dot(s1) / s1_len;
    if (q0 >= 0 && q0 <= s0.norm() && q1 >= 0 && q1 <= s1.norm() && t >= ray.range_min && t <= ray.range_max)
    {
        interaction.entry_dist = t;
        interaction.exit_dist = t;
        interaction.normal = normal;
        interaction.entry_point = ray.getPoint(t);
        interaction.uv[0] = q0;
        interaction.uv[1] = q1;
        interaction.if_ball = false;
        if (material != nullptr)
        {
            //td::cout<<"dsdsds"<< interaction.entry_point;
            interaction.lighting_model = material->evaluate(interaction);
            //std::cout<< interaction.lighting_model.diffusion<<std::endl;
        }
        interaction.type = Interaction::Type::GEOMETRY;
        return true;
    }
    return false;
}


/**
 * Sphere class
 */

Sphere::Sphere(Eigen::Vector3f p0, float r, Material* mat)
    : p0(p0)
    , radius(r)
{
    setMaterial(mat);
}

bool Sphere::rayIntersection(Interaction& interaction, const Ray& ray) const
{
    //std::cout << (ray.origin - p0).dot(ray.origin - p0)<<" ";
    Eigen::Vector3f L = ray.origin - p0;
    float a = ray.direction.dot(ray.direction);
    float b = 2 * ray.direction.dot(L);
    float c = L.dot(L) - radius * radius;
    float delta = b * b - 4 * a * c;
   
    
    if(delta <= 0)
        return false;
    //std::cout << delta<< " ";
    float t1 = (-b - sqrt(delta)) / 2 * a;
    float t2 = (-b + sqrt(delta)) / 2 * a;
   
    if(t1 < ray.range_min && t2 < ray.range_min )
        return false;
    if(t1 < ray.range_min && t2 >= ray.range_min && t2 <= ray.range_max ){
        interaction.entry_dist = t2;
       // interaction.exit_dist = t;
      
        interaction.entry_point = ray.getPoint(t2);
         interaction.normal = (interaction.entry_point - p0).normalized();

         float x = interaction.entry_point[0] - p0[0];
        float z = interaction.entry_point[2] - p0[2];
      //  std::cout<<x<<" "<<y;
        
        

        if(x == 0 && z < 0)
            interaction.uv[0] = 0.5;
        else if(x == 0 && z > 0)
            interaction.uv[0] = 0.0;
        else{
            interaction.uv[0] = atan(z / x)/ ( PI);
            if(x > 0 && z < 0)
                interaction.uv[0] += 0.5;
            if(x < 0 && z < 0)
                interaction.uv[0] += 0.5;
            if(x < 0 && z > 0)
                interaction.uv[0] += 1;

        }
      // if(int(log2((interaction.entry_point - ray.origin).norm()  + 1)) <= 0 )
       //std::cout << int(log2((interaction.entry_point - ray.origin).norm() + 1))<<" ";
        interaction.uv[1] = -(acos( (interaction.entry_point[1] - p0[1]) / radius)- PI ) / PI;
       if (material != nullptr)
        {
            
            interaction.lighting_model = material->evaluate(interaction);
        }
        interaction.type = Interaction::Type::GEOMETRY;
        interaction.if_ball = true;
        return true;
    }
     if(t1 >= ray.range_min && t1 <= ray.range_max ){
        interaction.entry_dist = t1;
        if(t2 <= ray.range_max)
            interaction.exit_dist = t2;
       
        interaction.entry_point = ray.getPoint(t1);
         interaction.normal = (interaction.entry_point - p0).normalized();
        float x = interaction.entry_point[0] - p0[0];
        float z = interaction.entry_point[2] - p0[2];
      //  std::cout<<x<<" "<<y;
        
        

        if(x == 0 && z < 0)
            interaction.uv[0] = 0.5;
        else if(x == 0 && z > 0)
            interaction.uv[0] = 0.0;
        else{
            interaction.uv[0] = atan(z / x)/ ( PI);
            if(x > 0 && z < 0)
                interaction.uv[0] += 0.5;
            if(x < 0 && z < 0)
                interaction.uv[0] += 0.5;
            if(x < 0 && z > 0)
                interaction.uv[0] += 1;

        }
      // if(int(log2((interaction.entry_point - ray.origin).norm()  + 1)) <= 0 )
       //std::cout << int(log2((interaction.entry_point - ray.origin).norm() + 1))<<" ";
        interaction.uv[1] = -(acos( (interaction.entry_point[1] - p0[1]) / radius)- PI ) / PI;
        //std::cout<<"  "<< interaction.uv[1]<<"dd";
       // interaction.uv[1] = (interaction.entry_point[1] - p0[1] + radius)/ 2 /  radius;
        if (material != nullptr)
        {
            interaction.lighting_model = material->evaluate(interaction);
        }
        interaction.type = Interaction::Type::GEOMETRY;
        interaction.if_ball = true;
        
        return true;
    }
    return false;
    //// TODO
    UNREACHABLE;
}