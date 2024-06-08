/* BIBLIOTECA ---------------- COMUNICAÇÃO.H  */
#ifndef comunicacao_h
#define comunicacao_h
#include <windows.h>

// Definição de um tipo de dados enumerado para as opções do menu
typedef enum
{
    OPC_PC = 1,
    OPC_ARDUINO,
    OPC_SAIR
} MenuOpcao;

// Definição de um tipo de dados mais simples para HANDLE
typedef HANDLE SerialHandle;

SerialHandle setup_serial(const char *portname);
void send_message(SerialHandle hSerial, const char *message);
void read_message(SerialHandle hSerial);
void enviar_mensagem_pc(SerialHandle hSerial);
void receber_mensagem_arduino(SerialHandle hSerial);
void tela_principal();
void tela_escolha();

#endif

//  gcc comunicacao.c main.c -o main.exe
// COMPILAR > .\main.exe