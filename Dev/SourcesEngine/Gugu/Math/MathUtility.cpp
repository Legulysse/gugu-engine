////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Math/MathUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

//----------------------------------------------
// Angles

double ToRadians(double _fDegrees)
{
    return _fDegrees * Math::DegreesToRadians;
}

double ToDegrees(double _fRadians)
{
    return _fRadians * Math::RadiansToDegrees;
}

float ToRadiansf(float _fDegrees)
{
    return _fDegrees * Math::DegreesToRadians;
}

float ToDegreesf(float _fRadians)
{
    return _fRadians * Math::RadiansToDegrees;
}

//----------------------------------------------
// Interpolations

namespace impl
{
    const float c1 = 1.70158f;
    const float c2 = c1 * 1.525f;
    const float c3 = c1 + 1;
    const float c4 = (2 * Math::Pi) / 3;
    const float c5 = (2 * Math::Pi) / 4.5f;

    float EaseBounceOut(float ratio)
    {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;

        if (ratio < 1 / d1)
        {
            return n1 * ratio * ratio;
        }
        else if (ratio < 2 / d1)
        {
            return n1 * (ratio -= (1.5f / d1)) * ratio + .75f;
        }
        else if (ratio < 2.5f / d1)
        {
            return n1 * (ratio -= (2.25f / d1)) * ratio + .9375f;
        }
        else
        {
            return n1 * (ratio -= (2.625f / d1)) * ratio + .984375f;
        }
    }
}

float EaseInQuad(float ratio)
{
    return ratio * ratio;
}

float EaseOutQuad(float ratio)
{
    return 1 - (1 - ratio) * (1 - ratio);
}

float EaseInOutQuad(float ratio)
{
    return (ratio < 0.5f) ? 2 * ratio * ratio : 1 - std::pow(-2 * ratio + 2, 2) / 2;
}

float EaseInCubic(float ratio)
{
    return ratio * ratio * ratio;
}

float EaseOutCubic(float ratio)
{
    return 1 - std::pow(1 - ratio, 3);
}

float EaseInOutCubic(float ratio)
{
    return (ratio < 0.5f) ? 4 * ratio * ratio * ratio : 1 - std::pow(-2 * ratio + 2, 3) / 2;
}

float EaseInQuart(float ratio)
{
    return ratio * ratio * ratio * ratio;
}

float EaseOutQuart(float ratio)
{
    return 1 - std::pow(1 - ratio, 4);
}

float EaseInOutQuart(float ratio)
{
    return (ratio < 0.5f) ? 8 * ratio * ratio * ratio * ratio : 1 - std::pow(-2 * ratio + 2, 4) / 2;
}

float EaseInQuint(float ratio)
{
    return ratio * ratio * ratio * ratio * ratio;
}

float EaseOutQuint(float ratio)
{
    return 1 - std::pow(1 - ratio, 5);
}

float EaseInOutQuint(float ratio)
{
    return (ratio < 0.5) ? 16 * ratio * ratio * ratio * ratio * ratio : 1 - std::pow(-2 * ratio + 2, 5) / 2;
}

float EaseInSine(float ratio)
{
    return 1 - std::cos(ratio * Math::Pi / 2);
}

float EaseOutSine(float ratio)
{
    return std::sin(ratio * Math::Pi / 2);
}

float EaseInOutSine(float ratio)
{
    return -(std::cos(Math::Pi * ratio) - 1) / 2;
}

float EaseInExpo(float ratio)
{
    return ratio == 0 ? 0 : std::pow(2.f, 10 * ratio - 10);
}

float EaseOutExpo(float ratio)
{
    return ratio == 1 ? 1 : 1 - std::pow(2.f, -10 * ratio);
}

float EaseInOutExpo(float ratio)
{
    return ratio == 0 ? 0 : ratio == 1 ? 1 : ratio < 0.5f ?
        std::pow(2.f, 20 * ratio - 10) / 2 :
        (2 - std::pow(2.f, -20 * ratio + 10)) / 2;
}

float EaseInCirc(float ratio)
{
    return 1 - std::sqrt(1 - std::pow(ratio, 2));
}

float EaseOutCirc(float ratio)
{
    return std::sqrt(1 - std::pow(ratio - 1, 2));
}

float EaseInOutCirc(float ratio)
{
    return (ratio < 0.5) ? (1 - std::sqrt(1 - std::pow(2 * ratio, 2))) / 2 : (std::sqrt(1 - std::pow(-2 * ratio + 2, 2)) + 1) / 2;
}

float EaseInBack(float ratio)
{
    return impl::c3 * ratio * ratio * ratio - impl::c1 * ratio * ratio;
}

float EaseOutBack(float ratio)
{
    return 1 + impl::c3 * std::pow(ratio - 1, 3) + impl::c1 * std::pow(ratio - 1, 2);
}

float EaseInOutBack(float ratio)
{
    return (ratio < 0.5f) ?
        (std::pow(2 * ratio, 2) * ((impl::c2 + 1) * 2 * ratio - impl::c2)) / 2 :
        (std::pow(2 * ratio - 2, 2) * ((impl::c2 + 1) * (ratio * 2 - 2) + impl::c2) + 2) / 2;
}

float EaseInElastic(float ratio)
{
    return ratio == 0 ? 0 : ratio == 1 ? 1 :
        -std::pow(2.f, 10 * ratio - 10) * std::sin((ratio * 10 - 10.75f) * impl::c4);
}

float EaseOutElastic(float ratio)
{
    return ratio == 0 ? 0 : ratio == 1 ? 1 :
        std::pow(2.f, -10 * ratio) * std::sin((ratio * 10 - 0.75f) * impl::c4) + 1;
}

float EaseInOutElastic(float ratio)
{
    return ratio == 0 ? 0 : ratio == 1 ? 1 : ratio < 0.5f ?
        -(std::pow(2.f, 20 * ratio - 10) * std::sin((20 * ratio - 11.125f) * impl::c5)) / 2 :
        std::pow(2.f, -20 * ratio + 10) * std::sin((20 * ratio - 11.125f) * impl::c5) / 2 + 1;
}

float EaseInBounce(float ratio)
{
    return 1 - impl::EaseBounceOut(1 - ratio);
}

float EaseOutBounce(float ratio)
{
    return impl::EaseBounceOut(ratio);
}

float EaseInOutBounce(float ratio)
{
    return (ratio < 0.5f) ? (1 - impl::EaseBounceOut(1 - 2 * ratio)) / 2 : (1 + impl::EaseBounceOut(2 * ratio - 1)) / 2;
}

}   // namespace gugu
