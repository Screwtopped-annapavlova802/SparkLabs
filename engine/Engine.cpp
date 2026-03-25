#include "Engine.h"
#include <iostream>

#ifdef SPARKLABS_PLATFORM_WINDOWS
#include <Windows.h>
#else
#include <unistd.h>
#include <time.h>
#endif

namespace SparkLabs {

Engine* Engine::s_Instance = nullptr;

Engine* Engine::GetInstance() {
    if (!s_Instance) {
        s_Instance = new Engine();
    }
    return s_Instance;
}

Engine::Engine()
    : m_Running(false)
    , m_Initialized(false)
    , m_DeltaTime(0.0f)
    , m_TotalTime(0.0)
{
}

Engine::~Engine() {
    Shutdown();
}

void Engine::Initialize() {
    if (m_Initialized) return;

    std::cout << "Initializing SparkLabs Engine..." << std::endl;

    m_Initialized = true;
    std::cout << "Engine initialized successfully!" << std::endl;
}

void Engine::Shutdown() {
    if (!m_Initialized) return;

    std::cout << "Shutting down SparkLabs Engine..." << std::endl;

    m_Running = false;
    m_Scene.Reset();
    m_Initialized = false;

    std::cout << "Engine shutdown complete." << std::endl;
}

void Engine::Run() {
    if (!m_Initialized) {
        Initialize();
    }

    if (m_Running) {
        std::cout << "Engine is already running!" << std::endl;
        return;
    }

    m_Running = true;
    std::cout << "Starting engine main loop..." << std::endl;
    MainLoop();
}

void Engine::Stop() {
    m_Running = false;
}

void Engine::SetScene(Scene* scene) {
    m_Scene = SmartPtr<Scene>(scene);
}

Scene* Engine::GetScene() const {
    return m_Scene.Get();
}

void Engine::MainLoop() {
#ifdef SPARKLABS_PLATFORM_WINDOWS
    LARGE_INTEGER frequency, lastTime, currentTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastTime);
#else
    struct timespec lastTime, currentTime;
    clock_gettime(CLOCK_MONOTONIC, &lastTime);
#endif

    while (m_Running) {
#ifdef SPARKLABS_PLATFORM_WINDOWS
        QueryPerformanceCounter(&currentTime);
        m_DeltaTime = static_cast<float32>(
            (currentTime.QuadPart - lastTime.QuadPart) /
            static_cast<double>(frequency.QuadPart)
        );
        lastTime = currentTime;
#else
        clock_gettime(CLOCK_MONOTONIC, &currentTime);
        m_DeltaTime = static_cast<float32>(
            (currentTime.tv_sec - lastTime.tv_sec) +
            (currentTime.tv_nsec - lastTime.tv_nsec) / 1e9
        );
        lastTime = currentTime;
#endif

        m_TotalTime += m_DeltaTime;

        Update(m_DeltaTime);
        Render();

#ifdef SPARKLABS_PLATFORM_WINDOWS
        Sleep(1);
#else
        usleep(1000);
#endif
    }
}

void Engine::Update(float32 deltaTime) {
    if (m_Scene) {
        m_Scene->Update(deltaTime);
    }
}

void Engine::Render() {
}

}
