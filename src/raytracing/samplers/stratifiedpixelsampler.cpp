#include <raytracing/samplers/stratifiedpixelsampler.h>
#include <iostream>
#include <functional>

StratifiedPixelSampler::StratifiedPixelSampler():PixelSampler(){}

StratifiedPixelSampler::StratifiedPixelSampler(int samples) : PixelSampler(samples), mersenne_generator(), unif_distribution(0,1){}

QList<glm::vec2> StratifiedPixelSampler::GetSamples(int x, int y)
{
    //makes NxN grid and samples randomly once in each grid space

    //pixel is from x to (x+1)
    //          and y to (y+1)

    QList<glm::vec2> result;

    for (int i = 0; i < samples_sqrt; i++) {
        for (int j = 0; j < samples_sqrt; j++) {
            float Xcoord = x + i/samples_sqrt + rand()%2;
            float Ycoord = y + j/samples_sqrt + rand()%2;
            result.push_back(glm::vec2(Xcoord, Ycoord));
        }
    }
    return result;
}
