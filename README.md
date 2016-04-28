# ProjetTER
Données générales 
Exposé du problème :

Titre du mémoire : Interface interactive de reconnaissance de personne par la voix et l'image avec Webcam 

Ce projet s’inscrit dans notre parcours d’études master informatique saint Jérôme, dans le département Informatique.  
L’idée est de fournir une alternative à l’utilisation d’une interface de reconnaissance de personnes par la voix ou l'image et la reconnaissance du locuteur en identifiant de manière fiable un locuteur dont la voix a été préalablement enregistrée une Webcam et de réaliser une application prototype modulaire et facilement réutilisable sous QT, mais aussi d’évaluer le potentiel de cette plateforme pour du traitement d’image complexe et de son. 
 
 
Données techniques : 
Description du produit

Quelques points illustrant le fonctionnement de l'interface interactive : 

Une fenêtre comme interface d’accueil comportant deux boutons : 
Widget webcam : fenêtre pour la webcam.    
	Bouton s’inscrire : qui permet l'ajout de l'utilisateur dans la base de données. 
	Bouton connexion : qui permet d’identifier l’utilisateur afficher sur le widget. 
	Webcam et     puis la redirection vers la fenêtre profile de l'utilisateur si le résultat est vrai. 

Description d’environnement de développement :

Afin de réaliser une application performante réutilisable et percutante et faciliter la tâche de l’implémentation on utilise les outils suivants :

	OpenCV : bibliothèque open source de computer vision. 
OpenCV apporte à un ordinateur diverses fonctionnalités permettant de traiter des données vidéo provenant d’une webcam, d’images fixes ou toute autre source, afin de les analyser et de les traiter.  On pourra par exemple faire toutes sortes de traitements classiques d’image (saturation, rotation etc.), mais également accéder à des fonctionnalités plus avancées telles que la détection de mouvement, ou encore le repérage d’un visage au sein d’un scène. 

Qt et C++ :  Framework orienté objet et développé en C++ par Nokia. 

Il offre des composants d'interface graphique (widgets), d'accès aux données, l'analyse XM… etc. Qt est par certains aspects un Framework simple et fonctionnel pour concevoir des interfaces graphiques, et qui permet la portabilité des applications. (Compatible avec les librairies open cv).

	BD : MySQL.
Une base de données MySQL avec tous les visages de l’utilisateur (plusieurs photos par personne (10 avec la même position)).

	System d’exploitation Linux : Ubuntu 14.04
	CMake -gui : version  2.13
	Visual basic 2013

