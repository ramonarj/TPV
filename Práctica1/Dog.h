const unsigned int SPEED = 50;

class Dog{
private:
    unsigned int w, h;
    unsigned int x, y;
    int dirX;
    int dirY;
    Texture* texture;
    Game* game;

public:
    void render()
    {
        texture-> renderFrame(...); //en los puntos suspensivos hay que poner la expresion que tiene la division esa to rara entre 6
    }
    void update()
    {
        x = (x + (dirX * SPEED)) % WIN_WIDTH;
    }

}