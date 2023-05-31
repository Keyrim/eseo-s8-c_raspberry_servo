/**
 *  Fichier : term_servo.h
 *  Description : Surcouche du terminal pour la commande des servos moteurs
 *  Auteur : Théo 2023
 */

#ifndef _TERM_SERVO_
#define _TERM_SERVO_

/* ************************************* Includes *********************************************** */

/* ************************************* Public macros ****************************************** */

/* ************************************* Public type definition ********************************* */

/* ************************************* Public variables *************************************** */

/* ************************************* Public functions *************************************** */
/**
 * @brief Ajoute un servo-moteur.
 *
 * @param args Tableau de chaînes de caractères contenant les arguments de la commande.
 *             - args[0] : Numéro de la broche du servo-moteur (entier non signé).
 * @return 0 en cas de succès, -1 en cas d'échec.
 */
int TERM_SERVO_add_servo(char** args);

/**
 * @brief Configure le rapport cyclique d'un servo-moteur.
 *
 * @param args Tableau de chaînes de caractères contenant les arguments de la commande.
 *             - args[0] : Identifiant du servo-moteur (entier non signé).
 *             - args[1] : Valeur du rapport cyclique (entier non signé).
 * @return 0 en cas de succès, -1 en cas d'échec.
 */
int TERM_SERVO_set_pwm(char** args);

/**
 * @brief Arrête tous les servo-moteurs.
 *
 * @param args Tableau de chaînes de caractères contenant les arguments de la commande (non utilisé).
 * @return 0 en cas de succès, -1 en cas d'échec.
 */
int TERM_SERVO_stop(char** args);

/* ************************************* Public callback functions ****************************** */

#endif /* _TERM_SERVO_*/