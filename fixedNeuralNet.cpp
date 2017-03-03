#include "fixedNeuralNet.h"
#include "timing.cpp"

/*
size = (board width * height)

size inputs: disabled = -1, 0-7 are colors
3 outputs: left, right, space
3 hidden layers, 100 nodes each



*/

static float fast_sigmoid(float f) {
    return(f / (1.0f + fabs(f)));
}

static bool activationFunc(float f) {
    return(abs(f) > 0.5f);
}

neuralNet::neuralNet(game* _game) {
    mGameManager = gameManager(_game);
    mNUM_INPUTS = mGameManager.getNodeCount() + 3; //gameManagerValues
    mRandomEngine = mtEngine();

    initializePopulations();
}

void neuralNet::initializePopulations() {
    printf("---beginning species creation\n");
    uint64 t_begin = GetTimeMs64();
    int finalCount = mPOPULATION_COUNT * mSPECIES_COUNT;
    int counter = 0;
    mPopulations.resize(mPOPULATION_COUNT);
    for (population& pop : mPopulations) {
        pop.resize(mSPECIES_COUNT);
        for (species& s : pop) {
            s = species(mNUM_OUTPUTS, mNUM_INPUTS, mNUM_HIDDEN_LAYERS, &mRandomEngine);
            counter++;
            printf("%i/%i\r", counter, finalCount);
        }
    }
    printf("\n---finished species creation | duration: %ims\n", GetTimeMs64() - t_begin);
}

void neuralNet::start(int _iterations) {
    for (int i = 0; i < _iterations; i++) {
        for (population& pop : mPopulations) {
            std::pair<int, int> firstBest(-1, -1); //index, value
            std::pair<int, int> secondBest(-1, -1);
            for (int i = 0; i < pop.size(); i++)  {
                uint64 t_begin = GetTimeMs64();
                playGame(pop[i]);
                int fitness = pop[i].mFitness;
                printf("game #%i/%i | fitness: %i\n", i, _iterations, fitness);
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
        if (commands.size() > 0) { 
            timeoutCounter = 0; }
        else { timeoutCounter++; }
        mGameManager.updateGameState(commands);
    }
    return(0);
}

float neuralNet::processActivations(float _valueIndex, std::vector<float>& _weights, int size) {
    float sum = 0.0f;
    for (int j = 0; j < size; j++) {
        sum += (_valueIndex * _weights[j]);
    }
    return(fast_sigmoid(sum));
}

keyList neuralNet::runNetwork(species& s) {
    //inputs into 1st layer
    
    int size = s.mInputValues.size();
    for (int i = 0; i < size; i++) {
        s.mHiddenValues[0][i] = processActivations(s.mInputValues[i], s.mHiddenWeights[0][i], size);
    }
    //1st layer into n layer
    for (int x = 1; x < mNUM_HIDDEN_LAYERS; x++) {
        for (int i = 0; i < size; i++) {
            s.mHiddenValues[x][i] = processActivations(s.mHiddenValues[x - 1][i], s.mHiddenWeights[x][i], size);
        }
    }
    
    //n layer into output
    //outputs are L/R/Space
    for (int i = 0; i < s.mOutputWeights.size(); i++) {
        s.mOutputValues[i] = processActivations(s.mHiddenValues[mNUM_HIDDEN_LAYERS - 1][i], s.mOutputWeights[i], size);
    }

    keyList k;
    if (activationFunc(s.mOutputValues[0])) { k.emplace_back(sf::Keyboard::Left); }
    if (activationFunc(s.mOutputValues[1])) { k.emplace_back(sf::Keyboard::Right); }
    if (activationFunc(s.mOutputValues[2])) { k.emplace_back(sf::Keyboard::Space); }

    return(k);
}

species::species() {}
species::species(int _outputCount, int _nodeCount, int _hiddenLayerCount, mtEngine* _randEngine) {
    mRandEngine = _randEngine;

    mOutputValues.resize(_outputCount);
    mOutputWeights.resize(_outputCount);
    for (std::vector<float>& w : mOutputWeights) {
        w.resize(_nodeCount);
        for (float& f : w) {
            f = mRandEngine->getFloat();
        }
    }
    
    
    mHiddenWeights.resize(_hiddenLayerCount);
    for (std::vector<std::vector<float>>& i : mHiddenWeights) {
        mHiddenValues.emplace_back(std::vector<float>(_nodeCount, 0.0f));
        i.resize(_nodeCount);
        for (std::vector<float>& j : i) {
            j.resize(_nodeCount);
            for (float& f : j) {
                f = mRandEngine->getFloat();
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
