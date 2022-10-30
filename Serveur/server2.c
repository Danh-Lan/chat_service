#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


#include "server2.h"
#include "client2.h"
#include "groupe.h"

const char CHAT_HISTORY[sizeof "./historique/historique.txt"] = "./historique/historique.txt";

static void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
   }
#endif
}

static void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}

static void app(void)
{
   SOCKET sock = init_connection();
   char buffer[BUF_SIZE];
   /* the index for the array */
   int actual = 0;
   int max = sock;
   int nbGroupe = 0;
   /* an array for all clients */
   Client clients[MAX_CLIENTS];
   Groupe groupes[MAX_GROUPES];
   fd_set rdfs;

   while(1)
   {
      int i = 0;
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the connection socket */
      FD_SET(sock, &rdfs);

      /* add socket of each client */
      for(i = 0; i < actual; i++)
      {
         FD_SET(clients[i].sock, &rdfs);
      }

      if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }

      /* something from standard input : i.e keyboard */
      if(FD_ISSET(STDIN_FILENO, &rdfs))
      {
         /* stop process when type on keyboard */
         break;
      }
      else if(FD_ISSET(sock, &rdfs))
      {
         /* new client */
         SOCKADDR_IN csin = { 0 };
         size_t sinsize = sizeof csin;
         int csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
         if(csock == SOCKET_ERROR)
         {
            perror("accept()");
            // write_history();
            continue;
         }

         /* after connecting the client sends its name */
         if(read_client(csock, buffer) == -1)
         {
            /* disconnected */
            continue;
         }

         /* what is the new maximum fd ? */
         max = csock > max ? csock : max;

         FD_SET(csock, &rdfs);

         Client c = { csock };
         strncpy(c.name, buffer, BUF_SIZE - 1);
         clients[actual] = c;
         actual++;
         
         /* envoie l'histoire du chat a nouveau client */
         send_history(c);
      }
      else
      {
         int i = 0;
         for(i = 0; i < actual; i++)
         {
            /* a client is talking */
            if(FD_ISSET(clients[i].sock, &rdfs))
            {
               Client client = clients[i];
               int c = read_client(clients[i].sock, buffer);
               char msg[BUF_SIZE];
               strcpy(msg, buffer);
               /* client disconnected */
               if(c == 0)
               {
                  closesocket(clients[i].sock);
                  remove_client(clients, i, &actual);
                  strncpy(buffer, client.name, BUF_SIZE - 1);
                  strncat(buffer, " disconnected !", BUF_SIZE - strlen(buffer) - 1);
                  send_message_to_all_clients(clients, client, actual, buffer, 1);
                  write_history(client, buffer, 1);
               }
               else
               {  
              
                char* commande;
                commande = strtok(buffer, " ");
                
                if (strcmp(commande, "/send1") == 0) {
                    // commande is useless here
                    char *receiver_name, *message;
                    commande = strtok(NULL, " ");
                    receiver_name = commande;
                    commande = strtok(NULL, "\n");
                    message = commande;
                    send_message_to_one_client(clients, clients[i], actual, receiver_name, message, 0);
                }
                else if (strcmp(commande, "/create") == 0){
                    char *nom;
                    commande = strtok(NULL, " ");
                    nom = commande;
                    Groupe room;
                    strncpy(room.nom, nom, BUF_SIZE - 1);
                    
                    // creator is part of the group
                    strcpy(room.membres[0], clients[i].name);
                    room.nombre_membre = 1;
                    
                    char* membre;
                    while(commande!=NULL){
                       commande = strtok(NULL, " ");
                       if (commande==NULL) break; // VERY IMPORTANT LINE, DONT REMOVE THIS
                       membre = commande;
                       strcpy(room.membres[room.nombre_membre], membre);
                       room.nombre_membre++;
                    }
                    
                    int existe = 0;
                    int j = 0;
                    for (j = 0; j < nbGroupe; j++) {
                        if (strcmp(groupes[j].nom, room.nom) == 0) {
                            existe = 1;
                            break;
                        }
                    }
                    
                    if (existe == 1) {
                        char server_message[sizeof "groupe existe déjà"] = "groupe existe déjà";
                        send_message_to_one_client(clients, clients[i], actual, clients[i].name, server_message, 1);
                    } else {
                        groupes[nbGroupe] = room;
                        nbGroupe++;
                    }
                }
                else if (strcmp(commande,"/join")==0){
                    // commande is useless here
                    char *groupe_nom, *membre_ajout;
                    commande = strtok(NULL, " ");
                    groupe_nom = commande;
                    commande = strtok(NULL, " ");
                    membre_ajout = commande;

                    Groupe chat;
                    int j = 0;
                    int group_existe = 0;
                    for (j = 0; j < nbGroupe; j++) {
                        if (strcmp(groupes[j].nom, groupe_nom) == 0) {
                            group_existe = 1;
                            chat = groupes[j];
                            break;
                        }
                    }

                    if (group_existe == 0) {
                        char server_message[sizeof "groupe non existe"] = "groupe non existe";
                        send_message_to_one_client(clients, clients[i], actual, clients[i].name, server_message, 1);
                    } else {
                        join_groupe(clients,client,actual,membre_ajout,&chat);
                        // update the group
                        for (j = 0; j < nbGroupe; j++) {
                            if (strcmp(groupes[j].nom, groupe_nom) == 0) {
                                groupes[j] = chat;
                                break;
                            }
                        }  
                    }
                }
                else if (strcmp(commande, "/send2") == 0) {
                    // commande is useless here
                    char *groupe, *message;
                    commande = strtok(NULL, " ");
                    groupe = commande;
                    commande = strtok(NULL, "\n");
                    message = commande;
                    send_message_to_groupe(clients, clients[i], actual, groupe, message, 0);
                }
                else {
                  send_message_to_all_clients(clients, client, actual, msg, 0);
                  write_history(client, msg, 0);
                }
                
               }
               break;
            }
         }
      }
   }

   clear_clients(clients, actual);
   end_connection(sock);
}

static void clear_clients(Client *clients, int actual)
{
   int i = 0;
   for(i = 0; i < actual; i++)
   {
      closesocket(clients[i].sock);
   }
}

static void remove_client(Client *clients, int to_remove, int *actual)
{
   /* we remove the client in the array */
   memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
   /* number client - 1 */
   (*actual)--;
}

static void send_message_to_all_clients(Client *clients, Client sender, int actual, const char *buffer, char from_server)
{
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   for(i = 0; i < actual; i++)
   {
      /* we don't send message to the sender */
      if(sender.sock != clients[i].sock)
      {
         if(from_server == 0)
         {
            strncpy(message, sender.name, BUF_SIZE - 1);
            strncat(message, " : ", sizeof message - strlen(message) - 1);
         }
         strncat(message, buffer, sizeof message - strlen(message) - 1);
         write_client(clients[i].sock, message);
      }
   }
       
}

static int init_connection(void)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
   {
      perror("bind()");
      exit(errno);
   }

   if(listen(sock, MAX_CLIENTS) == SOCKET_ERROR)
   {
      perror("listen()");
      exit(errno);
   }
   return sock;
}

static void end_connection(int sock)
{
   closesocket(sock);
}

static int read_client(SOCKET sock, char *buffer)
{
   int n = 0;

   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      /* if recv error we disonnect the client */
      n = 0;
   }

   buffer[n] = 0;

   return n;
}

static void write_client(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

static void write_history(Client sender, const char* buffer, char from_server) {
    FILE *f;
    f = fopen(CHAT_HISTORY, "a");
    
    char message[BUF_SIZE];
    message[0] = 0;
   
    if(f == NULL) {
        printf("Erreur lors de l'ouverture d'un fichier");
        exit(1);
    } else {
        if (from_server == 0) {
            strncpy(message, sender.name, BUF_SIZE - 1);
            strncat(message, " : ", sizeof message - strlen(message) - 1);
        }
         
        strncat(message, buffer, sizeof message - strlen(message) - 1);
        strncat(message, CRLF, sizeof message - strlen(message) - 1);
        
        fputs(message, f);
        fclose(f);
    }
}
static void send_history(Client client) {
   
    FILE* fichier = fopen(CHAT_HISTORY, "r");
    //char caractereActuel;
    char buf[BUF_SIZE];

    if (fichier != NULL) {
        while (fgets(buf,BUF_SIZE, fichier)!=NULL) {
            write_client(client.sock, buf);
        }
    }
}
static void send_message_to_one_client(Client *clients, Client sender, int actual, const char* receiver_name, const char *buffer, char from_server){
    int i = 0;
    char message[BUF_SIZE];
    message[0] = 0;
    int found = 0;
    for(i = 0; i < actual; i++)
   {
      if(strcmp(clients[i].name, receiver_name)==0)
      {
        found = 1;
         if(from_server == 0)
         {
            strncpy(message, sender.name, BUF_SIZE - 1);
            strncat(message, " : ", sizeof message - strlen(message) - 1);
         }
         strncat(message, buffer, sizeof message - strlen(message) - 1);
         write_client(clients[i].sock, message);
      }
   }

    if (found == 0) {
        printf("Client non trouvé..\n");
    }
    
}    

static void send_message_to_groupe(Client *clients, Client sender, int actual, Groupe *groupe, const char *buffer, char from_server){
    int i = 0, j = 0;
    char message[BUF_SIZE];
    message[0] = 0;
    int found = 0;
    for(i = 0; i < actual; i++)
   {
   for(j=0;j < (*groupe).nombre_membre;j++){
      if(strcmp(clients[i].name, (*groupe).membres[j])==0)
      {
        found = 1;
         if(from_server == 0)
         {
            strncpy(message, sender.name, BUF_SIZE - 1);
            strncat(message, " : ", sizeof message - strlen(message) - 1);
         }
         strncat(message, buffer, sizeof message - strlen(message) - 1);
         write_client(clients[i].sock, message);
      }
     }
   }

    if (found == 0) {
        printf("Client non trouvé..\n");
    }
    
}   
  
static void join_groupe(Client *clients, Client addedBy, int actual, char *added_name, Groupe *groupe){

   int i = 0;
   int found_membre =0;
    char message[BUF_SIZE];
    strcpy(message, "Nouveau membre ajouté : ");
    for(i = 0; i < actual; i++)
   {
      if(strcmp(clients[i].name, added_name)==0)
      {
         found_membre = 1;
         strncpy(groupe->membres[groupe->nombre_membre],added_name, BUF_SIZE - 1);
         groupe->nombre_membre++;
         //strncat(message, buffer, sizeof message - strlen(message) - 1);
         strncat(message,added_name, sizeof message - strlen(message) - 1);
         //send_message_to_groupe(clients,addedBy,actual,&groupe, message , 0);
         break;
      }
   }

    if (found_membre == 0) {
        printf("Client non trouvé..\n");
    }
    
} 
int main(int argc, char **argv)
{
    init();

   app();

   end();

   return EXIT_SUCCESS;
}
