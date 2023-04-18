## ProjetGestionVersion
### Projet de structure de données LU2IN006

Tout au long du semestre, nous avons pu implémenter graduellement un programme de gestion de version, à la manière de Git.
Vous trouverez ici une rapide description de ce qu'il est possible de faire.

**Attention ! Il est important de placer tous les fichiers et les repertoires que vous voulez sauvegarder dans ce répertoire ou le programme ne fonctionnera pas !**

* Déplacez-vous dans le répertoire contenant les fichiers du projet à l'aide de la commande UNIX 'cd'.

* Commencez par compiler le programme avec la commande '**_make_**'.

* Initialiser le dépot local avec la commande '**_./myGit init_**'.

* Vous devez maintenant ajouter des fichiers à la zone de préparation grace à la commande '**_./myGit add <nom_fichier1,...>_**'.

* Vous pouvez vider la zone de préparation avec la commande '**_./myGit clear-add_**'.

* Vous pouvez lister tous les fichiers de la zone de préparation avec la commande '**_./myGit add-list_**'.

* Vous pouvez commit les fichiers de la zone de préparation avec la commande '**_./myGit commit <nom_de_la_branche> [message_du_commit]_**'.

* Vous pouvez lister toutes les branches du projet avec la commande '**_./myGit refs-list_**'.

* Vous pouvez supprimer une branche avec la commande '**_./myGit delete-ref <nom_de_la_branche>_**'.

* Vous pouvez créer une nouvelle branche au dépot avec la commande '**_./myGit branch <nom_de_la_branche>_**' ou la commande '**_./myGit create-refs <nom_de_la_branche> <hash_de_la_reference>_**'.

* Vous pouvez savoir quelle est la branche courante avec la commande '**_./myGit get-current-branch_**'.

* Vous pouvez changer de branche avec la commande '**_./myGit checkout-branch <nom_de_la_branche>_**'.

* Vous pouvez afficher tout l'historique de commits d'une branche avec la commande '**_./myGit branch-print <nom_de_la_branche>_**'.

* Vous pouvez changer de version (commit) avec la commande '**_./myGit checkout-commit <pattern_du_hash_du_commit>_**'.

* Vous pouvez fusionner deux branches avec la commande '**_./myGit merge <nom_de_la_branche_distante>_**'.

* Vous pouvez obtenir plus de renseignements sur les commandes et leur utilisation avec la commande '**_./myGit help_**'.


**Dernier avertissement ! A cause du choix d'implémentation du projet, il faut veiller à ne pas ajouter les instantanes et leur repertoires dans la zone de preparation ou bien le depot local sera ruiné lors d'un commit/merge.  
De plus, si les fichiers sont placés dans des sous-répertoires, en cas de conflits lors d'un merge, vous n'aurez que la possibilité de choisir entre les versions des répertoires en entiers (les fichiers qu'ils contiennent ne seront pas pris en compte individuellement). Cela est aussi causé par l'implémentation actuelle des différentes fonctions (le fait de comparer les hashs pour comparer les versions notamment).**
