////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Math/FixedFloat.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/MathUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

FixedFloat::FixedFloat()
{
    m_value = 0;
}

FixedFloat::FixedFloat(const FixedFloat& _oFixedFloat)
{
    Copy(_oFixedFloat);
}

FixedFloat::FixedFloat(int32 _iValue)
{
    FromInt32(_iValue);
}

FixedFloat::~FixedFloat()
{
}

FixedFloat& FixedFloat::SetRawData(int32 _iValue)
{
    m_value = _iValue;
    return *this;
}

FixedFloat& FixedFloat::Copy(const FixedFloat& _oFixedFloat)
{
    m_value = _oFixedFloat.m_value;
    return *this;
}

FixedFloat& FixedFloat::FromInt32(int32 _iValue)
{
    m_value = 0;
    return Add(_iValue);
}

FixedFloat& FixedFloat::operator = (FixedFloat _oFixedFloat)
{
    return Copy(_oFixedFloat);
}

FixedFloat& FixedFloat::operator = (int32 _iValue)
{
    return FromInt32(_iValue);
}

FixedFloat& FixedFloat::Add(FixedFloat _oFixedFloat)
{
    m_value += _oFixedFloat.m_value;
    return *this;
}

FixedFloat& FixedFloat::Add(int32 _iValue)
{
    m_value += GUGU_FIXEDFLOAT_TO_RAW(_iValue);
    return *this;
}

FixedFloat& FixedFloat::AddDecimal(int32 _iValue, int32 _iNbDecimals)
{
    if(_iNbDecimals > 0)
    {
        m_value += (GUGU_FIXEDFLOAT_TO_RAW(_iValue) / Power(10, _iNbDecimals));
    }
    return *this;
}

FixedFloat& FixedFloat::AddDecimal3(int32 _iValue)
{
    return AddDecimal(_iValue, 3);
}

FixedFloat& FixedFloat::operator += (FixedFloat _oFixedFloat)
{
    return Add(_oFixedFloat);
}

FixedFloat& FixedFloat::operator += (int32 _iValue)
{
    return Add(_iValue);
}

FixedFloat& FixedFloat::operator ++ ()
{
    return Add(1);
}
FixedFloat FixedFloat::operator ++ (int)
{
    FixedFloat oFixedResult(*this);
    Add(1);
    return oFixedResult;
}

FixedFloat& FixedFloat::Substract(FixedFloat _oFixedFloat)
{
    m_value -= _oFixedFloat.m_value;
    return *this;
}

FixedFloat& FixedFloat::Substract(int32 _iValue)
{
    m_value -= GUGU_FIXEDFLOAT_TO_RAW(_iValue);
    return *this;
}

FixedFloat& FixedFloat::operator -= (FixedFloat _oFixedFloat)
{
    return Substract(_oFixedFloat);
}

FixedFloat& FixedFloat::operator -= (int32 _iValue)
{
    return Substract(_iValue);
}

FixedFloat& FixedFloat::operator -- ()
{
    return Substract(1);
}

FixedFloat FixedFloat::operator -- (int)
{
    FixedFloat oFixedResult(*this);
    Substract(1);
    return oFixedResult;
}

FixedFloat& FixedFloat::Multiply(FixedFloat _oFixedFloat)
{
    m_value = (int32)GUGU_FIXEDFLOAT_FROM_RAW(((int64)m_value) * ((int64)_oFixedFloat.m_value));
    return *this;
}

FixedFloat& FixedFloat::Multiply(int32 _iValue)
{
    m_value = (int32)(m_value * _iValue);
    return *this;
}

FixedFloat& FixedFloat::operator *= (FixedFloat _oFixedFloat)
{
    return Multiply(_oFixedFloat);
}

FixedFloat& FixedFloat::operator *= (int32 _iValue)
{
    return Multiply(_iValue);
}

FixedFloat& FixedFloat::Divide(FixedFloat _oFixedFloat)
{
    m_value = (int32)((GUGU_FIXEDFLOAT_TO_RAW((int64)m_value)) / ((int64)_oFixedFloat.m_value));
    return *this;
}

FixedFloat& FixedFloat::Divide(int32 _iValue)
{
    m_value = (int32)(((int64)m_value) / ((int64)_iValue));
    return *this;
}

FixedFloat& FixedFloat::operator /= (FixedFloat _oFixedFloat)
{
    return Divide(_oFixedFloat);
}

FixedFloat& FixedFloat::operator /= (int32 _iValue)
{
    return Divide(_iValue);
}

FixedFloat FixedFloat::operator - () const
{
    FixedFloat oFixedResult;
    oFixedResult.SetRawData( -m_value );
    return oFixedResult;
}

int32 FixedFloat::GetRawData() const
{
    return m_value;
}

float FixedFloat::ToFloat() const
{
    return (float(m_value) / GUGU_FIXEDFLOAT_UNIT);
}

int32 FixedFloat::ToInt32() const
{
    return GUGU_FIXEDFLOAT_FROM_RAW(m_value);
}

FixedFloat::operator int32 () const
{
    return ToInt32();
}

FixedFloat::operator float () const
{
    return ToFloat();
}

FixedFloat operator + (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB)
{
    FixedFloat oFixedResult(_oFixedFloatA);
    return oFixedResult.Add(_oFixedFloatB);
}

FixedFloat operator + (FixedFloat _oFixedFloat, int32 _iValue)
{
    FixedFloat oFixedResult(_oFixedFloat);
    return oFixedResult.Add(_iValue);
}

FixedFloat operator + (int32 _iValue, FixedFloat _oFixedFloat)
{
    FixedFloat oFixedResult(_oFixedFloat);
    return oFixedResult.Add(_iValue);
}

FixedFloat operator - (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB)
{
    FixedFloat oFixedResult(_oFixedFloatA);
    return oFixedResult.Substract(_oFixedFloatB);
}

FixedFloat operator - (FixedFloat _oFixedFloat, int32 _iValue)
{
    FixedFloat oFixedResult(_oFixedFloat);
    return oFixedResult.Substract(_iValue);
}

FixedFloat operator - (int32 _iValue, FixedFloat _oFixedFloat)
{
    FixedFloat oFixedResult(-_oFixedFloat);
    return oFixedResult.Add(_iValue);
}

FixedFloat operator * (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB)
{
    FixedFloat oFixedResult(_oFixedFloatA);
    return oFixedResult.Multiply(_oFixedFloatB);
}

FixedFloat operator * (FixedFloat _oFixedFloat, int32 _iValue)
{
    FixedFloat oFixedResult(_oFixedFloat);
    return oFixedResult.Multiply(_iValue);
}

FixedFloat operator * (int32 _iValue, FixedFloat _oFixedFloat)
{
    FixedFloat oFixedResult(_oFixedFloat);
    return oFixedResult.Multiply(_iValue);
}

FixedFloat operator / (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB)
{
    FixedFloat oFixedResult(_oFixedFloatA);
    return oFixedResult.Divide(_oFixedFloatB);
}

FixedFloat operator / (FixedFloat _oFixedFloat, int32 _iValue)
{
    FixedFloat oFixedResult(_oFixedFloat);
    return oFixedResult.Divide(_iValue);
}

FixedFloat operator / (int32 _iValue, FixedFloat _oFixedFloat)
{
    FixedFloat oFixedResult(_iValue);
    return oFixedResult.Divide(_oFixedFloat);
}

int32 operator % (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB)
{
    return (_oFixedFloatA.ToInt32() % _oFixedFloatB.ToInt32());
}

int32 operator % (FixedFloat _oFixedFloat, int32 _iValue)
{
    return (_oFixedFloat.ToInt32() % _iValue);
}

int32 operator % (int32 _iValue, FixedFloat _oFixedFloat)
{
    return (_iValue % _oFixedFloat.ToInt32());
}

bool operator == (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB)
{
    return (_oFixedFloatA.GetRawData() == _oFixedFloatB.GetRawData());
}

bool operator == (FixedFloat _oFixedFloat, int32 _iValue)
{
    return (_oFixedFloat == FixedFloat(_iValue));
}

bool operator == (int32 _iValue, FixedFloat _oFixedFloat)
{
    return (FixedFloat(_iValue) == _oFixedFloat);
}

bool operator != (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB)
{
    return !(_oFixedFloatA == _oFixedFloatB);
}

bool operator != (FixedFloat _oFixedFloat, int32 _iValue)
{
    return !(_oFixedFloat == _iValue);
}

bool operator != (int32 _iValue, FixedFloat _oFixedFloat)
{
    return !(_iValue == _oFixedFloat);
}

bool operator > (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB)
{
    return (_oFixedFloatA.GetRawData() > _oFixedFloatB.GetRawData());
}

bool operator > (FixedFloat _oFixedFloat, int32 _iValue)
{
    return (_oFixedFloat.GetRawData() > _iValue);
}

bool operator > (int32 _iValue, FixedFloat _oFixedFloat)
{
    return (_iValue > _oFixedFloat.GetRawData());
}

bool operator < (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB)
{
    return (_oFixedFloatA.GetRawData() < _oFixedFloatB.GetRawData());
}

bool operator < (FixedFloat _oFixedFloat, int32 _iValue)
{
    return (_oFixedFloat.GetRawData() < _iValue);
}

bool operator < (int32 _iValue, FixedFloat _oFixedFloat)
{
    return (_iValue < _oFixedFloat.GetRawData());
}

bool operator >= (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB)
{
    return (_oFixedFloatA.GetRawData() >= _oFixedFloatB.GetRawData());
}

bool operator >= (FixedFloat _oFixedFloat, int32 _iValue)
{
    return (_oFixedFloat.GetRawData() >= _iValue);
}

bool operator >= (int32 _iValue, FixedFloat _oFixedFloat)
{
    return (_iValue >= _oFixedFloat.GetRawData());
}

bool operator <= (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB)
{
    return (_oFixedFloatA.GetRawData() <= _oFixedFloatB.GetRawData());
}

bool operator <= (FixedFloat _oFixedFloat, int32 _iValue)
{
    return (_oFixedFloat.GetRawData() <= _iValue);
}

bool operator <= (int32 _iValue, FixedFloat _oFixedFloat)
{
    return (_iValue <= _oFixedFloat.GetRawData());
}

}   // namespace gugu
