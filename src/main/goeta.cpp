#include <iostream>

#include "neuron/Neuron.h"
#include "neuron/Axon.h"

#include "types/aliases.h"


int main(int argc, char *argv[]) {
    try {
        #pragma region learningPhase
            Neuron learningNeuron(5000, 0.3);
            Axon learningAxon(4, 3);

            learningAxon.seedData("./src/resources/train.csv");
            learningAxon.addCell(5);

            learningNeuron.attach(learningAxon);
            learningNeuron.learn();
        #pragma endregion

        #pragma region guessingPhase
            Axon guessAxon(4, 3);
            guessAxon.seedData("./src/resources/test.csv");

            Neuron guessNeuron;
            guessNeuron.attach(learningAxon);

            Node guessResult    = guessNeuron.guess(guessAxon);
        #pragma endregion

        #pragma region compareResult
            Node endData        = guessAxon.getEndNode();

            float successResult = 0;
            for (int row = 0; row < endData.rows(); row++) {

                int actualResultIdx = 0;
                for (int col = 0; col < endData.row(row).cols(); col++) {
                    if (endData(row, col) > 0) {
                        actualResultIdx = col;
                    }
                }

                if (guessResult(row, actualResultIdx) == guessResult.row(row).maxCoeff()) {
                    successResult++;
                }
            }

            std::cout << "accuracy: " << (successResult / guessResult.rows()) << std::endl;
        #pragma endregion

        return 0;
    } catch (std::exception& e) {
        std::cerr << e.what();
        return 1;
    }
    
    return 0;
}