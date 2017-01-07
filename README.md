# FreeWill

![futurama freewill unit](splash.jpg)

![doc build badge](https://readthedocs.org/projects/freewill/badge/?version=latest)

FreeWill is a deeplearning library implemented in C++. The purpose of writing FreeWill is for me to understand deeplearning in detail. In addition to the library itself, I will try to write detailed document and examples.

There are few areas interest me:

TEST

LSTM
Reinforcement Learning

The implementation is purely on CPU, but later I may extend it to GPU.
I'm also interested in creating a UI for this library.

The first goal of this project is matching https://github.com/karpathy/convnetjs feature wise.

To build:
CMAKE_PREFIX_PATH=/home/shiy/Qt/5.7/gcc_64 cmake .. -DCMAKE_BUILD_TYPE=debug