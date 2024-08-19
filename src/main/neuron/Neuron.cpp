#include "Neuron.h"

#include <vector>
#include <string>

#include "neuron/Axon.h"

#include "types/aliases.h"
#include "utils/common.h"


Neuron::Neuron() {}
Neuron::Neuron(int epoch, float learningRate) {
    this->epoch         = epoch;
    this->learningRate  = learningRate;
};

void Neuron::attach(Axon &learningAxon) {
    this->learningAxon = &learningAxon;
}

void Neuron::feedForward(Node inputNode, std::vector<Cell> &isolatedCell) {
    Node node                               = (inputNode * isolatedCell.at(0).weight) 
                                                + isolatedCell.at(0).bias.replicate(inputNode.rows(), 1);
    isolatedCell.at(0).node                 = node.array().unaryExpr(&sigmoid);

    for (int cellIndex = 1; cellIndex < isolatedCell.size(); cellIndex++) {
        node                                = (isolatedCell.at(cellIndex - 1).node * isolatedCell.at(cellIndex).weight)
                                                + isolatedCell.at(cellIndex).bias.replicate(isolatedCell.at(cellIndex - 1).node.rows(), 1);
        isolatedCell.at(cellIndex).node     = node.array().unaryExpr(&sigmoid);
    }
}

void Neuron::backProp(std::vector<Cell> &isolatedCell) {
    int cellIndex                       = isolatedCell.size() - 1;
    Link errorSlope                     = this->learningAxon->getOutputNode() - isolatedCell.at(cellIndex).node;
    Link delta                          = isolatedCell.at(cellIndex).node.array().unaryExpr(&sigmoidPrime);
    isolatedCell.at(cellIndex).delta    = errorSlope.cwiseProduct(delta);

    cellIndex--;

    while (-1 < cellIndex) {
        errorSlope                          = isolatedCell.at(cellIndex + 1).delta * isolatedCell.at(cellIndex + 1).weight.transpose();
        delta                               = isolatedCell.at(cellIndex).node.array().unaryExpr(&sigmoidPrime);
        isolatedCell.at(cellIndex).delta    = errorSlope.cwiseProduct(delta);

        cellIndex--;
    }
}

void Neuron::linkAdjustment(std::vector<Cell> &isolatedCell) {
    Link weight                 = (this->learningAxon->getInputNode().transpose() * isolatedCell.at(0).delta) * this->learningRate;
    isolatedCell.at(0).weight   = isolatedCell.at(0).weight + weight;

    Link bias                   = isolatedCell.at(0).delta.colwise().sum() * this->learningRate;
    isolatedCell.at(0).bias     = isolatedCell.at(0).bias + bias;

    for (int cellIndex = 1; cellIndex < isolatedCell.size(); cellIndex++) {
        Link weight                         = (isolatedCell.at(cellIndex - 1).node.transpose() * isolatedCell.at(cellIndex).delta) * this->learningRate;
        isolatedCell.at(cellIndex).weight   = isolatedCell.at(cellIndex).weight + weight;

        Link bias                           = isolatedCell.at(cellIndex).delta.colwise().sum() * this->learningRate;
        isolatedCell.at(cellIndex).bias     = isolatedCell.at(cellIndex).bias + bias;
    }
}

void Neuron::learn() {
    std::vector<Cell> isolatedCell = this->learningAxon->getIsolatedCells();
    isolatedCell.push_back(this->learningAxon->getLastCell());

    if (!this->epoch) {
        throw std::runtime_error("learning cannot start without epoch size");
    }

    if (!this->learningRate) {
        throw std::runtime_error("learning cannot start without learning rate");
    }

    if (!isolatedCell.size()) {
        throw std::runtime_error("empty isolated cell, please add cell first then attach to neuron");
    }

    for (int i = 0; i < this->epoch; i++) {
        this->feedForward(this->learningAxon->getInputNode(), isolatedCell);
        this->backProp(isolatedCell);

        this->linkAdjustment(isolatedCell);
    }

    this->learningAxon->updateIsolatedCells(isolatedCell);
}

Node Neuron::guess(Axon guessAxon) {
    std::vector<Cell> isolatedCell = this->learningAxon->getIsolatedCells();

    if (!isolatedCell.size()) {
        throw std::runtime_error("empty learning axon, please attach learnt axon");
    }

    this->feedForward(guessAxon.getInputNode(), isolatedCell);
    return isolatedCell.at(isolatedCell.size() - 1).node;
}

Neuron::~Neuron() {};