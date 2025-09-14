#include "window.h"

#include <iostream>

namespace simplegl
{

std::optional<Window> Window::Create(int window_width, int window_height) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error inirializing SDL.\n";
        return std::nullopt;
    }

    auto resultOpt = std::optional{Window()};
    Window& result = resultOpt.value();

    result._window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_BORDERLESS
    );

    if (!result._window) {
        std::cerr << "Error creating SDL window.\n";
        return std::nullopt;
    }

    result._renderer = SDL_CreateRenderer(result._window, -1, 0);

    if (!result._renderer) {
        std::cerr << "Error creating SDL renderer.\n";
        return std::nullopt;
    }

    result._window_height = window_height;
    result._window_width = window_width;

    SDL_SetWindowFullscreen(result._window, SDL_WINDOW_FULLSCREEN);
    
    return resultOpt;
}

void Window::setupDrawingBuffer() {

    auto colorBufferTexture = SDL_CreateTexture(
        _renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        _window_width,
        _window_height
    );

    if (colorBufferTexture) {
        _color_buffer = std::vector<uint32_t>(_window_width*_window_height, static_cast<uint32_t>(0));
        SDL_DestroyTexture(_color_buffer_texture);
        _color_buffer_texture = colorBufferTexture;
    }    
}

void Window::drawPixel(int x, int y, uint32_t pixel_value) {
    y = _window_height - 1 - y; 
    if ((x >= 0) && (x < _window_width) && (y >= 0) && (y < _window_height)) {
        _color_buffer[(_window_width * y) + x] = pixel_value;
    }
}

void Window::clearColorBuffer(uint32_t color) {
    for (int y = 0; y < _window_height; ++y) {
        for (int x = 0; x < _window_width; ++x) {
            drawPixel(x, y, color);
        }
    }  
}

void Window::renderColorBuffer() {

    SDL_UpdateTexture(
        _color_buffer_texture,
        nullptr,
        _color_buffer.data(),
        static_cast<int>(_window_width * sizeof(uint32_t))
    );

    SDL_RenderCopy(
        _renderer,
        _color_buffer_texture,
        nullptr,
        nullptr
    );
}

void Window::drawGrid(int multiple) {
    for (int y = 0; y < _window_height; ++y) {
        for (int x = 0; x < _window_width; ++x) {
            if ((y%multiple == 0) || (x%multiple == 0)) {
                drawPixel(x, y, 0xFF444444);
            }
        }
    }
}

void Window::drawRectangle(int x_pos, int y_pos, int width, int height, uint32_t color) {
    int y_final_pos = y_pos + height;
    int x_final_pos = x_pos + width;
    for (int y = y_pos; y < y_final_pos; ++y) {
        for (int x = x_pos; x < x_final_pos; ++x) {
            drawPixel(x, y, color);
        }
    }
}

void Window::drawLine(int x0, int y0, int x1, int y1, uint32_t color) {
    int sideX = std::abs(x1 - x0);
    int sideY = std::abs(y1 - y0);

    int sideLength = sideX >= sideY ? sideX : sideY;

    double stepX = (x1 - x0) / static_cast<double>(sideLength);
    double stepY = (y1 - y0) / static_cast<double>(sideLength);

    for (int i = 0; i < sideLength; ++i) {
        int x = x0 + std::round(i*stepX);
        int y = y0 + std::round(i*stepY);
        drawPixel(x,y,color);
    }

}

void Window::drawTriagnle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

void Window::renderPresent() {
    SDL_RenderPresent(_renderer);
}

Window::~Window() {
    if (_is_moved) return;
    if (_color_buffer_texture) SDL_DestroyTexture(_color_buffer_texture);
    if (_renderer) SDL_DestroyRenderer(_renderer);
    if (_window) SDL_DestroyWindow(_window);
    SDL_Quit();
}

Window::Window(Window &&other) :
_window_width{other._window_width},
_window_height{other._window_height},
_color_buffer{std::move(other._color_buffer)},
_window{other._window},
_renderer{other._renderer},
_color_buffer_texture{other._color_buffer_texture},
_is_moved{other._is_moved} {
    other._is_moved = true;
}

Window &Window::operator=(Window &&other) {
    if (this != &other) {
        std::swap(*this, other);
    }
    return *this;
}

}