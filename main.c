/* BIBLIOTECA ---------------- MAIN.C  */
// teste
#include <stdio.h>
#include "comunicacao.h"

int main()
{
    char *portname = "COM3";                       // Nome da porta serial
    SerialHandle hSerial = setup_serial(portname); // Configura a porta serial
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        return 1; // Sai do programa se a configuração da porta serial falhar
    }

    char choice;

    tela_principal(); // Exibe a tela principal

    // Loop principal do programa
    while (1)
    {
        tela_escolha();        // Exibe o menu de escolha do dispositivo
        scanf(" %c", &choice); // Lê a escolha do usuário

        switch (choice - '0')
        {
        case OPC_PC:
            enviar_mensagem_pc(hSerial); // Envia uma mensagem do PC para o Arduino
            break;
        case OPC_ARDUINO:
            receber_mensagem_arduino(hSerial); // Recebe uma mensagem do Arduino para o PC
            break;
        case OPC_SAIR:
            printf("Saindo...\n");
            CloseHandle(hSerial); // Fecha a porta serial
            return 0;             // Sai do programa
        default:
            printf("Opção inválida! Tente novamente.\n");
            break;
        }
    }
}
