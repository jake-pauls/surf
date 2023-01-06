#include "Application.h"

int main(int argc, char* argv[])
{
    core::InitializeLogging();

    wv::Application* app = new wv::Application();
    app->Run();
    delete app;

    core::KillLogging();

    return 0;
}