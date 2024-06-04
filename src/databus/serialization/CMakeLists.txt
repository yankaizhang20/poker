# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# 序列化 API，定义了各种数据类型的序列化、反序列化的计算接口
# （具体实现均需由数据定义者实现）
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#poker_add_protobuf(serialization-basic_proto)

poker_add_library(databus_serialization DEPENDS serialization-basic_proto macro)
