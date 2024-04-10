# 场景描述

## entity.h

entity.h 为业务实体数据定义，**不可修改**

## test.h

test.h 为对业务数据的处理过程，其中暗含 interface.h 中 Help 提供的用户接口，用户使用这些接口对业务实
体数据做处理。根据各种 Derive 类型的模板参数种类（BaseA、BaseB）做不同的处理，但是当模板参数类型相同时，
进行相同的处理

良好的设计不应随意更改接口，且无权要求用户做出调整，因此**不可修改**用户使用的接口形式，即，不可修改 test.h。

为了可维护性，要求对含有相同的模板参数的 Derive 类型的处理只有一处代码， 即，**不可**为不同 Derive 特化
实现。

# 问题

请实现 Help 中的 func 接口，完成 test.h 中对 Derive1、Derive2、SpecialDerive 类型的处理
