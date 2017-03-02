#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "graphics.h"
#include "game.h"
#include "fixedNeuralNet.h"


int main()
{
    bool nnMode = true;
    srand(std::time(NULL));
    
    game theGame;

    if (nnMode) {
        neuralNet nn(&theGame);
        nn.start(1000);
    }
    else {
        sf::RenderWindow window;
        graphics g(&theGame, &window);
        g.runGraphicsLoop();
    }
    
    return 0;
}
