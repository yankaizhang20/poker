poker_add_library(param_backend_yaml
        DEPENDS param PRIVATE reflect
        IMPORTS yaml-cpp
        LIBRARY ${YAML_CPP_LIBRARIES}
)

poker_add_test(param_backend_yaml)