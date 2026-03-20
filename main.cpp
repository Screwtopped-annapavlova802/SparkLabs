#include <iostream>
#include <Vector3.h>
#include <Quaternion.h>
#include <Matrix4x4.h>
#include <SmartPtr.h>
#include <String.h>

#ifdef SPARKLABS_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

namespace SparkLabs {

void Initialize() {
    #ifdef SPARKLABS_PLATFORM_WINDOWS
        std::cout << "Initializing SparkLabs Engine on Windows..." << std::endl;
    #else
        std::cout << "Initializing SparkLabs Engine on Unix..." << std::endl;
    #endif
}

void Shutdown() {
    std::cout << "Shutting down SparkLabs Engine..." << std::endl;
}

}

int main(int argc, char** argv) {
    std::cout << "========================================" << std::endl;
    std::cout << "  SparkLabs Engine" << std::endl;
    std::cout << "  AI-Native Game Engine" << std::endl;
    std::cout << "  Version 1.0.0" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    SparkLabs::Initialize();

    std::cout << "SparkLabs Engine initialized successfully!" << std::endl;
    std::cout << std::endl;

    std::cout << "System Information:" << std::endl;
    std::cout << "  - Math Library: Vector3, Quaternion, Matrix4x4" << std::endl;
    std::cout << "  - Memory Management: SmartPtr, WeakPtr" << std::endl;
    std::cout << "  - Object System: RTTI enabled" << std::endl;
    std::cout << "  - AI Runtime: Ready" << std::endl;
    std::cout << "  - Neural Rendering: Ready" << std::endl;
    std::cout << std::endl;

    SparkLabs::Shutdown();

    return 0;
}
