#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <string>

#include "neuron/Axon.h"
#include "types/aliases.h"


class Neuron {
    private:
        int     epoch           = 0;
        float   learningRate    = 0.0f;
        Axon    *learningAxon;

        void    feedForward(Node inputNode, std::vector<Cell> &isolatedCell);
        void    backProp(std::vector<Cell> &isolatedCell);
        void    linkAdjustment(std::vector<Cell> &isolatedCell);

    public:
        Neuron();
        Neuron(int epoch, float learningRate);

        void attach(Axon &learningAxon);
        void learn();
        Node guess(Axon guessAxon);

        ~Neuron();
};

#endif