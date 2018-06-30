#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/Types.h"

#include <SFML/System/Vector2.hpp>

#include <cmath>
#include <algorithm>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
//--------------------------------
// Constants

namespace Math
{
	const float	Pi			= 3.14159265358979323846f;		// Pi
	const float PiDivTwo	= 1.57079632679489661923f;		// Pi / 2
	const float	PiDivFour	= 0.785398163397448309616f;		// Pi / 4
	const float	OneDivPi	= 0.318309886183790671538f;		// 1 / Pi
	const float	TwoDivPi	= 0.636619772367581343076f;		// 2 / Pi

    const float DegreesToRadians = 0.0174532925199f;    // Pi / 180
    const float RadiansToDegrees = 57.2957795131f;      // 180 / Pi

	const float Epsilon		= 0.001f;
}

//--------------------------------
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

//--------------------------------
// Angles

double ToRadians(double _fDegrees);
double ToDegrees(double _fRadians);

float ToRadiansf(float _fDegrees);
float ToDegreesf(float _fRadians);

//--------------------------------
// Vectors

template <typename T>
float LengthSquare(const sf::Vector2<T>& _kVector);

template <typename T>
float Length(const sf::Vector2<T>& _kVector);

template <typename T>
sf::Vector2<T> Normalize(const sf::Vector2<T>& _kVector);

template <typename T>
sf::Vector2<T> Rotate(const sf::Vector2<T>& _kVector, float _fRadians);

}   // namespace gugu

////////////////////////////////////////////////////////////////
// Template Implementation

#include "Gugu/Utility/Math.tpp"
