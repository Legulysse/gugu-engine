#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"

#include <cmath>
#include <algorithm>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
//----------------------------------------------
// Constants

namespace Math
{
    const float Pi          = 3.14159265358979323846f;      // Pi
    const float PiDivTwo    = 1.57079632679489661923f;      // Pi / 2
    const float PiDivFour   = 0.785398163397448309616f;     // Pi / 4
    const float OneDivPi    = 0.318309886183790671538f;     // 1 / Pi
    const float TwoDivPi    = 0.636619772367581343076f;     // 2 / Pi

    const float DegreesToRadians = 0.0174532925199f;    // Pi / 180
    const float RadiansToDegrees = 57.2957795131f;      // 180 / Pi

    const float Epsilon     = 0.001f;
}

//----------------------------------------------
// General

template <typename T>
bool IsInRange(T _tValue, T _tMin, T _tMax);

template <typename T>
bool IsInRangeUnordered(T _tValue, T _tLimitA, T _tLimitB);

template <typename T>
T Max(T _tValue, T _tMax);

template <typename T>
T Min(T _tValue, T _tMin);

template <typename T>
T Clamp(T _tValue, T _tMin, T _tMax);

template <typename T>
T ClampUnordered(T _tValue, T _tLimitA, T _tLimitB);

template <typename T>
T Distance(T _tValue1, T _tValue2);

template <typename T>
T Absolute(T _tValue);    //Visual has a good abs() templated for int, float, etc... but CodeBlocks doesn't

template <typename T>
T Power(const T& n, const int& p);

//----------------------------------------------
// Angles

double ToRadians(double _fDegrees);
double ToDegrees(double _fRadians);

float ToRadiansf(float _fDegrees);
float ToDegreesf(float _fRadians);

//----------------------------------------------
// Interpolations

template <typename T>
T Lerp(const T& min, const T& max, float ratio);

template <typename T>
float InverseLerp(const T& min, const T& max, const T& value);

template <typename T>
T RemapLerp(const T& minA, const T& maxA, const T& minB, const T& maxB, const T& value);

// Easing methods from https://github.com/ai/easings.net/blob/master/src/easings/easingsFunctions.ts
float EaseInQuad(float ratio);
float EaseOutQuad(float ratio);
float EaseInOutQuad(float ratio);

float EaseInCubic(float ratio);
float EaseOutCubic(float ratio);
float EaseInOutCubic(float ratio);

float EaseInQuart(float ratio);
float EaseOutQuart(float ratio);
float EaseInOutQuart(float ratio);

float EaseInQuint(float ratio);
float EaseOutQuint(float ratio);
float EaseInOutQuint(float ratio);

float EaseInSine(float ratio);
float EaseOutSine(float ratio);
float EaseInOutSine(float ratio);

float EaseInExpo(float ratio);
float EaseOutExpo(float ratio);
float EaseInOutExpo(float ratio);

float EaseInCirc(float ratio);
float EaseOutCirc(float ratio);
float EaseInOutCirc(float ratio);

float EaseInBack(float ratio);
float EaseOutBack(float ratio);
float EaseInOutBack(float ratio);

float EaseInElastic(float ratio);
float EaseOutElastic(float ratio);
float EaseInOutElastic(float ratio);

float EaseInBounce(float ratio);
float EaseOutBounce(float ratio);
float EaseInOutBounce(float ratio);

}   // namespace gugu

////////////////////////////////////////////////////////////////
// Template Implementation

#include "Gugu/Math/MathUtility.tpp"
