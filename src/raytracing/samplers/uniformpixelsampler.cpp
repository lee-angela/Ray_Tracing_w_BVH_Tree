#include <raytracing/samplers/uniformpixelsampler.h>

UniformPixelSampler::UniformPixelSampler():PixelSampler()
{}

UniformPixelSampler::UniformPixelSampler(int samples):PixelSampler(samples)
{}

QList<glm::vec2> UniformPixelSampler::GetSamples(int x, int y)
{
    //desired pixel is from x to (x+1)
    //                  and y to (y+1)
    QList<glm::vec2> result;
    for (int i = 0; i < samples_sqrt; i++) {
        for (int j = 0; j < samples_sqrt; j++) {
            float Xcoord = x + i/samples_sqrt;
            float Ycoord = y + j/samples_sqrt;
            result.push_back(glm::vec2(Xcoord, Ycoord));
        }
    }

    return result;
}
