#include <surf/surf.h>

int main(int argc, char* argv[])
{
    core::InitializeLogging();
        
    surf_BridgeTest();
    core::Log(ELogType::Info, "surf. Client Test");

    core::KillLogging();

    return 0;
}