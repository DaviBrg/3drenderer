#include "display.h"

#include <stdbool.h>

SimpleGLDisplay* initialize_display(void) {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error inirializing SDL.\n");
        return NULL;
    }

    SimpleGLDisplay* display = calloc(1, sizeof(SimpleGLDisplay));

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);

    display->window_width = display_mode.w;
    display->window_height = display_mode.h;

    display->window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        display->window_width,
        display->window_height,
        SDL_WINDOW_BORDERLESS
    );

    if (!display->window) {
        fprintf(stderr, "Error creating SDL window.\n");
        SDL_Quit();
        free(display);
        return NULL;
    }

    display->renderer = SDL_CreateRenderer(display->window, -1, 0);

    if (!display->renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        SDL_DestroyWindow(display->window);
        SDL_Quit();
        free(display);
        return NULL;
    }

    // SDL_SetWindowFullscreen(display->window, SDL_WINDOW_FULLSCREEN);

    return display;
}

void draw_pixel(SimpleGLDisplay* display, int x, int y, uint32_t pixel_value) {
    if ((x >= 0) && (x < display->window_width) && (y >= 0) && (y < display->window_height)) {
        display->color_buffer[(display->window_width * y) + x] = pixel_value;
    }
}

void clear_color_buffer(SimpleGLDisplay* display, uint32_t color) {
    for (int y = 0; y < display->window_height; ++y) {
        for (int x = 0; x < display->window_width; ++x) {
            draw_pixel(display, x, y, color);
        }
    }  
}

void render_color_buffer(SimpleGLDisplay* display) {
    SDL_UpdateTexture(
        display->color_buffer_texture,
        NULL,
        display->color_buffer,
        (int) (display->window_width * sizeof(uint32_t))
    );

    SDL_RenderCopy(
        display->renderer,
        display->color_buffer_texture,
        NULL,
        NULL
    );
}

void draw_grid(SimpleGLDisplay* display, int multiple) {
    for (int y = 0; y < display->window_height; ++y) {
        for (int x = 0; x < display->window_width; ++x) {
            if ((y%multiple == 0) || (x%multiple == 0)) {
                draw_pixel(display, x, y, 0xFF444444);
            }
        }
    }
}

void draw_rect(SimpleGLDisplay* display, int x_pos, int y_pos, int width, int height, uint32_t color) {
    int y_final_pos = y_pos + height;
    int x_final_pos = x_pos + width;
    for (int y = y_pos; y < y_final_pos; ++y) {
        for (int x = x_pos; x < x_final_pos; ++x) {
            draw_pixel(display, x, y, color);
        }
    }
}

void destroy_display(SimpleGLDisplay* display) {
    free(display->color_buffer);
    SDL_DestroyRenderer(display->renderer);
    SDL_DestroyWindow(display->window);
    SDL_Quit();
    free(display);
}