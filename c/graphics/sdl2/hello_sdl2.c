#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

// Very tiny bitmap font for the example.
// Only supports uppercase letters and space.
void draw_text(SDL_Renderer *r, int x, int y, const char *text) {
    // 8x8 pixel font for simplicity
    static const unsigned char font[][8] = {
        // 'A'
        {0x18, 0x24, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x00},
        // 'B'
        {0x7C, 0x42, 0x42, 0x7C, 0x42, 0x42, 0x7C, 0x00},
        // 'C'
        {0x3C, 0x42, 0x40, 0x40, 0x40, 0x42, 0x3C, 0x00},
        // ...
        // (We will only map A–Z and use a placeholder for others)
    };

    // For simplicity: only draws uppercase A–C + space.
    // Everything else becomes a block.
    for (int i = 0; text[i]; i++) {
        unsigned char c = text[i];
        const unsigned char *glyph;

        if (c == ' ')
            glyph = NULL;
        else if (c >= 'A' && c <= 'C')
            glyph = font[c - 'A'];
        else
            glyph = font[0]; // fallback = A

        if (glyph) {
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    if (glyph[row] & (1 << (7 - col))) {
                        SDL_RenderDrawPoint(r, x + col, y + row);
                    }
                }
            }
        }
        x += 8;
    }
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow(
        "Hello SDL2",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_SHOWN
    );

    if (!win) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(
        win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!ren) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    int running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;
        }

        // Background color (dark blue-ish)
        SDL_SetRenderDrawColor(ren, 30, 30, 50, 255);
        SDL_RenderClear(ren);

        // Text color
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

        // Draw text
        draw_text(ren, 50, 50, "HELLO WORLD!");

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
