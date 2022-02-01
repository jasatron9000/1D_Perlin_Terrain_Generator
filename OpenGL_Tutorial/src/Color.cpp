#include <math.h>
#include <iostream>
#include <string>

#include "inc/Color.h"

RGB HSV2RGB(const float hue, const float saturation, const float value)
{
    float C = value * saturation;
    float dH = hue / 60.0f;
    float X = C * (1.0f - (float)fabs((fmod(dH, 2.0f) - 1)));
    RGB colour = { 0.0f, 0.0f, 0.0f };

    if (dH >= 0.0f && dH < 1.0f) {
        colour = { C, X, 0.0f };
    }
    else if (dH >= 1.0f && dH < 2.0f) {
        colour = { X, C, 0.0f };
    }
    else if (dH >= 2.0f && dH < 3.0f) {
        colour = { 0.0f, C, X };
    }
    else if (dH >= 3.0f && dH < 4.0f) {
        colour = { 0.0f, X, C };
    }
    else if (dH >= 4.0f && dH < 5.0f) {
        colour = { X, 0.0f, C };
    }
    else if (dH >= 5.0f && dH < 6.0f) {
        colour = { C, 0.0f, X };
    }
    else {
        colour = { 0.0f, 0.0f, 0.0f };
    }

    float m = value - C;

    return { colour.R + m, colour.G + m, colour.B + m };
}

RGB NormRGB(const unsigned char R, const unsigned char G, const unsigned char B)
{
    return {(float)R / 255.0f, (float)G / 255.0f, (float)B / 255.0f, };
}

RGB HEX2RGB(const std::string hexCode)
{
    static const std::string hexList = "0123456789ABCDEF";

    for (char h : hexCode) {
        std::cout << hexCode << std::endl;
    }

    return RGB();
}

