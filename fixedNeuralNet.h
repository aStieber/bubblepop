#pragma once
#include <vector>
#include <random>
#include <chrono>
#include <Windows.h>
#include "game.h"

#define population std::vector<species>
#define keyList std::vector<sf::Keyboard::Key>
class mtEngine;

class species  {
public:
    species ();
    species(mtEngine* _randEngine);
    species(int _outputCount, int _NodeCount, int _hiddenLayerCount, mtEngine* _randEngine);
    std::vector<float> mInputValues;
    std::vector<float> mOutputValues;
    std::vector<std::vector<float>> mOutputWeights;

    std::vector<std::vector<float>> mHiddenValues;
    std::vector<std::vector<std::vector<float>>> mHiddenWeights;//fuck
    int mFitness = -1;

    mtEngine* mRandEngine;


};

class gameManager {
public:
    gameManager();
    gameManager(game* _game);
    void initializeGame();

    int getNodeCount();
    void updateGameState(keyList& _input=keyList());

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
    void start(int _iterations);

private:
    
    const int mPOPULATION_COUNT = 10;
    const int mSPECIES_COUNT = 50;
    const int mNUM_HIDDEN_LAYERS = 8;
    const int mNUM_OUTPUTS = 3;
    int mNUM_INPUTS;
    std::vector<population> mPopulations;
    mtEngine mRandomEngine;

    gameManager mGameManager;  

    void initializePopulations();
    int playGame(species& s);
    keyList runNetwork(species& s);

};