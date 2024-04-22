#pragma once

#include <vector>
#include <queue>
#include <string> 
#include "IScript.h"

namespace Core {
    namespace Manager {

        class ScriptManager {
        public:
            ScriptManager() = default;
            ~ScriptManager() = default;

            template <typename T, class U>
            bool AddScript()
            {
                Interface::IScript* script = static_cast<Interface::IScript*>(new U);

                script->Initialize();

                scriptsToStart.push(script);
                scripts.push_back(script);

                return true;
            }

            bool StartScripts();

            IScript* GetIScriptByName(const std::string& _name) const;

            void UpdateScripts();
            void Stop();

        private:
            std::vector<IScript*> scripts;
            std::queue<IScript*> scriptsToStart;
        };

    } 
} 