/**
 *  Fichier : main.c
 *  Description : Fichier principal du code
 *  Auteur : Théo 2023
 */

/* ************************************* Includes *********************************************** */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "servo.h"
#include "term.h"

/* ************************************* Private macros ***************************************** */

/* ************************************* Private type definition ******************************** */

/* ************************************* Private functions prototypes *************************** */

/* ************************************* Private variables ************************************** */

/* ************************************* Public variables *************************************** */

/* ************************************* Private functions ************************************** */

/* ************************************* Public functions *************************************** */

int main (void)
{
    printf("Si vous avez besoin d'aide, tapez la commande help.\n");
    char buf[256] = { 0 };
    for(;;)
    {
        printf("> ");
        fgets(buf, sizeof(buf), stdin);
        TERM_receive_command(buf);
    }

    return 0;
}
/* ************************************* Public callback functions ****************************** */
