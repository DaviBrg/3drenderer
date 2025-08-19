#ifndef SIMPLE_GL_DISPLAY_H
#define SIMPLE_GL_DISPLAY_H

#include <stdint.h>

#include <SDL2/SDL.h>

typedef struct {
    int window_width;
    int window_height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    uint32_t* color_buffer;
    SDL_Texture* color_buffer_texture;
} SimpleGLDisplay;

SimpleGLDisplay* initialize_display(void);

void draw_pixel(SimpleGLDisplay* display, int x, int y, uint32_t pixel_value);

void clear_color_buffer(SimpleGLDisplay* display, uint32_t color);

void render_color_buffer(SimpleGLDisplay* display);

void draw_grid(SimpleGLDisplay* display, int multiple);

void draw_rect(SimpleGLDisplay* display, int x_pos, int y_pos, int width, int height, uint32_t color);

void destroy_display(SimpleGLDisplay* display);

#endif