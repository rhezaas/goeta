#ifndef AXON_H
#define AXON_H

#include <vector>
#include <string>

#include "types/structs/Cell.h"
#include "types/aliases.h"


class Axon {
    private:
        Node primalNode;
        int primalNodeLength;

        Node endNode;
        int endNodeLength;

        std::vector<Cell> isolatedCells;
        Cell lastCell;

        void adjustLastCell();

    public:
        Axon();
        Axon(int primalNodeLength, int endNodeLength);

        void seedData(std::string filename);
        void addCell(int nodes);

        Node getPrimalNode();
        Node getEndNode();
        std::vector<Cell> getIsolatedCells();
        Cell getLastCell();

        void updateLastCell(Cell cell);
        void updateIsolatedCells(std::vector<Cell> cells);

        ~Axon();
};

#endif