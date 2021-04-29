#pragma once
#include <Eigen/Dense>
#include <utility>
#include <vector>
#include "ray.hpp"
#include "interaction.hpp"
#include "geometry.hpp"



typedef std::pair<Eigen::Vector3f, Eigen::Vector3f> LightSamplePair;


/**
 * Base class of lights
 */

class Light
{
protected:
	Eigen::Vector3f position;
	Eigen::Vector3f color;

public:
	Light(Eigen::Vector3f pos, Eigen::Vector3f rgb);
	Eigen::Vector3f getPosition() const;
	Eigen::Vector3f getColor() const;
	void load_texture();
	Texture get_tex() const;
	virtual std::vector<LightSamplePair> samples() const = 0;
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) const = 0;
};


/**
 * Point lights
 */

class PointLight : public Light
{
public:
	PointLight(Eigen::Vector3f pos, Eigen::Vector3f rgb);
	virtual std::vector<LightSamplePair> samples() const override;
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) const override;
};


/**
 * Area lights
 */
class AreaLight : public Light
{
protected:
	Parallelogram geometry_delegation;
	Eigen::Vector2f area_size;

public:
	AreaLight(Eigen::Vector3f pos, Eigen::Vector3f rgb, Eigen::Vector2f size);
	virtual std::vector<LightSamplePair> samples() const override;
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) const override;
};



/**
 * Cube lights
 */
class CubeLight : public Light
{
protected:
	Parallelogram geometry_delegation1;

	Eigen::Vector3f area_size;

public:

	
	CubeLight(Eigen::Vector3f pos, Eigen::Vector3f rgb, Eigen::Vector3f size,
	Material* tex_front ,
	Material* tex_back ,
	Material* tex_left ,
	Material* tex_right ,
	Material* tex_top ,
	Material* tex_bottom 
	);
	
	virtual std::vector<LightSamplePair> samples() const override;
	virtual bool rayIntersection(Interaction& interaction, const Ray& ray) const override;
};
