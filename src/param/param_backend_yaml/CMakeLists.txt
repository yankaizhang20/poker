poker_add_library(param_backend_yaml
        DEPENDS param
        IMPORTS yaml-cpp
        IMPORTS_AS yaml-cpp yaml-cpp::yaml-cpp
)

poker_add_test(param_backend_yaml)