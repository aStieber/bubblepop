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
	void checkForDestruction(int _triggerPos);
	void checkColorMatch(int _triggerPos, std::vector<bool>& _visitedVec, std::vector<node*>& _matchingNodeSet);

		
private:
};



class node {
public:
	node();
	node(int _pos, std::vector<int>* _adjacencyList, bool _isDisabled, sf::Color _color=getRandomColor());

	bool mIsDisabled =true;
	sf::Color mColor;
	int mPos = -1;
	
	std::vector<int>* mAdjacencyList;
	std::vector<node*> mNodeAdjacencyList;

	void updateAdjacencies(std::vector<node>& _graph);
private:
	static std::vector<sf::Color> colorVec; 
	static sf::Color getRandomColor();
};
