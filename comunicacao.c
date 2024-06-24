/* BIBLIOTECA ---------------- COMUNICAÇÃO.C */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "comunicacao.h"

ListaEncadeada *lista_mensagens = NULL;

// Função para criar uma nova lista encadeada
ListaEncadeada *criar_lista()
{
    ListaEncadeada *lista = (ListaEncadeada *)malloc(sizeof(ListaEncadeada));
    if (lista != NULL)
    {
        lista->cabeca = NULL;
        lista->tamanho = 0;
    }
    return lista;
}

// Função para adicionar uma mensagem à lista encadeada
void adicionar_mensagem(ListaEncadeada *lista, const char *mensagem)
{
    No *novo_no = (No *)malloc(sizeof(No));
    if (novo_no != NULL)
    {
        novo_no->mensagem = strdup(mensagem); // Copia a mensagem
        novo_no->proximo = NULL;

        if (lista->cabeca == NULL)
        {
            lista->cabeca = novo_no;
        }
        else
        {
            No *temp = lista->cabeca;
            while (temp->proximo != NULL)
            {
                temp = temp->proximo;
            }
            temp->proximo = novo_no;
        }
        lista->tamanho++;
    }
}

// Função para imprimir todas as mensagens da lista encadeada
void imprimir_mensagens(const ListaEncadeada *lista)
{
    No *atual = lista->cabeca;
    while (atual != NULL)
    {
        printf("Mensagem: %s\n", atual->mensagem);
        atual = atual->proximo;
    }
}

// Função para liberar a memória usada pela lista encadeada
void liberar_lista(ListaEncadeada *lista)
{
    No *atual = lista->cabeca;
    while (atual != NULL)
    {
        No *temp = atual;
        atual = atual->proximo;
        free(temp->mensagem);
        free(temp);
    }
    free(lista);
}

ManipuladorSerial configurar_serial(const char *nome_porta)
{
    // Abre a porta serial especificada (nome_porta)
    ManipuladorSerial manipulador = CreateFile(nome_porta, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    // Verifica se a porta foi aberta com sucesso
    if (manipulador == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            printf("Erro: Porta %s não encontrada\n", nome_porta);
        }
        else
        {
            printf("Erro: ao abrir porta %s\n", nome_porta);
        }
        return INVALID_HANDLE_VALUE;
    }

    // Configura os parâmetros da porta serial
    DCB parametros_serial = {0};
    parametros_serial.DCBlength = sizeof(parametros_serial);

    if (!GetCommState(manipulador, &parametros_serial))
    {
        printf("Erro: ao obter parâmetros da porta serial\n");
        CloseHandle(manipulador);
        return INVALID_HANDLE_VALUE;
    }

    parametros_serial.BaudRate = CBR_9600;   // Configura a taxa de transmissão
    parametros_serial.ByteSize = 8;          // Configura o tamanho dos dados (8 bits)
    parametros_serial.StopBits = ONESTOPBIT; // Configura um bit de parada
    parametros_serial.Parity = NOPARITY;     // Sem paridade

    if (!SetCommState(manipulador, &parametros_serial))
    {
        printf("Erro: ao configurar parâmetros da porta serial\n");
        CloseHandle(manipulador);
        return INVALID_HANDLE_VALUE;
    }

    // Configura os timeouts da porta serial
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(manipulador, &timeouts))
    {
        printf("Erro: ao configurar timeouts da porta serial\n");
        CloseHandle(manipulador);
        return INVALID_HANDLE_VALUE;
    }

    return manipulador; // Retorna o identificador da porta serial configurada
}

// Função para enviar uma mensagem para o Arduino
void enviar_mensagem(ManipuladorSerial manipulador, const char *mensagem)
{
    DWORD bytes_escritos;
    size_t tamanho_mensagem = strlen(mensagem) + 2; // Inclui espaço para o '\n' e o '\0'
    char *buffer = (char *)malloc(tamanho_mensagem * sizeof(char));

    if (buffer == NULL)
    {
        printf("Erro: memoria insuficiente\n");
        return;
    }

    snprintf(buffer, tamanho_mensagem, "%s\n", mensagem); // Adiciona uma nova linha ao final da mensagem

    // Escreve a mensagem na porta serial
    if (!WriteFile(manipulador, buffer, strlen(buffer), &bytes_escritos, NULL))
    {
        printf("Erro: ao enviar mensagem\n");
    }
    else
    {
        printf("Mensagem enviada: %s\n", buffer);
    }

    free(buffer); // Libera a memória alocada
}

// Função para ler uma mensagem do Arduino
void ler_mensagem(ManipuladorSerial manipulador)
{
    DWORD bytes_lidos;
    char buffer[256];
    // Lê a mensagem da porta serial
    if (ReadFile(manipulador, buffer, sizeof(buffer) - 1, &bytes_lidos, NULL))
    {
        buffer[bytes_lidos] = '\0'; // Adiciona um terminador nulo para formar uma string válida

        // Aloca memória dinamicamente para a mensagem lida
        char *mensagem = (char *)malloc((bytes_lidos + 1) * sizeof(char));
        if (mensagem == NULL)
        {
            printf("Erro: memória insuficiente\n");
            return;
        }
        strncpy(mensagem, buffer, bytes_lidos + 1);

        printf("Recebido: %s", mensagem);

        // Adicionar mensagem à lista encadeada
        adicionar_mensagem(lista_mensagens, mensagem);

        free(mensagem); // Libera a memória alocada
    }
    else
    {
        printf("Erro: ao ler mensagem\n");
    }
}

// Função que exibe a tela principal do sistema
void exibir_tela_principal()
{
    printf("========================================\n");
    printf("         Sistema de Comunicacao         \n");
    printf("    ₊˚⊹✧˖°   TRIO DE OURO   ₊˚⊹✧˖°    \n");
    printf("========================================\n");
    printf("Pressione Enter para continuar...");
    getchar();
}

// Função que exibe o menu de escolha do dispositivo
void exibir_menu_escolha()
{
    printf("\nEscolha quem irá enviar a mensagem:\n");
    printf("1. PC\n");
    printf("2. Arduino\n");
    printf("3. Mostrar lista de mensagens anteriores\n");
    printf("4. Sair\n");
    printf("Escolha uma opcao: ");
}

// Função para enviar uma mensagem do PC para o Arduino
void enviar_mensagem_pc(ManipuladorSerial manipulador)
{
    char mensagem[17];
    printf("Digite sua mensagem (máx 16 caracteres): ");
    scanf(" %16s", mensagem);               // Lê a mensagem do usuário (máximo de 16 caracteres)
    enviar_mensagem(manipulador, mensagem); // Envia a mensagem para o Arduino
    printf("Mensagem enviada para o Arduino.\n");
}

// Função para receber uma mensagem do Arduino para o PC
void receber_mensagem_arduino(ManipuladorSerial manipulador)
{
    printf("Aguardando mensagem do Arduino...\n");
    ler_mensagem(manipulador); // Lê a mensagem do Arduino
}

// Função para mostrar mensagens recebidas
void mostrar_mensagens()
{
    if (lista_mensagens != NULL && lista_mensagens->tamanho > 0)
    {
        printf("\nMensagens recebidas:\n");
        imprimir_mensagens(lista_mensagens); // Imprime as mensagens da lista encadeada
    }
    else
    {
        printf("\nNenhuma mensagem recebida.\n");
    }
}
