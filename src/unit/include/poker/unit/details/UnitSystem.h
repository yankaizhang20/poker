//
// Created by zyk on 2024-6-4
//

#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>

#include <poker/type_traits.h>


namespace poker::unit::details
{
    // clang-format off
    poker_concept(UnitTypeEnum, class T,
        (
            std::is_enum_v<T>                         and
            std::is_same_v<T, decltype(T::Default)>   and
            std::is_same_v<T, decltype(T::Undefined)> and
            int(T::Default)   == 0                    and
            int(T::Undefined) == -1
        )
    );

    poker_concept(UnitClass, class TUnit, class TUnitType,
        (
            UnitTypeEnum<TUnitType>::enable                        and
            std::is_same_v<decltype(TUnit::UNIT), const TUnitType> and
            std::is_same_v<decltype(std::declval<TUnit>().Get()), double>
        )
    );
    // clang-format on

    /**
     * @return 指定单位系统的单位比例系数
     * @note 由不同单位类型特化实现。
     */
    template < class EUnitType >
    std::vector< double > Ratios();

    /**
     * @return 指定单位系统的反比例系数
     */
    template < class EUnitType >
    std::vector< double > InverseRatios()
    {
        auto r = Ratios< EUnitType >();
        for (auto &i : r)
            i = 1.0 / i;
        return r;
    }

    template < class EUnitType >
    class UnitSystem
    {
        static_assert(UnitTypeEnum< EUnitType >::enable);

    public:
        UnitSystem()                       = default;
        UnitSystem(const UnitSystem &)     = default;
        UnitSystem(UnitSystem &&) noexcept = default;

        template < class TUnit >
        UnitSystem(const TUnit &unit)   // NOLINT(google-explicit-constructor)
        {
            static_assert(UnitClass< TUnit, EUnitType >::enable);
            Set(unit);
        }

        UnitSystem(EUnitType unit_type, double value)
        {
            Set(unit_type, value);
        }

        /**
         * @brief 零值
         */
        const static UnitSystem zero;

        /**
         * @brief 最大值
         */
        const static UnitSystem maximal;

        // 单位类型
        using UnitType = EUnitType;

        // 单位系统类型
        using UnitSystemType = UnitSystem;

    public:
        void Set(EUnitType unit_type, double value)
        {
            _value = value * GetInverseRatio(unit_type);
        }

        template < class TUnit >
        void Set(double value)
        {
            static_assert(UnitClass< TUnit, EUnitType >::enable);
            Set(TUnit::UNIT, value);
        }

        template < class TUnit >
        void Set(const TUnit &unit)
        {
            static_assert(UnitClass< TUnit, EUnitType >::enable);

            Set(TUnit::UNIT, unit.Get());
        }

        void Set(const UnitSystem &o)
        {
            _value = o._value;
        }

        [[nodiscard]]
        double Get(EUnitType unit_type) const
        {
            return _value * GetRatio(unit_type);
        }

        template < class TUnit >
        [[nodiscard]]
        double Get() const
        {
            static_assert(UnitClass< TUnit, EUnitType >::enable);

            return Get(TUnit::UNIT);
        }

        UnitSystem &operator= (const UnitSystem &)     = default;
        UnitSystem &operator= (UnitSystem &&) noexcept = default;

        template < class TUnit >
        UnitSystem &operator= (const TUnit &value)
        {
            static_assert(UnitClass< TUnit, EUnitType >::enable);

            Set(value);
            return *this;
        }

    public:
        UnitSystem &operator+= (const UnitSystem &o)
        {
            _value += o._value;
            return *this;
        }

        UnitSystem &operator-= (const UnitSystem &o)
        {
            _value -= o._value;
            return *this;
        }

        UnitSystem &operator*= (double scale)
        {
            _value *= scale;
            return *this;
        }

        UnitSystem &operator/= (double scale)
        {
            _value /= scale;
            return *this;
        }

        UnitSystem operator+ (const UnitSystem &o) const
        {
            UnitSystem r;
            r._value = this->_value + o._value;
            return r;
        }

        UnitSystem operator- (const UnitSystem &o) const
        {
            UnitSystem r;
            r._value = this->_value - o._value;
            return r;
        }

        UnitSystem operator* (double scale) const
        {
            UnitSystem r;
            r._value = this->_value * scale;
            return r;
        }

        UnitSystem operator/ (double scale) const
        {
            UnitSystem r;
            r._value = this->_value / scale;
            return r;
        }

        double operator/ (const UnitSystem &other) const
        {
            return this->_value / other._value;
        }

        friend UnitSystem operator* (double scale, const UnitSystem &x)
        {
            return x * scale;
        }

        UnitSystem operator+ () const
        {
            return *this;
        }

        UnitSystem operator- () const
        {
            UnitSystem r;
            r._value = -_value;
            return r;
        }

        inline auto operator< (const UnitSystem &o) const
        {
            return _value < o._value;
        }

        inline auto operator<= (const UnitSystem &o) const
        {
            return _value <= o._value;
        }

        inline auto operator> (const UnitSystem &o) const
        {
            return _value > o._value;
        }

        inline auto operator>= (const UnitSystem &o) const
        {
            return _value >= o._value;
        }

        inline auto operator== (const UnitSystem &o) const
        {
            return _value == o._value;
        }

        inline auto operator!= (const UnitSystem &o) const
        {
            return _value != o._value;
        }

    public:
        static double Ratio(EUnitType from, EUnitType to)
        {
            return GetInverseRatio(from) * GetRatio(to);
        }

    private:
        static double GetRatio(EUnitType unit_type)
        {
            static std::vector< double > ratios = Ratios< EUnitType >();

            auto unit_index = std::size_t(unit_type);
            assert(unit_index < ratios.size());

            return ratios[ unit_index ];
        }

        static double GetInverseRatio(EUnitType unit_type)
        {
            static std::vector< double > inverse_ratios = InverseRatios< EUnitType >();

            auto unit_index = std::size_t(unit_type);
            assert(unit_index < inverse_ratios.size());

            return inverse_ratios[ unit_index ];
        }

    private:
        double _value = 0;
    };

    template < class EUnitType >
    const UnitSystem< EUnitType > UnitSystem< EUnitType >::zero;

    template < class EUnitType >
    const UnitSystem< EUnitType > UnitSystem< EUnitType >::maximal(EUnitType::Default,
                                                                   std::numeric_limits< double >::max());
}   // namespace poker::unit::details
