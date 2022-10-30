#ifndef SERVER_H
#define SERVER_H

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error not defined for this platform

#endif

#define CRLF        "\r\n"
#define PORT         1977
#define MAX_CLIENTS             100
#define MAX_GROUPES             10
#define MAX_MEMBRE_GROUPES      10

#define BUF_SIZE    1024

#include "client2.h"
#include "groupe.h"

static void init(void);
static void end(void);
static void app(void);
static int init_connection(void);
static void end_connection(int sock);
static int read_client(SOCKET sock, char *buffer);
static void write_client(SOCKET sock, const char *buffer);
static void send_message_to_all_clients(Client *clients, Client client, int actual, const char *buffer, char from_server);
static void remove_client(Client *clients, int to_remove, int *actual);
static void clear_clients(Client *clients, int actual);
static void write_history(Client client, const char* buffer, char from_server);
static void send_history(Client client);
static void send_message_to_one_client(Client *clients, Client sender, int actual, const char* receiver_name, const char *buffer, char from_server);
//static void create_group(Client *client, int actual, Groupe *room);
//static void join_groupe(Client *clients, Client added_by, int actual, const char *added_name, Groupe *groupe);
//static void send_message_to_groupe(Client *clients, Client sender, int actual, Groupe groupe_receiver, const char *buffer, char from_server);
#endif /* guard */
