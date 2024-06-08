/* BIBLIOTECA -------------- COMUNICAÇÃO.H  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "comunicacao.h"

// Função para configurar a porta serial
SerialHandle setup_serial(const char *portname)
{
    // Abre a porta serial especificada (portname)
    SerialHandle hSerial = CreateFile(portname, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    // Verifica se a porta foi aberta com sucesso
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            printf("Error: Porta %s não encontrada\n", portname);
        }
        else
        {
            printf("Error: ao abrir porta %s\n", portname);
        }
        return INVALID_HANDLE_VALUE;
    }

    // Configura os parâmetros da porta serial
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        printf("Error: ao obter parâmetros da porta serial\n");
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    dcbSerialParams.BaudRate = CBR_9600;   // Configura a taxa de transmissão
    dcbSerialParams.ByteSize = 8;          // Configura o tamanho dos dados (8 bits)
    dcbSerialParams.StopBits = ONESTOPBIT; // Configura um bit de parada
    dcbSerialParams.Parity = NOPARITY;     // Sem paridade

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        printf("Error: ao configurar parâmetros da porta serial\n");
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    // Configura os timeouts da porta serial
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        printf("Error: ao configurar timeouts da porta serial\n");
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    return hSerial; // Retorna o identificador da porta serial configurada
}

// Função para enviar uma mensagem para o Arduino
void send_message(SerialHandle hSerial, const char *message)
{
    DWORD bytes_written;
    size_t message_len = strlen(message) + 2; // Inclui espaço para o '\n' e o '\0'
    char *buffer = (char *)malloc(message_len * sizeof(char));

    if (buffer == NULL)
    {
        printf("Error: memória insuficiente\n");
        return;
    }

    snprintf(buffer, message_len, "%s\n", message); // Adiciona uma nova linha ao final da mensagem

    // Escreve a mensagem na porta serial
    if (!WriteFile(hSerial, buffer, strlen(buffer), &bytes_written, NULL))
    {
        printf("Error: ao enviar mensagem\n");
    }
    else
    {
        printf("Mensagem enviada: %s\n", buffer);
    }

    free(buffer); // Libera a memória alocada
}

// Função para ler uma mensagem do Arduino
void read_message(SerialHandle hSerial)
{
    DWORD bytes_read;
    char buffer[256];
    // Lê a mensagem da porta serial
    if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytes_read, NULL))
    {
        buffer[bytes_read] = '\0'; // Adiciona um terminador nulo para formar uma string válida

        // Aloca memória dinamicamente para a mensagem lida
        char *message = (char *)malloc((bytes_read + 1) * sizeof(char));
        if (message == NULL)
        {
            printf("Error: memória insuficiente\n");
            return;
        }
        strncpy(message, buffer, bytes_read + 1);

        printf("Recebido: %s", message);

        free(message); // Libera a memória alocada
    }
    else
    {
        printf("Error: ao ler mensagem\n");
    }
}

// Função que exibe a tela principal do sistema
void tela_principal()
{
    printf("========================================\n");
    printf("        Sistema de Comunicacao          \n");
    printf("         QUARTETO FANTASTICO            \n");
    printf("========================================\n");
    printf("Pressione Enter para continuar...");
    getchar(); // Aguarda o usuário pressionar Enter
}

// Função que exibe o menu de escolha do dispositivo
void tela_escolha()
{
    printf("\nEscolha quem irá enviar a mensagem:\n");
    printf("1. PC\n");
    printf("2. Arduino\n");
    printf("3. Voltar\n");
    printf("Escolha uma opcao: ");
}

// Função para enviar uma mensagem do PC para o Arduino
void enviar_mensagem_pc(SerialHandle hSerial)
{
    char message[17];
    printf("Digite sua mensagem (máx 16 caracteres): ");
    scanf(" %16s", message);        // Lê a mensagem do usuário (máximo de 16 caracteres)
    send_message(hSerial, message); // Envia a mensagem para o Arduino
    printf("Mensagem enviada para o Arduino.\n");
}

// Função para receber uma mensagem do Arduino para o PC
void receber_mensagem_arduino(SerialHandle hSerial)
{
    printf("Aguardando mensagem do Arduino...\n");
    read_message(hSerial); // Lê a mensagem do Arduino
}
