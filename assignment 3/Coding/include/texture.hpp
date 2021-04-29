#pragma once
#include <string>
#include "Eigen/Dense"


/**
 * Texture objects
 */

class Texture
{
protected:
    /* An array with width x height x 3 elements
       Data arranged in r0, g0, b0, r1, g1, b1, ..., ri, gi, bi, ...
       (0, 0) is located at the left-top corner, (1, 1) is located at the right-bottom corner */
    float* data;
    unsigned int width;
    unsigned int height;

public:
    Texture();
    bool isNull() const;
    Eigen::Vector2i getDimension() const;
    Eigen::Vector3f getPixel(float u, float v) const;
    void clear();
    virtual ~Texture();

    static Texture loadFromPath(std::string path);
    static Texture generateCheckerboard(Eigen::Vector3f primary_rgb, Eigen::Vector3f secondary_rgb, unsigned int num_ckers=4);
};
