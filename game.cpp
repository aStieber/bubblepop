#include "game.h"
#include <stdio.h>

# define M_PI           3.141592635f  /* pi */

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
        bool arst = b.mMeterPos.x >= mGameDimensions.x;
        bool neio = b.mMeterPos.x <= 0.5;
        if (arst || neio) {
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

game::game() {
    mNUM_NODES = mBOARD_HEIGHT * mBOARD_WIDTH;
    mNodeGraph = nodeGraph(mBOARD_WIDTH, mBOARD_HEIGHT);
    mShooter.mGameDimensions = sf::Vector2f((mBOARD_WIDTH * 2)  + .5, (2 * mBOARD_HEIGHT) + 5);
    mShooter.setOrigin(mShooter.mGameDimensions);

    updateCollisionMesh();
    
}


void game::runPhysicsFrame(std::vector<sf::Keyboard::Key>& _keys) {    
    mShooter.update(_keys, mActiveBullets);

}