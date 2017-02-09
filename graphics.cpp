#include "graphics.h"
#include <time.h>
#include <stdio.h>

graphics::graphics() {}

graphics::graphics(game* _game, sf::RenderWindow* _rW) {
    mMasterWindow = _rW;
    mMasterWindow->create(sf::VideoMode(mMasterWindowWidth, mMasterWindowHeight), "make the bubbles gone", sf::Style::Close);
    mGame = _game;
    mStationaryNodeRenderTexture.create(mGameWindowWidth, mGameWindowHeight);
    mShooterRenderTexture.create(mGameWindowWidth, mMasterWindowHeight);
    mRadius = mGameWindowWidth / ((mGame->mNodeGraph.mBOARD_WIDTH * 2) + 1);
    mGameRenderOffsetFromMaster = sf::Vector2f((mMasterWindowWidth - mGameWindowWidth) / 2, (mMasterWindowHeight - mGameWindowHeight) / 2);
}

void graphics::runGraphicsLoop() {
    sf::Clock clock;
    sf::Time gameTimeAcc;
    sf::Time windowRefreshTimeAcc;

    while (true) {
        sf::Time t = clock.restart();
        gameTimeAcc += t;
        windowRefreshTimeAcc += t;
        while (gameTimeAcc >= gameInterval) {
            std::vector<sf::Keyboard::Key> keys = collectInputsFromDevices();

            mGame->runPhysicsFrame(keys);
            gameTimeAcc -= gameInterval;
        }

        if (windowRefreshTimeAcc >= windowRefreshInterval) { updateWindow(windowRefreshTimeAcc); }
    }

}


std::vector<sf::Keyboard::Key> graphics::collectInputsFromDevices() {
    std::vector<sf::Keyboard::Key> keySet;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { keySet.emplace_back(sf::Keyboard::Left); }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { keySet.emplace_back(sf::Keyboard::Right); }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) { keySet.emplace_back(sf::Keyboard::R); }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { keySet.emplace_back(sf::Keyboard::Space); }

    sf::Event event;
    while (mMasterWindow->pollEvent(event)) {
        if (event.type == sf::Event::Closed) { mMasterWindow->close(); exit(0); }
    }

    return(keySet);
}

void graphics::updateWindow(sf::Time& windowRefreshTimeAcc) {
    mMasterWindow->clear(sf::Color(20, 20, 20));

    updateStationaryNodeRenderTexture();
    sf::Sprite gameTextureSprite(mStationaryNodeRenderTexture.getTexture());
    gameTextureSprite.setPosition(mGameRenderOffsetFromMaster);
    mMasterWindow->draw(gameTextureSprite);	

    updateShooterRenderTexture();
    sf::Sprite shooterTextureSprite(mShooterRenderTexture.getTexture());
    shooterTextureSprite.setPosition(mGameRenderOffsetFromMaster);
    mMasterWindow->draw(shooterTextureSprite);

    

    mMasterWindow->display();
    windowRefreshTimeAcc -= windowRefreshInterval;
}

sf::CircleShape graphics::getBall() {
    sf::CircleShape circle;
    circle.setRadius(mRadius);
    circle.setOutlineThickness(mRadius / -8);
    circle.setOutlineColor(sf::Color(140, 140, 140));
    sf::Vector2f center(mRadius, mRadius);
    circle.setOrigin(center);

    return(circle);
}

void graphics::updateShooterRenderTexture() {
    mShooterRenderTexture.clear(sf::Color::Transparent);

    sf::Sprite arrow = mGame->mShooter.getArrowSprite();
    arrow.setPosition(meterToGamePixels(arrow.getPosition()));
    mShooterRenderTexture.draw(arrow);

    sf::CircleShape circle = getBall();
    circle.setFillColor(mGame->mShooter.mLoadedBullet.mColor);
    circle.setPosition(meterToGamePixels(mGame->mShooter.getOrigin()));
    mShooterRenderTexture.draw(circle);

    for (bullet& b : mGame->mActiveBullets) {
        circle.setFillColor(b.mColor);
        circle.setPosition(meterToGamePixels(b.mMeterPos));
        mShooterRenderTexture.draw(circle);
    }


    mShooterRenderTexture.display();
}

void graphics::updateStationaryNodeRenderTexture() {
	mStationaryNodeRenderTexture.clear(sf::Color(160, 200, 220));
    sf::CircleShape circle = getBall();
    for (node& currentNode : mGame->mNodeGraph.mGraph) {
        if (!currentNode.mIsDisabled) {
            circle.setFillColor(currentNode.mColor);
            circle.setPosition(getStationaryCirclePixelCenter(currentNode.mIndex));
            mStationaryNodeRenderTexture.draw(circle);
        }
    }
    mStationaryNodeRenderTexture.display();
}

sf::Vector2f graphics::getStationaryCirclePixelCenter(int _pos) {
    return(meterToGamePixels(mGame->mNodeGraph.mGraph[_pos].mMeterPos));
}

sf::Vector2f graphics::meterToGamePixels(sf::Vector2f _meterPos) {
    return(sf::Vector2f(_meterPos.x * mRadius, _meterPos.y * mRadius));
}

