#include "game.h"

game::game() {
	mNUM_NODES = mBOARD_HEIGHT * mBOARD_WIDTH;
	mNodeGraph = nodeGraph(mBOARD_WIDTH, mBOARD_HEIGHT);

}
