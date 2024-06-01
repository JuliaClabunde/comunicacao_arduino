#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Função para configurar a porta serial
HANDLE setup_serial(const char *portname)
{
    // Abre a porta serial especificada (portname)
    HANDLE hSerial = CreateFile(portname, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

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
void send_message(HANDLE hSerial, const char *message)
{
    DWORD bytes_written;
    // Escreve a mensagem na porta serial
    if (!WriteFile(hSerial, message, strlen(message), &bytes_written, NULL))
    {
        printf("Error: ao enviar mensagem\n");
    }
}

// Função para ler uma mensagem do Arduino
void read_message(HANDLE hSerial)
{
    char buffer[256];
    DWORD bytes_read;
    // Lê a mensagem da porta serial
    if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytes_read, NULL))
    {
        buffer[bytes_read] = '\0'; // Adiciona um terminador nulo para formar uma string válida
        printf("Recebido: %s\n", buffer);
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
    printf("        Sistema de Comunicação          \n");
    printf("          Nome dos Integrantes          \n");
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
    printf("Escolha uma opção: ");
}

// Função para enviar uma mensagem do PC para o Arduino
void enviar_mensagem_pc(HANDLE hSerial)
{
    char message[17];
    printf("Digite sua mensagem (máx 16 caracteres): ");
    scanf(" %16s", message);        // Lê a mensagem do usuário (máximo de 16 caracteres)
    send_message(hSerial, message); // Envia a mensagem para o Arduino
    printf("Mensagem enviada para o Arduino.\n");
}

// Função para receber uma mensagem do Arduino para o PC
void receber_mensagem_arduino(HANDLE hSerial)
{
    printf("Aguardando mensagem do Arduino...\n");
    read_message(hSerial); // Lê a mensagem do Arduino
}

int main()
{
    char *portname = "COM3";                 // Nome da porta serial
    HANDLE hSerial = setup_serial(portname); // Configura a porta serial
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

        switch (choice)
        {
        case '1':
            enviar_mensagem_pc(hSerial); // Envia uma mensagem do PC para o Arduino
            break;
        case '2':
            receber_mensagem_arduino(hSerial); // Recebe uma mensagem do Arduino para o PC
            break;
        case '3':
            printf("Saindo...\n");
            CloseHandle(hSerial); // Fecha a porta serial
            return 0;             // Sai do programa
        default:
            printf("Opção inválida! Tente novamente.\n");
            break;
        }
    }
}
