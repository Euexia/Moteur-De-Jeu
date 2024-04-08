#include "first_app.h"
#include "Managers/SoundSystemManager.h"
#include "EngineTestCode.h"
#include "CoreEngine.h"
#include "TCP/Client/TCPClientStart.h"
#include "TCP/Server/TCPServerStart.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    EngineTestCode engineTestCode;
    engineTestCode.TestCode();

    printf("You have entered %d arguments:\n", argc);

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "clientTCP") == 0)
        {
            TCPClientStart client;
            client.TCPClient();
        }
        if (strcmp(argv[i], "serverTCP") == 0)
        {
            TCPServerStart server;
            server.TCPServer();
        }
    }

    //SoundSystemManager sound;
    //SoundClass soundSample;
    //SoundGroup soundGroup;
    //Channel channel;

    //sound.createSound(&soundSample, "");
    //sound.playSound(soundSample, true, 5, 0.9f, &channel);

    /**
     * @brief Fonction principale du programme.
     *
     * Cette fonction cr�e une instance de l'application FirstApp et lance son ex�cution en appelant la m�thode Run().
     *
     * @return EXIT_SUCCESS si l'application s'est termin�e avec succ�s, EXIT_FAILURE sinon.
     */
     lve::FirstApp app{};

     const Engine* engine = Engine::GetInstance();

     engine->Init();

     ////SceneModule* scene_module = engine->GetModuleManager()->GetModule<SceneModule>();
     ////scene_module->SetScene<MenuScene>();

     try {
         engine->Run();
         //app.Run();
     }
     catch (const std::exception& e) {
         std::cerr << e.what() << std::endl;
         return EXIT_FAILURE;
     }

     ////sound.releaseSound(soundSample);


     return EXIT_SUCCESS;
}