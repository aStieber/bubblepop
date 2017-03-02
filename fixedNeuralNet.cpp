#include "fixedNeuralNet.h"

/*
size = (board width * height)

size inputs: disabled = -1, 0-7 are colors
3 outputs: left, right, space
3 hidden layers, 100 nodes each



*/

static float fast_sigmoid(float f) {
    return(f / (1.0f + fabs(f)));
}

neuralNet::neuralNet(game* _game) {
    mGameManager = gameManager(_game);
    mNUM_INPUTS = mGameManager.getNodeCount() + 3; //gameManagerValues
    mRandomEngine = mtEngine();

    initializePopulations();
}

void neuralNet::initializePopulations() {
    mPopulations.resize(mPOPULATION_COUNT);
    for (population& pop : mPopulations) {
        pop.resize(mSPECIES_COUNT);
        for (species& s : pop) {
            s = species(mNUM_OUTPUTS, mNUM_INPUTS, mNUM_HIDDEN_LAYERS, &mRandomEngine);
        }
    }
}

void neuralNet::start(int _iterations) {
    OutputDebugStringA("---Start---\n");

    //each species plays game, compare fitness, take top 2 in each population
    //randomly adjust their values and create a new top 10
    for (int i = 0; i < _iterations; i++) {
        for (population& pop : mPopulations) {
            std::pair<int, int> firstBest(-1, -1); //index, value
            std::pair<int, int> secondBest(-1, -1);
            for (int i = 0; i < pop.size(); i++)  {
                playGame(pop[i]);
                int fitness = pop[i].mFitness;
                if (fitness > std::get<1>(secondBest)) {
                    if (fitness > std::get<1>(firstBest)) {
                        secondBest = firstBest;
                        firstBest = std::pair<int, int>(i, fitness);
                    }
                    else {
                        secondBest = std::pair<int, int>(i, fitness);
                    }
                }
            }
        }
    }

}

int neuralNet::playGame(species& s) {
    mGameManager.initializeGame();
    int timeout = 100;
    int timeoutCounter = 0;
    while (timeoutCounter < timeout) {
        do { mGameManager.updateGameState(); } while (mGameManager.mIsBulletInFlight);
        s.mInputValues = std::vector<float>(mGameManager.mGraphState.begin(), mGameManager.mGraphState.end());
        
        //currentAngle, Loaded bullet color
        std::vector<float> extraInputs{mGameManager.mShooterAngle, (float)mGameManager.mLoadedBulletColor, (float)mGameManager.mUpcomingBulletColor};
        s.mInputValues.insert(s.mInputValues.end(), extraInputs.begin(), extraInputs.end());
        
        keyList commands = runNetwork(s);
        mGameManager.updateGameState(commands);
        //if commands is empty, increase countTimeout
    }
    return(0);
}

keyList neuralNet::runNetwork(species& s) {
    //inputs into 1st layer
    //1st layer into n layer
    //n layer into output
    return(keyList());
}

species::species() {}
species::species(int _outputCount, int _NodeCount, int _hiddenLayerCount, mtEngine* _randEngine) {
    mRandEngine = _randEngine;

    mOutputValues.resize(_outputCount);
    mOutputWeights.resize(_outputCount);
    for (std::vector<float> w : mOutputWeights) {
        for (float f : w) {
            f = fast_sigmoid(mRandEngine->getFloat());
        }
    }
    
    
    mHiddenWeights.resize(_hiddenLayerCount);
    for (std::vector<std::vector<float>>& index2 : mHiddenWeights) {
        mHiddenValues.emplace_back(std::vector<float>(_NodeCount, 0.0f));
        for (std::vector<float>& fuck : index2) {
            fuck.resize(_NodeCount);
            for (float& f : fuck) {
                f = fast_sigmoid(mRandEngine->getFloat());
            }
        }
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
    initializeGame();
}

void gameManager::initializeGame() {
    mGame->reset();
    updateGameState();
}

int gameManager::getNodeCount() {
    return(mGame->mNUM_NODES);
}

void gameManager::updateGameState(keyList& _input) {
    mGame->runPhysicsFrame(_input);
    mGraphState = mGame->getGraphState();
    mIsBulletInFlight = mGame->getIsBulletInFlight();
    mShooterAngle = mGame->mShooter.getCurrentAngle();
    mLoadedBulletColor = mGame->mShooter.mLoadedBullet.mColorIndex;
    mUpcomingBulletColor = mGame->mShooter.mUpcomingBullet.mColorIndex;
}
