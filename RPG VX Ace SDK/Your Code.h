#pragma once
#include "RPG VX Ace SDK.h"

void RGSSYourCustomInitCode();

// Place your custom DLL calls here.
// Please prefix your functions with RGSSApi to allow RPG Maker VX Ace to call them.

#define DegToRad( x ) ( x*0.0174532925 )

RGSSApi bool Invert(unsigned int object);
RGSSApi bool Monochrome(unsigned int object);
RGSSApi bool Saturation(unsigned int object, unsigned int amount_in);
RGSSApi bool Brightness(unsigned int object, int amount);
RGSSApi bool Contrast(unsigned int object, int amount);
RGSSApi bool Filter33(unsigned int object, int _11, int _12, int _13, int _21, int _22, int _23, int _31, int _32, int _33, int scale);
RGSSApi bool Mode7(unsigned int object, int offset_x, int offset_y,int rotation);
RGSSApi bool Sepia(unsigned int object);
RGSSApi bool Transform33(unsigned int object, int _11, int _12, int _13, int _21, int _22, int _23, int _31, int _32, int _33);
RGSSApi bool Noise(unsigned int object,int range, int iterations, int Mask);
RGSSApi bool Vignette(unsigned int object,int radius);
