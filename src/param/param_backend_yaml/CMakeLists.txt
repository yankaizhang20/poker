find_package(yaml-cpp REQUIRED)

poker_add_library(param_backend_yaml
        DEPENDS param PRIVATE reflect
        LIBRARY ${YAML_CPP_LIBRARIES}
)

poker_add_test(param_backend_yaml)