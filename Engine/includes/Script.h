#pragma once

#include "IScript.h"
#include "CoreEngine.h"
#include "Modules/TimeModule.h"
#include "Modules/WindowModule.h"
#include "rhi.h"
#include "Modules/InputModule.h"



    class Script : public IScript
    {
    public:
        Script();
        ~Script() = default;

        TimeModule* GetTimeModule() const;
        WindowModule* GetWindowModule() const;
        RHIModule* GetRenderInterfaceModule() const;
        InputModule* GetInputModule() const;

    private:
        TimeModule* timeModuleReference = nullptr;
        WindowModule* windowModuleReference = nullptr;
        RHIModule* renderInterfaceModuleReference = nullptr;
        InputModule* inputModuleReference = nullptr;
    };
