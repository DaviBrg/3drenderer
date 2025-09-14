#include <chrono>
#include <cmath>
#include <numbers>
#include <thread>

#include "mesh.h"
#include "window.h"
#include "vec.h"

namespace
{

constexpr double fovFactor = 640.0;
constexpr unsigned targetFps = 60;
constexpr auto targetFpsTime = std::chrono::milliseconds(1000/targetFps);
constexpr int windowWidth = 1920;
constexpr int windowHeight = 1080;
constexpr uint32_t colorYellow = 0xFFFFFF00;
constexpr uint32_t colorGreen = 0xFF00FF00;

std::vector<simplegl::point3_t> pointsToProject;
std::vector<simplegl::point2_t> pointsToDraw;
simplegl::point3_t camera_position = {0.0, 0.0, -5.0};

}

simplegl::vec3_t getPointFromUV(double u, double v) {
    const double cosU = std::cos(u);
    return simplegl::vec3_t{
        cosU*std::cos(v),
        std::sin(u),
        cosU*std::sin(v)};
}

simplegl::Mesh buildSphereMesh(unsigned int geometryLevel) {
    auto result = simplegl::Mesh();
    constexpr double uStart = std::numbers::pi*0.5;
    constexpr double vStart = 0.0;
    double step = (std::numbers::pi)/geometryLevel;

    double previousU = uStart;
    double previousV = vStart;

    for (unsigned int uIdx = 1; uIdx <= geometryLevel; ++uIdx) {

        const double u = uStart - (uIdx*step);

        for (unsigned int vIdx = 1; vIdx <= geometryLevel*2; ++vIdx) {

            const double v = vStart + (vIdx*step);

            const auto triangle1 = simplegl::Triangle{getPointFromUV(previousU, previousV), getPointFromUV(previousU, v), getPointFromUV(u, v)};
            result.addTriangle(triangle1);

            const auto triangle2 = simplegl::Triangle{getPointFromUV(u, v), getPointFromUV(u, previousV), getPointFromUV(previousU, previousV)};
            result.addTriangle(triangle2);

            previousV = v;
        }
        previousU = u;
    }

    return result;
}

void processInput(bool& keep_runing) {
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
    simplegl::point2_t projected_point = {(fovFactor * point.x)/point.z,(fovFactor * point.y)/point.z};
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

simplegl::point3_t rotateX(simplegl::point3_t const& point, double angle) {
    simplegl::point3_t projected_point = {point.x, std::sin(-angle)*point.z + std::cos(-angle)*point.y, std::cos(-angle)*point.z - std::sin(-angle)*point.y};
    return projected_point;
}

simplegl::point3_t rotateY(simplegl::point3_t const& point, double angle) {
    simplegl::point3_t projected_point = {std::sin(angle)*point.z + std::cos(angle)*point.x, point.y, std::cos(angle)*point.z - std::sin(angle)*point.x};
    return projected_point;
}

simplegl::point3_t rotateZ(simplegl::point3_t const& point, double angle) {
    simplegl::point3_t projected_point = {std::cos(angle)*point.x - std::sin(angle)*point.y, std::sin(angle)*point.x + std::cos(angle)*point.y, point.z};
    return projected_point;
}

void update() {
    static double t = 0;
    const int widhtDividedBy2 = windowWidth/2;
    const int heightDividedBy2 = windowHeight/2;
    pointsToDraw.clear();
    for (simplegl::point3_t point : pointsToProject) {

        point = rotateX(point, t);
        point = rotateY(point, t);
        point = rotateZ(point, t);
        point = scale(point, std::abs(::cos(t)) + 0.25);
        point = translate(point, 2*std::cos(t), 2*std::sin(t), 0.0);

        // Put camera at distance 5 from the origin
        point = translate(point,0 , 0, -camera_position.z);

        // Bring origin to the center of the screen
        auto projectedPoint = project(point);
        projectedPoint.x += widhtDividedBy2;
        projectedPoint.y += heightDividedBy2;

        pointsToDraw.emplace_back(projectedPoint);
    }
    t += 0.0125;

}

void render(simplegl::Window & window) {

    window.drawGrid(12);
    
    for (unsigned i = 0; i < pointsToDraw.size(); i+=3) {

        window.drawTriagnle(
            pointsToDraw[i].x,
            pointsToDraw[i].y,
            pointsToDraw[i + 1].x,
            pointsToDraw[i + 1].y,
            pointsToDraw[i + 2].x,
            pointsToDraw[i + 2].y,
            colorGreen);
    }

    for (simplegl::point2_t const & pointToDraw : pointsToDraw) {
        window.drawRectangle(
            pointToDraw.x,
            pointToDraw.y,
            4,
            4,
            colorYellow
        );
    }
    

    window.renderColorBuffer();
    window.clearColorBuffer(0xFF000000);
    window.renderPresent();
}

void waitFrameTime(std::chrono::system_clock::time_point start_time_point, std::chrono::milliseconds target_fps_time) {
    auto processing_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time_point);
        if (processing_time < target_fps_time) {
            auto remaining_time = target_fps_time - processing_time;
            std::this_thread::sleep_for(remaining_time);
        }
}

int main() {

    auto mesh = buildSphereMesh(10);

    pointsToProject = mesh.vertexes();

    auto window_opt = simplegl::Window::Create(windowWidth, windowHeight);
    bool keep_running = window_opt.has_value();

    if (!keep_running) {
        return 1;
    }

    auto& window = *window_opt;
    window.setupDrawingBuffer();

    while(keep_running)
    {
        auto start_time_point = std::chrono::system_clock::now();
        processInput(keep_running);
        update();
        render(window);
        waitFrameTime(start_time_point, targetFpsTime);
    }

    
    return 0;
}