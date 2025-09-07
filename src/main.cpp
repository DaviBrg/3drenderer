#include <chrono>
#include <cmath>
#include <thread>

#include "window.h"
#include "vec.h"


namespace
{
constexpr int N_POINTS = 9*9*9;
constexpr double fov_factor = 640.0;
constexpr unsigned target_fps = 60;
constexpr auto target_fps_time = std::chrono::milliseconds(1000/target_fps);
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

simplegl::point3_t translate(simplegl::point3_t const& point, double factor_x, double factor_y, double factor_z) {
    simplegl::point3_t projected_point = {point.x + factor_x, point.y + factor_y, point.z + factor_z};
    return projected_point;
}

simplegl::point3_t scale(simplegl::point3_t const& point, double factor_x, double factor_y, double factor_z) {
    simplegl::point3_t projected_point = {point.x*factor_x, point.y*factor_y, point.z*factor_z};
    return projected_point;
}

simplegl::point3_t scale(simplegl::point3_t const& point, double factor) {
    simplegl::point3_t projected_point = {point.x*factor, point.y*factor, point.z*factor};
    return projected_point;
}

simplegl::point3_t rotate_x(simplegl::point3_t const& point, double angle) {
    simplegl::point3_t projected_point = {point.x, std::sin(-angle)*point.z + std::cos(-angle)*point.y, std::cos(-angle)*point.z - std::sin(-angle)*point.y};
    return projected_point;
}

simplegl::point3_t rotate_y(simplegl::point3_t const& point, double angle) {
    simplegl::point3_t projected_point = {std::sin(angle)*point.z + std::cos(angle)*point.x, point.y, std::cos(angle)*point.z - std::sin(angle)*point.x};
    return projected_point;
}

simplegl::point3_t rotate_z(simplegl::point3_t const& point, double angle) {
    simplegl::point3_t projected_point = {std::cos(angle)*point.x - std::sin(angle)*point.y, std::sin(angle)*point.x + std::cos(angle)*point.y, point.z};
    return projected_point;
}

void update() {
    static double t = 0;
    for (int i = 0; i < N_POINTS; ++i) {
        simplegl::point3_t point = cube_points[i];
        point = rotate_x(point, t);
        point = rotate_y(point, t);
        point = rotate_z(point, t);
        point = scale(point, std::abs(::cos(t)) + 0.25);
        point = translate(point, 2*std::cos(t), 2*std::sin(t), 0.0);

        point.z -= camera_position.z;

        simplegl::point2_t projected_point = project(point);
        projected_points[i] = projected_point;
    }
    t += 0.0125;

}

void render(simplegl::Window & window) {

    window.drawGrid(12);

    for (int i = 0; i < N_POINTS; ++i) {
        window.drawRectangle(
            projected_points[i].x + window.width()/2,
            -projected_points[i].y + window.height()/2,
            4,
            4,
            0xFFFFFF00
        );
    }

    window.renderColorBuffer();
    window.clearColorBuffer(0xFF000000);
    window.renderPresent();
}

void wait_frame_time(std::chrono::system_clock::time_point  time_start, std::chrono::milliseconds target_fps_time) {
    auto processing_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - time_start);
        if (processing_time < target_fps_time) {
            auto remaining_time = target_fps_time - processing_time;
            std::this_thread::sleep_for(remaining_time);
        }
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
        auto time_start = std::chrono::system_clock::now();
        process_input(keep_running);
        update();
        render(window);
        wait_frame_time(time_start, target_fps_time);

        
    }

    
    return 0;
}