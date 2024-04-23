# main executable
set(MAIN ${CMAKE_CURRENT_SOURCE_DIR}/main)
add_executable(goeta
    ${MAIN}/goeta.cpp
)

# Global Libs
include_directories(${PROJECT_SOURCE_DIR}/kernels)

# Sub modules
add_subdirectory(${MAIN}/types)
target_include_directories(types INTERFACE ${MAIN})

add_subdirectory(${MAIN}/utils)
target_include_directories(utils INTERFACE ${MAIN})

add_subdirectory(${MAIN}/neuron)
target_include_directories(neuron PRIVATE ${MAIN})

# Link to submodules
target_link_libraries(goeta PRIVATE
    types
    utils
    neuron
)