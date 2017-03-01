#pragma once
#include <vector>
#include <random>
#include <chrono>
#include "game.h"

#define neuron std::tuple<float, float>
class mtEngine;

class species  {
public:
    species ();
    species(int _outputCount, int _hiddenNodeCount, int _hiddenLayerCount, mtEngine* _randEngine);
    std::vector<float> mInputs;
    std::vector<float> mOutputs;
    std::vector<std::vector<neuron>> mHiddenLayers;

    mtEngine* mRandEngine;

    //void activate(inputs) // 
};

class gameManager {
public:
    gameManager();
    gameManager(game* _game);

    int getNodeCount();
    void updateGameState(bool _pressLeft=false, bool _pressRight=false, bool _pressSpace=false);

    bool mIsBulletInFlight;
    float mShooterAngle;
    std::vector<short> mGraphState;
    short mLoadedBulletColor;
    short mUpcomingBulletColor;


private:
    game* mGame;
    
    
    
};

class mtEngine {
public:
    mtEngine();
    float getFloat();
    //double getDouble();
private:
    std::mt19937 mGenerator;
    std::uniform_real_distribution<float> mDistribution;
};

class neuralNet {
public:
    neuralNet(game* _game);

private:
    
    const int mPOPULATION_COUNT = 10;
    const int mSPECIES_COUNT = 50;
    const int mHIDDEN_NODE_PER_LAYER_COUNT = 20;
    const int mNUM_HIDDEN_LAYERS = 20;
    const int mNUM_OUTPUTS = 3;
    int mNUM_INPUTS;
    std::vector<species> mPopulation;
    mtEngine mRandomEngine;

    gameManager mGameManager;  

    void initializePopulations();
};