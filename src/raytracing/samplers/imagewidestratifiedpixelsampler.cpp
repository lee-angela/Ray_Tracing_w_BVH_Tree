#include <raytracing/samplers/imagewidestratifiedpixelsampler.h>

#include <raytracing/samplers/stratifiedpixelsampler.h>
#include <iostream>
#include <functional>

ImageWideStratifiedPixelSampler::ImageWideStratifiedPixelSampler():PixelSampler(){}

ImageWideStratifiedPixelSampler::ImageWideStratifiedPixelSampler(int samples) : PixelSampler(samples), mersenne_generator(), unif_distribution(0,1){}

QList<glm::vec2> ImageWideStratifiedPixelSampler::GetSamples(int x, int y)
{

    //desired pixel is from x to (x+1)
    //                  and y to (y+1)

    //randomly sample N points in this pixel, choose the one that is farthest from the ones chosen previously
    QList<glm::vec2> randoms;
    for (int i = 0; i < samples_sqrt*samples_sqrt; i++) {
        float Xrand = x + rand()%2; //add random number between 0 and 1 to the x and y coords
        float Yrand = y + rand()%2;
        randoms.push_back(glm::vec2(Xrand, Yrand));
    }

    glm::vec2 thePickedSample;

    if (this->pickedSamples.size() == 0) { //if this is first pixel determined
        thePickedSample = randoms[samples_sqrt];

    } else {
        for (int i = 0; i < samples_sqrt*samples_sqrt; i++) {
            glm::vec2 temp = randoms[i];
            for (int j = 0; j < pickedSamples.size(); j++) {
                //if this picked sample is not at least the minimum distance apart from all
                // already picked samples, then try different sample.
                if ((pickedSamples[j][0] - temp[0]) < 0.25 &&
                        (pickedSamples[j][1] - temp[1]) < 0.25) {
                    break; //move onto next random[i] of outer loop
                }
                //if it is at least min distance, keep checking against all pickedSamples.
            }
            //if it reaches here, then the temp pixel is at least min distance from all other picked pixels
            //we can stop iterating here (take first random pixel that passes the test)
            thePickedSample = temp;
            break;
        }
    }

    if (thePickedSample[0] < x || thePickedSample[1] < y) {
        thePickedSample = glm::vec2(x+0.5f,y+0.5f); //set in middle of pixel if not correctly assigned random.
    }

    this->pickedSamples.push_back(thePickedSample); //pick random sample from this pixel and add to the samples
    QList<glm::vec2> sample;
    sample.push_back(thePickedSample);
    return sample;
}
