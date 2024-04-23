# declare library
set(TYPES ${MAIN}/types)

add_library(types INTERFACE
    ${TYPES}/structs/Cell.h

    ${TYPES}/aliases.h
)