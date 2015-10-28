#include "BPBase.h"

/* 
 * @class BPLayer
 *  Back Propagation Neural Networks
 *  which Input Layer, Hidden Layer,
 *  Output Layer
 * @function
 *  Recognize the handwritten digis
 */
class BPLayer {
    public:
        // Constructor
        BPLayer(int, int, int, double, double);
        
        // Training and Recognizing
        void Initialize();
        void ForwardPropagate(Set);
        void BackPropagate();
        Result Recognize(Set);
        void Setting(int, Input*, Hidden*);
        
        // Sigmoid Function and Squared Error Function
        double G(double);
        double GetError();
        
        // Get Nodes Number of Layers
        int GetInputLayerNum();
        int GetHiddenLayerNum();
        int GetOutputLayerNum();
        
        // Get Layers
        Input* GetInput();
        Hidden* GetHidden();
        Output* GetOutput();

    private:
        int InputLayerNum;  // Number of Input Layer Node
        int HiddenLayerNum; // Number of Hidden Layer Node
        int OutputLayerNum; // Number of Output Layer Node

        double learningRate;// Learning Rate
        double habitRate;   // Habit Rate

        Input* InputLayer;  // Input Layer Nodes
        Hidden* HiddenLayer;// Hidden Layer Nodes
        Output* OutputLayer;// Output Layer nodes
};

