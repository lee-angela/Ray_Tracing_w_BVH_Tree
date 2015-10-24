#include <raytracing/samplers/randompixelsampler.h>

RandomPixelSampler::RandomPixelSampler():PixelSampler()
{}

RandomPixelSampler::RandomPixelSampler(int samples):PixelSampler(samples)
{}

QList<glm::vec2> RandomPixelSampler::GetSamples(int x, int y)
{
    //desired pixel is from x to (x+1)
    //                  and y to (y+1)
    QList<glm::vec2> result;
    for (int i = 0; i < samples_sqrt*samples_sqrt; i++) {
        float Xrand = x + rand()%2; //add random number between 0 and 1 to the x and y coords
        float Yrand = y + rand()%2;
        result.push_back(glm::vec2(Xrand, Yrand));
    }
    return result;
}
