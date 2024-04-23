# declare library
set(NEURON ${MAIN}/neuron)

add_library(neuron
    ${NEURON}/Axon.cpp
    ${NEURON}/Neuron.cpp
)

target_link_libraries(neuron PRIVATE
    types
    utils
)