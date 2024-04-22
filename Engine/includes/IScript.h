#pragma once

#include <string> 

#define DECLARE_SCRIPT(name, subtype) \
		void SetDefaultScriptName() override { scriptName = name; } \
		void SetDefaultScriptType() override { id = subtype; }


        class IScript
        {
        public:
            virtual ~IScript() = default;

            virtual void Initialize() {}

            virtual void Start() {}

            virtual void Update() {}

            virtual void Update(const float _delta) {}

            virtual void Destruct() {}

            virtual void SetDefaultScriptName() = 0;

            virtual void SetDefaultScriptType() = 0;

            void SetScriptName(const std::string& _newName = "Unnamed script") { scriptName = _newName; }

            std::string GetScriptName() const { return scriptName; }

        protected:
            std::string scriptName = "Unnamed Script";
            int id;
        };

