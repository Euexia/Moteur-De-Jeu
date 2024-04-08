#pragma once

#include <cstddef>
#include <fmod.hpp>
#include <iostream>

#include "Module.h"

/**
 * @brief Classe SoundClass : G�re le syst�me Sound utilisant la biblioth�que FMOD
 */
using SoundClass = FMOD::Sound*;

/**
 * @brief Classe SoundGroup : G�re le syst�me SoundGroup utilisant la biblioth�que FMOD
 */
using SoundGroup = FMOD::SoundGroup*;

/**
 * @brief Classe Channel : G�re les canaux audio utilisant la biblioth�que FMOD
 */
using Channel = FMOD::Channel*;

/**
 * @brief Classe SoundSystemModule : G�re le syst�me audio utilisant la biblioth�que FMOD
 */
class SoundSystemModule final : public Module
{
	public:
		/**
		 * @brief Constructeur par d�faut de la classe SoundSystemModule
		 */
		SoundSystemModule();

		/**
		 * @brief Cr�e un objet Sound � partir d'un fichier audio sp�cifi�
		 * @param p_Sound Pointeur vers l'objet Sound cr��
		 * @param _pathAudio Chemin du fichier audio � charger
		 */
		void CreateSound(SoundClass* p_Sound, const char* p_pathAudio) const;

		/**
		 * @brief Cr�e un objet SoundGroup � partir d'un fichier audio sp�cifi�
		 * @param _pSoundGroup Pointeur vers l'objet SoundGroup cr��
		 * @param _pathAudio Chemin du fichier audio � charger
		 */
		void CreateSoundGroup(SoundGroup* p_pSoundGroup, const char* p_pathAudio) const;

		/**
		 * @brief Obtient le groupe de sons principal
		 * @param _pSound Pointeur vers le groupe de sons principal
		 */
		void GetMasterSoundGroup(SoundGroup* p_pSound) const;

		/**
		 * @brief Joue un son sp�cifi�
		 * @param _pSound Pointeur vers l'objet Sound � jouer
		 * @param _isPlay Bool�en indiquant si le son doit �tre jou� imm�diatement
		 * @param _loopCount Nombre de fois que le son doit �tre jou� en boucle
		 * @param _volume Volume du son � jouer
		 * @param p_channelPtr Pointeur vers le canal sur lequel le son est jou�
		 */
		void PlaySound(SoundClass _pSound, bool _isPlay, int _loopCount, float _volume, Channel* p_channelPtr) const;

		/**
		 * @brief Lib�re les ressources utilis�es par un son sp�cifi�
		 * @param _pSound Pointeur vers l'objet Sound � lib�rer
		 */
		void ReleaseSound(SoundClass _pSound);

		/**
		 * @brief Cr�e un groupe de canaux pour le syst�me audio
		 * @param pp_channelGroup Pointeur vers le groupe de canaux cr��
		 */
		// ReSharper disable once CppInconsistentNaming
		void CreateChannelGroup(FMOD::ChannelGroup** pp_channelGroup) const;

		/**
		 * @brief Ajoute un son � un groupe de canaux sp�cifi�
		 * @param _pSound Pointeur vers l'objet Sound � ajouter au groupe de canaux
		 * @param _channelGroup Pointeur vers le groupe de canaux auquel ajouter le son
		 */
		void AddSoundToGroup(SoundClass _pSound, FMOD::ChannelGroup* p_channelGroup) const;

		/**
		 * @brief D�finit la distance minimum et maximum pour un son 3D
		 * @param _min Distance minimum du son
		 * @param _max Distance maximum du son
		 * @return La distance minimum et maximum d�finie
		 */
		float Set3DMinMaxDistance(const float _min, const float _max)
		{
			min = _min;
			max = _max;
		}

		/**
		 * @brief Obtient la distance minimum et maximum pour un son 3D
		 * @return La distance minimum et maximum du son
		 */
		float Get3DMinMaxDistance() { return min, max; }

		/**
		 * @brief Obtient le nombre de boucles d'un son sp�cifi�
		 * @param _pSound Pointeur vers l'objet Sound
		 * @param p_loopCount Pointeur vers la variable pour stocker le nombre de boucles
		 * @return Le nombre de boucles du son sp�cifi�
		 */
		static int GetLoopCount(SoundClass _pSound, int* p_loopCount);

		/**
		 * @brief Attache un groupe de canaux � un port sp�cifi� pour le rendu audio
		 * @param _portType Type de port auquel attacher le groupe de canaux
		 * @param _portIndex Index du port auquel attacher le groupe de canaux
		 * @param p_channelGroup Pointeur vers le groupe de canaux � attacher
		 * @param _passThru Indique si le signal doit �tre achemin� directement sans traitement (par d�faut, false)
		 * @return R�sultat de l'op�ration d'attache du groupe de canaux au port
		 */
		FMOD_RESULT AttachChannelGroupToPort(FMOD_PORT_TYPE      _portType, FMOD_PORT_INDEX _portIndex,
		                                     FMOD::ChannelGroup* p_channelGroup, bool       _passThru = false) const;

		/**
		 * @brief D�tache un groupe de canaux d'un port auquel il �tait pr�c�demment attach�
		 * @param p_channelGroup Pointeur vers le groupe de canaux � d�tacher du port
		 * @return R�sultat de l'op�ration de d�tachement du groupe de canaux du port
		 */
		FMOD_RESULT DetachChannelGroupFromPort(FMOD::ChannelGroup* p_channelGroup) const;

		/**
		 * @brief D�finit les propri�t�s de r�verb�ration pour une instance de r�verb�ration sp�cifi�e
		 * @param _instance Instance de r�verb�ration pour laquelle d�finir les propri�t�s
		 * @param p_prop Pointeur vers les propri�t�s de r�verb�ration � d�finir
		 * @return R�sultat de l'op�ration de d�finition des propri�t�s de r�verb�ration
		 */
		FMOD_RESULT SetReverbProperties(int _instance, const FMOD_REVERB_PROPERTIES* p_prop) const;

		/**
		 * @brief Obtient les propri�t�s de r�verb�ration pour une instance de r�verb�ration sp�cifi�e
		 * @param _instance Instance de r�verb�ration pour laquelle obtenir les propri�t�s
		 * @param p_prop Pointeur vers les propri�t�s de r�verb�ration � remplir avec les valeurs obtenues
		 * @return R�sultat de l'op�ration de r�cup�ration des propri�t�s de r�verb�ration
		 */
		FMOD_RESULT GetReverbProperties(int _instance, FMOD_REVERB_PROPERTIES* p_prop) const;


		/**
		    * @brief Initialise le module.
		    */
		void Init() override;

		/**
		 * @brief D�marre le module.
		 */
		void Start() override;

		/**
		 * @brief Effectue une mise � jour fixe du module.
		 */
		void FixedUpdate() override;

		/**
		 * @brief Met � jour le module.
		 */
		void Update() override;

		/**
		 * @brief Fonction pr�-rendu du module.
		 */
		void PreRender() override;

		/**
		 * @brief Rendu du module.
		 */
		void Render() override;

		/**
		 * @brief Rendu de l'interface graphique du module.
		 */
		void RenderGui() override;

		/**
		 * @brief Fonction post-rendu du module.
		 */
		void PostRender() override;

		/**
		 * @brief Lib�re les ressources utilis�es par le module.
		 */
		void Release() override;

		/**
		 * @brief Finalise le module.
		 */
		void Finalize() override;

	private:
		FMOD::System*       system;       // Pointeur vers l'objet syst�me FMOD
		FMOD::ChannelGroup* channelGroup; // Pointeur vers le groupe de canaux utilis�

		// Variables pour la distance minimum et maximum d'un son 3D
		float min;
		float max;
};
