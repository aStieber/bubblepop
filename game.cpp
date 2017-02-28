#include "game.h"
#include <stdio.h>

#define M_PI 3.141592635f  /* pi */

std::vector<sf::Color> bullet::colorVec = { sf::Color::White, sf::Color::Magenta, sf::Color::Yellow,
sf::Color::Red, sf::Color::Blue, sf::Color::Green };

sf::Color bullet::getRandomColor() {
    return(colorVec[rand() % colorVec.size()]);
}
bullet::bullet() {
    mColor = getRandomColor();
}
shooter::shooter() {
    mArrowTexture.loadFromFile("images/shooter_arrow.png");
    mArrowSprite.setTexture(mArrowTexture);
    updateMagazine();
}

bool shooter::adjustAim(float _speed) {
    if (((abs(mCurrentAngle)) < mAngleLimit) || (mCurrentAngle <= -mAngleLimit && _speed > 0) || (mCurrentAngle >= mAngleLimit && _speed < 0)) {
        mCurrentAngle += _speed;
        mArrowSprite.rotate(_speed);
        return(true);
    }
    return(false);
}

void shooter::fireBullet(std::vector<bullet>& _activeBullets) {
    mLoadedBullet.mWasFired = true;
    mLoadedBullet.mMeterPos = getOrigin();
    mLoadedBullet.mVector = sf::Vector2f(mBulletSpeed * sinf((M_PI * mCurrentAngle) / 180.0), mBulletSpeed * -cosf((M_PI * mCurrentAngle) / 180.0));
    //printf("CurrentAngle: %.02f  | x: %.02f  | y: %.02f \n", mCurrentAngle, mLoadedBullet.mVector.x, mLoadedBullet.mVector.y);
    _activeBullets.push_back(mLoadedBullet);

    updateMagazine();
}

void shooter::setOrigin(sf::Vector2f _meterPos) {
    mArrowSprite.setOrigin(10, 80);
    mOrigin = sf::Vector2f(_meterPos.x / 2, _meterPos.y);
    mArrowSprite.setPosition(mOrigin.x, mOrigin.y);
}

sf::Sprite shooter::getArrowSprite() { return(mArrowSprite); }

sf::Vector2f shooter::getOrigin() { return(mOrigin); }

void shooter::updateMagazine() {
    while (mLoadedBullet.mWasFired) {
        mLoadedBullet = mUpcomingBullet;
        mUpcomingBullet = bullet();
    }
}

void shooter::updateActiveBullets(std::vector<bullet>& _activeBullets) {
    for (bullet& b : _activeBullets) {
        b.mMeterPos.x += b.mVector.x;
        b.mMeterPos.y += b.mVector.y;
        if (b.mMeterPos.x >= mGameDimensions.x || b.mMeterPos.x <= 0.5) {
            b.mVector.x *= -1;
        }
    }
}



void shooter::update(std::vector<sf::Keyboard::Key>& _keys, std::vector<bullet>& _activeBullets) {
    updateMagazine();
    updateActiveBullets(_activeBullets );
    
    for (sf::Keyboard::Key& k : _keys) {
        switch (k) {
        case sf::Keyboard::Right:
            adjustAim(mAimSpeed);
            break;
        case sf::Keyboard::Left:
            adjustAim(-mAimSpeed);
            break;
        case sf::Keyboard::Space:
            if (_activeBullets.size() <= 0) {
                fireBullet(_activeBullets);
            }
            break;
        default:
            break;
        }
    }
}

void game::reset() {
    mNUM_NODES = mBOARD_HEIGHT * mBOARD_WIDTH;
    mNodeGraph = nodeGraph(mBOARD_WIDTH, mBOARD_HEIGHT);
    mActiveBullets.clear();
    mShooter.mGameDimensions = sf::Vector2f((mBOARD_WIDTH * 2) + .5f, (2 * mBOARD_HEIGHT));
    mShooter.setOrigin(mShooter.mGameDimensions);
    mStatus = STATUS_ALIVE;
    updateFaceNodes();
}

game::game() {
    reset();
}

void game::updateFaceNodes() {
    mFaceNodes.clear();
    for (node& n : mNodeGraph.mGraph) {
        if (!n.mIsDisabled && (n.mNodeAdjacencyList.size() < n.mAdjacencyList->size())) {
            mFaceNodes.emplace_back(&n);
        }
    }
}

float game::getDistanceBetweenPoints(sf::Vector2f _a, sf::Vector2f _b) {
    return(sqrtf((_a.x - _b.x) * (_a.x - _b.x) + (_a.y - _b.y) * (_a.y - _b.y))); //pythagorean
}

void game::clearDestroyedNodes() {
    mDestroyedNodes.clear();
}

int game::translateBulletToClosestAdjacentNode(bullet& b, node& n) {
    int minIndex = -1;
    float min = FLT_MAX;

    for (int i : *n.mAdjacencyList) {
        if (mNodeGraph.mGraph[i].mIsDisabled) {
            float dist = getDistanceBetweenPoints(mNodeGraph.mGraph[i].mMeterPos, b.mMeterPos);
            if (dist < min) {
                min = dist;
                minIndex = i;
            }
        }
    }
    if (minIndex >= 0) {
        mNodeGraph.mGraph[minIndex].mColor = b.mColor;
        mNodeGraph.mGraph[minIndex].mIsDisabled = false;
    }
    return(minIndex);

}

int game::translateBulletToClosestCeilingNode(bullet& b) {
    int minIndex = -1;
    float minDist = FLT_MAX;
    for (int i = 0; i < mBOARD_WIDTH; i++) {
        float tmpDist = getDistanceBetweenPoints(mNodeGraph.mGraph[i].mMeterPos, b.mMeterPos);
        if (tmpDist < minDist) {
            minDist = tmpDist;
            minIndex = i;
        }
    }
    if (minIndex >= 0) {
        mNodeGraph.mGraph[minIndex].mColor = b.mColor;
        mNodeGraph.mGraph[minIndex].mIsDisabled = false;
    }
    return(minIndex);
}


void game::updateBulletGraphInteraction() {
    if (mActiveBullets.size() > 0) {
        std::vector<bullet> tmpActiveBulletsVec;
        for (bullet& b : mActiveBullets) {
            bool destroyBullet = false;
            if (b.mMeterPos.y <= 0.5f) {
                int index = translateBulletToClosestCeilingNode(b);
                mDestroyedNodes = mNodeGraph.checkForDestruction(index);
                destroyBullet = true;
            }
            else {
                for (node* n : mFaceNodes) {
                    if (getDistanceBetweenPoints(b.mMeterPos, n->mMeterPos) <= sqrt(3)) {
                        if (n->mIndex > (mBOARD_HEIGHT * (mBOARD_WIDTH - 1))) {
                            mStatus = STATUS_LOST;
                        }
                        else {
                            mDestroyedNodes = mNodeGraph.checkForDestruction(translateBulletToClosestAdjacentNode(b, *n));
                            destroyBullet = true;
                            break;
                        }
                       
                    }
                }
            }
            if (!destroyBullet) { tmpActiveBulletsVec.push_back(b); }
        }
        mActiveBullets = tmpActiveBulletsVec;
    }
}


void game::runPhysicsFrame(std::vector<sf::Keyboard::Key>& _keys) {    
    mShooter.update(_keys, mActiveBullets);
    updateBulletGraphInteraction();
    updateDestroyedNodes();
    if (checkForVictory()) { mStatus = STATUS_WON; }
    else {
        mNodeGraph.updateNodeAdjacencies();
        updateFaceNodes();
    }
    
}

void game::updateDestroyedNodes() {
    for (int i : mDestroyedNodes) {
        mNodeGraph.mGraph[i].mIsDisabled = true;
    }
}

std::vector<int> game::getDestroyedNodeIndexes() {
    return (mDestroyedNodes);
}

bool game::checkForVictory() {
    for (node& n : mNodeGraph.mGraph) {
        if (!n.mIsDisabled) {
            return(false);
        }
    }
    return(true);
}
