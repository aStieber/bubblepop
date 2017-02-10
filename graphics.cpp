#include "graphics.h"
#include <time.h>
#include <stdio.h>

graphics::graphics() {}

graphics::graphics(game* _game, sf::RenderWindow* _rW) {
    mMasterWindow = _rW;
    mMasterWindow->create(sf::VideoMode(mMasterWindowWidth, mMasterWindowHeight), "make the bubbles gone", sf::Style::Close);
    mGameRenderOffsetFromMaster = sf::Vector2f((mMasterWindowWidth - mGameWindowWidth) / 2, (mMasterWindowHeight - mGameWindowHeight) / 2);
    mGame = _game;
    mRadius = (float)mGameWindowWidth / (float)((mGame->mNodeGraph.mBOARD_WIDTH * 2) + 1);

    mStationaryNodeRenderTexture.create(mGameWindowWidth, mGameWindowHeight);
    mShooterRenderTexture.create(mGameWindowWidth, mMasterWindowHeight);
    mBackgroundRenderTexture.create(mGameWindowWidth, mGameWindowHeight);
    mBackgroundRenderTexture.clear(sf::Color(160, 200, 220));
    
    
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

void graphics::updateDestroyedNodes() {
    std::vector<destroyedNode> tmpDNs;
    if (mGame->getDestroyedNodeIndexes().size() > 0) {
        tmpDNs = createDestroyedNodes(mGame->getDestroyedNodeIndexes());
    }
    else if (mDestroyedNodes.size() > 0) {
        tmpDNs = mDestroyedNodes;
    }
    else { return; }
    mDestroyedNodes.clear();
    for (destroyedNode dN : tmpDNs) {
        if (dN.mColor.a >= dN.mTransparencyIncreaseRate) {
            dN.mColor.a -= dN.mTransparencyIncreaseRate;
            dN.mMeterPos.y += .01;
            mDestroyedNodes.emplace_back(dN);
        }
    }
}

std::vector<destroyedNode> graphics::createDestroyedNodes(std::vector<int> _nodeIndexes) {
    std::vector<destroyedNode> tmp(_nodeIndexes.size());
    for (int i = 0; i < _nodeIndexes.size(); i++) {
        node* n = &mGame->mNodeGraph.mGraph[_nodeIndexes[i]];
        tmp[i].mColor = n->mColor;
        tmp[i].mMeterPos = n->mMeterPos;
        tmp[i].mIndex = n->mIndex;
    }
    mGame->clearDestroyedNodes();
    return(tmp);

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

    sf::Sprite backgroundSprite(mBackgroundRenderTexture.getTexture());
    backgroundSprite.setPosition(mGameRenderOffsetFromMaster);
    mMasterWindow->draw(backgroundSprite);

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

    updateDestroyedNodes();
    for (destroyedNode& dN : mDestroyedNodes) {
        circle.setFillColor(dN.mColor);
        circle.setPosition(meterToGamePixels(dN.mMeterPos));
        //circle.setPosition(getStationaryCirclePixelCenter(dN.mIndex));
        mShooterRenderTexture.draw(circle);
    }


    for (bullet& b : mGame->mActiveBullets) {
        circle.setFillColor(b.mColor);
        circle.setPosition(meterToGamePixels(b.mMeterPos));
        mShooterRenderTexture.draw(circle);
    }


    mShooterRenderTexture.display();
}

sf::Text graphics::DEBUG_getText(std::string text, sf::Font& font) {
    sf::Text tmp(text, font);
    tmp.setCharacterSize(20);
    tmp.setFillColor(sf::Color::Black);
    tmp.setOrigin(mRadius / 2, mRadius / 2);
    return(tmp);
}

void graphics::updateStationaryNodeRenderTexture() {
    sf::Font FONT;
    FONT.loadFromFile("Inconsolata-Regular.ttf");
	mStationaryNodeRenderTexture.clear(sf::Color::Transparent);
    sf::CircleShape circle = getBall();
    for (node& currentNode : mGame->mNodeGraph.mGraph) {
        circle.setPosition(getStationaryCirclePixelCenter(currentNode.mIndex));
        if (!currentNode.mIsDisabled) {
            circle.setFillColor(currentNode.mColor);
            mStationaryNodeRenderTexture.draw(circle);
            sf::Text t = DEBUG_getText(std::to_string(currentNode.mIndex), FONT);
            t.setPosition(circle.getPosition());


            mStationaryNodeRenderTexture.draw(t);
        }
    }
    mStationaryNodeRenderTexture.display();
}

/*
    void graphics::updateStationaryNodeRenderTexture() {
    
    mStationaryNodeRenderTexture.clear(sf::Color::Transparent);
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
*/

sf::Vector2f graphics::getStationaryCirclePixelCenter(int _pos) {
    return(meterToGamePixels(mGame->mNodeGraph.mGraph[_pos].mMeterPos));
}

sf::Vector2f graphics::meterToGamePixels(sf::Vector2f _meterPos) {
    return(sf::Vector2f(_meterPos.x * mRadius, _meterPos.y * mRadius));
}

