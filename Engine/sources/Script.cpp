#include "Script.h"

Scripts::Script::Script()
{
	timeModuleReference = Engine::GetInstance()->GetModuleManager()->GetModule<TimeModule>();
	// windowModuleReference = Engine::GetInstance()->GetModule<Module::WindowModule>();
	// renderInterfaceModuleReference = Engine::GetInstance()->GetModule<Module::RenderInterface>();
	inputModuleReference = GetInstance()->GetModule<InputModule>();
}

Module::Time::Clock* Scripts::Script::GetClock() const
{
	return clockModuleReference;
}

Module::Display::Window* Scripts::Script::GetWindow() const
{
	return windowModuleReference;
}

Module::Render::RenderInterface* Scripts::Script::GetRenderInterface() const
{
	return renderInterfaceModuleReference;
}

Module::Tools::Logs::Logger* Scripts::Script::GetLogger() const
{
	return loggerModuleReference;
}

Module::Inputs::Input* Scripts::Script::GetInputs() const
{
	return inputsModuleReference;
}
