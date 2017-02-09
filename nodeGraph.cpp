#include "nodeGraph.h"
#include <iostream>



void nodeGraph::populateAdjacencyMasterList() {
    mAdjacencyMasterList.clear();
    for (int i = 0; i < (mBOARD_HEIGHT * mBOARD_WIDTH); i++) {
        std::vector<int> tmpAdjacencyList;
        bool isEvenRow = ((int)(i / mBOARD_WIDTH) % 2 == 0);
        int leftMod = isEvenRow ? -1 : 0;
        int rightMod = isEvenRow ? 0 : 1;
        bool leftAllowed = ((i % 16) != 0);
        bool rightAllowed = (((i + 1) % 16) != 0);

        if (leftAllowed) { tmpAdjacencyList.push_back(i - 1); } //left
        if (rightAllowed) { tmpAdjacencyList.push_back(i + 1); }//right
        if (i >= mBOARD_WIDTH) { //not top row
            if (leftAllowed) { tmpAdjacencyList.push_back(i - mBOARD_WIDTH + leftMod); } //upper left
            if (rightAllowed) { tmpAdjacencyList.push_back(i - mBOARD_WIDTH + rightMod); } //upper right			
        }
        if (i < ((mBOARD_HEIGHT - 1) * mBOARD_WIDTH)) { //not bottom row
            if (leftAllowed) { tmpAdjacencyList.push_back(i + mBOARD_WIDTH + leftMod); }
            if (rightAllowed) { tmpAdjacencyList.push_back(i + mBOARD_WIDTH + rightMod); }
        }

        mAdjacencyMasterList.emplace_back(std::move(tmpAdjacencyList));
    }
}


nodeGraph::nodeGraph() {

}

nodeGraph::nodeGraph(int _BOARD_WIDTH, int _BOARD_HEIGHT) {
    mBOARD_HEIGHT = _BOARD_HEIGHT;
    mBOARD_WIDTH = _BOARD_WIDTH;
    std::vector<int> mGraph(_BOARD_HEIGHT * _BOARD_WIDTH, NULL);
    populateAdjacencyMasterList();
    populateInitialGraph();
    
}

void nodeGraph::populateInitialGraph() { //add randomness
    mGraph.resize(mAdjacencyMasterList.size());
    int numberOfRowsToGenerate = 3;
    for (int i = 0; i < mGraph.size() ; i++) {
        mGraph[i] = node(i, &mAdjacencyMasterList[i], (i >= (mBOARD_WIDTH * numberOfRowsToGenerate)));
    }
    updateNodeAdjacencies();
    updateMeterPositions();
}

void nodeGraph::updateNodeAdjacencies() {
    for (node& n : mGraph) { n.updateAdjacencies(mGraph); }
}

void nodeGraph::updateMeterPositions() {
    for (node& n : mGraph) {
        bool isEvenRow = ((n.mIndex / mBOARD_WIDTH) % 2 == 0);
        float horizontalOffset = (isEvenRow ? 1 : 2) + (float)((n.mIndex % mBOARD_WIDTH) * 2);
        float verticalOffset = 1 + (sqrtf(3) * (n.mIndex / mBOARD_WIDTH));
        n.mMeterPos = sf::Vector2f(horizontalOffset, verticalOffset);
    }
}

void nodeGraph::addNode(int _Index) {
    mGraph[_Index] = node(_Index, &mAdjacencyMasterList[_Index], false);
    updateNodeAdjacencies();
}

void nodeGraph::checkForDestruction(int _triggerPos) {
    std::vector<node*> matchingNodeSet;
    std::vector<bool> visitedVec(mBOARD_HEIGHT * mBOARD_WIDTH, false);
    checkColorMatch(_triggerPos, visitedVec, matchingNodeSet);
    if (matchingNodeSet.size() >= 3) {
        //destroy em
    }

    //for each node adjacent to a destroyed node, verify it is anchored
    
}

void nodeGraph::checkColorMatch(int _triggerPos, std::vector<bool>& _visitedVec, std::vector<node*>& _matchingNodeSet) {
    _visitedVec[_triggerPos] = true;
    _matchingNodeSet.emplace_back(&mGraph[_triggerPos]);
    for (node* n : mGraph[_triggerPos].mNodeAdjacencyList) {
        if (n->mColor == mGraph[_triggerPos].mColor && !_visitedVec[n->mIndex]) {
            checkColorMatch(n->mIndex, _visitedVec, _matchingNodeSet);
        }
    }
}



node::node() {
    mColor = getRandomColor();
}



node::node(int _pos, std::vector<int>* _adjacencyList, bool _isDisabled, sf::Color _color) {
    mIndex = _pos;
    mAdjacencyList = _adjacencyList;
    mIsDisabled = _isDisabled;
    mColor = _color;
}

void node::updateAdjacencies(std::vector<node>& _graph) {
    if (mIsDisabled) { return; }

    mNodeAdjacencyList.clear();	
    for (int i = 0; i < mAdjacencyList->size(); i++) {
        if (!_graph[i].mIsDisabled) {
            mNodeAdjacencyList.push_back(&_graph[i]);
        }
    }
}

std::vector<sf::Color> node::colorVec = {	sf::Color::White, sf::Color::Magenta, sf::Color::Yellow,
                                            sf::Color::Red, sf::Color::Blue, sf::Color::Green };

sf::Color node::getRandomColor() {
    return(colorVec[rand() % colorVec.size()]);
}

