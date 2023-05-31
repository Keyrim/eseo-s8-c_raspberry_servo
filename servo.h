/**
 *  Fichier : servo.h
 *  Description : Permet d'instancier de nouveaux servo-moteurs
 *  Auteur : Théo 2023
 */

#ifndef _SERVO_H_
#define _SERVO_H_

/* ************************************* Includes *********************************************** */
#include <stdint.h>
#include <gpiod.h>

/* ************************************* Public macros ****************************************** */

/* ************************************* Public type definition ********************************* */
typedef void* servo_ptr_t;

/* ************************************* Public variables *************************************** */

/* ************************************* Public functions *************************************** */
/**
 * @brief Crée une nouvelle instance de servo-moteur.
 *
 * @param pin Numéro de la broche du servo-moteur.
 * @param gpio Numéro du GPIO utilisé.
 * @return Un pointeur vers la structure servo_ptr_t représentant le servo-moteur, ou NULL en cas d'erreur.
 */
servo_ptr_t SERVO_create(uint16_t pin, uint8_t gpio);

/**
 * @brief Configure le rapport cyclique du servo-moteur.
 *
 * @param servo_ptr Pointeur vers la structure servo_ptr_t représentant le servo-moteur.
 * @param pwm Valeur du rapport cyclique (entre 0 et 255).
 */
void SERVO_set_pwm(servo_ptr_t servo_ptr, uint8_t pwm);

/**
 * @brief Détruit une instance de servo-moteur.
 *
 * @param servo_ptr Pointeur vers la structure servo_ptr_t représentant le servo-moteur.
 */
void SERVO_destroy(servo_ptr_t servo_ptr);


/* ************************************* Public callback functions ****************************** */

#endif /* _SERVO_H_*/