#include "graph.h"
#include <iostream>

game::game() {
	nG.populateAdjacencyMasterList(BOARD_WIDTH, BOARD_HEIGHT);
	nG.populateInitialGraph(BOARD_WIDTH, BOARD_HEIGHT);
}

void nodeGraph::populateAdjacencyMasterList(int _BOARD_WIDTH, int _BOARD_HEIGHT) {
	adjacencyMasterList.clear();
	for (size_t i = 0; i < (_BOARD_HEIGHT * _BOARD_WIDTH); i++) {
		std::vector<int> tmpAdjacencyList;
		bool isEvenRow = ((int)(i / _BOARD_WIDTH) % 2 == 0);
		int leftMod = isEvenRow ? -1 : 0;
		int rightMod = isEvenRow ? 0 : 1;
		bool leftAllowed = ((i % 16) != 0);
		bool rightAllowed = (((i + 1) % 16) != 0);

		if (leftAllowed) { tmpAdjacencyList.push_back(i - 1); } //left
		if (rightAllowed) { tmpAdjacencyList.push_back(i + 1); }//right
		if (i >= _BOARD_WIDTH) { //not top row
			if (leftAllowed) { tmpAdjacencyList.push_back(i - _BOARD_WIDTH + leftMod); } //upper left
			if (rightAllowed) { tmpAdjacencyList.push_back(i - _BOARD_WIDTH + rightMod); } //upper right			
		}
		if (i < ((_BOARD_HEIGHT - 1) * _BOARD_WIDTH)) { //not bottom row
			if (leftAllowed) { tmpAdjacencyList.push_back(i + _BOARD_WIDTH + leftMod); }
			if (rightAllowed) { tmpAdjacencyList.push_back(i + _BOARD_WIDTH + rightMod); }
		}

		adjacencyMasterList.push_back(tmpAdjacencyList);
	}
}

nodeGraph::nodeGraph(int _BOARD_WIDTH, int _BOARD_HEIGHT) {
	std::vector<int> Graph(_BOARD_HEIGHT * _BOARD_WIDTH, NULL);
}

void nodeGraph::populateInitialGraph(int _BOARD_WIDTH, int _BOARD_HEIGHT) { //add randomness
	for (size_t i = 0; i < (_BOARD_WIDTH * 3); i++) {
		Graph[i] = node(i, &adjacencyMasterList[i], false);
	}
	updateNodeAdjacencies();
}

void nodeGraph::updateNodeAdjacencies() {
	for (node n : Graph) { n.updateAdjacencies(Graph); }
}

node::node(int _pos, std::vector<int>* _adjacencyList, bool _isDisabled) {
	pos = _pos;
	adjacencyList = _adjacencyList;
	isDisabled = _isDisabled;
}

void node::updateAdjacencies(std::vector<node>& _graph) {
	nodeAdjacencyList.clear();
	for (int i : *adjacencyList) {
		if (!_graph[i].isDisabled) {
			nodeAdjacencyList.push_back(&_graph[i]);
		}
	}
}