.text
.global OndaExpansionASMB
.global QUIT
.global DELAY
.global DELAY_3
.global DisplayBinary
.global LedOutput
.global Delay
.global Clear

OndaExpansionASMB:
    PUSH {R0,R4,R5,R6,R7,R8,R9,R10,R11,LR}
    BL Clear
    LDR R11, =DELAY_3        // Cargar la dirección de DELAY_3
    LDR R6, [R11]            // Cargar el valor de DELAY_3
    LDR R9, =DELAY           // Cargar la dirección de DELAY
    STR R6, [R9]             // Asignar DELAY_3 a DELAY
    LDR R10, =QUIT           // Cargar la dirección de QUIT
    MOV R4, #0x18            // Inicializar DISPLAY con 00011000

while:
    MOV R5, #0               // Inicializar i = 0 para bucle for

for_outer:
    LDR R7, [R10]            // Cargar el valor de QUIT
    CMP R7, #1               // Comparar QUIT con 1
    BEQ break                // Si QUIT es 1, salir del bucle

    MOV R0, R4
    MOV R1, #3
    BL DisplayBinary         // Mostrar el estado binario de los LEDs

    MOV R0, R4
    BL LedOutput             // Enviar el estado a los LEDs

    ORR R4, R4, R4, LSL #1   // Desplazar a la izquierda y combinar para la siguiente etapa
                
    LDR R6, [R9]             // Cargar el valor de DELAY
    MOV R0, R6
    BL Delay                 // Generar delay

    ADD R5, R5, #1
    CMP R5, #3               // Comparar i con 3
    BLT for_outer            // Si i < 3, repetir el bucle

    MOV R0, #0               // Apagar todos los LEDs
    MOV R1, #3
    BL DisplayBinary
    BL LedOutput

    LDR R7, [R10]            // Cargar el valor de QUIT
    CMP R7, #0
    BEQ while                // Si QUIT es 0, repetir el bucle

break:
    STR R6, [R11]
    MOV R0, #0
    STR R0, [R10]
    POP {R0,R4,R5,R6,R7,R8,R9,R10,R11,PC}

.data
.end

