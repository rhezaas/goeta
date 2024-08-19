#ifndef AXON_H
#define AXON_H

#include <vector>
#include <string>

#include "types/structs/Cell.h"
#include "types/aliases.h"


class Axon {
    private:
        Node inputNode;
        int inputNodeLength;

        Node outputNode;
        int outputNodeLength;

        std::vector<Cell> isolatedCells;
        Cell lastCell;

        void adjustLastCell();

    public:
        Axon();
        Axon(int inputNodeLength, int outputNodeLength);

        void seedData(std::string filename);
        void addCell(int nodes);

        Node getInputNode();
        Node getOutputNode();
        std::vector<Cell> getIsolatedCells();
        Cell getLastCell();

        void updateLastCell(Cell cell);
        void updateIsolatedCells(std::vector<Cell> cells);

        ~Axon();
};

#endif