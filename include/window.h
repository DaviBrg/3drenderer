#pragma once

#include <vector>
#include <optional>

#include <SDL2/SDL.h>

namespace simplegl
{

struct Window {


static std::optional<Window> Create(int window_width, int window_height);

void setupDrawingBuffer();

void drawPixel(int x, int y, uint32_t pixel_value);

void clearColorBuffer(uint32_t color);

void renderColorBuffer();

void drawGrid(int multiple);

void drawRectangle(int x_pos, int y_pos, int width, int height, uint32_t color);

void drawLine(int x0, int y0, int x1, int y1, uint32_t color);

void drawTriagnle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void renderPresent();

int width() const {
    return _window_width;
}

int height() const {
    return _window_height;
}

SDL_Texture* colorBufferTexture() {
    return _color_buffer_texture;
}

void setColorBufferTexture(SDL_Texture* colorBufferTexture) {
    _color_buffer_texture = colorBufferTexture;
}

std::vector<uint32_t>& colorBuffer() {
    return _color_buffer;
}

~Window();

Window(const Window&) = delete;
const Window& operator=(const Window&) = delete;
Window(Window&&);
Window& operator=(Window&&);

private:

Window() = default;

int _window_width = 0;
int _window_height = 0;
std::vector<uint32_t> _color_buffer;
SDL_Window* _window = nullptr;
SDL_Renderer* _renderer = nullptr;
SDL_Texture* _color_buffer_texture = nullptr;
bool _is_moved = false;

};

}
