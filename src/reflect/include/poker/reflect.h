//
// Created by zyk on 24-5-27.
//

#pragma once

#include "./reflect/entity/ParameterView.h"


namespace poker::reflect
{
    template < class T >
    void Bind(const T &value, ParameterView &parameter);
}