# ProjetGestionVersion
## Projet de structure de données LU2IN006

Tout au long du semestre, nous avons pu implémenter graduellement un programme de gestion de version, à la manière de Git.
Vous trouverez ici une rapide description de ce qu'il est possible de faire.

**Attention ! Il est important de placer tous les fichiers et les repertoires que vous voulez sauvegarder dans ce répertoire ou le programme ne fonctionnera pas !**

0 - Déplacez-vous dans le répertoire contenant les fichiers du projet à l'aide de la commande UNIX 'cd'.

1 - Commencez par compiler le programme avec la commande '**_make_**'.

2 - Initialiser le dépot local avec la commande '**_./myGit init_**'.

3 - Vous devez maintenant ajouter des fichiers à la zone de préparation grace à la commande '**_./myGit add <nom_fichier1,...>_**'.

4 - Vous pouvez vider la zone de préparation avec la commande '**_./myGit clear-add_**'.

5 - Vous pouvez lister tous les fichiers de la zone de préparation avec la commande '**_./myGit add-list_**'.

6 - Vous pouvez commit les fichiers de la zone de préparation avec la commande '**_./myGit commit <nom_de_la_branche> [message_du_commit]_**'.

7 - Vous pouvez lister toutes les branches du projet avec la commande '**_./myGit refs-list_**'.

8 - Vous pouvez supprimer une branche avec la commande '**_./myGit delete-ref <nom_de_la_branche>_**'.

9 - Vous pouvez créer une nouvelle branche au dépot avec la commande '**_./myGit branch <nom_de_la_branche>_**' ou la commande '**_./myGit create-refs <nom_de_la_branche> <hash_de_la_reference>_**'.

10 - Vous pouvez savoir quelle est la branche courante avec la commande '**_./myGit get-current-branch_**'.

11 - Vous pouvez changer de branche avec la commande '**_./myGit checkout-branch <nom_de_la_branche>_**'.

12 - Vous pouvez afficher tout l'historique de commits d'une branche avec la commande '**_./myGit branch-print <nom_de_la_branche>_**'.

13 - Vous pouvez changer de version (commit) avec la commande '**_./myGit checkout-commit <pattern_du_hash_du_commit>_**'.

14 - Vous pouvez fusionner deux branches avec la commande '**_./myGit merge <nom_de_la_branche_distante>_**'.

15 - Vous pouvez obtenir plus de renseignements sur les commandes et leur utilisation avec la commande '**_./myGit help_**'.


**Dernier avertissement ! A cause du choix d'implémentation du projet, il faut veuiller à ne pas ajouter les instantanes et leur repertoires dans la zone de preparation ou bien le depot local sera ruiné lors d'un commit/merge.**
