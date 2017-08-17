#include "chip8.h"

using std::string;
using std::printf;
using std::ifstream;

void usage(const string&);
inline bool fexists(const string& fname){
    ifstream f(fname.c_str());
    return f.good();
}

int main( int argc, char *argv[] ){
    srand(time(NULL));
    string rom_fn="";
    bool debug = true;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("SDL couldnot initialize! SDL_error: %s\n", SDL_GetError());
        return -1;
    }
    
    SDL_Window *win = SDL_CreateWindow("Hello Wordl!",  SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (win == nullptr){
        printf("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr){
        cleanup(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    const char *imagePath = "../img/hello1.bmp";
    SDL_Surface *bmp = SDL_LoadBMP(imagePath);
    if (bmp == nullptr){
        cleanup(ren, win);
        printf("SDL_LoadBMP error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
    cleanup(bmp);
    if (tex == nullptr){
        cleanup(ren, win);
        printf("SDL_CreateTextureFromSurface Error: %s \n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    for(auto i = 0; i < 3; i++){
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, NULL, NULL);
        SDL_RenderPresent(ren);
        SDL_Delay(1000);
    }

    printf("Cleaning up!!!\n");
    
    cleanup(ren, tex, win);
    SDL_Quit();

    if (argc == 1 || argc >= 3 ){
        usage(argv[0]);
    }
    else{
        rom_fn = argv[1];
        if (!fexists(rom_fn)){
            printf("\nFile %s doesn't exists, check ROM path and run emualtor again.\n", rom_fn.c_str());
            return -1;
        }
        vm chip;
        chip.initialize(debug);
        chip.run(rom_fn);
    }
    return 0;
}

void usage(const string& fname){
    printf("Help:\n./%s [optional arguments] <rom_file>\n", fname.c_str());
    printf("\n\noptional arguments:\n");
    printf("\t\t-d\t--debug\t\t\t runs loaded rom in debug mode\n");
}
