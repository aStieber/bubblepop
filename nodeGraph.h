#pragma once
#include <vector>
#include <SFML\Graphics.hpp>


class node;
class game;
class nodeGraph;


class nodeGraph {
public:
	nodeGraph();
	nodeGraph(int _BOARD_WIDTH, int _BOARD_HEIGHT);
	std::vector<node> mGraph;
	std::vector<std::vector<int>> mAdjacencyMasterList;

	void populateInitialGraph(int _BOARD_WIDTH, int _BOARD_HEIGHT);
	void updateNodeAdjacencies();
	void populateAdjacencyMasterList(int _BOARD_WIDTH, int _BOARD_HEIGHT);

	void addNode(int _Pos);
	void runPhysicsStep(int _triggerPos);
	void checkColorMatch(int _triggerPos, std::vector<bool>& _visitedVec, std::vector<node*>& _matchingNodeSet);

	int mBOARD_HEIGHT, mBOARD_WIDTH;
private:
};



class node {
public:
	node();
	node(int _pos, std::vector<int>* _adjacencyList, bool _isDisabled);



	bool mIsDisabled =true;
	int mColor;
	int mPos = -1;
	
	
	std::vector<int>* mAdjacencyList;
	std::vector<node*> mNodeAdjacencyList;

	void updateAdjacencies(std::vector<node>& _graph);
};
