
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// =====================================================
// Estrutura dos timestamps
// =====================================================

// Data e hora em que a imagem foi capturada.
typedef struct {
    char valor[20];
} Timestamp;

// Compara as datas.
// Nesse formato de data e hora,
// comparar as strings ja funciona na ordem certa.
int comparar(Timestamp a, Timestamp b) {
    return strcmp(a.valor, b.valor);
}


// =====================================================
// Lista encadeada sem ordenacao
// =====================================================

// Cada no tem uma data e o endereco do proximo.
typedef struct No {
    Timestamp dado;
    struct No *proximo;
} No;

// Aqui ficam o inicio da lista e o total de itens.
typedef struct {
    No *inicio;
    int quantidade;
} Lista;

// Comeca com a lista vazia.
void iniciarLista(Lista *lista) {
    lista->inicio = NULL;
    lista->quantidade = 0;
}

// Adiciona o novo item no comeco.
void inserirLista(Lista *lista, Timestamp dado) {

    No *novo = malloc(sizeof(No));

    if (novo == NULL) return;

    novo->dado = dado;
    novo->proximo = lista->inicio;

    lista->inicio = novo;
    lista->quantidade++;
}

// Busca o valor e tira ele da lista.
int removerLista(Lista *lista, Timestamp dado) {

    No *atual = lista->inicio;
    No *anterior = NULL;

    while (atual != NULL) {

        if (comparar(atual->dado, dado) == 0) {

            if (anterior == NULL) {
                lista->inicio = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }

            free(atual);
            lista->quantidade--;

            return 1;
        }

        anterior = atual;
        atual = atual->proximo;
    }

    return 0;
}

// Para mostrar em ordem, primeiro passo tudo para um vetor.
// Depois ordeno esse vetor.
void listarLista(Lista *lista) {

    if (lista->quantidade == 0) {
        printf("Lista vazia.\n");
        return;
    }

    Timestamp *vetor = malloc(lista->quantidade * sizeof(Timestamp));

    if (vetor == NULL) return;

    No *atual = lista->inicio;
    int i = 0;

    // Passa os valores da lista para o vetor.
    while (atual != NULL) {
        vetor[i] = atual->dado;
        atual = atual->proximo;
        i++;
    }

    // Ordena usando insercao.
    for (int j = 1; j < lista->quantidade; j++) {

        Timestamp chave = vetor[j];
        int k = j - 1;

        while (k >= 0 && comparar(vetor[k], chave) > 0) {
            vetor[k + 1] = vetor[k];
            k--;
        }

        vetor[k + 1] = chave;
    }

    // Mostra os valores ja ordenados.
    for (int j = 0; j < lista->quantidade; j++) {
        printf("%s\n", vetor[j].valor);
    }

    free(vetor);
}

 // Junta as duas metades ordenadas.
void intercalar(Timestamp vetor[], int inicio, int meio, int fim) {

    int tamanho = fim - inicio + 1;

    Timestamp *auxiliar = malloc(tamanho * sizeof(Timestamp));

    if (auxiliar == NULL) return;

    int i = inicio;
    int j = meio + 1;
    int k = 0;

    // Vai pegando o menor valor de cada metade.
    while (i <= meio && j <= fim) {

        if (comparar(vetor[i], vetor[j]) <= 0) {
            auxiliar[k] = vetor[i];
            i++;
        } else {
            auxiliar[k] = vetor[j];
            j++;
        }

        k++;
    }

    // Coloca o que faltou da primeira metade.
    while (i <= meio) {
        auxiliar[k] = vetor[i];
        i++;
        k++;
    }

    // Coloca o restante da segunda metade.
    while (j <= fim) {
        auxiliar[k] = vetor[j];
        j++;
        k++;
    }

    // Passa o resultado de volta para o vetor.
    for (i = 0; i < tamanho; i++) {
        vetor[inicio + i] = auxiliar[i];
    }

    free(auxiliar);
}

 // Separa o vetor para fazer o merge sort.
void mergeSort(Timestamp vetor[], int inicio, int fim) {

    // Com um item so nao precisa ordenar.
    if (inicio >= fim) {
        return;
    }

    // Calcula onde fica o meio.
    int meio = (inicio + fim) / 2;

    // Ordena o lado esquerdo.
    mergeSort(vetor, inicio, meio);

    // Ordena o lado direito.
    mergeSort(vetor, meio + 1, fim);

    // Junta tudo de novo.
    intercalar(vetor, inicio, meio, fim);
}

// Faz a ordenacao sem mostrar os dados na tela.
int ordenarLista(Lista *lista) {

    if (lista->quantidade == 0) return 0;

    Timestamp *vetor = malloc(lista->quantidade * sizeof(Timestamp));

    if (vetor == NULL) return -1;

    No *atual = lista->inicio;
    int i = 0;

    // Copia a lista para um vetor.
    while (atual != NULL && i < lista->quantidade) {

        vetor[i] = atual->dado;
        atual = atual->proximo;
        i++;
    }

    // Aplica merge sort no vetor.
    mergeSort(vetor, 0, lista->quantidade - 1); 

    int processados = i;
    
    free(vetor);

    return processados;
}

// Limpa a memoria usada pela lista.
void liberarLista(Lista *lista) {

    No *atual = lista->inicio;

    while (atual != NULL) {
        No *temporario = atual;
        atual = atual->proximo;
        free(temporario);
    }

    lista->inicio = NULL;
    lista->quantidade = 0;
}


// =====================================================
// Lista encadeada ordenada
// =====================================================

// Usa o mesmo tipo de no da outra lista.
typedef struct NoOrdenado {
    Timestamp dado;
    struct NoOrdenado *proximo;
} NoOrdenado;

typedef struct {
    NoOrdenado *inicio;
    int quantidade;
} ListaOrdenada;

// Deixa a lista pronta para uso.
void iniciarOrdenada(ListaOrdenada *lista) {
    lista->inicio = NULL;
    lista->quantidade = 0;
}

// Insere o valor mantendo a ordem.
void inserirOrdenado(ListaOrdenada *lista, Timestamp dado) {

    NoOrdenado *novo = malloc(sizeof(NoOrdenado));

    if (novo == NULL) return;

    novo->dado = dado;

    // Se nao tiver nada ou o valor vier antes do primeiro,
    // ele entra logo no comeco.
    if (lista->inicio == NULL ||
        comparar(dado, lista->inicio->dado) < 0) {

        novo->proximo = lista->inicio;
        lista->inicio = novo;

    } else {

        NoOrdenado *atual = lista->inicio;

        // Anda pela lista ate achar onde inserir.
        while (atual->proximo != NULL &&
               comparar(atual->proximo->dado, dado) <= 0) {

            atual = atual->proximo;
        }

        novo->proximo = atual->proximo;
        atual->proximo = novo;
    }

    lista->quantidade++;
}

// Procura e remove o valor.
int removerOrdenado(ListaOrdenada *lista, Timestamp dado) {

    NoOrdenado *atual = lista->inicio;
    NoOrdenado *anterior = NULL;

    while (atual != NULL) {

        if (comparar(atual->dado, dado) == 0) {

            if (anterior == NULL) {
                lista->inicio = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }

            free(atual);
            lista->quantidade--;

            return 1;
        }

        anterior = atual;
        atual = atual->proximo;
    }

    return 0;
}

// Aqui ja esta tudo em ordem, entao e so percorrer.
void listarOrdenado(ListaOrdenada *lista) {

    NoOrdenado *atual = lista->inicio;

    if (atual == NULL) {
        printf("Lista vazia.\n");
        return;
    }

    while (atual != NULL) {
        printf("%s\n", atual->dado.valor);
        atual = atual->proximo;
    }
}

// Percorre sem mostrar os valores.
int percorrerOrdenadaParaTeste(ListaOrdenada *lista) {

    int quantidade = 0;
    NoOrdenado *atual = lista->inicio;

    while (atual != NULL) {
        quantidade++;
        atual = atual->proximo;
    }

    return quantidade;
}

// Libera os nos da lista.
void liberarOrdenada(ListaOrdenada *lista) {

    NoOrdenado *atual = lista->inicio;

    while (atual != NULL) {
        NoOrdenado *temporario = atual;
        atual = atual->proximo;
        free(temporario);
    }

    lista->inicio = NULL;
    lista->quantidade = 0;
}


// =====================================================
// Arvore binaria de busca
// =====================================================

// Cada no guarda uma data e aponta para dois filhos.
typedef struct NoArvore {
    Timestamp dado;
    struct NoArvore *esquerda;
    struct NoArvore *direita;
} NoArvore;

// Cria um no novo.
NoArvore *criarNo(Timestamp dado) {

    NoArvore *novo = malloc(sizeof(NoArvore));

    if (novo == NULL) return NULL;

    novo->dado = dado;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

// Coloca o valor na arvore.
// Menores vao para a esquerda, os outros para a direita.
NoArvore *inserirArvore(NoArvore *raiz, Timestamp dado) {

    if (raiz == NULL) {
        return criarNo(dado);
    }

    if (comparar(dado, raiz->dado) < 0) {
        raiz->esquerda = inserirArvore(raiz->esquerda, dado);
    } else {
        raiz->direita = inserirArvore(raiz->direita, dado);
    }

    return raiz;
}

// Busca o menor valor desse trecho da arvore.
NoArvore *menorNo(NoArvore *raiz) {

    while (raiz != NULL && raiz->esquerda != NULL) {
        raiz = raiz->esquerda;
    }

    return raiz;
}

// Procura o valor para remover.
NoArvore *removerArvore(NoArvore *raiz, Timestamp dado) {

    if (raiz == NULL) {
        return NULL;
    }

    if (comparar(dado, raiz->dado) < 0) {

        raiz->esquerda = removerArvore(raiz->esquerda, dado);

    } else if (comparar(dado, raiz->dado) > 0) {

        raiz->direita = removerArvore(raiz->direita, dado);

    } else {

        // Sem filho esquerdo: usa o filho da direita.
        if (raiz->esquerda == NULL) {
            NoArvore *temporario = raiz->direita;
            free(raiz);
            return temporario;
        }

        // Sem filho direito: usa o da esquerda.
        if (raiz->direita == NULL) {
            NoArvore *temporario = raiz->esquerda;
            free(raiz);
            return temporario;
        }

        // Com dois filhos, precisa trocar o valor.
        // Pega o menor do lado direito.
        NoArvore *temporario = menorNo(raiz->direita);

        raiz->dado = temporario->dado;

        raiz->direita = removerArvore(raiz->direita, temporario->dado);
    }

    return raiz;
}

// Percurso em ordem:
// primeiro esquerda, depois o no e por ultimo direita.
// Assim os valores saem ordenados.
void listarArvore(NoArvore *raiz) {

    if (raiz != NULL) {

        listarArvore(raiz->esquerda);

        printf("%s\n", raiz->dado.valor);

        listarArvore(raiz->direita);
    }
}

// Faz o percurso sem imprimir nada.
int percorrerArvoreParaTeste(NoArvore *raiz) {

    if (raiz == NULL) {
        return 0;
    }

    int quantidade = 0;

    // Passa pelo lado esquerdo.
    quantidade += percorrerArvoreParaTeste(raiz->esquerda);

    // Conta este no.
    quantidade++;

    // Depois passa pelo lado direito.
    quantidade += percorrerArvoreParaTeste(raiz->direita);

    return quantidade;
}

// Libera os nos da arvore.
void liberarArvore(NoArvore *raiz) {

    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}


// =====================================================
// Testes das estruturas
// =====================================================


// =====================================================
// Geracao dos dados de teste
// =====================================================

// Sorteia uma data e um horario.
void gerarTimestampAleatorio(Timestamp *t) {

    int ano = 2020 + rand() % 7;
    int mes = 1 + rand() % 12;
    int dia = 1 + rand() % 28;

    int hora = rand() % 24;
    int minuto = rand() % 60;
    int segundo = rand() % 60;

    // Monta a data no formato pedido.
    snprintf(t->valor, sizeof(t->valor),
             "%04d-%02d-%02d %02d:%02d:%02d",
             ano, mes, dia, hora, minuto, segundo);
}

// Grava os valores gerados em um TXT.
void gerarArquivoDados(int quantidade, const char *nomeArquivo) {

    FILE *arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo %s\n", nomeArquivo);
        return;
    }

    for (int i = 0; i < quantidade; i++) {

        Timestamp t;

        gerarTimestampAleatorio(&t);

        fprintf(arquivo, "%s\n", t.valor);
    }

    fclose(arquivo);

    printf("Arquivo gerado: %s (%d elementos)\n",
           nomeArquivo, quantidade);
}

// Prepara os cinco arquivos de teste.
void gerarTodosArquivos() {

    int tamanhos[] = {1000, 5000, 10000, 50000, 100000};

    char nomeArquivo[100];

    for (int i = 0; i < 5; i++) {

        snprintf(nomeArquivo, sizeof(nomeArquivo),
                 "dados/timestamps_%d.txt", tamanhos[i]);

        gerarArquivoDados(tamanhos[i], nomeArquivo);
    }
}


// =====================================================
// Leitura dos arquivos
// =====================================================


 // Le o arquivo e coloca os valores no vetor.
int carregarArquivo(const char *nomeArquivo,
                    Timestamp dados[], int capacidade) {

    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir: %s\n", nomeArquivo);
        return -1;
    }

    char linha[100];
    int quantidade = 0;

    // Pega uma linha por vez.
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        // Tira o enter do final.
        linha[strcspn(linha, "\r\n")] = '\0';

        // Verifica o tamanho da data.
        if (strlen(linha) != 19) {
            continue;
        }

        // Nao deixa passar do limite do vetor.
        if (quantidade >= capacidade) {
            break;
        }

        strcpy(dados[quantidade].valor, linha);

        quantidade++;
    }

    fclose(arquivo);

    return quantidade;
}


// =====================================================
// Estatisticas dos tempos
// =====================================================

// Pega o menor, a mediana e o maior tempo.
double calcularEstatisticas(double tempos[], int quantidade) {

    // Coloca os tempos em ordem crescente.
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = i + 1; j < quantidade; j++) {

            if (tempos[i] > tempos[j]) {
                double auxiliar = tempos[i];
                tempos[i] = tempos[j];
                tempos[j] = auxiliar;
            }
        }
    }

    // Acha a mediana.
    double mediana = (tempos[4] + tempos[5]) / 2.0;

    printf("Minimo: %.6f segundos\n", tempos[0]);
    printf("Mediana: %.6f segundos\n", mediana);
    printf("Maximo: %.6f segundos\n", tempos[9]);

    return mediana;
}


int main() {

    // Abre o CSV dos resultados.
    FILE *arquivoResultados = fopen("resultados/resultados.csv", "w");

    if (arquivoResultados == NULL) {
        printf("Erro ao criar o arquivo de resultados.\n");
        return 1;
    }

    // Escreve a primeira linha do CSV.
    fprintf(arquivoResultados,
        "Quantidade,Insercao Lista,Remocao Lista,Listagem Lista,"
        "Insercao Ordenada,Remocao Ordenada,Listagem Ordenada,"
        "Insercao Arvore,Remocao Arvore,Listagem Arvore\n");

    // Quantidades usadas nos testes.
    int tamanhos[] = {1000, 5000, 10000, 50000, 100000};

    // Repete o teste para cada tamanho.
    for (int i = 0; i < 5; i++) {

        char nomeArquivo[100];

        snprintf(nomeArquivo, sizeof(nomeArquivo),
                 "dados/timestamps_%d.txt", tamanhos[i]);

        // Separa espaco para os dados.
        Timestamp *dados = malloc(tamanhos[i] * sizeof(Timestamp));

        if (dados == NULL) {
            printf("Erro ao reservar memoria.\n");
            return 1;
        }

        // Carrega os valores do TXT.
        int quantidade = carregarArquivo(
            nomeArquivo, dados, tamanhos[i]
        );

        if (quantidade == -1) {
            free(dados);
            return 1;
        }

        // Tempos das 10 insercoes.
        double temposLista[10];
        double temposOrdenada[10];
        double temposArvore[10];

        // Tempos das remocoes.
        double temposRemocaoLista[10];
        double temposRemocaoOrdenada[10];
        double temposRemocaoArvore[10];

        // Tempos das listagens.
        double temposListagemLista[10];
        double temposListagemOrdenada[10];
        double temposListagemArvore[10];

        // Prepara as tres estruturas.
        Lista lista;
        ListaOrdenada ordenada;
        NoArvore *arvore = NULL;

  
        printf("\n===== TESTE COM %d ELEMENTOS =====\n", tamanhos[i]);

        // Repete a insercao na lista comum 10 vezes.
        for (int repeticao = 0; repeticao < 10; repeticao++) {

            iniciarLista(&lista);

            clock_t inicio = clock();

            for (int j = 0; j < quantidade; j++) {
                inserirLista(&lista, dados[j]);
            }

            clock_t fim = clock();

            temposLista[repeticao] =
                (double)(fim - inicio) / CLOCKS_PER_SEC;

            liberarLista(&lista);
        }

        // Repete a insercao na lista ordenada 10 vezes.
        for (int repeticao = 0; repeticao < 10; repeticao++) {

            iniciarOrdenada(&ordenada);

            clock_t inicio = clock();

            for (int j = 0; j < quantidade; j++) {
                inserirOrdenado(&ordenada, dados[j]);
            }

            clock_t fim = clock();

            temposOrdenada[repeticao] =
                (double)(fim - inicio) / CLOCKS_PER_SEC;

            liberarOrdenada(&ordenada);
        }

        // Repete a insercao na arvore 10 vezes.
        for (int repeticao = 0; repeticao < 10; repeticao++) {

            arvore = NULL;

            clock_t inicio = clock();

            for (int j = 0; j < quantidade; j++) {
                arvore = inserirArvore(arvore, dados[j]);
            }

            clock_t fim = clock();

            temposArvore[repeticao] =
                (double)(fim - inicio) / CLOCKS_PER_SEC;

            liberarArvore(arvore);
            
            if (repeticao == 9) {
                printf ("Teste da Arvore Finalizado\n" );
            }
        }

        // LISTAGEM DA LISTA COMUM
        for (int repeticao = 0; repeticao < 10; repeticao++) {

            Lista lista;
            iniciarLista(&lista);

            // Preenche a lista antes de contar o tempo.
            for (int j = 0; j < quantidade; j++) {
                inserirLista(&lista, dados[j]);
            }

            clock_t inicio = clock();

            int processados = ordenarLista(&lista);

            if (processados != quantidade) {
                printf("Erro na listagem da lista nao ordenada!\n");
            }

            clock_t fim = clock();

            temposListagemLista[repeticao] =
                (double)(fim - inicio) / CLOCKS_PER_SEC;

            liberarLista(&lista);
        }

        // Repete a remocao na lista comum 10 vezes.
        for (int repeticao = 0; repeticao < 10; repeticao++) {

            Lista lista;
            iniciarLista(&lista);

            // Preenche a lista antes do teste.
            for (int j = 0; j < quantidade; j++) {
                inserirLista(&lista, dados[j]);
            }

            clock_t inicio = clock();

            // Agora remove os itens.
            for (int j = 0; j < quantidade; j++) {
                removerLista(&lista, dados[j]);
            }

            clock_t fim = clock();

            temposRemocaoLista[repeticao] =
                (double)(fim - inicio) / CLOCKS_PER_SEC;

            liberarLista(&lista);
        }

        // LISTAGEM DA LISTA ORDENADA
                for (int repeticao = 0; repeticao < 10; repeticao++) {

            ListaOrdenada lista;
            iniciarOrdenada(&lista);

            // Preenche a lista antes do teste.
            for (int j = 0; j < quantidade; j++) {
                inserirOrdenado(&lista, dados[j]);
            }

            clock_t inicio = clock();

            int processados = 0;

            // Repete o percurso 1000 vezes para medir melhor.
            for (int lote = 0; lote < 1000; lote++) {
                processados = percorrerOrdenadaParaTeste(&lista);
            }

            clock_t fim = clock();

            // Tira a media por percurso.
            temposListagemOrdenada[repeticao] =
                ((double)(fim - inicio) / CLOCKS_PER_SEC) / 1000.0;

            if (processados != quantidade) {
                printf("Erro na listagem da lista ordenada!\n");
            }

            liberarOrdenada(&lista);
        }

        // LISTAGEM DA ARVORE
            for (int repeticao = 0; repeticao < 10; repeticao++) {

            NoArvore *raiz = NULL;

            // Monta a arvore antes do teste.
            for (int j = 0; j < quantidade; j++) {
                raiz = inserirArvore(raiz, dados[j]);
            }

            clock_t inicio = clock();

            int processados = 0;

            // Repete o percurso 1000 vezes para medir melhor.
            for (int lote = 0; lote < 1000; lote++) {
                processados = percorrerArvoreParaTeste(raiz);
            }

            clock_t fim = clock();

            // Tira a media por percurso.
            temposListagemArvore[repeticao] =
                ((double)(fim - inicio) / CLOCKS_PER_SEC) / 1000.0;

            if (processados != quantidade) {
                printf("Erro na listagem da arvore!\n");
            }

            liberarArvore(raiz);
        }
        

        // Repete a remocao na lista ordenada 10 vezes.
        for (int repeticao = 0; repeticao < 10; repeticao++) {

            iniciarOrdenada(&ordenada);

            // Preenche tudo antes de marcar o tempo.
            for (int j = 0; j < quantidade; j++) {
                inserirOrdenado(&ordenada, dados[j]);
            }

            // Conta so o tempo de remover.
            clock_t inicio = clock();

            for (int j = 0; j < quantidade; j++) {
                removerOrdenado(&ordenada, dados[j]);
            }

            clock_t fim = clock();

            temposRemocaoOrdenada[repeticao] =
                (double)(fim - inicio) / CLOCKS_PER_SEC;

            liberarOrdenada(&ordenada);
        }

        // Repete a remocao na arvore 10 vezes.
        for (int repeticao = 0; repeticao < 10; repeticao++) {

            arvore = NULL;

            // Monta a arvore antes de marcar o tempo.
            for (int j = 0; j < quantidade; j++) {
                arvore = inserirArvore(arvore, dados[j]);
            }

            // Conta so o tempo de remover.
            clock_t inicio = clock();

            for (int j = 0; j < quantidade; j++) {
                arvore = removerArvore(arvore, dados[j]);
            }

            clock_t fim = clock();

            temposRemocaoArvore[repeticao] =
                (double)(fim - inicio) / CLOCKS_PER_SEC;

            liberarArvore(arvore);
        }

        
        

    // Mostra os tempos e separa as medianas.
        double medianas[9];

        printf("\n--- INSERCAO: LISTA NAO ORDENADA ---\n");
        medianas[0] = calcularEstatisticas(temposLista, 10);

        printf("\n--- REMOCAO: LISTA NAO ORDENADA ---\n");
        medianas[1] = calcularEstatisticas(temposRemocaoLista, 10);

        printf("\n--- LISTAGEM: LISTA NAO ORDENADA ---\n");
        medianas[2] = calcularEstatisticas(temposListagemLista, 10);


        printf("\n--- INSERCAO: LISTA ORDENADA ---\n");
        medianas[3] = calcularEstatisticas(temposOrdenada, 10);

        printf("\n--- REMOCAO: LISTA ORDENADA ---\n");
        medianas[4] = calcularEstatisticas(temposRemocaoOrdenada, 10);

        printf("\n--- LISTAGEM: LISTA ORDENADA ---\n");
        medianas[5] = calcularEstatisticas(temposListagemOrdenada, 10);


        printf("\n--- INSERCAO: ARVORE BINARIA ---\n");
        medianas[6] = calcularEstatisticas(temposArvore, 10);

        printf("\n--- REMOCAO: ARVORE BINARIA ---\n");
        medianas[7] = calcularEstatisticas(temposRemocaoArvore, 10);

        printf("\n--- LISTAGEM: ARVORE BINARIA ---\n");
        medianas[8] = calcularEstatisticas(temposListagemArvore, 10);

        // Grava as medianas no CSV.
        fprintf(arquivoResultados,
            "%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
            quantidade,
            medianas[0], medianas[1], medianas[2],
            medianas[3], medianas[4], medianas[5],
            medianas[6], medianas[7], medianas[8]);

        // Libera o vetor de dados.
        free(dados);
        printf("Memoria dos dados liberada.\n");
    }
    
        // Fecha o CSV.
        fclose(arquivoResultados);
    

    printf("\nArquivo resultados.csv salvo com sucesso!\n");

    return 0;
}
    