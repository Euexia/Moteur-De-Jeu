#pragma once

#include <string> 

/**
 * @brief Macro pour d�clarer le nom et le type par d�faut d'un script.
 *
 * Cette macro permet de d�finir le nom et le type par d�faut d'un script.
 * Elle doit �tre utilis�e dans la d�finition d'une classe de script d�riv�e d'IScript.
 * @param name Le nom par d�faut du script.
 * @param subtype Le type par d�faut du script.
 */
#define DECLARE_SCRIPT(name, subtype) \
		void SetDefaultScriptName() override { scriptName = name; } \
		void SetDefaultScriptType() override { id = subtype; }

 /**
  * @brief Interface de base pour les scripts.
  *
  * Cette classe d�finit une interface de base pour tous les scripts utilis�s dans le moteur de jeu.
  * Les scripts peuvent �tre initialis�s, d�marr�s, mis � jour et d�truits.
  * Ils peuvent �galement avoir un nom et un type par d�faut.
  */
class IScript
{
public:
    /**
     * @brief Destructeur virtuel par d�faut.
     */
    virtual ~IScript() = default;

    /**
     * @brief Initialise le script.
     */
    virtual void Initialize() {}

    /**
     * @brief D�marre le script.
     */
    virtual void Start() {}

    /**
     * @brief Met � jour le script.
     */
    virtual void Update() {}

    /**
     * @brief Met � jour le script avec un delta de temps sp�cifi�.
     * @param _delta Le delta de temps depuis la derni�re mise � jour.
     */
    virtual void Update(const float _delta) {}

    /**
     * @brief Destructeur le script.
     */
    virtual void Destruct() {}

    /**
     * @brief D�finit le nom par d�faut du script.
     * @param _newName Le nouveau nom du script.
     */
    void SetScriptName(const std::string& _newName = "Unnamed script") { scriptName = _newName; }

    /**
     * @brief Obtient le nom du script.
     * @return Le nom du script.
     */
    std::string GetScriptName() const { return scriptName; }

protected:
    std::string scriptName = "Unnamed Script";
    int id;
};

