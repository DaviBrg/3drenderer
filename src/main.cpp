// #include <stdbool.h>

// #include "SimpleGL/display.h"
// #include "SimpleGL/vector.h"

// #define N_POINTS (9*9*9)
// point3_t cube_points[N_POINTS];
// point2_t projected_points[N_POINTS];

// point3_t camera_position = {0,0,-5};

// float fov_factor = 640;

// void setup(SimpleGLDisplay *display) {
//     display->color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * display->window_width* display-> window_height);

//     display->color_buffer_texture = SDL_CreateTexture(
//         display->renderer,
//         SDL_PIXELFORMAT_ARGB8888,
//         SDL_TEXTUREACCESS_STREAMING,
//         display->window_width,
//         display->window_height
//     );

//     int point_counter = 0;
//     for (float x = -1; x <= 1; x += 0.25) {
//         for (float y = -1; y <= 1; y += 0.25) {
//             for (float z = -1; z <= 1; z += 0.25) {
//                 point3_t point = {x, y, z};
//                 cube_points[point_counter++] = point;
//             }
//         }
//     }

// }

// void process_input(bool* keep_runing) {
//     SDL_Event event;
//     while(SDL_PollEvent(&event)) {
//         switch(event.type) {
//         case SDL_QUIT:
//             *keep_runing = false;
//             break;
//         case SDL_KEYDOWN:
//             if(event.key.keysym.sym == SDLK_ESCAPE) {
//                 *keep_runing = false;
//             }
//             break;
//         }
//     }
// }

// point2_t project(SimpleGLDisplay *display, point3_t point) {
//     display++;
//     point2_t projected_point = {
//         .x = (fov_factor * point.x)/point.z,
//         .y = (fov_factor * point.y)/point.z
//     };
//     return projected_point;
// }

// void update(SimpleGLDisplay *display) {
//     for (int i = 0; i < N_POINTS; ++i) {
//         point3_t point = cube_points[i];

//         point.z -= camera_position.z;

//         point2_t projected_point = project(display, point);
//         projected_points[i] = projected_point;
//     }
// }

// void render(SimpleGLDisplay *display) {
//     draw_grid(display, 10);

//     for (int i = 0; i < N_POINTS; ++i) {
//         draw_rect(
//             display,
//             projected_points[i].x + display->window_width/2,
//             projected_points[i].y + display->window_height/2,
//             4,
//             4,
//             0xFFFFFF00
//         );
//     }

//     render_color_buffer(display);
//     clear_color_buffer(display, 0xFF000000);
//     SDL_RenderPresent(display->renderer);
// }

// int main(void) {
//     SimpleGLDisplay *display = initialize_display();
//     bool keep_running = (display != NULL);

//     if (!keep_running) {
//         return 1;
//     }

//     setup(display);

//     while(keep_running)
//     {
//         process_input(&keep_running);
//         update(display);
//         render(display);
//     }

//     destroy_display(display);
    
//     return 0;
// }

#include "window.h"
#include "vec.h"

namespace
{
constexpr int N_POINTS = 9*9*9;
constexpr double fov_factor = 640.0;
simplegl::point3_t cube_points[N_POINTS] = {};
simplegl::point2_t projected_points[N_POINTS] = {};
simplegl::point3_t camera_position = {0.0, 0.0, -5.0};
}

void process_input(bool& keep_runing) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            keep_runing = false;
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE) {
                keep_runing = false;
            }
            break;
        }
    }
}

simplegl::point2_t project(simplegl::point3_t const& point) {
    simplegl::point2_t projected_point = {(fov_factor * point.x)/point.z,(fov_factor * point.y)/point.z};
    return projected_point;
}

void update() {
    for (int i = 0; i < N_POINTS; ++i) {
        simplegl::point3_t point = cube_points[i];

        point.z -= camera_position.z;

        simplegl::point2_t projected_point = project(point);
        projected_points[i] = projected_point;
    }
}

void render(simplegl::Window & window) {

    window.drawGrid(10);

    for (int i = 0; i < N_POINTS; ++i) {
        window.drawRectangle(
            projected_points[i].x + window.width()/2,
            projected_points[i].y + window.height()/2,
            4,
            4,
            0xFFFFFF00
        );
    }

    window.renderColorBuffer();
    window.clearColorBuffer(0xFF000000);
    window.renderPresent();
}

int main() {

    int point_counter = 0;
    for (float x = -1; x <= 1; x += 0.25) {
        for (float y = -1; y <= 1; y += 0.25) {
            for (float z = -1; z <= 1; z += 0.25) {
                simplegl::point3_t point = {x, y, z};
                cube_points[point_counter++] = point;
            }
        }
    }

    auto window_opt = simplegl::Window::Create(1920,1080);
    bool keep_running = window_opt.has_value();

    if (!keep_running) {
        return 1;
    }

    auto& window = *window_opt;
    window.setupDrawingBuffer();

    while(keep_running)
    {
        process_input(keep_running);
        update();
        render(window);
    }

    
    return 0;
}