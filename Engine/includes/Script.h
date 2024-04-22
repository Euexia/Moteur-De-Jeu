#pragma once

#include "IScript.h"
#include "Modules/TimeModule.h"
#include "Modules/WindowModule.h"
//#include "Modules/RenderInterface.h"
#include "Modules/InputModule.h"

namespace Scripts
{
    class Script : public IScript
    {
    public:
        Script();
        ~Script() = default;

        TimeModule* GetTimeModule() const;
        //Modules::WindowModule* GetWindowModule() const; 
        //Render::RenderInterface* GetRenderInterfaceModule() const;
        InputModule* GetInputModule() const;

    private:
        TimeModule* timeModuleReference = nullptr;
        //Modules::WindowModule* windowModuleReference = nullptr; 
        //Render::RenderInterface* renderInterfaceModuleReference = nullptr;
        InputModule* inputModuleReference = nullptr;
    };
}
