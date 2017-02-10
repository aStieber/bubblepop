#include "nodeGraph.h"
#include <iostream>
#include <algorithm>



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
    int numberOfRowsToGenerate = 4;
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

void nodeGraph::checkForDrops(std::vector<int>& _doomedIndexes) {
    std::vector<int> confirmedSafeIndexes;
    std::vector<int> visitedIndexes(_doomedIndexes);
    for (int i : _doomedIndexes) {
        for (node* n : mGraph[i].mNodeAdjacencyList) {
            verifyIsSafe(n->mIndex, confirmedSafeIndexes, visitedIndexes);
        }
    }
    
    std::sort(confirmedSafeIndexes.begin(), confirmedSafeIndexes.end());
    std::sort(visitedIndexes.begin(), visitedIndexes.end());
    
    std::vector<int> newDoomed(visitedIndexes.size() - confirmedSafeIndexes.size());
    std::set_difference(confirmedSafeIndexes.begin(), confirmedSafeIndexes.end(), visitedIndexes.begin(), visitedIndexes.end(), newDoomed.begin());
    
    //_doomedIndexes = newDoomed;



}

bool nodeGraph::verifyIsSafe(int _triggerIndex, std::vector<int>& _confirmedSafeIndexes, std::vector<int>& _visitedIndexes) {
    _visitedIndexes.emplace_back(_triggerIndex);
    bool result = _triggerIndex < mBOARD_WIDTH;
    if (result) {
        _confirmedSafeIndexes.emplace_back(_triggerIndex);
        return(true);
    }

    
    for (node* n : mGraph[_triggerIndex].mNodeAdjacencyList) {
        bool isVisited = false;
        for (int i : _visitedIndexes) {
            if (n->mIndex == i) {
                isVisited = true;
            }
        }
        if (isVisited) {
            for (int i : _confirmedSafeIndexes) {
                if (n->mIndex == i) {
                    _confirmedSafeIndexes.emplace_back(_triggerIndex);
                    return(true);
                }
            }
        }
        else result & verifyIsSafe(n->mIndex, _confirmedSafeIndexes, _visitedIndexes);  
    }
    
    if (result) { _confirmedSafeIndexes.emplace_back(_triggerIndex); }
    return(result);
}

std::vector<int> nodeGraph::checkForDestruction(int _triggerPos) {
    std::vector<int> doomedIndexes;

    std::vector<bool> visitedVec(mBOARD_HEIGHT * mBOARD_WIDTH, false);

    mGraph[_triggerPos].updateAdjacencies(mGraph);
    checkColorMatch(_triggerPos, visitedVec, doomedIndexes);
    if (doomedIndexes.size() >= 3) {
        //for each node adjacent to a destroyed node, verify it is anchored
        
        checkForDrops(doomedIndexes);
       
        


        return(doomedIndexes);
    }

    return(std::vector<int>());

    
    
}

void nodeGraph::checkColorMatch(int _triggerIndex, std::vector<bool>& _visitedVec, std::vector<int>& _matchingNodeIndexes) {
    _visitedVec[_triggerIndex] = true;
    _matchingNodeIndexes.emplace_back(_triggerIndex);
    for (node* n : mGraph[_triggerIndex].mNodeAdjacencyList) {
        if (n->mColor == mGraph[_triggerIndex].mColor && !_visitedVec[n->mIndex]) {
            checkColorMatch(n->mIndex, _visitedVec, _matchingNodeIndexes);
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
    for (int i : *mAdjacencyList) {
        if (!_graph[i].mIsDisabled) {
            mNodeAdjacencyList.emplace_back(&_graph[i]);
        }
    }
}

std::vector<sf::Color> node::colorVec = {	sf::Color::White, sf::Color::Magenta, sf::Color::Yellow,
                                            sf::Color::Red, sf::Color::Blue, sf::Color::Green };

sf::Color node::getRandomColor() {
    return(colorVec[rand() % colorVec.size()]);
}

