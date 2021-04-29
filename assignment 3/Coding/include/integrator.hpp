#pragma once
#include "scene.hpp"
#include "camera.hpp"
#include "interaction.hpp"


/**
 * Base class of integrator
 */

class Integrator
{
protected:

	Scene* scene;
	Camera* camera;
	

public:
	Integrator(Scene* scn, Camera* cam);
	virtual ~Integrator() = default;
	
	virtual void render() = 0;
	virtual Eigen::Vector3f radiance(const Interaction& interaction, const Ray& ray) const = 0;
};


/**
 * Phong lighting integrator
 */

class PhongLightingIntegrator : public Integrator
{
public:

	PhongLightingIntegrator(Scene* scn, Camera* cam);
	virtual void render() override;
	virtual Eigen::Vector3f radiance(const Interaction& interaction, const Ray& ray) const override;
};
