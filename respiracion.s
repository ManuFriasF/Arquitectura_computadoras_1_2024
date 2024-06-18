.text
.global RespiracionASMB
.global QUIT
.global DELAY
.global DELAY_4
.global DisplayBinary
.global LedOutput
.global Delay
.global Clear

RespiracionASMB:
    PUSH {R0,R4,R5,R6,R7,R8,R9,R10,R11,LR}
    BL Clear
    LDR R11, =DELAY_4 // Cargar la dirección de DELAY_4
    LDR R6, [R11] // Cargar el valor de DELAY_4
    LDR R9, =DELAY // Cargar la dirección de DELAY
    STR R6, [R9] // Asignar DELAY_4 a DELAY
    LDR R10, =QUIT // Cargar la dirección de QUIT
    LDR R8, =table // Cargar la dirección de la tabla

while:
    MOV R5, #0 // Inicializar i = 0 para bucle for
for:
    LDR R7, [R10] // Cargar el valor de QUIT
    CMP R7, #1 // Comparar QUIT con 1
    BEQ break // Si QUIT es 1, salir del bucle

    MOV R4, #0 // Limpiar DISPLAY
    LDRB R4, [R8, R5] // Cargar DISPLAY con table[i]

    MOV R0, R4
    MOV R1, #4
    BL DisplayBinary // Mostrar el estado binario de los LEDs

    MOV R0, R4
    BL LedOutput // Enviar el estado a los LEDs

    LDR R6, [R9] // Cargar el valor de DELAY
    MOV R0, R6
    BL Delay // Generar delay

    ADD R5, R5, #1
    CMP R5, #8 // Comparar i con 8
    BLT for // Si i < 8, repetir el bucle

    LDR R7, [R10] // Cargar el valor de QUIT
    CMP R7, #0
    BEQ while // Si QUIT es 0, repetir el bucle

break:
    STR R6, [R11]
    MOV R0, #0
    STR R0, [R10]
    POP {R0,R4,R5,R6,R7,R8,R9,R10,R11,PC}
        
.data
        
table: // Tabla de valores
    .byte 0x18
    .byte 0x3C
    .byte 0x7E
    .byte 0xFF
    .byte 0x7E
    .byte 0x3C
    .byte 0x18
    .byte 0x00
    
.end

