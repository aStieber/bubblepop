#pragma once
#include <vector>
#include <SFML\Graphics.hpp>



class game {
public:
	game();

	const int BOARD_WIDTH = 16;
	const int BOARD_HEIGHT = 12;
	
	nodeGraph nG;

private:
	
	



};


class nodeGraph {
public:
	nodeGraph();
	nodeGraph(int _BOARD_WIDTH, int _BOARD_HEIGHT);
	std::vector<node> Graph;
	std::vector<std::vector<int>> adjacencyMasterList;

	void populateInitialGraph(int _BOARD_WIDTH, int _BOARD_HEIGHT);
	void updateNodeAdjacencies();
	void populateAdjacencyMasterList(int _BOARD_WIDTH, int _BOARD_HEIGHT);

private:
};



class node {
public:
	node(int _pos, std::vector<int>* _adjacencyList, bool _isDisabled);

	bool isDisabled =true;
	int color;
	int pos = -1;
	
	
	std::vector<int>* adjacencyList;
	std::vector<node> nodeAdjacencyList;

	void updateAdjacencies(std::vector<node>& _graph);
	
private:

};