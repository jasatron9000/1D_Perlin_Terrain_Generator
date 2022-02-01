#pragma once

struct RGB {
    float R;
    float G;
    float B;
};

RGB HSV2RGB(const float hue, const float saturation, const float value);
RGB NormRGB(const unsigned char R, const unsigned char G, const unsigned char B);
RGB HEX2RGB(const std::string hexCode);