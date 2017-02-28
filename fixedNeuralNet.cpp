#include "fixedNeuralNet.h"

/*
size = (board width * height)

size inputs: disabled = -1, 0-7 are colors
3 outputs: left, right, space
3 hidden layers, 100 nodes each



*/

float fast_sigmoid(float f) {
    return(f / (1 + fabs(f)));
}

neuralNet::neuralNet(game* _game) {
    mGameManager = gameManager(_game);
    mNUM_INPUTS = mGameManager.getNodeCount();

    mRandomEngine = mtEngine();

    initializePopulations();
}

void neuralNet::initializePopulations() {
    mPopulation.resize(mPOPULATION_COUNT);
    for (species & s : mPopulation) {
        s = species(mNUM_INPUTS, mNUM_OUTPUTS, mHIDDEN_NODE_PER_LAYER_COUNT, mNUM_HIDDEN_LAYERS, &mRandomEngine);
    }
}

species ::species() {}
species::species(int _inputCount, int _outputCount, int _hiddenNodeCount, int _hiddenLayerCount, mtEngine* _randEngine) {
    mRandEngine = _randEngine;
    mInputs.resize(_inputCount);
    mOutputs.resize(_outputCount);
    mHiddenLayers.reserve(_hiddenLayerCount * _hiddenLayerCount);
    for (int j = 0; j < _hiddenLayerCount; j++) {
        std::vector<float> layer(_hiddenNodeCount);
        for (int i = 0; i < _hiddenNodeCount; i++) {
            layer[i] = fast_sigmoid(mRandEngine->getFloat());
        }
        mHiddenLayers.push_back(layer);
    }
}

mtEngine::mtEngine() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    mDistribution = std::uniform_real_distribution<float>(0.0, 1.0);
    mGenerator = std::mt19937(seed);
}
float mtEngine::getFloat() {
    return(mDistribution(mGenerator));
}

gameManager::gameManager() {}
gameManager::gameManager(game* _game) {
    mGame = _game;
}

int gameManager::getNodeCount() {
    return(mGame->mNUM_NODES);
}