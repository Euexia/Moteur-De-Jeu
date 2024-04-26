#pragma once
#include "Modules/SoundSystemModule.h"
#include <imgui.h>

/**
 * @brief Classe pour g�rer l'interface audio dans ImGui.
 */
class ImGUIAudio {
public:

    /**
    * @brief Constructeur de la classe ImGUIAudio.
    * @param _soundModule Module du syst�me audio.
    */
    ImGUIAudio(SoundSystemModule* _soundModule);

    /**
    * @brief Fonction pour dessiner les contr�les audio dans l'interface utilisateur.
    */
    void DrawAudioControls();

private:
    SoundSystemModule* _soundModule; ///< Module du syst�me audio.
};
