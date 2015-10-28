#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>
using namespace std;

#define random(x) (rand() % x)
#define LINESIZE 160
#define SETSIZE 64
#define LEARNINGRATE 0.1
#define HABITRATE 0.4

// Data Item
struct Set {
    int num;
    int data[SETSIZE];
};

/* Input Layer Node
 * @param value
 *  the input value
 * @param weights
 *  the weights point to every Hidden Layer Node
 * @param weightDis
 *  the distance between every weight change,
 *  first initializes as 0  
 */
struct Input {
    double value;
    double* weights;
    double weightDis;
};

/* Hidden Layer Node
 * @param input
 *  the input value received from Input Layer
 * @param output
 *  the output value transfered by sigmoid function
 * @param weights
 *  the weights point to every Output Layer Node
 * @param weightDis
 *  the distance between every weight change,
 *  first initializes as 0  
 * @param error
 *  the distance between target and output
 * @param back
 *  the value calculated by error * sigmoid function's \
 *  Derivative(output), which will return to Input Layer
 */
struct Hidden{
    double input;
    double output;
    double* weights;
    double weightDis;
    double error;
    double back;
};

/* Output Layer Node
 * @param input
 *  the input value received from Hidden Layer
 * @param output
 *  the output value transfered by sigmoid function
 * @param error
 *  the distance between target and output
 * @param target
 *  the target output
 * @param back
 *  the value calculated by error * sigmoid function's \
 *  Derivative(output), which will return to Hidden Layer
 */
struct Output {
    double input;
    double output;
    double error;
    double target;
    double back;
};

/* Result after recognization
 * @param number
 *  the number recognized
 * @param rate
 *  the recognizing rate of the number
 */
struct Result {
    int number;
    double rate;
    Result(int _number, double _rate) {
        number = _number;
        rate = _rate;
    }
};
