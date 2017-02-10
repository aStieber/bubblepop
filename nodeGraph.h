#pragma once
#include <vector>
#include <SFML\Graphics.hpp>


class node;
class nodeGraph;

class nodeGraph {
public:
    nodeGraph();
    nodeGraph(int _BOARD_WIDTH, int _BOARD_HEIGHT);

    std::vector<node> mGraph;
    std::vector<std::vector<int>> mAdjacencyMasterList;
    int mBOARD_HEIGHT, mBOARD_WIDTH;

    void populateInitialGraph();
    void updateNodeAdjacencies();
    void populateAdjacencyMasterList();

    void addNode(int _Pos);
    std::vector<int> checkForDestruction(int _triggerPos);
    void updateMeterPositions();
        
private:
    void checkColorMatch(int _triggerPos, std::vector<bool>& _visitedVec, std::vector<int>& _matchingNodeSet);
    void checkForDrops(std::vector<int>& _doomedIndexes);
    bool verifyIsSafe(int _triggerIndex, std::vector<int>& _confirmedSafeIndexes, std::vector<int>& _visitedIndexes);
};



class node {
public:
    node(); //triggerBall form
    node(int _pos, std::vector<int>* _adjacencyList, bool _isDisabled, sf::Color _color=getRandomColor());

    bool mIsDisabled = true;
    sf::Color mColor;
    int mIndex = -1;
    sf::Vector2f mMeterPos;
    
    std::vector<int>* mAdjacencyList;
    std::vector<node*> mNodeAdjacencyList;

    void updateAdjacencies(std::vector<node>& _graph);
private:
    static std::vector<sf::Color> colorVec; 
    static sf::Color getRandomColor();
};
