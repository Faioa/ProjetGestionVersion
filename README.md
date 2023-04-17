# ProjetGestionVersion
Projet de structure de données LU2IN006

Tout au long du semestre, nous avons pu implémenter graduellement un programme de gestion de version, à la manière de Git.
Vous trouverez ici une rapide description de ce qu'il est possible de faire.

0 - Déplacez-vous dans le répertoire contenant les fichiers du projet.
# Attention ! Il est important de placer tous les fichiers et les repertoires que vous voulez sauvegarder dans ce répertoire ou le programme ne fonctionnera pas !

1 - Commencez par compiler le programme avec la commande 'make'.

2 - Initialiser le dépot local avec la commande './myGit init'

3 - Vous devez maintenant ajouter des fichiers à la zone de préparation grace à la commande './myGit add <nom_fichier1,...>'

4 - Vous pouvez créer une nouvelle branche au dépot avec la commande './myGit branch <nom_de_la_branche>' ou la commande './myGit create-refs <nom_de_la_branche> <hash_de_la_reference>'

5 - Vous pouvez commit les fichiers de la zone de préparation avec la commande './myGit commit <nom_de_la_branche> [message_du_commit]'

6 - Vous pouvez fusionner deux branches avec la commande './myGit merge <nom_de_la_branche_distante>'

7 - Vous pouvez lister tous les fichiers de la zone de préparation avec la commande './myGit add-list'

8 - Vous pouvez lister toutes les branches du projet avec la commande './myGit refs-list'

9 - Vous pouvez supprimer une branche avec la commande './myGit delete-ref <nom_de_la_branche>'

10 - Vous pouvez vider la zone de préparation avec la commande './myGit clear-add'

11 - Vous pouvez savoir quelle est la branche courante avec la commande './myGit get-current-branch'

12 - Vous pouvez afficher tout l'historique de commits d'une branche avec la commande './myGit branch-print'

13 - Vous pouvez changer de branche avec la commande './myGit checkout-branch <nom_de_la_branche>'

14 - Vous pouvez changer de version (commit) avec la commande './myGit checkout-commit <pattern_du_hash_du_commit>'

15 - Vous pouvez obtenir plus de renseignement sur les commandes et leur utilisation avec la commande './myGit help'