#pragma once
#include "nodeGraph.h"

class game {
public:
	game();
	const int mBOARD_WIDTH = 16;
	const int mBOARD_HEIGHT = 12;
	int mNUM_NODES;
	nodeGraph mNodeGraph;
};

