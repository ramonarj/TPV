//Game.h

#include "Dog.h"
#include "Helicopter.h"
#include <string>

using namespace std;

const unsigned int FRAME_RATE = 50;
const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;
const unsigned int DOG_SIZE = 100;
string IMAGES_PATH = "images/";

const unsigned int NUM_TEXTURES = 3;



class Game{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool error = false;
    Dog dog;
    Helicopter helicopter;

    //Texture* textures [NUM_TEXTURES]
    Texture* backgroundText;
    Texture* dogText;
    Texture* helicopterText;
public:
    ...
    void run()
    {
        while (!exit)
        {
            ...
            handleEnvents(); //Método que controla los eventos (while(pollevent))
            update();
            render();
            ...
        }
    }

    void update()
    {
        dog.update();
        helicopter.update();

    }

    void render()
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(...);
        backgroundText->render(); // SDL_Rect inicializacion con llave {0, 0, WIN_WIDTH, WIN_HEIGTH}
        dog.render();
        SDL_RenderPresent(...);
    }

}