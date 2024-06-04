//
// Created by zyk on 24-6-4.
//

#include <poker/unit.h>


int main()
{
    poker::unit::Velocity v;

    v.Set< pokeru::kmh >(3.0);

    std::cout << sizeof(v) << " " << *reinterpret_cast< double * >(&v) << std::endl;

    std::cout << (2.0 * v * 3.0 / 1.5).Get< pokeru::kmh >() << std::endl;

    std::cout << (2.0 * pokeru::deg(5) * 3.0 / 1.5).Get< pokeru::deg >() << std::endl;

    poker::unit::Angle angle;
    angle = pokeru::deg(90);

    angle.Set< pokeru::deg >(80);

    poker::unit::Radian rad;

    double theta = rad.Get();

    std::cout << poker::unit::Degree::maximal.Get(poker::unit::AngleUnit::Default) << std::endl;

    return 0;
}