#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <curses.h>
#include <unistd.h>
#include "EasyPIO.h"

// CONSTANTES
#define PASSWORD "12345" // Contraseña para el acceso
#define QUIT_KEY 'q' // Tecla para salir
#define DELAY_INTERVAL 250 // Intervalo de cambio de delay
#define DEFAULT_DELAY 1000 // Delay por defecto
#define PASSWORD_LENGTH 5

// Variables globales
unsigned int QUIT;
size_t DELAY;
size_t DELAY_1 = DEFAULT_DELAY;
size_t DELAY_2 = DEFAULT_DELAY;
size_t DELAY_3 = DEFAULT_DELAY;
size_t DELAY_4 = DEFAULT_DELAY;

// Declaración de funciones
void Delay(size_t a);
void MoveCursorToOrigin();
void Clear();
unsigned int Login();
void DisplayBinary(unsigned char DISPLAY, unsigned int option);
void LedOutput(unsigned char DISPLAY);
void *KeyListener(void *arg);
void *AutoFantastico(void *arg);
void *ElChoque(void *arg);
void *OndaExpansion(void *arg);
void *Respiracion(void *arg); // Cambiar nombre de la función
void App();
void pinSetup(void);

// Función para configurar los pines
void pinSetup(void) {
    pioInit();

    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);
    pinMode(18, OUTPUT);
    pinMode(23, OUTPUT);
    pinMode(24, OUTPUT);
    pinMode(25, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(7, OUTPUT);
}

// Función para generar un delay
void Delay(size_t a) {
    a = a * 100000;
    while (a--)
        ;
}

// Función para mover el cursor al origen
void MoveCursorToOrigin() {
    unsigned int i = 10;
    while (i--)
        printf("\033[F");
}

// Función para limpiar la pantalla
void Clear() {
    printf("\033[2J");
    MoveCursorToOrigin();
}

// Función de login
unsigned int Login() {
    char *right_password = PASSWORD;
    char input[6]; // Aumentar el tamaño del buffer para evitar desbordamientos
    int i = 0;
    char c;
    printf("Ingrese su password de 5 dígitos: ");

    while (1) {
        c = getchar();

        if (c == '\n') {
            input[i] = '\0';
            break;
        } else if (c == 127 || c == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            if (i < 5) {
                printf("*");
                input[i] = c;
                i++;
            }
        }
        if (i == 5)
            break;
    }

    input[i] = '\0';

    if (strcmp(input, right_password) == 0) {
        printf("\n\rAcceso concedido\n\r");
        return 1;
    } else {
        printf("\n\rAcceso denegado\n\r");
        return 0;
    }
}

// Función para mostrar el estado binario de los LEDs
void DisplayBinary(unsigned char DISPLAY, unsigned int option) {
    char display[9]; // Aumentar el tamaño del buffer para incluir el terminador nulo
    int i = 0;
    for (unsigned int POINTER = 0x80; POINTER > 0; POINTER = POINTER >> 1) {
        if (POINTER & DISPLAY) {
            display[i] = '*';
        } else {
            display[i] = '_';
        }
        i++;
    }
    display[i] = '\0'; // Añadir el terminador nulo al final de la cadena

    // Mostrar la secuencia en ejecución
    switch (option) {
    case 1:
        printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mAuto Fantástico\033[0m\n\r\n\r");
        break;
    case 2:
        printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mEl Choque\033[0m\n\r\n\r");
        break;
    case 3:
        printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mOnda de Expansión\033[0m\n\r\n\r");
        break;
    case 4:
        printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mRespiración\033[0m\n\r\n\r"); // Cambiar nombre de la secuencia
        break;
    case 0:
        break;
    }

    printf("%s", display);

    if (option) {
        printf("\n\r\n\r");
        printf("\033[1;30mPresione la tecla Q para salir\033[0m\n\r");
        printf("\033[1;30mDelay: \033[0m");
        printf("%zu ms", DELAY);  // Mostrar el valor correcto de DELAY en milisegundos
    }

    MoveCursorToOrigin();
}

// Función para enviar el estado a los LEDs
void LedOutput(unsigned char DISPLAY) {
    // Mapear cada bit del DISPLAY a un pin GPIO en el orden correcto
    digitalWrite(14, (DISPLAY >> 7) & 0x01); // Bit 7
    digitalWrite(15, (DISPLAY >> 6) & 0x01); // Bit 6
    digitalWrite(18, (DISPLAY >> 5) & 0x01); // Bit 5
    digitalWrite(23, (DISPLAY >> 4) & 0x01); // Bit 4
    digitalWrite(24, (DISPLAY >> 3) & 0x01); // Bit 3
    digitalWrite(25, (DISPLAY >> 2) & 0x01); // Bit 2
    digitalWrite(8,  (DISPLAY >> 1) & 0x01); // Bit 1
    digitalWrite(7,  DISPLAY & 0x01);        // Bit 0
}

// Función que escucha las teclas presionadas
void *KeyListener(void *arg) {
    while (!QUIT) {
        int key = getch();
        if (key == QUIT_KEY)
            QUIT = 1;
        else if (key == KEY_UP) {
            if (DELAY > DELAY_INTERVAL)
                DELAY -= DELAY_INTERVAL;
        } else if (key == KEY_DOWN) {
            DELAY += DELAY_INTERVAL;
        }
    }
    return NULL;
}

// Función para la secuencia Auto Fantástico
void *AutoFantastico(void *arg) {
    Clear();
    while (!QUIT) {
        unsigned char DISPLAY = 0x80;
        for (int i = 0; i < 7; i++) {
            if (QUIT)
                break;
            DisplayBinary(DISPLAY, 1);
            LedOutput(DISPLAY);
            DISPLAY = DISPLAY >> 1;
            Delay(DELAY);
        }
        for (int i = 0; i < 7; i++) {
            if (QUIT)
                break;
            DisplayBinary(DISPLAY, 1);
            LedOutput(DISPLAY);
            DISPLAY = DISPLAY << 1;
            Delay(DELAY);
        }
    }
    return NULL;
}

// Función para la secuencia El Choque
void *ElChoque(void *arg) {
    Clear();
    unsigned int table[] = {0x42, 0x24, 0x18, 0x24, 0x42, 0x81};
    unsigned char DISPLAY = 0x81;
    while (!QUIT) {
        int table_size = sizeof(table) / sizeof(table[0]);
        for (int i = 0; i < table_size; i++) {
            if (QUIT)
                break;
            DisplayBinary(DISPLAY, 2);
            LedOutput(DISPLAY);
            DISPLAY = table[i];
            Delay(DELAY);
        }
    }
    return NULL;
}


// Función para la secuencia Onda de Expansión
void *OndaExpansion(void *arg) {
    Clear();
    while (!QUIT) {
        unsigned char DISPLAY = 0x18; // 00011000
        // Encender desde el centro hacia afuera
        for (int i = 0; i < 4; i++) {
            if (QUIT)
                break;
            DisplayBinary(DISPLAY, 3);
            LedOutput(DISPLAY);
            DISPLAY = (DISPLAY >> 1) | (DISPLAY << 1);
            Delay(DELAY);
        }
        // Apagar todos los LEDs al mismo tiempo
        DisplayBinary(0x00, 3);
        LedOutput(0x00);
        Delay(DELAY);
    }
    return NULL;
}

// Función para la secuencia Respiración
void *Respiracion(void *arg) {
    Clear();
    unsigned int table[] = {0x18, 0x3C, 0x7E, 0xFF, 0x7E, 0x3C, 0x18, 0x00};
    unsigned char DISPLAY = 0;
    while (!QUIT) {
        int table_size = sizeof(table) / sizeof(table[0]);
        for (int i = 0; i < table_size; i++) {
            if (QUIT)
                break;
            DisplayBinary(DISPLAY, 4);
            LedOutput(DISPLAY);
            DISPLAY = table[i];
            Delay(DELAY);
        }
    }
    return NULL;
}

// Función principal de la aplicación
void App() {
    unsigned char option[2]; // Aumentar el tamaño del buffer para evitar desbordamientos
    Clear();

    int i = 0, a = 0;
    for (i = 0; i < 3; i++) {
        Delay(2000);
        Clear();
        if (Login())
            break;
    }

    if (i == 3)
        exit(0);

    do {
        Delay(2000);
        DisplayBinary(0, 0);
        Clear();
        printf("\033[?25h");
        QUIT = 0;

        printf("------ S E C U E N C I A S  D E  L U C E S ------\n\r");
        printf("1. Auto Fantástico\n\r");
        printf("2. El Choque\n\r");
        printf("3. Onda de Expansión\n\r");
        printf("4. Respiración\n\r"); // Cambiar nombre de la secuencia
        printf("0. Salir\n\r");
        printf("-------------------------------------------------\n\r");
        printf("\n\rSeleccione una opción: ");

        scanf("%1s", option); // Leer solo un carácter en option

        printf("\033[?25l");

        pthread_t threads[2]; // Mover declaración de threads fuera del switch

        switch (option[0]) {
        case '1':
            DELAY = DELAY_1;
            pthread_create(&threads[0], NULL, KeyListener, NULL);
            pthread_create(&threads[1], NULL, AutoFantastico, NULL);
            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
            DELAY_1 = DELAY;
            break;
        case '2':
            DELAY = DELAY_2;
            pthread_create(&threads[0], NULL, KeyListener, NULL);
            pthread_create(&threads[1], NULL, ElChoque, NULL);
            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
            DELAY_2 = DELAY;
            break;
        case '3':
            DELAY = DELAY_3;
            pthread_create(&threads[0], NULL, KeyListener, NULL);
            pthread_create(&threads[1], NULL, OndaExpansion, NULL);
            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
            DELAY_3 = DELAY;
            break;
        case '4':
            DELAY = DELAY_4;
            pthread_create(&threads[0], NULL, KeyListener, NULL);
            pthread_create(&threads[1], NULL, Respiracion, NULL);
            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
            DELAY_4 = DELAY;
            break;
        case '0':
            printf("\n\rSaliendo del programa...\n\r");
            Delay(2000);
            Clear();
            a = 1;
            break;
        default:
            printf("\n\rIngrese una opción válida\n\r");
            break;
        }
    } while (a == 0);
}

// Función principal
int main() {
    pinSetup();

    initscr();              // Inicializar la pantalla de ncurses
    keypad(stdscr, TRUE);   // Habilitar la entrada de teclado especial
    nodelay(stdscr, TRUE);  // Configurar getch() para no bloquear
    noecho();               // No mostrar los caracteres ingresados en pantalla

    App();

    endwin();
    return 0;
}