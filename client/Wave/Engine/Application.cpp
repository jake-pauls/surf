#include "Application.h"

wv::Application::Application()
	: m_Window(new Window)
{
}

wv::Application::~Application()
{
	delete m_Window;
}

void wv::Application::Run()
{
	core::Log(ELogType::Info, "Starting the wave");

	// Start systems and run the renderer here...
	m_Window->Init();

	Teardown();
}

void wv::Application::Teardown()
{
	core::Log(ELogType::Info, "Tearing down the wave");
}
