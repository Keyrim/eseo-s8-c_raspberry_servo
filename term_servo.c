/**
 *  Fichier : term_servo.c
 *  Description : Surcouche du module terminale pour la commande des servos moteurs
 *  Auteur : Théo 2023
 */


/* ************************************* Includes *********************************************** */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "servo.h"
#include "term_servo.h"

/* ************************************* Private macros ***************************************** */
#define NB_MAX_SERVO 4
/* ************************************* Private type definition ******************************** */

/* ************************************* Private functions prototypes *************************** */

/* ************************************* Private variables ************************************** */
static servo_ptr_t servos[NB_MAX_SERVO] = { 0 };
static int nb_servos = 0;
/* ************************************* Public variables *************************************** */

/* ************************************* Private functions ************************************** */

/* ************************************* Public functions *************************************** */
/**
 * @brief Ajoute un servo-moteur.
 *
 * @param args Tableau de chaînes de caractères contenant les arguments de la commande.
 *             - args[0] : Numéro de la broche du servo-moteur (entier non signé).
 * @return 0 en cas de succès, -1 en cas d'échec.
 */
int TERM_SERVO_add_servo(char** args)
{
    if(nb_servos == NB_MAX_SERVO)
    {
        printf("Nombre de servo max atteint.\n");
        return -1;
    }
    int pin_number = atoi(args[0]);
    servo_ptr_t new_servo = SERVO_create(pin_number, 0);
    if(new_servo)
    {
        printf("Servo à l'id %d ajouté sur le pin %d.\n",nb_servos , pin_number);
        servos[nb_servos++] = new_servo;
        return 0;
    }
    else
    {
        printf("Echec dans l'ajout d'un servo sur le pin %d\n.", pin_number);
        return -1;
    }
}

/**
 * @brief Configure le rapport cyclique d'un servo-moteur.
 *
 * @param args Tableau de chaînes de caractères contenant les arguments de la commande.
 *             - args[0] : Identifiant du servo-moteur (entier non signé).
 *             - args[1] : Valeur du rapport cyclique (entier non signé).
 * @return 0 en cas de succès, -1 en cas d'échec.
 */
int TERM_SERVO_set_pwm(char** args)
{
    int servo_id = atoi(args[0]);
    if(servo_id >= nb_servos)
    {
        printf("Le servo demandé (%d) n'existe pas.\n", servo_id);
        return -1;
    }
    int pwm = atoi(args[1]);
    SERVO_set_pwm(servos[servo_id], pwm);
    return 0;
}

/**
 * @brief Arrête tous les servo-moteurs.
 *
 * @param args Tableau de chaînes de caractères contenant les arguments de la commande (non utilisé).
 * @return 0 en cas de succès, -1 en cas d'échec.
 */
int TERM_SERVO_stop(char** args)
{
    if(nb_servos <= 0)
    {
        printf("Aucun servos n'est en vie.\n");
        return -1;
    }
    while(nb_servos--)
    {
        SERVO_destroy(servos[nb_servos]);
    }
    nb_servos = 0;
    return 0;
}
/* ************************************* Public callback functions ****************************** */
