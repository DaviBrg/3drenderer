#include <cassert>
#include <chrono>
#include <cmath>
#include <numbers>
#include <thread>

#include "objloader.h"
#include "window.h"
#include "vec.h"

namespace
{

constexpr double fovFactor = 640.0;
constexpr unsigned targetFps = 60;
constexpr auto targetFpsTime = std::chrono::milliseconds(1000/targetFps);
constexpr int windowWidth = 1920; // max: 3840
constexpr int windowHeight = 1080; // max: 2160
constexpr int widhtDividedBy2 = windowWidth/2;
constexpr int heightDividedBy2 = windowHeight/2;
constexpr uint32_t colorGreen = 0xFF00FF00;
constexpr double rotationStep = 3.1417/96.0;
int rotationXIndex = 0;
int rotationYIndex = 0;
double rotationX = 0;
double rotationY = 0;
std::array<simplegl::vec3_t,3> transformedVertices = {};

std::vector<simplegl::vec2_t> vertexesToRender;
simplegl::vec3_t camera_position = {0.0, 0.0, 0.0};

simplegl::Mesh const & getMeshToRender() {
    static simplegl::Mesh const meshToRender = [](){
        // auto opt = simplegl::ObjLoader::load("../objects/teapot.obj");
        // assert(opt.has_value());
        // return opt.value();
        return simplegl::Mesh::buildCylinder(10);
        // return simplegl::Mesh::buildSphere(8);
    }();
    return meshToRender;
}

}

void processInput(bool& keep_runing, bool& pause) {
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
            else if(event.key.keysym.sym == SDLK_UP) {
                rotationXIndex++;
                rotationX = rotationStep*rotationXIndex;
            }
            else if(event.key.keysym.sym == SDLK_DOWN) {
                rotationXIndex--;
                rotationX = rotationStep*rotationXIndex;
            }
            else if(event.key.keysym.sym == SDLK_RIGHT) {
                rotationYIndex--;
                rotationY = rotationStep*rotationYIndex;
            }
            else if(event.key.keysym.sym == SDLK_LEFT) {
                rotationYIndex++;
                rotationY = rotationStep*rotationYIndex;
            }
            break;
        }
    }
}

simplegl::vec2_t project(simplegl::vec3_t const& point) {
    simplegl::vec2_t projected_point = {(fovFactor * point.x)/point.z,(fovFactor * point.y)/point.z};
    return projected_point;
}

simplegl::vec3_t translate(simplegl::vec3_t const& point, double factor_x, double factor_y, double factor_z) {
    simplegl::vec3_t projected_point = {point.x + factor_x, point.y + factor_y, point.z + factor_z};
    return projected_point;
}

simplegl::vec3_t scale(simplegl::vec3_t const& point, double factor_x, double factor_y, double factor_z) {
    simplegl::vec3_t projected_point = {point.x*factor_x, point.y*factor_y, point.z*factor_z};
    return projected_point;
}

simplegl::vec3_t scale(simplegl::vec3_t const& point, double factor) {
    simplegl::vec3_t projected_point = {point.x*factor, point.y*factor, point.z*factor};
    return projected_point;
}

simplegl::vec3_t rotateX(simplegl::vec3_t const& point, double angle) {
    simplegl::vec3_t projected_point = {point.x, std::sin(-angle)*point.z + std::cos(-angle)*point.y, std::cos(-angle)*point.z - std::sin(-angle)*point.y};
    return projected_point;
}

simplegl::vec3_t rotateY(simplegl::vec3_t const& point, double angle) {
    simplegl::vec3_t projected_point = {std::sin(angle)*point.z + std::cos(angle)*point.x, point.y, std::cos(angle)*point.z - std::sin(angle)*point.x};
    return projected_point;
}

simplegl::vec3_t rotateZ(simplegl::vec3_t const& point, double angle) {
    simplegl::vec3_t projected_point = {std::cos(angle)*point.x - std::sin(angle)*point.y, std::sin(angle)*point.x + std::cos(angle)*point.y, point.z};
    return projected_point;
}

void update() {
    vertexesToRender.clear();

    simplegl::Mesh const & meshToRender = getMeshToRender();

    for (unsigned int faceIdx = 0; faceIdx < meshToRender.faces().size(); ++faceIdx) {
        simplegl::Face const & face = meshToRender.faces()[faceIdx];

        for (unsigned int i = 0; i < 3; ++i) {
            simplegl::vec3_t vertex = meshToRender.vertices()[face.indexes[i].vertex];

            vertex = rotateX(vertex, rotationX);
            vertex = rotateY(vertex, rotationY);

            // Put camera at distance 5 from the origin
            vertex = translate(vertex, 0.0, 0.0, 5.0);

            transformedVertices[i] = vertex;
        }
        
        // Face culling
        simplegl::vec3_t ab = transformedVertices[1] - transformedVertices[0];
        simplegl::vec3_t ac = transformedVertices[2] - transformedVertices[0];
        simplegl::vec3_t normal = simplegl::cross(ab, ac);
        simplegl::vec3_t triangleCamera = (camera_position - transformedVertices[0]);
        auto dotProduct = simplegl::dot(normal, triangleCamera);
        if (dotProduct < 0.0) {
            continue;
        }
        
        for (simplegl::vec3_t const & transformedVertex : transformedVertices) {
            auto vertexToRender = project(transformedVertex);
            vertexToRender.x += widhtDividedBy2;
            vertexToRender.y += heightDividedBy2;
            vertexesToRender.emplace_back(vertexToRender);
        }

    }

}

void render(simplegl::Window & window) {

    window.drawGrid(12);
    
    for (unsigned i = 0; i < vertexesToRender.size(); i+=3) {
        window.drawTriagnle(
            vertexesToRender[i].x,
            vertexesToRender[i].y,
            vertexesToRender[i + 1].x,
            vertexesToRender[i + 1].y,
            vertexesToRender[i + 2].x,
            vertexesToRender[i + 2].y,
            colorGreen);
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
    auto window_opt = simplegl::Window::Create(windowWidth, windowHeight);
    bool keep_running = window_opt.has_value();
    bool pause = false;

    if (!keep_running) {
        return 1;
    }

    auto& window = *window_opt;
    window.setupDrawingBuffer();

    while(keep_running)
    {
        auto start_time_point = std::chrono::system_clock::now();
        processInput(keep_running, pause);
        if (!pause) {
            update();
            render(window);
        }
        waitFrameTime(start_time_point, targetFpsTime);
    }
    
    return 0;
}