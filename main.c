#include <stdio.h>
#include <windows.h>
#include "comunicacao.h"

int main()
{
    ManipuladorSerial manipulador;
    char nome_porta[] = "COM3"; // Nome da porta serial (ajuste conforme necessário)
    int opcao;

    lista_mensagens = criar_lista();

    exibir_tela_principal();

    manipulador = configurar_serial(nome_porta);
    if (manipulador == INVALID_HANDLE_VALUE)
    {
        return 1; // Sai do programa se a porta serial não puder ser aberta
    }

    do
    {
        exibir_menu_escolha();
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer de entrada

        switch (opcao)
        {
        case OPC_PC:
            enviar_mensagem_pc(manipulador);
            break;
        case OPC_ARDUINO:
            receber_mensagem_arduino(manipulador);
            break;
        case OPC_MOSTRAR:
            mostrar_mensagens();
            break;
        case OPC_SAIR:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != OPC_SAIR);

    liberar_lista(lista_mensagens);
    CloseHandle(manipulador); // Fecha a porta serial

    return 0;
}
