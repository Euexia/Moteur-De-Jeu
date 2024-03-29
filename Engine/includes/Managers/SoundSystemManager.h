#pragma once

#include <fmod.hpp>
#include <cstddef>
#include <iostream>

/**
 * @brief Classe SoundClass : G�re le syst�me Sound utilisant la biblioth�que FMOD
 */
typedef FMOD::Sound* SoundClass;
/**
 * @brief Classe SoundClass : G�re le syst�me SoundGroup utilisant la biblioth�que FMOD
 */
typedef FMOD::SoundGroup* SoundGroup;

typedef FMOD::Channel* Channel;

typedef FMOD::ChannelGroup* ChannelGroup;

/**
 * @brief Classe SoundSystemManager : G�re le syst�me audio utilisant la biblioth�que FMOD
 */
class SoundSystemManager
{
public:
    /**
     * @brief Constructeur par d�faut de la classe SoundSystemManager
     */
    SoundSystemManager();

    /**
     * @brief Cr�e un objet Sound � partir d'un fichier audio sp�cifi�
     * @param pSound Pointeur vers l'objet Sound cr��
     * @param pathAudio Chemin du fichier audio � charger
     */
    void createSound(SoundClass* _pSound, const char* _pathAudio);

    void createSoundGroup(SoundGroup* _pSoundGroup, const char* _pathAudio);

    void getMasterSoundGroup(SoundGroup* pSound);

    /**
     * @brief Joue un son sp�cifi�
     * @param pSound Pointeur vers l'objet Sound � jouer
     * @param isPlay Bool�en indiquant si le son doit �tre jou� imm�diatement
     * @param loopCount Int indiquant combien de fois l'audio doit ce jouer
     */
    void playSound(SoundClass _pSound, bool _isPlay, int loopCount, float volume, Channel* channelPtr, ChannelGroup* channelGroupPtr);

    /**
     * @brief Lib�re les ressources utilis�es par un son sp�cifi�
     * @param pSound Pointeur vers l'objet Sound � lib�rer
     */
    void releaseSound(SoundClass _pSound);

    /**
     * @brief Cr�e un groupe de canaux pour le syst�me audio
     * @param channelGroup Pointeur vers le groupe de canaux cr��
     */
    void createChannelGroup(FMOD::ChannelGroup** _channelGroup);

    /**
     * @brief Ajoute un son � un groupe de canaux sp�cifi�
     * @param pSound Pointeur vers l'objet Sound � ajouter au groupe de canaux
     * @param channelGroup Pointeur vers le groupe de canaux auquel ajouter le son
     */
    void addSoundToGroup(SoundClass _pSound, FMOD::ChannelGroup* _channelGroup);

    /**
    * @brief Set une distance minimum et maximum pour un son 3D
    * @param _min float pour la variable minimum
    * @param _max float pour la variable maximum
    */
    float set3DMinMaxDistance(float _min, float _max) { return min, max; }

    /**
    * @brief Get une distance minimum et maximum pour un son 3D
    * @return R�sultat de la variable minimum et maximum
    */
    float get3DMinMaxDistance() { return min, max; }

    int getLoopCount(SoundClass _pSound, int* loopcount);

    /**
     * @brief Attache un groupe de canaux � un port sp�cifi� pour le rendu audio.
     * @param portType Type de port auquel attacher le groupe de canaux (par ex. FMOD_PORT_TYPE_PS3_SPDIF).
     * @param portIndex Index du port auquel attacher le groupe de canaux.
     * @param channelGroup Pointeur vers le groupe de canaux � attacher.
     * @param passThru Indique si le signal doit �tre achemin� directement sans traitement (par d�faut, false).
     * @return R�sultat de l'op�ration d'attache du groupe de canaux au port.
     */
    FMOD_RESULT attachChannelGroupToPort(FMOD_PORT_TYPE _portType, FMOD_PORT_INDEX _portIndex, FMOD::ChannelGroup* _channelGroup, bool _passThru = false);

    /**
     * @brief D�tache un groupe de canaux d'un port auquel il �tait pr�c�demment attach�.
     * @param channelGroup Pointeur vers le groupe de canaux � d�tacher du port.
     * @return R�sultat de l'op�ration de d�tachement du groupe de canaux du port.
     */
    FMOD_RESULT detachChannelGroupFromPort(FMOD::ChannelGroup* _channelGroup);

    /**
     * @brief D�finit les propri�t�s de r�verb�ration pour une instance de r�verb�ration sp�cifi�e.
     * @param instance Instance de r�verb�ration pour laquelle d�finir les propri�t�s.
     * @param prop Pointeur vers les propri�t�s de r�verb�ration � d�finir.
     * @return R�sultat de l'op�ration de d�finition des propri�t�s de r�verb�ration.
     */
    FMOD_RESULT setReverbProperties(int _instance, const FMOD_REVERB_PROPERTIES* _prop);

    /**
     * @brief Obtient les propri�t�s de r�verb�ration pour une instance de r�verb�ration sp�cifi�e.
     * @param instance Instance de r�verb�ration pour laquelle obtenir les propri�t�s.
     * @param prop Pointeur vers les propri�t�s de r�verb�ration � remplir avec les valeurs obtenues.
     * @return R�sultat de l'op�ration de r�cup�ration des propri�t�s de r�verb�ration.
     */
    FMOD_RESULT getReverbProperties(int _instance, FMOD_REVERB_PROPERTIES* _prop);

private:
    FMOD::System* system; // Pointeur vers l'objet syst�me FMOD
    FMOD::ChannelGroup* channelGroup; // Pointeur vers le groupe de canaux utilis�

    // float variable
    float min;
    float max;
};
