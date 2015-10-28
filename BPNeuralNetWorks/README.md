Artificial-Intelligence Project - BP Neural NetWork
==========================

Basic Functions
-----------------
1. 指定隐藏层节点数、学习率以及惯性系数，初始化神经网络，使用训练数据训练后使用测试数据测试，并输出正确率
2. 从data/result.txt导入训练得到的最佳的参数（包括边的权重），使用测试数据测试，输出最佳的正确率

Dependencies
------------------
1. ubuntu 14.10
2. g++ based on gcc v4.9.1


Compile
------------------
在根目录下执行命令`make`


Run
------------------
`./run.sh [a] [b] [c] [d]`

###Parameters
* a - 模式, 0为训练模式(默认), 1为配置模式(使用当前最好的神经网络配置)
* b - 隐藏层数目(默认167)
* c - 学习系数(默认0.1)
* d - 惯性系数(默认0.4)
    

Examples(under root directory)
-------------------
1. 训练模式
`./run.sh 0 167 0.1 0.4`
2. 配置模式
`./run.sh 1`


Details
------------------
由于对平台要求相对较高,故测试时可以直接通过运行脚本运行可执行文件.
