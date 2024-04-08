# Moteur 3D avec Vulkan

Ce projet est un moteur 3D développé en C++ avec l'utilisation de Vulkan pour les graphismes. Il intègre également plusieurs autres librairies pour diverses fonctionnalités.

## Technologies utilisées

- **C++**: Le langage de programmation principal pour le développement du moteur.
- **Vulkan**: Une API graphique moderne et performante utilisée pour le rendu 3D.
- **Bullet Physics**: Une librairie open-source pour la simulation de la physique en temps réel.
- **Dear ImGui**: Une bibliothèque GUI (Graphical User Interface) utilisée pour créer des interfaces utilisateur dans les applications 3D.
- **GLFW**: Une bibliothèque multiplateforme pour la création de fenêtres, la gestion des contextes OpenGL et Vulkan, et la gestion des entrées.
- **FMOD**: Une librairie audio multiplateforme pour la gestion du son.
- **Winsock**: La librairie Windows pour la programmation des sockets.
- **tinyObjLoader**: Une bibliothèque pour le chargement de fichiers obj.
- **JSON for Modern C++**: Une bibliothèque pour la manipulation de fichiers JSON en C++.
- **GLM**: Une bibliothèque mathématique pour les graphiques en C++.

## Fonctionnalités

- Rendu graphique 3D en temps réel grâce à Vulkan.
- Simulation de la physique des objets grâce à Bullet Physics.
- Interface utilisateur intuitive avec Dear ImGui pour la configuration et le contrôle du moteur.
- Gestion de fenêtres et des entrées avec GLFW.
- Gestion audio avec FMOD.
- Communication réseau avec Winsock.
- Chargement de modèles 3D à partir de fichiers obj avec tinyObjLoader.
- Manipulation de fichiers de configuration au format JSON avec JSON for Modern C++.
- Utilisation des fonctionnalités de la bibliothèque standard du C++.
- Calculs mathématiques pour les graphiques avec GLM.

## Comment utiliser

1. **Installation des dépendances**:

   - Assurez-vous d'avoir Vulkan SDK installé sur votre système.
   - Téléchargez et compilez les librairies Bullet Physics, Dear ImGui, GLFW, FMOD, tinyObjLoader, JSON for Modern C++, std_lib et GLM, assurez-vous qu'elles sont accessibles dans votre projet.

2. **Compilation du projet**:

   - Clonez ce dépôt sur votre machine.
   - Utilisez un compilateur C++ compatible avec les fonctionnalités C++17 et configurez le projet pour inclure les librairies nécessaires.

3. **Exécution du moteur**:
   - Lancez l'application compilée.
   - Utilisez l'interface utilisateur pour charger des modèles 3D, définir les paramètres de simulation physique et interagir avec le moteur.
