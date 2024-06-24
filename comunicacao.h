/* BIBLIOTECA ---------------- COMUNICAÇÃO.H */
#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include <windows.h>

// Definição de um tipo de dados enumerado para as opções do menu
typedef enum
{
    OPC_PC = 1,
    OPC_ARDUINO,
    OPC_MOSTRAR,
    OPC_SAIR
} MenuOpcao;

// Definição de um tipo de dados mais simples para HANDLE
typedef HANDLE ManipuladorSerial;

// Estrutura do nó da lista encadeada
typedef struct No
{
    char *mensagem;
    struct No *proximo;
} No;

// Estrutura da lista encadeada
typedef struct
{
    No *cabeca;
    int tamanho;
} ListaEncadeada;

extern ListaEncadeada *lista_mensagens;

ListaEncadeada *criar_lista();
void adicionar_mensagem(ListaEncadeada *lista, const char *mensagem);
void imprimir_mensagens(const ListaEncadeada *lista);
void liberar_lista(ListaEncadeada *lista);

ManipuladorSerial configurar_serial(const char *nome_porta);
void enviar_mensagem(ManipuladorSerial manipulador, const char *mensagem);
void ler_mensagem(ManipuladorSerial manipulador);
void enviar_mensagem_pc(ManipuladorSerial manipulador);
void receber_mensagem_arduino(ManipuladorSerial manipulador);
void exibir_tela_principal();
void exibir_menu_escolha();
void mostrar_mensagens();

#endif

//  gcc comunicacao.c main.c -o main.exe
// COMPILAR > .\main.exe