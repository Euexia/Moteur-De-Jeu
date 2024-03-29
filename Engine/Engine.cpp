#include "first_app.h"
#include "Managers/SoundSystemManager.h"
#include "EngineTestCode.h"

#include <iostream>
#include <cstdlib>

int main() {
    EngineTestCode engineTestCode;
    engineTestCode.TestCode();

    SoundSystemManager sound;
    SoundClass soundSample;
    SoundGroup soundGroup;
    Channel channel;
    ChannelGroup channelGroup;

    sound.createSound(&soundSample, "");
    sound.playSound(soundSample, true, 5, 0.9f, &channel, &channelGroup);

    /**
     * @brief Fonction principale du programme.
     *
     * Cette fonction cr�e une instance de l'application FirstApp et lance son ex�cution en appelant la m�thode Run().
     *
     * @return EXIT_SUCCESS si l'application s'est termin�e avec succ�s, EXIT_FAILURE sinon.
     */
    lve::FirstApp app{};

    try {
        app.Run();

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    sound.releaseSound(soundSample);

    return EXIT_SUCCESS;
}