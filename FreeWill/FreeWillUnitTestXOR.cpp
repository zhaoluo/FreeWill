#include "FreeWillUnitTest.h"
#include "Tensor/Tensor.h"
#include "Operator/DotProductWithBias.h"
#include "Operator/DotProductWithBiasDerivative.h"
#include "Operator/Activation.h"
#include "Operator/ActivationDerivative.h"
#include "Operator/CrossEntropyLoss.h"
#include "Operator/SigmoidCrossEntropyLossDerivative.h"
#include "Operator/ElementwiseAdd.h"
#include "Operator/ElementwiseProduct.h"

void FreeWillUnitTest::xorTest()
{
    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> input({2,4});
    input.init();

    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> firstLayerActivation({2 ,4});
    firstLayerActivation.init();

    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> firstLayerNeuronDerivative({2, 4});
    firstLayerNeuronDerivative.init();

    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> secondLayerActivation({1,4});
    secondLayerActivation.init();
   
    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> secondLayerNeuronDerivative({1,4});
    secondLayerNeuronDerivative.init(); 
    
    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> cost({4});
    cost.init();

    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> label({1, 4});
    label.init();

    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> firstLayerWeight({2,2});
    firstLayerWeight.init();
    firstLayerWeight.randomize();

    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> firstLayerBias({2});
    firstLayerBias.init();
    firstLayerBias.randomize();

    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> firstLayerWeightDerivative({2,2});
    firstLayerWeightDerivative.init();

    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> firstLayerBiasDerivative({2});
    firstLayerBiasDerivative.init();

    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> secondLayerWeight({1, 2});
    secondLayerWeight.init();
    secondLayerWeight.randomize();

    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> secondLayerBias({1});
    secondLayerBias.init();
    secondLayerBias.randomize();
 
    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> secondLayerWeightDerivative({1,2});
    secondLayerWeightDerivative.init();

    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> secondLayerBiasDerivative({1});
    secondLayerBiasDerivative.init();

    FreeWill::Tensor<FreeWill::CPU_NAIVE, float> inputNeuronDerivative({2,4});
    inputNeuronDerivative.init();

    FreeWill::DotProductWithBias<FreeWill::CPU_NAIVE, float> firstLayerFullyConnected(true);
    firstLayerFullyConnected.setInputParameter("Input", &input);
    firstLayerFullyConnected.setInputParameter("Weight", &firstLayerWeight);
    firstLayerFullyConnected.setInputParameter("Bias", &firstLayerBias);
    firstLayerFullyConnected.setOutputParameter("Output", &firstLayerActivation);
    
    QVERIFY(firstLayerActivation.init());

    FreeWill::Activation<FreeWill::SIGMOID, FreeWill::CPU_NAIVE, float> firstLayerSigmoid;
    firstLayerSigmoid.setInputParameter("Input", &firstLayerActivation);
    firstLayerSigmoid.setOutputParameter("Output", &firstLayerActivation);
    
    QVERIFY(firstLayerSigmoid.init());

    FreeWill::DotProductWithBias<FreeWill::CPU_NAIVE, float> secondLayerFullyConnected(true);
    secondLayerFullyConnected.setInputParameter("Input", &firstLayerActivation);
    secondLayerFullyConnected.setInputParameter("Weight", &secondLayerWeight);
    secondLayerFullyConnected.setInputParameter("Bias", &secondLayerBias);
    secondLayerFullyConnected.setOutputParameter("Output", &secondLayerActivation);

    QVERIFY(secondLayerFullyConnected.init());

    FreeWill::Activation<FreeWill::SIGMOID, FreeWill::CPU_NAIVE, float> secondLayerSigmoid;
    secondLayerSigmoid.setInputParameter("Input", &secondLayerActivation);
    secondLayerSigmoid.setOutputParameter("Output", &secondLayerActivation);

    QVERIFY(secondLayerSigmoid.init());

    FreeWill::CrossEntropyLoss<FreeWill::CPU_NAIVE, float> crossEntropyLoss;
    crossEntropyLoss.setInputParameter("Input", &secondLayerActivation);
    crossEntropyLoss.setInputParameter("Label", &label);
    crossEntropyLoss.setOutputParameter("Cost", &cost);

    QVERIFY(crossEntropyLoss.init());

    FreeWill::SigmoidCrossEntropyLossDerivative<FreeWill::CPU_NAIVE, float> sigmoidCrossEntropyLossDerivative;
    sigmoidCrossEntropyLossDerivative.setInputParameter("Input", &secondLayerActivation);
    sigmoidCrossEntropyLossDerivative.setInputParameter("Label", &label);
    sigmoidCrossEntropyLossDerivative.setOutputParameter("Output", &secondLayerNeuronDerivative);

    QVERIFY(sigmoidCrossEntropyLossDerivative.init());

    FreeWill::DotProductWithBiasDerivative<FreeWill::CPU_NAIVE, float> secondLayerDotProductWithBiasDerivative(true);
    secondLayerDotProductWithBiasDerivative.setInputParameter("InputActivation", &firstLayerActivation);
    secondLayerDotProductWithBiasDerivative.setInputParameter("OutputDelta", &secondLayerNeuronDerivative);
    secondLayerDotProductWithBiasDerivative.setInputParameter("Weight", &secondLayerWeight);

    secondLayerDotProductWithBiasDerivative.setOutputParameter("WeightGrad", &secondLayerWeightDerivative);
    secondLayerDotProductWithBiasDerivative.setOutputParameter("BiasGrad", &secondLayerBiasDerivative);
    secondLayerDotProductWithBiasDerivative.setOutputParameter("InputDelta", &firstLayerNeuronDerivative);

    QVERIFY(secondLayerDotProductWithBiasDerivative.init());

    FreeWill::ActivationDerivative<FreeWill::SIGMOID, FreeWill::CPU_NAIVE, float> firstLayerSigmoidDerivative;
    firstLayerSigmoidDerivative.setInputParameter("Output", &firstLayerActivation);
    firstLayerSigmoidDerivative.setInputParameter("OutputDelta", &firstLayerNeuronDerivative);
    firstLayerSigmoidDerivative.setOutputParameter("InputDelta", &firstLayerNeuronDerivative);

    QVERIFY(firstLayerSigmoidDerivative.init());

    FreeWill::DotProductWithBiasDerivative<FreeWill::CPU_NAIVE, float> firstLayerDotProductWithBiasDerivative(true);
    firstLayerDotProductWithBiasDerivative.setInputParameter("InputActivation", &input);
    firstLayerDotProductWithBiasDerivative.setInputParameter("OutputDelta", &firstLayerNeuronDerivative);
    firstLayerDotProductWithBiasDerivative.setInputParameter("Weight", &firstLayerWeight);

    firstLayerDotProductWithBiasDerivative.setOutputParameter("WeightGrad", &firstLayerWeightDerivative);
    firstLayerDotProductWithBiasDerivative.setOutputParameter("BiasGrad", &firstLayerBiasDerivative);
    firstLayerDotProductWithBiasDerivative.setOutputParameter("InputDelta", &inputNeuronDerivative);
    
    QVERIFY(firstLayerDotProductWithBiasDerivative.init()); 

    float learningRate = 0.02;
    FreeWill::ElementwiseAdd<FreeWill::CPU_NAIVE, float> mergeWithFirstLayerWeight(-learningRate);
    mergeWithFirstLayerWeight.setInputParameter("Operand", &firstLayerWeight);
    mergeWithFirstLayerWeight.setInputParameter("Operand", &firstLayerWeightDerivative);
    mergeWithFirstLayerWeight.setOutputParameter("Result", &firstLayerWeight);
    QVERIFY(mergeWithFirstLayerWeight.init());

    FreeWill::ElementwiseAdd<FreeWill::CPU_NAIVE, float> mergeWithFirstLayerBias(-learningRate);
    mergeWithFirstLayerBias.setInputParameter("Operand", &firstLayerBias);
    mergeWithFirstLayerBias.setInputParameter("Operand", &firstLayerBiasDerivative);
    mergeWithFirstLayerBias.setOutputParameter("Result", &firstLayerBias);
    QVERIFY(mergeWithFirstLayerBias.init());

    FreeWill::ElementwiseAdd<FreeWill::CPU_NAIVE, float> mergeWithSecondLayerWeight(-learningRate);
    mergeWithSecondLayerWeight.setInputParameter("Operand", &secondLayerWeight);
    mergeWithSecondLayerWeight.setInputParameter("Operand", &secondLayerWeightDerivative);
    mergeWithSecondLayerWeight.setOutputParameter("Result", &secondLayerWeight);
    QVERIFY(mergeWithSecondLayerWeight.init());

    FreeWill::ElementwiseAdd<FreeWill::CPU_NAIVE, float> mergeWithSecondLayerBias(-learningRate);
    mergeWithSecondLayerBias.setInputParameter("Operand", &secondLayerBias);
    mergeWithSecondLayerBias.setInputParameter("Operand", &secondLayerBiasDerivative);
    mergeWithSecondLayerBias.setOutputParameter("Result", &secondLayerBias);
    QVERIFY(mergeWithSecondLayerBias.init());

    for (int e = 0;e<4;++e)
    {
        int a = e & 0x1;
        int b = (e >> 1) & 0x1;
        int c = a^b;
        input[2*e + 0] = a;
        input[2*e + 1] = b;
        label[e+0] = c;
    }



    for(unsigned int i = 1; i< 250000; ++i)
    {
        firstLayerFullyConnected.evaluate();

        firstLayerSigmoid.evaluate();

        secondLayerFullyConnected.evaluate();
        secondLayerSigmoid.evaluate();
        crossEntropyLoss.evaluate();
        secondLayerWeightDerivative.clear();
        secondLayerBiasDerivative.clear();

        firstLayerWeightDerivative.clear();
        firstLayerBiasDerivative.clear();
        
        sigmoidCrossEntropyLossDerivative.evaluate();
        secondLayerDotProductWithBiasDerivative.evaluate();
        firstLayerSigmoidDerivative.evaluate();
        firstLayerDotProductWithBiasDerivative.evaluate();
       
        if (i%500000 == 0 && i!=0)
        {
           learningRate*=0.5;
        }
           
        mergeWithFirstLayerWeight.setRate(-learningRate );
        mergeWithFirstLayerBias.setRate(-learningRate );
        mergeWithSecondLayerWeight.setRate(-learningRate );
        mergeWithSecondLayerBias.setRate(-learningRate );
        mergeWithFirstLayerWeight.evaluate();
        mergeWithFirstLayerBias.evaluate();
        mergeWithSecondLayerWeight.evaluate();
        mergeWithSecondLayerBias.evaluate();
    }

    qDebug() << "cost:" << cost[0];

    firstLayerFullyConnected.evaluate();
    firstLayerSigmoid.evaluate();
    secondLayerFullyConnected.evaluate();
    secondLayerSigmoid.evaluate();

    for (int i = 0; i<4; ++i)
    {
        qDebug() << "test" << i << ": a" << input[i*2] << "b" << input[i*2+1] << "c" << label[i] << "nn result:" << secondLayerActivation[i];
    }
}

void FreeWillUnitTest::xorTestGPU()
{
    FreeWill::Tensor<FreeWill::GPU_CUDA, float> input({2,4});
    input.init();

    FreeWill::Tensor<FreeWill::GPU_CUDA, float> firstLayerActivation({2 ,4});
    firstLayerActivation.init();

    FreeWill::Tensor<FreeWill::GPU_CUDA, float> firstLayerNeuronDerivative({2, 4});
    firstLayerNeuronDerivative.init();

    FreeWill::Tensor<FreeWill::GPU_CUDA, float> secondLayerActivation({1,4});
    secondLayerActivation.init();
   
    FreeWill::Tensor<FreeWill::GPU_CUDA, float> secondLayerNeuronDerivative({1,4});
    secondLayerNeuronDerivative.init(); 
    
    FreeWill::Tensor<FreeWill::GPU_CUDA, float> cost({4});
    cost.init();

    FreeWill::Tensor<FreeWill::GPU_CUDA, float> label({1, 4});
    label.init();

    FreeWill::Tensor<FreeWill::GPU_CUDA, float> firstLayerWeight({2,2});
    firstLayerWeight.init();
    firstLayerWeight.randomize();

    FreeWill::Tensor<FreeWill::GPU_CUDA, float> firstLayerBias({2});
    firstLayerBias.init();
    firstLayerBias.randomize();

    FreeWill::Tensor<FreeWill::GPU_CUDA, float> firstLayerWeightDerivative({2,2});
    firstLayerWeightDerivative.init();

    FreeWill::Tensor<FreeWill::GPU_CUDA, float> firstLayerBiasDerivative({2});
    firstLayerBiasDerivative.init();

    FreeWill::Tensor<FreeWill::GPU_CUDA, float> secondLayerWeight({1, 2});
    secondLayerWeight.init();
    secondLayerWeight.randomize();

    FreeWill::Tensor<FreeWill::GPU_CUDA, float> secondLayerBias({1});
    secondLayerBias.init();
    secondLayerBias.randomize();
 
    FreeWill::Tensor<FreeWill::GPU_CUDA, float> secondLayerWeightDerivative({1,2});
    secondLayerWeightDerivative.init();

    FreeWill::Tensor<FreeWill::GPU_CUDA, float> secondLayerBiasDerivative({1});
    secondLayerBiasDerivative.init();

    FreeWill::Tensor<FreeWill::GPU_CUDA, float> inputNeuronDerivative({2,4});
    inputNeuronDerivative.init();

    FreeWill::DotProductWithBias<FreeWill::GPU_CUDA, float> firstLayerFullyConnected(true);
    firstLayerFullyConnected.setInputParameter("Input", &input);
    firstLayerFullyConnected.setInputParameter("Weight", &firstLayerWeight);
    firstLayerFullyConnected.setInputParameter("Bias", &firstLayerBias);
    firstLayerFullyConnected.setOutputParameter("Output", &firstLayerActivation);
    
    QVERIFY(firstLayerActivation.init());

    FreeWill::Activation<FreeWill::SIGMOID, FreeWill::GPU_CUDA, float> firstLayerSigmoid;
    firstLayerSigmoid.setInputParameter("Input", &firstLayerActivation);
    firstLayerSigmoid.setOutputParameter("Output", &firstLayerActivation);
    
    QVERIFY(firstLayerSigmoid.init());

    FreeWill::DotProductWithBias<FreeWill::GPU_CUDA, float> secondLayerFullyConnected(true);
    secondLayerFullyConnected.setInputParameter("Input", &firstLayerActivation);
    secondLayerFullyConnected.setInputParameter("Weight", &secondLayerWeight);
    secondLayerFullyConnected.setInputParameter("Bias", &secondLayerBias);
    secondLayerFullyConnected.setOutputParameter("Output", &secondLayerActivation);

    QVERIFY(secondLayerFullyConnected.init());

    FreeWill::Activation<FreeWill::SIGMOID, FreeWill::GPU_CUDA, float> secondLayerSigmoid;
    secondLayerSigmoid.setInputParameter("Input", &secondLayerActivation);
    secondLayerSigmoid.setOutputParameter("Output", &secondLayerActivation);

    QVERIFY(secondLayerSigmoid.init());

    FreeWill::CrossEntropyLoss<FreeWill::GPU_CUDA, float> crossEntropyLoss;
    crossEntropyLoss.setInputParameter("Input", &secondLayerActivation);
    crossEntropyLoss.setInputParameter("Label", &label);
    crossEntropyLoss.setOutputParameter("Cost", &cost);

    QVERIFY(crossEntropyLoss.init());

    FreeWill::SigmoidCrossEntropyLossDerivative<FreeWill::GPU_CUDA, float> sigmoidCrossEntropyLossDerivative;
    sigmoidCrossEntropyLossDerivative.setInputParameter("Input", &secondLayerActivation);
    sigmoidCrossEntropyLossDerivative.setInputParameter("Label", &label);
    sigmoidCrossEntropyLossDerivative.setOutputParameter("Output", &secondLayerNeuronDerivative);

    QVERIFY(sigmoidCrossEntropyLossDerivative.init());

    FreeWill::DotProductWithBiasDerivative<FreeWill::GPU_CUDA, float> secondLayerDotProductWithBiasDerivative(true);
    secondLayerDotProductWithBiasDerivative.setInputParameter("InputActivation", &firstLayerActivation);
    secondLayerDotProductWithBiasDerivative.setInputParameter("OutputDelta", &secondLayerNeuronDerivative);
    secondLayerDotProductWithBiasDerivative.setInputParameter("Weight", &secondLayerWeight);

    secondLayerDotProductWithBiasDerivative.setOutputParameter("WeightGrad", &secondLayerWeightDerivative);
    secondLayerDotProductWithBiasDerivative.setOutputParameter("BiasGrad", &secondLayerBiasDerivative);
    secondLayerDotProductWithBiasDerivative.setOutputParameter("InputDelta", &firstLayerNeuronDerivative);

    QVERIFY(secondLayerDotProductWithBiasDerivative.init());

    FreeWill::ActivationDerivative<FreeWill::SIGMOID, FreeWill::GPU_CUDA, float> firstLayerSigmoidDerivative;
    firstLayerSigmoidDerivative.setInputParameter("Output", &firstLayerActivation);
    firstLayerSigmoidDerivative.setInputParameter("OutputDelta", &firstLayerNeuronDerivative);
    firstLayerSigmoidDerivative.setOutputParameter("InputDelta", &firstLayerNeuronDerivative);

    QVERIFY(firstLayerSigmoidDerivative.init());

    FreeWill::DotProductWithBiasDerivative<FreeWill::GPU_CUDA, float> firstLayerDotProductWithBiasDerivative(true);
    firstLayerDotProductWithBiasDerivative.setInputParameter("InputActivation", &input);
    firstLayerDotProductWithBiasDerivative.setInputParameter("OutputDelta", &firstLayerNeuronDerivative);
    firstLayerDotProductWithBiasDerivative.setInputParameter("Weight", &firstLayerWeight);

    firstLayerDotProductWithBiasDerivative.setOutputParameter("WeightGrad", &firstLayerWeightDerivative);
    firstLayerDotProductWithBiasDerivative.setOutputParameter("BiasGrad", &firstLayerBiasDerivative);
    firstLayerDotProductWithBiasDerivative.setOutputParameter("InputDelta", &inputNeuronDerivative);
    
    QVERIFY(firstLayerDotProductWithBiasDerivative.init()); 

    float learningRate = 0.02;
    FreeWill::ElementwiseAdd<FreeWill::GPU_CUDA, float> mergeWithFirstLayerWeight(-learningRate);
    mergeWithFirstLayerWeight.setInputParameter("Operand", &firstLayerWeight);
    mergeWithFirstLayerWeight.setInputParameter("Operand", &firstLayerWeightDerivative);
    mergeWithFirstLayerWeight.setOutputParameter("Result", &firstLayerWeight);
    QVERIFY(mergeWithFirstLayerWeight.init());

    FreeWill::ElementwiseAdd<FreeWill::GPU_CUDA, float> mergeWithFirstLayerBias(-learningRate);
    mergeWithFirstLayerBias.setInputParameter("Operand", &firstLayerBias);
    mergeWithFirstLayerBias.setInputParameter("Operand", &firstLayerBiasDerivative);
    mergeWithFirstLayerBias.setOutputParameter("Result", &firstLayerBias);
    QVERIFY(mergeWithFirstLayerBias.init());

    FreeWill::ElementwiseAdd<FreeWill::GPU_CUDA, float> mergeWithSecondLayerWeight(-learningRate);
    mergeWithSecondLayerWeight.setInputParameter("Operand", &secondLayerWeight);
    mergeWithSecondLayerWeight.setInputParameter("Operand", &secondLayerWeightDerivative);
    mergeWithSecondLayerWeight.setOutputParameter("Result", &secondLayerWeight);
    QVERIFY(mergeWithSecondLayerWeight.init());

    FreeWill::ElementwiseAdd<FreeWill::GPU_CUDA, float> mergeWithSecondLayerBias(-learningRate);
    mergeWithSecondLayerBias.setInputParameter("Operand", &secondLayerBias);
    mergeWithSecondLayerBias.setInputParameter("Operand", &secondLayerBiasDerivative);
    mergeWithSecondLayerBias.setOutputParameter("Result", &secondLayerBias);
    QVERIFY(mergeWithSecondLayerBias.init());

    firstLayerBias.copyFromHostToDevice();
    firstLayerWeight.copyFromHostToDevice();
    secondLayerBias.copyFromHostToDevice();
    secondLayerWeight.copyFromHostToDevice();

    for (int e = 0;e<4;++e)
    {
        int a = e & 0x1;
        int b = (e >> 1) & 0x1;
        int c = a^b;
        input[2*e + 0] = a;
        input[2*e + 1] = b;
        label[e+0] = c;
    }

    input.copyFromHostToDevice();
    label.copyFromHostToDevice();

    for(unsigned int i = 1; i< 250000; ++i)
    {
        firstLayerFullyConnected.evaluate();

        firstLayerSigmoid.evaluate();

        secondLayerFullyConnected.evaluate();
        secondLayerSigmoid.evaluate();
        crossEntropyLoss.evaluate();
        secondLayerWeightDerivative.clear();
        secondLayerBiasDerivative.clear();

        firstLayerWeightDerivative.clear();
        firstLayerBiasDerivative.clear();
        
        sigmoidCrossEntropyLossDerivative.evaluate();
        secondLayerDotProductWithBiasDerivative.evaluate();
        firstLayerSigmoidDerivative.evaluate();
        firstLayerDotProductWithBiasDerivative.evaluate();
       
        if (i%500000 == 0 && i!=0)
        {
           learningRate*=0.5;
        }
           
        mergeWithFirstLayerWeight.setRate(-learningRate );
        mergeWithFirstLayerBias.setRate(-learningRate );
        mergeWithSecondLayerWeight.setRate(-learningRate );
        mergeWithSecondLayerBias.setRate(-learningRate );
        mergeWithFirstLayerWeight.evaluate();
        mergeWithFirstLayerBias.evaluate();
        mergeWithSecondLayerWeight.evaluate();
        mergeWithSecondLayerBias.evaluate();
    }

    cost.copyFromDeviceToHost();
    qDebug() << "cost:" << cost[0];

    firstLayerFullyConnected.evaluate();
    firstLayerSigmoid.evaluate();
    secondLayerFullyConnected.evaluate();
    secondLayerSigmoid.evaluate();
    secondLayerActivation.copyFromDeviceToHost();
 
    for (int i =0;i<4;++i)
    {
        qDebug() << "test" << i << ": a" << input[i*2] << "b" << input[i*2+1] << "c" << label[i] << "nn result:" << secondLayerActivation[i];
    }
    
}

