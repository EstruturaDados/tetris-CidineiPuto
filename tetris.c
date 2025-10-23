#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma peça com tipo (caractere) e um identificador único
typedef struct {
    char tipo[2];
    int id;
} Peca;

// Definições de limites para fila, pilha e tamanho máximo de string
#define MAX_FILA 5
#define MAX_PILHA 4
#define MAX_STR_LEN 2

// Estrutura da pilha (usada para "reservar" peças)
typedef struct{
    Peca itens[MAX_FILA]; // Vetor de peças
    int topo;             // Índice do topo da pilha
} Pilha;

// Estrutura da fila (usada para armazenar peças na sequência de entrada)
typedef struct {
    Peca itens[MAX_FILA]; // Vetor de peças
    int inicio;           // Índice do primeiro elemento (cabeça)
    int fim;              // Índice do próximo espaço livre (cauda)
    int total;            // Quantidade total de peças na fila
} Fila;

// Limpa o buffer de entrada (evita problemas com scanf e getchar)
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Inicializa uma fila vazia
void inicializarFila(Fila *f){
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Inicializa uma pilha vazia
void inicializarPilha(Pilha *pil){
    pil->topo = -1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *pil){
    return pil->topo == -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *pil){
    return pil->topo == MAX_PILHA - 1;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f){
    return f->total == MAX_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f){
    return f->total == 0;
}

// Insere uma nova peça na fila (com ID automático)
void inserir(Fila *f, Peca p, int *idunico){
    if (filaCheia(f)){
        printf("Fila cheia. Nao e possivel inserir.\n");
        return;
    }
    p.id = (*idunico);  // Atribui ID único
    (*idunico)++;       // Incrementa o contador de IDs

    // Insere no final da fila (com controle circular)
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
}

// Remove uma peça da fila (a da frente)
void remover(Fila *f, Peca *p){
    if (filaVazia(f)){
        printf("Fila vazia. Nao e possivel remover.\n");
        return;
    }

    *p = f->itens[f->inicio];             // Pega o item do início
    f->inicio = (f->inicio + 1) % MAX_FILA; // Move o início circularmente
    f->total--;
}

// Move uma peça da fila para a pilha (reserva)
void inserirReserva(Fila *f, Pilha *pil, Peca *p){
    if (filaVazia(f)){
        printf("A fila esta vazia, impossivel inserir qualquer item na reserva.\n");
        return;
    }
    if (pilhaCheia(pil)){
        printf("A pilha esta cheia, impossivel inserir item\n");
        return;
    }

    remover(f, p); // Remove da fila
    pil->topo++;   // Aumenta o topo da pilha
    pil->itens[pil->topo] = *p; // Coloca a peça na pilha
}

// Retira uma peça da pilha (usar reserva)
void usarReserva(Pilha *pil, Peca *p){
    if (pilhaVazia(pil)){
        printf("A pilha esta vazia. Nao e possivel usar nenhuma reserva.");
        return;
    }

    *p = pil->itens[pil->topo]; // Retira o topo
    pil->topo--;                // Decrementa o topo
}

// Troca a peça do topo da pilha com a primeira da fila
void trocarPeca(Pilha *pil, Fila *f){
    if (pilhaVazia(pil)){
        printf("A pilha esta vazia, impossivel trocar.");
        return;
    }
    if (filaVazia(f)){
        printf("A fila esta vazia, impossivel trocar.");
        return;
    }

    // Troca simples entre topo da pilha e início da fila
    Peca temp =  f->itens[f->inicio];
    f->itens[f->inicio] = pil->itens[pil->topo];
    pil->itens[pil->topo] = temp;
}

// Troca múltiplas peças (3 primeiras da fila ↔ 3 do topo da pilha)
void trocarMultiplasPecas(Pilha *pil, Fila *f){
    if (pilhaVazia(pil) || filaVazia(f)){
        printf("A pilha e/ou a fila esta vazia, impossivel trocar.\n");
        return;
    }

    // Precisa ter pelo menos 3 peças em cada estrutura
    if (pil->topo < 2 || f->total < 3) {
        printf("Impossivel trocar: e necessario ter pelo menos 3 pecas na pilha e na fila.\n");
        return;
    }

    // Troca 3 peças entre as estruturas
    for (int i = 0; i < 3; i++) {
        int idx_fila = (f->inicio + i) % MAX_FILA; // Avança na fila circularmente
        int idx_pilha = pil->topo - i;             // Desce na pilha

        // Troca das peças
        Peca temp = f->itens[idx_fila];
        f->itens[idx_fila] = pil->itens[idx_pilha];
        pil->itens[idx_pilha] = temp;
    }
}

// Mostra o conteúdo atual da fila
void mostrarFila(Fila *f){
    printf("Fila: ");
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX_FILA){
        printf("[%s %d] ", f->itens[idx].tipo, f->itens[idx].id);
    }
    printf("\n");
}

// Mostra o conteúdo atual da pilha (do topo até a base)
void mostrarPilha(Pilha *pil){
    printf("Pilha [TOPO -> BASE]: \n");
    for (int i = pil->topo; i >= 0; i--){
        printf("[%s %d]\n", pil->itens[i].tipo, pil->itens[i].id);
    }
    printf("\n");
}

// Função principal com o menu interativo
int main(){
    int opcao = 0;
    int id_unico = 0;
    char tipo_peca[MAX_STR_LEN];
    int idade_Peca;
    Fila f;
    Pilha pil;

    // Inicializa as estruturas
    inicializarFila(&f);
    inicializarPilha(&pil);

    // Preenche a fila inicial com 5 peças
    Peca pecas[] = {
        {"T"}, {"O"}, {"I"}, {"O"}, {"L"}
    };
    for (int i = 0; i <= 4; i++){
        inserir(&f, pecas[i], &id_unico);
    }

    mostrarFila(&f);

    // Menu principal
    do {
        printf("==============================\n");
        printf("         MENU DA LISTA\n");
        printf("==============================\n");
        printf("1.  INSERIR PECA\n");
        printf("2.  JOGAR PECA\n");
        printf("3.  RESERVAR UMA PECA\n");
        printf("4.  USAR PECA RESERVADA\n");
        printf("5.  TROCAR PECA\n");
        printf("6.  TROCAR MULTIPLAS PECAS\n");
        printf("0.  SAIR\n");
        printf("\nEscolha o que deseja fazer: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao){

            // Inserir nova peça na fila
            case 1:
                printf("Digite o tipo da peca escolhida[I, O, T, L]: ");
                fgets(tipo_peca, MAX_STR_LEN, stdin);
                tipo_peca[strcspn(tipo_peca, "\n")] = '\0'; // Remove o '\n'

                if (strcmp(tipo_peca, "I") == 0 || strcmp(tipo_peca, "O") == 0 ||
                    strcmp(tipo_peca, "T") == 0 || strcmp(tipo_peca, "L") == 0){
                    Peca pecaescolhida;
                    strncpy(pecaescolhida.tipo, tipo_peca, sizeof(pecaescolhida.tipo));
                    pecaescolhida.tipo[sizeof(pecaescolhida.tipo) - 1] = '\0';
                    pecaescolhida.id = id_unico;
                    
                    inserir(&f, pecaescolhida, &id_unico);
                    printf("\n\n");
                    mostrarFila(&f);
                    limparBufferEntrada();
                    printf("Aperte espaco para continuar...\n");
                    getchar();
                    break;
                }
                printf("O tipo nao e correto. Por favor, digite um tipo valido!\n");
                break;

            // Remover (jogar fora) a primeira peça da fila
            case 2: {
                Peca removida;
                remover(&f, &removida);
                printf("A peca: %s foi removida com sucesso\n", removida.tipo);
                mostrarFila(&f);
                printf("Aperte espaco para continuar...\n");
                getchar();
                break;
            }

            // Reservar a primeira peça da fila (enviar para pilha)
            case 3: {
                Peca reserva;
                inserirReserva(&f, &pil, &reserva);
                printf("A peca [%s %d] foi para a reserva.\n\n", reserva.tipo, reserva.id);
                mostrarPilha(&pil);
                mostrarFila(&f);
                printf("Aperte espaco para continuar...\n");
                getchar();
                break;
            }
            
            // Usar uma peça da reserva (remover do topo da pilha)
            case 4:{
                Peca reservausada;
                usarReserva(&pil, &reservausada);
                printf("A peca reserva [%s %d] foi usada!\n", reservausada.tipo, reservausada.id);
                mostrarPilha(&pil);
                printf("Aperte espaco para continuar...\n");
                getchar();
                break;
            }

            // Trocar 1 peça entre pilha e fila
            case 5:{
                Peca trocada;
                trocarPeca(&pil, &f);
                mostrarPilha(&pil);
                mostrarFila(&f);
                printf("Aperte espaco para continuar...\n");
                getchar();
                break;
            }

            // Trocar 3 peças entre pilha e fila
            case 6:{
                trocarMultiplasPecas(&pil, &f);
                mostrarPilha(&pil);
                mostrarFila(&f);
                printf("Aperte espaco para continuar...\n");
                getchar();
                break;
            }

            // Encerrar o programa
            case 0:
                printf("Saindo...\n");
                break;
        
            // Opção inválida
            default:
                printf("Opcao invalida. Tente novamente.\n");
                printf("Aperte ESPAO para continuar...");
                getchar();
        }
    } while(opcao != 0); // Continua até o usuário escolher sair
    

    return 0;
}
