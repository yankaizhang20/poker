//
// Created by zyk on 2024-6-4
//

#pragma once


#include "./UnitSystem.h"


namespace poker::unit::details
{
    template<class EUnitType, EUnitType unit_type = EUnitType::Undefined>
    class Unit : public UnitSystem<EUnitType>
    {
        static_assert(UnitTypeEnum<EUnitType>::enable);
        using Base = UnitSystem<EUnitType>;

    public:
        // 本单位的具体枚举类型
        constexpr static EUnitType UNIT = unit_type;

    public:
        Unit(double value = 0) // NOLINT(google-explicit-constructor)
            : Base(UNIT, value)
        {}

        Unit(const Base & base) // NOLINT(google-explicit-constructor)
            : Base(base)
        {}

        Unit(const Unit & other) = default;

        using Base::Set;

        void Set(double value)
        {
            Base::Set(UNIT, value);
        }

        using Base::Get;

        [[nodiscard]] double Get() const
        {
            return Base::Get(UNIT);
        }

        explicit operator double () const
        {
            return Get();
        }

    public:
        Unit & operator=(double value)
        {
            Set(value);
            return *this;
        }

        Unit & operator=(const Unit & o) = default;

        Unit & operator=(const Base & value)
        {
            Base::operator=(value);
            return *this;
        }

        Unit & operator+=(const Base & o)
        {
            Base::operator+=(o);
            return *this;
        }

        Unit & operator-=(const Base & o)
        {
            Base::operator-=(o);
            return *this;
        }

        Unit & operator*=(double scale)
        {
            Base::operator*=(scale);
            return *this;
        }

        Unit & operator/=(double scale)
        {
            Base::operator/=(scale);
            return *this;
        }

        Unit operator+(const Base & o) const
        {
            return Unit(*this) += o;
        }

        Unit operator-(const Base & o) const
        {
            return Unit(*this) -= o;
        }

        Unit operator*(double scale) const
        {
            return Unit(*this) *= scale;
        }

        Unit operator/(double scale) const
        {
            return Unit(*this) /= scale;
        }

        friend Unit operator*(double scale, const Unit & x)
        {
            return x * scale;
        }

        Unit operator+() const
        {
            return *this;
        }

        Unit operator-() const
        {
            return Unit(Base::operator-());
        }
    };
}
