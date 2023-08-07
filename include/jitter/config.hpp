#ifndef __CONFIG_H__
#define __CONFIG_H__

struct Config
{
    float lambda1;
    float lambda2;
    float temperature;
    int maxIterations
    Config(float lambda1, float lambda2, float temperature, int maxIterations) : lambda1(lambda1), lambda2(lambda2), temperature(temperature), maxIterations(maxIterations) {};
};

#endif