#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "utils.hpp"


Texture::Texture()
    : data(nullptr)
    , width(0)
    , height(0)
{
}

bool Texture::isNull() const
{
    return data == nullptr || width == 0 || height == 0;
}

Eigen::Vector2i Texture::getDimension() const
{
    return Eigen::Vector2i(width, height);
}

Eigen::Vector3f Texture::getPixel(float u, float v) const
{
   // std::cout << height <<" "<<  width <<" \ "<<"--";
    u = clamp(u, 0, 1);
    v = clamp(v, 0, 1);

    //std::cout << u<< ' '<<v<<" "<< (int (u * height)+1) * width+(int(v * width)+ 1) * 3  <<"  =---  ";
    return Eigen::Vector3f(\
    data[(int (u * height) + (int(v * width))* height )* 3  ],\
    data[(int (u * height) + (int(v * width)) * height) * 3  + 1],\
    data[(int (u * height)  +(int(v * width))* height) * 3  + 2]);
    
}

void Texture::clear()
{
    width = 0;
    height = 0;
    delete data;
    data = nullptr;
}
    
Texture::~Texture()
{
    clear();
}

Texture Texture::loadFromPath(std::string path)
{
    int w, h, n;
    unsigned char* raw_data = stbi_load(path.c_str(), &w, &h, &n, 0);

    Texture tex;
    
    /* Only images with 1 or 3 channels are supported */
    if (raw_data && (n == 3 || n == 1))
    {
        tex.data = new float[w * h * 3];
        tex.width = w;
        tex.height = h;

        int ch_stride = n == 3 ? 1 : 0;
        for (int i = 0; i < w * h * n; i += n)
        {
            tex.data[i] = (float)raw_data[i] / 255.0f;
            tex.data[i + 1] = (float)raw_data[i + ch_stride] / 255.0f;
            tex.data[i + 2] = (float)raw_data[i + ch_stride * 2] / 255.0f;
        }

        stbi_image_free(raw_data);
    }

    return tex;
}

Texture Texture::generateCheckerboard(Eigen::Vector3f primary_rgb, Eigen::Vector3f secondary_rgb, unsigned int num_ckers)
{
    int checker_size = 32;
    int w = num_ckers * checker_size, h = num_ckers * checker_size;

    Eigen::Vector3f color_list[2];
    color_list[0] = primary_rgb;
    color_list[1] = secondary_rgb;

    Texture tex;
    tex.width = w;
    tex.height = h;
    tex.data = new float[tex.width * tex.height * 3];

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            Eigen::Vector3f color = color_list[((int)(x / checker_size) + (int)(y / checker_size)) % 2];
            tex.data[(y * w + x) * 3] = color[0];
            tex.data[(y * w + x) * 3 + 1] = color[1];
            tex.data[(y * w + x) * 3 + 2] = color[2];
        }
    }

    return tex;
}
