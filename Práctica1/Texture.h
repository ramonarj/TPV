class Texture{
private:
    SDL_Texture* texture = nullptr;
    SDL_Renderer* renderer = nullptr;
    unsigned int w = 0;
    unsigned int h = 0;
    unsigned int fw = 0;
    unsigned int fh = 0;
    unsigned int numRows = 0;
    unsigned int numCols = 0;

public:
    Texture(SDL_Renderer* r) : renderer(r) ();

    void free();
    bool load();
    void render() const;
    void renderFrame() const;
}