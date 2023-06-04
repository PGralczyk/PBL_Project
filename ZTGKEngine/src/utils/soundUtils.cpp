#pragma once

#include <AL\al.h>
#include <AL\alext.h>

using namespace std;

void setGlobalVolume(float v)
{
    if (0 <= v && v <= 1) 
    {
        alListenerf(AL_GAIN, v);
    }
}

float getGlobalVolume()
{
    ALfloat v;
    alGetListenerf(AL_GAIN, &v);
    return v;
}