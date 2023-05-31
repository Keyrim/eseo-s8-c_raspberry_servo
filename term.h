/**
 *  Fichier : term.h
 *  Description : Interface pour l'analyseur de commandes entrantes
 *  Auteur : Théo 2023
 */

#ifndef _TERM_H_
#define _TERM_H_

/* ************************************* Includes *********************************************** */
#include <string.h> 

/* ************************************* Public macros ****************************************** */

/* Define the maximum parameters */
#define MAX_PARAMS 10 

/* ************************************* Public type definition ********************************* */

/* ************************************* Public variables *************************************** */

/* ************************************* Public functions *************************************** */
/**
 * @brief Réceptionne et traite une commande entrante.
 * 
 * @param command Commande à traiter.
 * 
 * @return 0 si la commande est exécutée avec succès, -1 en cas d'erreur.
 */
int TERM_receive_command(char* command);

/* ************************************* Public callback functions ****************************** */

#endif /* _TERM_H_*/
