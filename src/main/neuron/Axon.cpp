#include "Axon.h"

#include <iostream>
#include <string>

#include "types/structs/Cell.h"
#include "types/aliases.h"

#include "utils/common.h"


Axon::Axon() {};
Axon::Axon(int primalNodeLength, int endNodeLength) {
    this->primalNodeLength  = primalNodeLength;
    this->endNodeLength     = endNodeLength;
}

void Axon::seedData(std::string filename) {
    CSVReader reader(filename);

    Node primalNode;
    Node endNode;

    int rowIndex = 0;
    for (CSVRow& row: reader) {
        int columnIndex = 0;

        for (CSVColumn& column: row) {
            if (!columnIndex) {
                primalNode.conservativeResize(rowIndex + 1, this->primalNodeLength);
                endNode.conservativeResize(rowIndex + 1, this->endNodeLength);
            }

            if (columnIndex < this->primalNodeLength) {
                primalNode(rowIndex, columnIndex) = column.get<float>();
            }

            if (this->primalNodeLength <= columnIndex && columnIndex < row.size()) {
                endNode(rowIndex, (columnIndex - (this->endNodeLength + 1))) = column.get<float>();
            }

            columnIndex++;
        }

        rowIndex++;
    }

    this->primalNode    = primalNode;
    this->endNode       = endNode;
}

void Axon::addCell(int nodes) {
    if (!this->primalNode.size()) {
        throw std::runtime_error("empty primal node, please seed data first");
    }

    if (!this->endNode.size()) {
        throw std::runtime_error("empty end node, please seed data first");
    }

    Node node;
    Link weight;
    Link bias(1, nodes);

    int lastCellIndex = this->isolatedCells.size() - 1;

    if (this->isolatedCells.size() > 0) {
        weight = Link(this->isolatedCells.at(lastCellIndex).weight.cols(), nodes);
    } else {
        weight = Link(this->primalNodeLength, nodes);
    }

    weight.setRandom();
    bias.setConstant(1);

    this->isolatedCells.push_back(Cell{
        Node::Zero(1, 1),
        weight,
        bias,
        Node::Zero(1, 1),
    });

    this->adjustLastCell();
}

void Axon::adjustLastCell() {
    int lastCellIndex = this->isolatedCells.size() - 1;

    Link weight(this->isolatedCells.at(lastCellIndex).weight.cols(), this->endNodeLength);
    Link bias(1, this->endNodeLength);

    weight.setRandom();
    bias.setConstant(1);

    this->lastCell = Cell{
        Node::Zero(1, 1),
        weight,
        bias,
        Node::Zero(1, 1),
    };
}

Node Axon::getPrimalNode() {
    return this->primalNode;
}

Node Axon::getEndNode() {
    return this->endNode;
}

std::vector<Cell> Axon::getIsolatedCells() {
    return this->isolatedCells;
}

Cell Axon::getLastCell() {
    return this->lastCell;
}

void Axon::updateLastCell(Cell cell) {
    this->lastCell = cell;
}

void Axon::updateIsolatedCells(std::vector<Cell> cells) {
    this->isolatedCells = cells;
}

Axon::~Axon() {}