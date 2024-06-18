.text
.global AutoFantasticoASMB
.global QUIT
.global DELAY
.global DELAY_1
.global DisplayBinary
.global LedOutput
.global Delay
.global Clear

AutoFantasticoASMB:
    PUSH {R0,R4,R5,R6,R7,R8,R9,R10,R11,LR}
    BL Clear
    LDR R11, =DELAY_1 // Cargar la dirección de DELAY_1
    LDR R6, [R11] // Cargar el valor de DELAY_1
    LDR R9, =DELAY // Cargar la dirección de DELAY
    STR R6, [R9] // Asignar DELAY_1 a DELAY
    LDR R10, =QUIT // Cargar la dirección de QUIT

while:
    MOV R8, #0 // Inicializar i = 0, para bucle for
    MOV R4, #0x80 // Inicializar DISPLAY con 10000000
for_outer:
    LDR R7, [R10] // Cargar el valor de QUIT
    CMP R7, #1 // Comparar QUIT con 1
    BEQ break // Si QUIT es 1, salir del bucle

    MOV R0, R4
    MOV R1, #1
    BL DisplayBinary // Mostrar el estado binario de los LEDs

    MOV R0, R4
    BL LedOutput // Enviar el estado a los LEDs

    LSR R4, R4, #1 // Desplazar a la derecha el valor de DISPLAY
                
    LDR R6, [R9] // Cargar el valor de DELAY
    MOV R0, R6
    BL Delay // Generar delay

    ADD R8, R8, #1
    CMP R8, #7 // Comparar i con 7
    BLT for_outer // Si i < 7, repetir el bucle

    MOV R5, #0 // Inicializar i = 0 para bucle inverso
for_inner:
    LDR R7, [R10] // Cargar el valor de QUIT
    CMP R7, #1 // Comparar QUIT con 1
    BEQ break // Si QUIT es 1, salir del bucle

    MOV R0, R4
    MOV R1, #1
    BL DisplayBinary // Mostrar el estado binario de los LEDs

    MOV R0, R4
    BL LedOutput // Enviar el estado a los LEDs

    LSL R4, R4, #1 // Desplazar a la izquierda el valor de DISPLAY

    LDR R6, [R9] // Cargar el valor de DELAY
    MOV R0, R6
    BL Delay // Generar delay

    ADD R5, R5, #1
    CMP R5, #7 // Comparar i con 7
    BLT for_inner // Si i < 7, repetir el bucle

    LDR R7, [R10] // Cargar el valor de QUIT
    CMP R7, #0
    BEQ while // Si QUIT es 0, repetir el bucle

break:
    STR R6, [R11]
    MOV R0, #0
    STR R0, [R10]
    POP {R0,R4,R5,R6,R7,R8,R9,R10,R11,PC}

.data
.end

