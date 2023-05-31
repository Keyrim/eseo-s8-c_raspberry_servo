/**
 *  Fichier : servo.c
 *  Description : Permet de commander plusieurs servomoteurs en simultanné
 *  Auteur : Théo 2023
 */

/* ************************************* Includes *********************************************** */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "servo.h"

/* ************************************* Private macros ***************************************** */
#define CONSUMER "Consumer"
#define US_TO_NS(us_) ((us_) * 1000)

/* ************************************* Private type definition ******************************** */
typedef struct
{
    pthread_t thread;
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    uint8_t pwm;
    bool stop;
}servo_t;

/* ************************************* Private functions prototypes *************************** */
static void* SERVO_run(void* arg);

/* ************************************* Private variables ************************************** */

/* ************************************* Public variables *************************************** */

/* ************************************* Private functions ************************************** */
/**
 * @brief Fonction d'exécution du thread du servo-moteur.
 *
 * @param arg Pointeur vers la structure servo_t représentant le servo-moteur.
 * @return void*
 */
static void* SERVO_run(void* arg)
{
    servo_t* servo = (servo_t*)arg;
    struct timespec ts = {0};
    for(;;)
    {
        // Check si on doit stop
        if((volatile bool)servo->stop)
        {
            gpiod_chip_close(servo->chip);
            break;
        }
        // Mise à jour pwm
        uint8_t pwm = (volatile uint8_t)servo->pwm;
        // Etat haut
        gpiod_line_set_value(servo->line, 1);
        ts.tv_nsec = US_TO_NS(1000 + pwm*10);
        nanosleep(&ts , NULL);
        // Etat bas
        gpiod_line_set_value(servo->line, 0);
        ts.tv_nsec = US_TO_NS(19000 - pwm*10);
        nanosleep(&ts , NULL);
    }
    return NULL;
}
/* ************************************* Public functions *************************************** */
/**
 * @brief Crée une nouvelle instance de servo-moteur.
 *
 * @param pin Numéro de la broche du servo-moteur.
 * @param gpio Numéro du GPIO utilisé.
 * @return Un pointeur vers la structure servo_ptr_t représentant le servo-moteur, ou NULL en cas d'erreur.
 */
servo_ptr_t SERVO_create(uint16_t pin, uint8_t gpio)
{
    servo_t* servo_ptr = malloc(sizeof(servo_t));

    if(servo_ptr == NULL)
    {
        return NULL;
    }

    servo_ptr->chip = gpiod_chip_open_by_number(gpio);
    if (!servo_ptr->chip)
    {
        free(servo_ptr);
        return NULL;
    }

    servo_ptr->line = gpiod_chip_get_line(servo_ptr->chip, pin);
    if (!servo_ptr->line)
    {
        free(servo_ptr);
        gpiod_chip_close(servo_ptr->chip);
        return NULL;
    }

    int ret = gpiod_line_request_output(servo_ptr->line, CONSUMER, 0);
    if (ret < 0)
    {
        free(servo_ptr);
        gpiod_chip_close(servo_ptr->chip);
        return NULL;
    }
    pthread_create(&servo_ptr->thread, NULL, SERVO_run, (void*)servo_ptr);
    servo_ptr->pwm = 0;
    servo_ptr->stop = false;
    return (servo_ptr_t)servo_ptr;
}

/**
 * @brief Configure le rapport cyclique du servo-moteur.
 *
 * @param servo_ptr Pointeur vers la structure servo_ptr_t représentant le servo-moteur.
 * @param pwm Valeur du rapport cyclique (entre 0 et 100).
 */
void SERVO_set_pwm(servo_ptr_t servo_ptr, uint8_t pwm)
{
    if(servo_ptr == NULL)
    {
        printf("SERVO_set_pwm : servo_ptr est NULL\n");
        return;
    }
    servo_t* servo = (servo_t*)servo_ptr;
    servo->pwm = pwm;
}

/**
 * @brief Détruit une instance de servo-moteur.
 *
 * @param servo_ptr Pointeur vers la structure servo_ptr_t représentant le servo-moteur.
 */
void SERVO_destroy(servo_ptr_t servo_ptr)
{
    if(servo_ptr == NULL)
    {
        printf("SERVO_destroy : servo_ptr est NULL\n");
        return;
    }
    servo_t* servo = (servo_t*)servo_ptr;
    servo->stop = true;
    pthread_join(servo->thread, NULL);
    free(servo_ptr);
}

/* ************************************* Public callback functions ****************************** */
