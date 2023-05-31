/**
 *  Fichier : term.c
 *  Description : Implémentation de l'analyseur de commandes entrantes
 *  Auteur : Théo 2023
 */

/* ************************************* Includes *********************************************** */
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Includes pour les commandes
#include "term_servo.h"
#include "term.h"

/* ************************************* Private macros ***************************************** */
#define CMD_COUNT (sizeof(commands)/sizeof(commands[0]))
#define DEF_COMMAND(name_, function_, nb_param_, helper_)  \
{                                               \
    .name = name_,                              \
    .helper = helper_,                          \
    .function = function_,                      \
    .nb_param = nb_param_                       \
}

/* ************************************* Private type definition ******************************** */
typedef struct 
{
    char* name;
    char* helper;
    int(*function)(char** args);
    uint8_t nb_param;
}command_t;

/* ************************************* Private functions prototypes *************************** */
// Fonctions pour les commandes
/**
 * @brief Commande pour arrêter le programme.
 * 
 * @param args Arguments de la commande (non utilisés pour cette commande).
 * 
 * @return Ne renvoie rien car le programme est arrêté.
 */
static int CMD_stop(char** args);

/**
 * @brief Commande pour afficher l'aide.
 * 
 * @param args Arguments de la commande (non utilisés pour cette commande).
 * 
 * @return 0 si la commande est exécutée avec succès.
 */
static int CMD_help(char** args);

/* ************************************* Private variables ************************************** */
static command_t commands[] = 
{
    //          Nom                 Fonction                Nb param,   Helper
    DEF_COMMAND("help",             CMD_help,               0,          "Affiche l'aide de toutes les commandes."),
    DEF_COMMAND("stop",             CMD_stop,               0,          "Arrêt sauvage du programme."),
    DEF_COMMAND("servo-add",        TERM_SERVO_add_servo,   1,          "Ajout d'un servomoteur, param1 pour le pin à utiliser."),
    DEF_COMMAND("servo-set",        TERM_SERVO_set_pwm,     2,          "Change la pwm d'un moteur, param1 pour l'id du servo, param2 pour la consigne de 0 à 100."),
    DEF_COMMAND("servo-stop",       TERM_SERVO_stop,        0,          "Deinit tous les servos."),
};

/* ************************************* Public variables *************************************** */

/* ************************************* Private functions ************************************** */
/**
 * @brief Récupère les paramètres d'une commande.
 * 
 * @param input Chaine de caractères contenant la commande et ses paramètres.
 * @param params Tableau de pointeurs sur char, pour stocker les paramètres extraits.
 * 
 * @return Nombre de paramètres extraits.
 */
static int TERM_get_params(char* input, char** params)
{
    const char s[2] = " ";  // On choisit l'espace comme séparateur
    char* token;
    int i = 0;

    // On récupère le premier token
    token = strtok(input, s);
    params[0] = token;

    // On parcourt les autres tokens
    while( token != NULL )
    {
        i++;
        token = strtok(NULL, s);
        params[i] = token;
    }
    return i;  // On renvoie le nombre de paramètres
}

/**
 * @brief Supprime les caractères de nouvelle ligne d'une chaîne de caractères.
 * 
 * @param input Chaîne de caractères dont on souhaite supprimer les caractères de nouvelle ligne.
 */
static void TERM_remove_new_line(char* input)
{
    int len = strlen(input);
    while(len--)
    {
        if(input[len] == '\n')
        {
            input[len] = '\0';
        }
    }
}

/**
 * @brief Récupère l'ID de la commande à partir de son nom.
 * 
 * @param name Nom de la commande dont on cherche l'ID.
 * 
 * @return ID de la commande, -1 si la commande n'est pas trouvée.
 */
static int TERM_cmd_id_from_name(char* name)
{
    int ret = -1;
    for(uint8_t i = 0; i < CMD_COUNT; i++)
    {
        if(strcmp(commands[i].name, name) == 0)
        {
            ret = i;
            break;
        }
    }
    return ret;
}

// Fonctions pour les commandes
/**
 * @brief Commande pour arrêter le programme.
 * 
 * @param args Arguments de la commande (non utilisés pour cette commande).
 * 
 * @return Ne renvoie rien car le programme est arrêté.
 */
static int CMD_stop(char** args)
{
    printf("Fin du programme.\n");
    exit(0);
}

/**
 * @brief Commande pour afficher l'aide.
 * 
 * @param args Arguments de la commande (non utilisés pour cette commande).
 * 
 * @return 0 si la commande est exécutée avec succès.
 */
static int CMD_help(char** args)
{
    printf("Pour utiliser une commande, tapez son nom puis ses paramètres séparés par des espaces.\n");
    printf("Voici une liste des commnandes disponibles :\n");
    for(uint8_t c = 0; c < CMD_COUNT; c++)
    {
        printf("- %s : %s\n", commands[c].name, commands[c].helper);
    }
    return 0;
}

/* ************************************* Public functions *************************************** */
/**
 * @brief Réceptionne et traite une commande entrante.
 * 
 * @param command Commande à traiter.
 * 
 * @return 0 si la commande est exécutée avec succès, -1 en cas d'erreur.
 */
int TERM_receive_command(char* command)
{
    char* args [MAX_PARAMS] = { 0 };
    int ret = 0;
    int cmd_id = 0;
    int nb_param = 0;

    // On retire d'éventuel \n qui nous embête après quand on veut parser
    TERM_remove_new_line(command);

    // Parse chaîne de caractère pour extraire les arguments
    ret = TERM_get_params(command, args);
    if(ret >= 0)
    {
        nb_param = ret - 1;
    }

    // On trouve la commande
    if(ret >= 0)
    {
        cmd_id = TERM_cmd_id_from_name(args[0]);
        if(cmd_id == -1)
        {
            printf("Command not found.\n");
            ret = -1;
        }
    }

    // On vérifie si le nombre de paramètre correspond au nombre de paramètre attendus
    if( ret >= 0)
    {
        if(commands[cmd_id].nb_param != nb_param)
        {
            printf("%d paramètres attendus mais %d paramètres données.\n", 
                    commands[cmd_id].nb_param,
                    nb_param);
            ret = -1;
        }
    }

    // On appelle la fonction associée à la commande
    if(ret >= 0)
    {
        commands[cmd_id].function(&args[1]);
    }

    return ret;
}

/* ************************************* Public callback functions ****************************** */
