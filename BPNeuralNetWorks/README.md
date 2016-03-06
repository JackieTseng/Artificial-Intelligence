Artificial-Intelligence Project - BP Neural NetWork
==========================

Basic Functions
-----------------
1. Assign the number of neurons in hidden layers, the learning rate and the momentum. Initialize the neural network. Train and test the network accuracy.
2. Import the best parameters including the weights from `data/result.txt`.

Dependencies
------------------
1. ubuntu 14.10
2. g++ based on gcc v4.9.1


Compile
------------------
use `make` command in the root.


Run
------------------
`./run.sh [a] [b] [c] [d]`

###Parameters
* a - mode, 0 means `train mode`(default), 1 means `set mode`(use the best setting after training)
* b - number of neuron in hidden layer(167 default)
* c - learning rate(0.1 default)
* d - momentum(0.4 default)
    

Examples(under root directory)
-------------------
1. train mode
`./run.sh 0 167 0.1 0.4`

2. seting mode
`./run.sh 1`


Details
------------------
Since the conditions is strict, you can run the script when testing.
