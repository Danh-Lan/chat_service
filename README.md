# TP programmation reseaux - service du chat

# Membre
NGUYEN Danh-Lan

AMMARI Anass

# Compilation
Tapez $make$ dans le répertoire principal, Makefile va compiler les Makefile dans les sous-répertoires Serveur et Client

# Execution
Pour le serveur : ./server

Pour le client : ./client [l'adresse IP qui tourne le serveur] [pseudo]

IMPORTANT : Pour vous déconnecter en tant que client, entrez ctrl+C. Ne fermez pas le terminal directement
# Commande
Commande | Parametre | Fonctionnement
------------ | ------------- | -------------
/send | [nom] [message] | envoyer message privé
/create | [groupe] [nom(s)] | créer un groupe
/join | [groupe] [nom] | faire rejoindre un client dans le groupe
/send_group | [groupe] | envoyer un message à un groupe

# Autre detail :
histoire/histoire.txt : enregistrer l'historique du chat global
