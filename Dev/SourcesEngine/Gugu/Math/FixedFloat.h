#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"

////////////////////////////////////////////////////////////////
// Macros

//#define GUGU_FIXEDFLOAT_USE_BITWISE

#ifdef GUGU_FIXEDFLOAT_USE_BITWISE

    #ifndef GUGU_FIXEDFLOAT_DECIMAL_BITS
    #define GUGU_FIXEDFLOAT_DECIMAL_BITS        (10)
    #endif

    #define GUGU_FIXEDFLOAT_UNIT                (1 << GUGU_FIXEDFLOAT_DECIMAL_BITS)

    #define GUGU_FIXEDFLOAT_TO_RAW(VALUE)       ((VALUE) << GUGU_FIXEDFLOAT_DECIMAL_BITS)
    #define GUGU_FIXEDFLOAT_FROM_RAW(VALUE)     ((VALUE) >> GUGU_FIXEDFLOAT_DECIMAL_BITS)

#else

    #define GUGU_FIXEDFLOAT_UNIT                (1000)

    #define GUGU_FIXEDFLOAT_TO_RAW(VALUE)       ((VALUE)*(GUGU_FIXEDFLOAT_UNIT))
    #define GUGU_FIXEDFLOAT_FROM_RAW(VALUE)     ((VALUE)/(GUGU_FIXEDFLOAT_UNIT))

#endif

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class FixedFloat        //BinaryFixedFloat, maybe I should also do a DecimalFixedFloat
{
public:

    //Constructors & Destructor
                    FixedFloat      ();
                    FixedFloat      (const FixedFloat& _oFixedFloat);
                    FixedFloat      (int32 _iValue);
                    ~FixedFloat     ();

    //Set
    FixedFloat&     SetRawData      (int32 _iValue);
    FixedFloat&     Copy            (const FixedFloat& _oFixedFloat);
    FixedFloat&     FromInt32       (int32 _iValue);

    FixedFloat&     operator =      (FixedFloat _oFixedFloat);
    FixedFloat&     operator =      (int32 _iValue);

    //Add
    FixedFloat&     Add             (FixedFloat _oFixedFloat);
    FixedFloat&     Add             (int32 _iValue);
    FixedFloat&     AddDecimal      (int32 _iValue, int32 _iNbDecimals);
    FixedFloat&     AddDecimal3     (int32 _iValue);

    FixedFloat&     operator +=     (FixedFloat _oFixedFloat);
    FixedFloat&     operator +=     (int32 _iValue);

    FixedFloat&     operator ++     ();     //Pre-increment
    FixedFloat      operator ++     (int);  //Post-increment

    //Substract
    FixedFloat&     Substract       (FixedFloat _oFixedFloat);
    FixedFloat&     Substract       (int32 _iValue);

    FixedFloat&     operator -=     (FixedFloat _oFixedFloat);
    FixedFloat&     operator -=     (int32 _iValue);

    FixedFloat&     operator --     ();     //Pre-decrement
    FixedFloat      operator --     (int);  //Post-decrement

    //Multiply
    FixedFloat&     Multiply        (FixedFloat _oFixedFloat);
    FixedFloat&     Multiply        (int32 _iValue);

    FixedFloat&     operator *=     (FixedFloat _oFixedFloat);
    FixedFloat&     operator *=     (int32 _iValue);

    //Divide
    FixedFloat&     Divide          (FixedFloat _oFixedFloat);
    FixedFloat&     Divide          (int32 _iValue);

    FixedFloat&     operator /=     (FixedFloat _oFixedFloat);
    FixedFloat&     operator /=     (int32 _iValue);

    //Negate
    FixedFloat      operator -      () const;

    //Get
    int32           GetRawData      () const;
    float           ToFloat         () const;
    int32           ToInt32         () const;

                    operator float  () const;
                    operator int32  () const;

private:

    int32 m_value;
};

//Binary operators
FixedFloat  operator +  (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB);
FixedFloat  operator +  (FixedFloat _oFixedFloat, int32 _iValue);
FixedFloat  operator +  (int32 _iValue, FixedFloat _oFixedFloat);

FixedFloat  operator -  (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB);
FixedFloat  operator -  (FixedFloat _oFixedFloat, int32 _iValue);
FixedFloat  operator -  (int32 _iValue, FixedFloat _oFixedFloat);

FixedFloat  operator *  (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB);
FixedFloat  operator *  (FixedFloat _oFixedFloat, int32 _iValue);
FixedFloat  operator *  (int32 _iValue, FixedFloat _oFixedFloat);

FixedFloat  operator /  (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB);
FixedFloat  operator /  (FixedFloat _oFixedFloat, int32 _iValue);
FixedFloat  operator /  (int32 _iValue, FixedFloat _oFixedFloat);

int32       operator %  (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB);
int32       operator %  (FixedFloat _oFixedFloat, int32 _iValue);
int32       operator %  (int32 _iValue, FixedFloat _oFixedFloat);

bool        operator == (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB);
bool        operator == (FixedFloat _oFixedFloat, int32 _iValue);
bool        operator == (int32 _iValue, FixedFloat _oFixedFloat);

bool        operator != (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB);
bool        operator != (FixedFloat _oFixedFloat, int32 _iValue);
bool        operator != (int32 _iValue, FixedFloat _oFixedFloat);

bool        operator >  (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB);
bool        operator >  (FixedFloat _oFixedFloat, int32 _iValue);
bool        operator >  (int32 _iValue, FixedFloat _oFixedFloat);

bool        operator <  (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB);
bool        operator <  (FixedFloat _oFixedFloat, int32 _iValue);
bool        operator <  (int32 _iValue, FixedFloat _oFixedFloat);

bool        operator >= (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB);
bool        operator >= (FixedFloat _oFixedFloat, int32 _iValue);
bool        operator >= (int32 _iValue, FixedFloat _oFixedFloat);

bool        operator <= (FixedFloat _oFixedFloatA, FixedFloat _oFixedFloatB);
bool        operator <= (FixedFloat _oFixedFloat, int32 _iValue);
bool        operator <= (int32 _iValue, FixedFloat _oFixedFloat);

}   // namespace gugu
