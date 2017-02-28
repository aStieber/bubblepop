#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "graphics.h"
#include "game.h"
#include "fixedNeuralNet.h"


int main()
{
    bool nnMode = true;
    
    game theGame;

    if (nnMode) {
        neuralNet nn(&theGame);
    }
    else {
        sf::RenderWindow window;
        graphics g(&theGame, &window);
        g.runGraphicsLoop();
    }
    
    return 0;
}
