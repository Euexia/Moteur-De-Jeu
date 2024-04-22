#include "Modules/ScriptModule.h"
#include "Modules/TimeModule.h"
#include "CoreEngine.h"

bool Core::Manager::ScriptManager::StartScripts()
{
    while (!scriptsToStart.empty())
    {
        scriptsToStart.front()->Start();
        scriptsToStart.pop();
    }

    return true;
}

IScript* Core::Manager::ScriptManager::GetIScriptByName(const std::string& _name) const
{
    for (IScript* script : scripts)
    {
        if (script->GetScriptName() == _name)
            return script;
    }

    return nullptr;
}


void Core::Manager::ScriptManager::UpdateScripts()
{
    StartScripts();
    

    TimeModule* timeModule = Engine::GetInstance()->GetModuleManager()->GetModule<TimeModule>();
    float deltaTime = timeModule->GetDeltaTime();

    for (IScript* script : scripts)
    {
        script->Update();
        script->Update(deltaTime);
    }
}

void Core::Manager::ScriptManager::Stop()
{
    std::queue<IScript*>().swap(scriptsToStart);

    for (IScript* script : scripts)
    {
        script->Destruct();
        delete script;
    }
}
