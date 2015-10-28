#include "BPLayer.h"

/*
 * Constructor
 * @param inputLayerNum
 *  the number of nodes in Input Layer
 * @param hiddenLayerNum
 *  the number of nodes in Hidden Layer
 * @param outputLayerNum
 *  the number of nodes in Output Layer
 * @param _learningRate
 *  the learning rate
 * @param _habitRate
 *  the habit rate
 */
BPLayer::BPLayer(int inputLayerNum, int hiddenLayerNum, int outputLayerNum, double _learningRate, double _habitRate) {
    InputLayerNum = inputLayerNum;
    HiddenLayerNum = hiddenLayerNum;
    OutputLayerNum = outputLayerNum;

    InputLayer = new Input[InputLayerNum];
    HiddenLayer = new Hidden[HiddenLayerNum];
    OutputLayer = new Output[OutputLayerNum];

    learningRate = _learningRate;
    habitRate = _habitRate;
}

/*
 * Initialize the weights
 */
void BPLayer::Initialize() {
    // Set Random seed by time
    srand((int)time(0));
    // Set Input Layers' weights ranged from -0.2 to 0.2
    for (int i = 0; i < InputLayerNum; i++) {
        InputLayer[i].weights = new double[HiddenLayerNum];
        for (int j = 0; j < HiddenLayerNum; j++) {
            InputLayer[i].weights[j] = (double)random(5) / 10 + -0.2;
        }
        InputLayer[i].weightDis = 0;
    }
    
    // Set Hidden Layers' weights ranged from -0.2 to 0.2
    for (int i = 0; i < HiddenLayerNum; i++) {
        HiddenLayer[i].weights = new double[OutputLayerNum];
        for (int j = 0; j < OutputLayerNum; j++) {
            HiddenLayer[i].weights[j] = (double)random(5) / 10 + -0.2;
        }
        HiddenLayer[i].weightDis = 0;
    }
}

/*
 * Forward Propagate
 * @param trainItem
 *  the item training
 */
void BPLayer::ForwardPropagate(Set trainItem) {
    // Set the Input Layer's value into train data
    for (int i = 0; i < InputLayerNum; i++) {
        InputLayer[i].value = trainItem.data[i];
    }

    // Set the Output Layer's target :
    // the target number : 0.9
    // other number : 0.1
    for (int i = 0; i < OutputLayerNum; i++) {
        if (i == trainItem.num) {
            OutputLayer[i].target = 0.9;
        } else {
            OutputLayer[i].target = 0.1;
        }
    }

    double total;
    // Calculate the input and output of Hidden Layer
    for (int i = 0; i < HiddenLayerNum; i++) {
        total = 0;
        for (int j = 0; j < InputLayerNum; j++) {
            total += InputLayer[j].value * InputLayer[j].weights[i];
        }
        HiddenLayer[i].input = total;
        HiddenLayer[i].output = G(total);
    }

    // Calculate the input, output, error and back of Output Layer
    for (int i = 0; i < OutputLayerNum; i++) {
        total = 0;
        for (int j = 0; j < HiddenLayerNum; j++) {
            total += HiddenLayer[j].output * HiddenLayer[j].weights[i];
        }
        OutputLayer[i].input = total;
        OutputLayer[i].output = G(total);
        // Calculate the error -> (Target - Output)
        OutputLayer[i].error = OutputLayer[i].target - OutputLayer[i].output;
        // Calculate the back  -> (error * G'(output))
        OutputLayer[i].back = OutputLayer[i].error * (OutputLayer[i].output) * (1 - OutputLayer[i].output);
    }
}

/*
 * Back Propagate
 * @Output Back Propagation
 *  1. ^W[jk](n) = a * x[k] * V[j] + u * ^W[jk](n - 1)
 *  2. x[k] = (T[k] - V[k]) * G'(V[k])
 *  @function
 *   Fix the weights between Output Layer and Hidden Layer
 *  @param
 *   1. n -> the n's backPropagation
 *   2. j -> the j's hidden node
 *   3. k -> the k's output node
 *   4. a -> learning rate
 *   5. u -> habit rate
 *   6. ^W[jk](n) -> the n's weightDis between hidden node[j] and Output node[k]
 *   7. x[k] -> the k's output node's back
 *   8. V[j] -> the j's hidden node's output
 *
 * @Hidden Back Propagation
 *   1. ^W[ij](n) = a * x[j] * V[i] + u * ^W[ij](n - 1)
 *   2. x[j] = G'(V[j]) * sum{k}(x[k] * W[jk])
 *   @function
 *    Fix the weights between current Hidden Layer and the Layer above
 *   @param
 *    1. i -> the i's layer node above(here is Input Layer)
 *    2. j -> the j's hidden node(now fixing)
 *    3. k -> the k's layer node follow(here is Output Layer)
 *    4. ^W[ij](n) -> the n's weightDis between Input node[i] and Hidden node[j]
 *    5. x[j] -> the j's hidden node's back
 *    6. V[i] -> the i's input node's output(value)
 *    7. sum{k}(x[k] * W[jk]) -> the sum of { all (k's back * W[jk]) }
 */
void BPLayer::BackPropagate() {
    double total;
    // Calculate the error and back of Hidden Layer
    for (int i = 0; i < HiddenLayerNum; i++) {
        total = 0;
        for (int j = 0; j < OutputLayerNum; j++) {
            total += HiddenLayer[i].weights[j] * OutputLayer[j].back;
        }
        HiddenLayer[i].error = total;
        HiddenLayer[i].back = HiddenLayer[i].error * (HiddenLayer[i].output) * (1 - HiddenLayer[i].output);
    }

    double temp;
    // Fix the weights between Output Layer and Hidden Layer
    // and restore the weightDis
    for (int i = 0; i < OutputLayerNum; i++) {
        for (int j = 0; j < HiddenLayerNum; j++) {
            temp = learningRate * OutputLayer[i].back * HiddenLayer[j].output
                   + habitRate * HiddenLayer[j].weightDis;
            HiddenLayer[j].weights[i] += temp;
            HiddenLayer[j].weightDis = temp;
        }
    }
    
    // Fix the weights between Hidden Layer and Input Layer
    // and restore the weightDis
    for (int i = 0; i < HiddenLayerNum; i++) {
        for (int j = 0; j < InputLayerNum; j++) {
            temp = learningRate * HiddenLayer[i].back * InputLayer[j].value
                   + habitRate * InputLayer[j].weightDis;
            InputLayer[j].weights[i] += temp; 
            InputLayer[j].weightDis = temp;
        }
    }
    
}

/*
 * Recognize the Digit
 * @param testCase
 *  the item recognizing
 * @return result
 *  the recognization result
 */
Result BPLayer::Recognize(Set testCase) {
    double total;
    double max = -1;
    double resultRate; // store the max recognization rate
    int resultNumber;  // store the max recognization number

    // Input the testing data
    for (int i = 0; i < InputLayerNum; i++) {
        InputLayer[i].value = testCase.data[i];
    }
    
    // Forward Propagation to Hidden Layer
    for (int i = 0; i < HiddenLayerNum; i++) {
        total = 0;
        for (int j = 0; j < InputLayerNum; j++) {
            total += InputLayer[j].value * InputLayer[j].weights[i];
        }
        HiddenLayer[i].input = total;
        HiddenLayer[i].output = G(total);
    }
    
    // Forward Propagation to Output Layer
    for (int i = 0; i < OutputLayerNum; i++) {
        total = 0;
        for (int j = 0; j < HiddenLayerNum; j++) {
            total += HiddenLayer[j].output * HiddenLayer[j].weights[i];
        }
        OutputLayer[i].input = total;
        OutputLayer[i].output = G(total);
        // Store the max number and rate
        if (OutputLayer[i].output > max) {
            max = OutputLayer[i].output;
            resultNumber = i;
            resultRate = OutputLayer[i].output;
        }
    }
    // return the result
    Result a = Result(resultNumber, resultRate);
    return a;
}

/*
 * Set the Netword's hidden node number and all weights
 * @param _hiddenNum
 *  the hidden node number
 * @param _input
 *  the input layer
 * @param _hidden
 *  the hidden layer
 */
void BPLayer::Setting(int _hiddenNum, Input* _input, Hidden* _hidden) {
    /*I can't delete this*/
    for (int i = 0; i < InputLayerNum; i++) {
        delete [] InputLayer[i].weights;
    }
    
    for (int i = 0; i < HiddenLayerNum; i++) {
        delete [] HiddenLayer[i].weights;
    }
    HiddenLayerNum = _hiddenNum;
    
    // Set the Input Layer's weights point to Hidden Layer
    for (int i = 0; i < InputLayerNum; i++) {
        InputLayer[i].weights = new double[HiddenLayerNum];
        for (int j = 0; j < HiddenLayerNum; j++) {
            InputLayer[i].weights[j] = _input[i].weights[j];
        }
    }
    
    // Set the Hidden Layer's weights point to Output Layer
    for (int i = 0; i < HiddenLayerNum; i++) {
        HiddenLayer[i].weights = new double[OutputLayerNum];
        for (int j = 0; j < OutputLayerNum; j++) {
            HiddenLayer[i].weights[j] = _hidden[i].weights[j];
        }
    }
}

/*
 * Sigmoid Function
 */
double BPLayer::G(double x) {
    return (1 / (1 + exp(-x)));
}

/*
 * Squared Error of Network
 */
double BPLayer::GetError() {
    double total = 0;
    for (int i = 0; i < OutputLayerNum; i++) {
        total += pow((OutputLayer[i].target - OutputLayer[i].output), 2) / 2;
    } 
    return total;
}

/*
 * Return node number of Input Layer
 */
int BPLayer::GetInputLayerNum() {
    return InputLayerNum;
}

/*
 * Return node number of Hidden Layer
 */
int BPLayer::GetHiddenLayerNum() {
    return HiddenLayerNum;
}

/*
 * Return node number of Output Layer
 */
int BPLayer::GetOutputLayerNum() {
    return OutputLayerNum;
}

/*
 * Return Input Layer
 */
Input* BPLayer::GetInput() {
    return InputLayer;
}

/*
 * Return Hidden Layer
 */
Hidden* BPLayer::GetHidden() {
    return HiddenLayer;
}

/*
 * Return Output Layer
 */
Output* BPLayer::GetOutput() {
    return OutputLayer;
}
