#ifndef GROUPE_H
#define GROUPE_H

#include "server2.h"

typedef struct{
    char nom[BUF_SIZE];
    char membres[MAX_MEMBRE_GROUPES][BUF_SIZE];
    int nombre_membre;
} Groupe;

#endif /* guard */
