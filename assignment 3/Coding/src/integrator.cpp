#ifndef NO_OMP
#include <omp.h>
#endif
#include "integrator.hpp"
#include "material.hpp"
#include "constant.hpp"
#include "light.hpp"
#include "utils.hpp"
#include "config.hpp"
#include <algorithm>
#include<math.h>
extern Config conf;


/**
 * Integrator class
 */

Integrator::Integrator(Scene* scn, Camera* cam)
    : scene(scn)
    , camera(cam)
{
}


/**
 * PhongLightingIntegrator class
 */

PhongLightingIntegrator::PhongLightingIntegrator(Scene* scn, Camera* cam)
    : Integrator(scn, cam)
{
}

void PhongLightingIntegrator::render()
{
    int dx, dy,vague_num = 0;

 float xx[3], yy[3];
    for(int i = 0; i < 3; i++){
        xx[i] = uniform_sample(0.0f,1.0f);
        yy[i] = uniform_sample(0.0f,1.0f);

    }
#ifndef NO_OMP
    #pragma omp parallel for private(dy)
#endif
    
    
    for (dx = 0; dx < camera->getFilm().resolution.x(); ++dx)
    {
        
        for (dy = 0; dy < camera->getFilm().resolution.y(); ++dy)
        {   
                
                
                
                Eigen::Vector3f cloor = Eigen::Vector3f(0,0,0); 
                for(int num = 0; num < conf.num_samples; num++){
                   // std::cout << dx<<" "<< dy<<" ---- ";
                     float x  = 2 * (float(dx) + xx[num])/ camera->getFilm().resolution.x() - 1;
                     float y = 1 - 2 * (float(dy) + yy[num])/ camera->getFilm().resolution.y();
                     Ray ray = camera->generateRay(float(x) , float(y) ); 
                    Interaction interaction;
                    
                    if(scene->intersection(ray, interaction) == true ){
                        cloor += radiance(interaction, ray);           
                
                          
                    }    
                        camera->setPixel(dx,dy,cloor/4);
            
               
                }
            }
            
        }
    }
    
    // std::cout << camera->getFilm().resolution.x()<<" "<<camera->getFilm().resolution.x()<<std::endl;
   
            //UNREACHABLE;



Eigen::Vector3f PhongLightingIntegrator::radiance(const Interaction& interaction, const Ray& ray) const
{
    /* If the ray hits a light source, return the light color */
    if (interaction.type == Interaction::Type::LIGHT)
    {
       // std::cout << interaction.uv<<"    ----------    ";
        return scene->getLight()->getColor();
    }

    /* Ambient component */
    Eigen::Vector3f color = Eigen::Vector3f(0,0,0) ;
    for(int i = 0; i < 36; i++){
            Eigen::Vector3f light_pos =  scene->getLight()->samples()[i].first;
            Ray light_to_point(interaction.entry_point,(light_pos - interaction.entry_point).normalized(),0.001);
                      
     

        Eigen::Vector3f amb_comp = scene->getAmbientLight();
        /* Diffuse component */
        Eigen::Vector3f dif_comp = std::max(light_to_point.direction.dot(interaction.normal) , 0.0f) * scene->getLight()->getColor();
    
        // std::cout <<dif_comp<<" ";
        /* Specular component */
        Eigen::Vector3f view =  -ray.direction.normalized();
        Eigen::Vector3f reflect = 2 * ( interaction.normal.dot(light_to_point.direction) * interaction.normal ) - light_to_point.direction;
        Eigen::Vector3f H = (view + light_to_point.direction).normalized();
       // Eigen::Vector3f spe_comp=  pow(std::max(H.dot(interaction.normal.normalized()),0.0f), interaction.lighting_model.shininess ) * scene->getLight()->getColor();

        Eigen::Vector3f spe_comp=  pow(std::max(view.dot(reflect.normalized()),0.0f), interaction.lighting_model.shininess ) * scene->getLight()->getColor();
        
        //std::cout << interaction.lighting_model.shininess<<"   ";

    //-------------

    //-------------
        if(!scene->isShadowed(light_to_point))
        color += (amb_comp + dif_comp ).cwiseProduct(interaction.lighting_model.diffusion)+\
            spe_comp.cwiseProduct(interaction.lighting_model.diffusion);
       else
       color += amb_comp.cwiseProduct(interaction.lighting_model.diffusion)\
            ;
       // color += amb_comp + dif_comp.cwiseProduct(interaction.lighting_model.diffusion) + spe_comp.cwiseProduct(interaction.lighting_model.specular);
    }
    return color/36;
    
}
