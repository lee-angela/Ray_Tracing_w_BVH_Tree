
#pragma once
#include <raytracing/samplers/pixelsampler.h>
#include <random>

class ImageWideStratifiedPixelSampler : public PixelSampler
{
public:
    ImageWideStratifiedPixelSampler();
    ImageWideStratifiedPixelSampler(int samples);
    virtual QList<glm::vec2> GetSamples(int x, int y);

    QList<glm::vec2> pickedSamples; //stores pixels as they are picked so that they can be compared to

protected:
    std::mt19937 mersenne_generator;
    std::uniform_real_distribution<float> unif_distribution;
};



