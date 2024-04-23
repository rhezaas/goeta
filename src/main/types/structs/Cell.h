#ifndef CELL_H
#define CELL_H

#include "types/aliases.h"


struct Cell {
    Node node;
    Link weight;
    Link bias;
    Link delta;
};

#endif