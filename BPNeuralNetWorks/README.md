Artificial-Intelligence Project - BP Neural NetWork
==========================

Basic Functions
-----------------
1. Assign the number of neurons in hidden layers, the learning rate and the momentum. Initialize the neural network. Train and test the network accuracy.
2. Import the best parameters including the weights from `data/result.txt`.

1. 指定隐藏层节点数、学习率以及惯性系数，初始化神经网络，使用训练数据训练后使用测试数据测试，并输出正确率
2. 从data/result.txt导入训练得到的最佳的参数（包括边的权重），使用测试数据测试，输出最佳的正确率

Dependencies
------------------
1. ubuntu 14.10
2. g++ based on gcc v4.9.1


Compile
------------------
use `make` command in the root

在根目录下执行命令`make`


Run
------------------
`./run.sh [a] [b] [c] [d]`

###Parameters
* a - mode, 0 means `train mode`(default), 1 means `set mode`(use the best setting after training)
* b - number of neuron in hidden layer(167 default)
* c - learning rate(0.1 default)
* d - momentum(0.4 default)

* a - 模式, 0为训练模式(默认), 1为配置模式(使用当前最好的神经网络配置)
* b - 隐藏层数目(默认167)
* c - 学习系数(默认0.1)
* d - 惯性系数(默认0.4)
    

Examples(under root directory)
-------------------
1. train mode(训练模式)
`./run.sh 0 167 0.1 0.4`
2. seting mode(配置模式)
`./run.sh 1`


Details
------------------
Since the conditions is strict, you can run the script when testing.

由于对平台要求相对较高,故测试时可以直接通过运行脚本运行可执行文件.
