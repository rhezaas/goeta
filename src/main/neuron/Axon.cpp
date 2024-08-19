#include "Axon.h"

#include <iostream>
#include <string>

#include "types/structs/Cell.h"
#include "types/aliases.h"

#include "utils/common.h"


Axon::Axon() {};
Axon::Axon(int inputNodeLength, int outputNodeLength) {
    this->inputNodeLength  = inputNodeLength;
    this->outputNodeLength     = outputNodeLength;
}

void Axon::seedData(std::string filename) {
    CSVReader reader(filename);

    Node inputNode;
    Node outputNode;

    int rowIndex = 0;
    for (CSVRow& row: reader) {
        int columnIndex = 0;

        for (CSVColumn& column: row) {
            if (!columnIndex) {
                inputNode.conservativeResize(rowIndex + 1, this->inputNodeLength);
                outputNode.conservativeResize(rowIndex + 1, this->outputNodeLength);
            }

            if (columnIndex < this->inputNodeLength) {
                inputNode(rowIndex, columnIndex) = column.get<float>();
            }

            if (this->inputNodeLength <= columnIndex && columnIndex < row.size()) {
                outputNode(rowIndex, (columnIndex - (this->outputNodeLength + 1))) = column.get<float>();
            }

            columnIndex++;
        }

        rowIndex++;
    }

    this->inputNode    = inputNode;
    this->outputNode       = outputNode;
}

void Axon::addCell(int nodes) {
    if (!this->inputNode.size()) {
        throw std::runtime_error("empty primal node, please seed data first");
    }

    if (!this->outputNode.size()) {
        throw std::runtime_error("empty end node, please seed data first");
    }

    Node node;
    Link weight;
    Link bias(1, nodes);

    int lastCellIndex = this->isolatedCells.size() - 1;

    if (this->isolatedCells.size() > 0) {
        weight = Link(this->isolatedCells.at(lastCellIndex).weight.cols(), nodes);
    } else {
        weight = Link(this->inputNodeLength, nodes);
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

    Link weight(this->isolatedCells.at(lastCellIndex).weight.cols(), this->outputNodeLength);
    Link bias(1, this->outputNodeLength);

    weight.setRandom();
    bias.setConstant(1);

    this->lastCell = Cell{
        Node::Zero(1, 1),
        weight,
        bias,
        Node::Zero(1, 1),
    };
}

Node Axon::getInputNode() {
    return this->inputNode;
}

Node Axon::getOutputNode() {
    return this->outputNode;
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