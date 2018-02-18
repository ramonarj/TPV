class PlayState: public GameState
{

    //unsigned int po to las variables;
    GameMap* map;
    PacMan* pac; 
    ....
    ....

    void handleEvent(SDL_Event& e)
    {
        if (e.type == SDL.KEYDOWN)
        {
            if (e.key....... == SDLK.ESCAPE)
                app->getStateMachine()->pushState(new PauseState(app, .../*podemos pasarle el PlayState para lo del guarda*/))
            else
                pac->handleEvent(e);
        }
    }

    void update()
    {
        GameState::update();
        if (.......)
        {
            //manejar paso de nivel
            loadLevel();
        }
    }  
}

class PauseState: public GameState
{
    PauseState(SDLApp* app, ....): GameState(app)
    {
        ......
        ......
        stage->pushBack(new Button(app, app->getTexture(ResumeButtonTexture), buttonW, buttonH, .....));
        ...
        ...
        stage->pushBack(new Button(.........................));
    }
}

class Button: public GameObject
{
    int x, y, w, h;
    Texture* texture;
    SDLApp* app;
    .......
    .......

    bool handleEvent(....)
    {
        if (es pulsacion de raton)
        {
            if (SDL_PointInRect(.....))
                cout << "Boton pulsado";
        }
    }
}