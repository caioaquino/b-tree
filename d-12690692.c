/* ######################################################## */
/*      ALUNO: Caio Luan Aquino Nogueira                    */
/*      N. USP: 12690692                                    */
/* ######################################################## */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define t 3 // ORDEM DA ARVORE B

#define VALOR_INVALIDO -2147483648

typedef struct ArvoreB
{
    int chaves[2 * t - 1];
    struct ArvoreB *filhos[t * 2];
    int qtdChaves;
    bool folha;
} ArvoreB;

void inicializarArvore(ArvoreB **x);
void BTreeInsert(ArvoreB **x, int k);
void BTreeSplitChild(ArvoreB *x, int i, ArvoreB *y);
void BTreeInsertNonFull(ArvoreB *x, int k);
void imprimirArvore(ArvoreB *x);
ArvoreB *BTreeSearch(ArvoreB *x, int k);
void BTreeDelete(ArvoreB **x, int k);
void BTreeDeleteKey(ArvoreB **raiz, ArvoreB *node, int key);
int BtreeFindKey(ArvoreB *x, int k);
void finalizaArvore(ArvoreB *x, FILE *saida);

void inicializarArvore(ArvoreB **x)
{ // INICIALIZA A ARVORE B
    ArvoreB *no = malloc(sizeof(ArvoreB));
    no->qtdChaves = 0;
    no->folha = true;
    *x = no;
}

void BTreeInsert(ArvoreB **x, int k)
{ // INSERE UMA CHAVE NA ARVORE B

    ArvoreB *r = (*x);

    if (r->qtdChaves == 2 * t - 1)
    {
        ArvoreB *s = (ArvoreB *)malloc(sizeof(ArvoreB));
        *x = s;
        s->qtdChaves = 0;
        s->folha = false;
        s->filhos[0] = r;
        BTreeSplitChild(s, 0, r);
        BTreeInsertNonFull(s, k);
    }
    else
    {
        BTreeInsertNonFull(r, k);
    }
}

void BTreeSplitChild(ArvoreB *x, int i, ArvoreB *y)
{                                                    //  DIVIDE UM NO DA ARVORE B
    ArvoreB *z = (ArvoreB *)malloc(sizeof(ArvoreB)); // aloca no
    z->folha = y->folha;
    z->qtdChaves = t - 1;
    int j;
    for (j = 0; j < t - 1; j++)
    {
        z->chaves[j] = y->chaves[j + t];
    }
    if (!y->folha)
    {
        for (j = 0; j < t; j++)
        {
            z->filhos[j] = y->filhos[j + t];
        }
    }
    y->qtdChaves = t - 1;
    for (j = x->qtdChaves; j >= i + 1; i--)
    {
        x->filhos[j + 1] = x->filhos[j];
    }
    x->filhos[i + 1] = z;
    for (j = x->qtdChaves - 1; j >= i; j--)
    {
        x->chaves[j + 1] = x->chaves[j];
    }
    x->chaves[i] = y->chaves[t - 1];
    x->qtdChaves++;
}

void BTreeInsertNonFull(ArvoreB *x, int k)
{ // INSERE EM UM NÓ COM ESPAÇOS DISPONÍVEIS
    int i = x->qtdChaves - 1;
    if (x->folha)
    {

        while (i >= 0 && k < x->chaves[i])
        {
            x->chaves[i + 1] = x->chaves[i];
            i--;
        }
        x->chaves[i + 1] = k;
        x->qtdChaves++;
    }
    else
    {
        while (i >= 0 && k < x->chaves[i])
        {
            i--;
        }
        i++;

        if (x->filhos[i]->qtdChaves == 2 * t - 1)
        {
            BTreeSplitChild(x, i, x->filhos[i]);
            if (k > x->chaves[i])
            {
                i++;
            }
        }
        BTreeInsertNonFull(x->filhos[i], k);
    }
}

void imprimirArvore(ArvoreB *x)
{ //    IMPRIME A ARVORE B
    int i;
    if (x)
    {
        for (i = 0; i < x->qtdChaves; i++)
        {
            printf("%d\n", x->chaves[i]);
        }
        if (!x->folha)
        {
            for (i = 0; i < x->qtdChaves + 1; i++)
            {
                imprimirArvore(x->filhos[i]);
            }
        }
    }
}

void finalizaArvore(ArvoreB *x, FILE *saida)
{ // ESCREVE A ARVORE NO ARQUIVO DE SAIDA
    int i;
    if (x)
    {
        for (i = 0; i < x->qtdChaves; i++)
        {
            fprintf(saida, "%i\n", x->chaves[i]);
        }
        if (!x->folha)
        {
            for (i = 0; i < x->qtdChaves + 1; i++)
            {
                finalizaArvore(x->filhos[i], saida);
            }
        }
    }
}

ArvoreB *BTreeSearch(ArvoreB *x, int k)
{ // PROCURA POR UMA CHAVE NA ARVORE B
    if (!x)
        return NULL;
    int i = 0;
    while (i < x->qtdChaves)
    {
        if (k < x->chaves[i])
            break;
        if (k == x->chaves[i])
            return x;
        i++;
    }

    if (x->folha)
        return NULL;
    else
        return BTreeSearch(x->filhos[i], k);
}

void BTreeDeleteKey(ArvoreB **raiz, ArvoreB *node, int key)
{ // DELETA UMA CHAVE DA ARVORE B
    int position = BtreeFindKey(node, key);
    if (position != VALOR_INVALIDO)
    {
        if (node->folha)
        {
            int itr = 0;
            for (itr = 0; itr < node->qtdChaves && node->chaves[itr] != key; itr++)
            {
            };
            for (; itr < node->qtdChaves; itr++)
            {
                if (itr != (2 * t) - 2)
                {
                    node->chaves[itr] = node->chaves[itr + 1];
                }
            }
            node->qtdChaves--;
            return;
        }
        if (!node->folha)
        {

            ArvoreB *prd = node->filhos[position];
            int predKey = 0;
            if (prd->qtdChaves >= t)
            {
                for (;;)
                {
                    if (prd->folha)
                    {
                        // System.out.println(prd->qtdChaves);
                        predKey = prd->chaves[prd->qtdChaves - 1];
                        break;
                    }
                    else
                    {
                        prd = prd->filhos[prd->qtdChaves];
                    }
                }
                BTreeDeleteKey(raiz, prd, predKey);
                node->chaves[position] = predKey;
                return;
            }

            ArvoreB *next_node = node->filhos[position + 1];
            if (next_node->qtdChaves >= t)
            {
                int nextKey = next_node->chaves[0];
                if (!next_node->folha)
                {
                    next_node = next_node->filhos[0];
                    for (;;)
                    {
                        if (next_node->folha)
                        {
                            nextKey = next_node->chaves[next_node->qtdChaves - 1];
                            break;
                        }
                        else
                        {
                            next_node = next_node->filhos[next_node->qtdChaves];
                        }
                    }
                }
                BTreeDeleteKey(raiz, next_node, nextKey);
                node->chaves[position] = nextKey;
                return;
            }

            int tmp = prd->qtdChaves + 1;
            prd->chaves[prd->qtdChaves++] = node->chaves[position];
            for (int itr = 0, jtr = prd->qtdChaves; itr < next_node->qtdChaves; itr++)
            {
                prd->chaves[jtr++] = next_node->chaves[itr];
                prd->qtdChaves++;
            }
            for (int itr = 0; itr < next_node->qtdChaves + 1; itr++)
            {
                prd->filhos[tmp++] = next_node->filhos[itr];
            }

            node->filhos[position] = prd;
            for (int itr = position; itr < node->qtdChaves; itr++)
            {
                if (itr != 2 * t - 2)
                {
                    node->chaves[itr] = node->chaves[itr + 1];
                }
            }
            for (int itr = position + 1; itr < node->qtdChaves + 1; itr++)
            {
                if (itr != 2 * t - 1)
                {
                    node->filhos[itr] = node->filhos[itr + 1];
                }
            }
            node->qtdChaves--;
            if (node->qtdChaves == 0)
            {
                if (node == (*raiz))
                {
                    *raiz = node->filhos[0];
                }
                node = node->filhos[0];
            }
            BTreeDeleteKey(raiz, prd, key);
            return;
        }
    }
    else
    {
        for (position = 0; position < node->qtdChaves; position++)
        {
            if (node->chaves[position] > key)
            {
                break;
            }
        }
        ArvoreB *tmp = node->filhos[position];
        if (tmp->qtdChaves >= t)
        {
            BTreeDeleteKey(raiz, tmp, key);
            return;
        }
        if (true)
        {
            ArvoreB *nd = NULL;
            int divider = -1;

            if (position != node->qtdChaves && node->filhos[position + 1]->qtdChaves >= t)
            {
                divider = node->chaves[position];
                nd = node->filhos[position + 1];
                node->chaves[position] = nd->chaves[0];
                tmp->chaves[tmp->qtdChaves++] = divider;
                tmp->filhos[tmp->qtdChaves] = nd->filhos[0];
                for (int itr = 1; itr < nd->qtdChaves; itr++)
                {
                    nd->chaves[itr - 1] = nd->chaves[itr];
                }
                for (int itr = 1; itr <= nd->qtdChaves; itr++)
                {
                    nd->filhos[itr - 1] = nd->filhos[itr];
                }
                nd->qtdChaves--;
                BTreeDeleteKey(raiz, tmp, key);
                return;
            }
            else if (position != 0 && node->filhos[position - 1]->qtdChaves >= t)
            {

                divider = node->chaves[position - 1];
                nd = node->filhos[position - 1];
                node->chaves[position - 1] = nd->chaves[nd->qtdChaves - 1];
                ArvoreB *child = nd->filhos[nd->qtdChaves];
                nd->qtdChaves--;

                for (int itr = tmp->qtdChaves; itr > 0; itr--)
                {
                    tmp->chaves[itr] = tmp->chaves[itr - 1];
                }
                tmp->chaves[0] = divider;
                for (int itr = tmp->qtdChaves + 1; itr > 0; itr--)
                {
                    tmp->filhos[itr] = tmp->filhos[itr - 1];
                }
                tmp->filhos[0] = child;
                tmp->qtdChaves++;
                BTreeDeleteKey(raiz, tmp, key);
                return;
            }
            else
            {
                ArvoreB *left = NULL;
                ArvoreB *right = NULL;
                bool is_last = false;
                if (position != node->qtdChaves)
                {
                    divider = node->chaves[position];
                    left = node->filhos[position];
                    right = node->filhos[position + 1];
                }
                else
                {
                    divider = node->chaves[position - 1];
                    right = node->filhos[position];
                    left = node->filhos[position - 1];
                    is_last = true;
                    position--;
                }
                for (int itr = position; itr < node->qtdChaves - 1; itr++)
                {
                    node->chaves[itr] = node->chaves[itr + 1];
                }
                for (int itr = position + 1; itr < node->qtdChaves; itr++)
                {
                    node->filhos[itr] = node->filhos[itr + 1];
                }
                node->qtdChaves--;
                left->chaves[left->qtdChaves++] = divider;

                for (int itr = 0, jtr = left->qtdChaves; itr < right->qtdChaves + 1; itr++, jtr++)
                {
                    if (itr < right->qtdChaves)
                    {
                        left->chaves[jtr] = right->chaves[itr];
                    }
                    left->filhos[jtr] = right->filhos[itr];
                }
                left->qtdChaves += right->qtdChaves;
                if (node->qtdChaves == 0)
                {
                    if (node == (*raiz))
                    {
                        *raiz = node->filhos[0];
                    }
                    node = node->filhos[0];
                }
                BTreeDeleteKey(raiz, left, key);
                return;
            }
        }
    }
}

void BTreeDelete(ArvoreB **x, int k)
{ // SOLICITA QUE UMA CHAVE SEJA DELETADA DA ARVORE B
    ArvoreB *s = BTreeSearch((*x), k);
    if (s)
        BTreeDeleteKey(x, s, k);
}

int BtreeFindKey(ArvoreB *x, int k)
{ // ENCONTRA A POSICAO DO NÓ NA ARVORE
    int j;
    for (j = 0; j < x->qtdChaves; j++)
    {
        if (x->chaves[j] == k)
        {
            return j;
        }
    }
    return VALOR_INVALIDO;
}
#define MAX_LINE_LENGTH 1000

int main(int argc, char *argv[])
{
    // Recebe os parâmeros de entrada e saída de arquivos
    FILE *entrada, *saida;
    entrada = fopen(argv[1], "rt");
    saida = fopen(argv[2], "w");
    char input[100];
    char operacao[100];
    int chave;

    ArvoreB *raiz;
    inicializarArvore(&raiz);

    // LOGICA PARA LEITURA DO ARQUIVO DE TEXTO
    while (true)
    {
        if (fgets(input, sizeof input, entrada) != NULL)
        {
            sscanf(input, "%99s%d", operacao, &chave);
            if (strcmp(operacao, "insere") == 0)
            {
                BTreeInsert(&raiz, chave);
            }
            else if (strcmp(operacao, "remove") == 0)
            {
                BTreeDelete(&raiz, chave);
            }
            else if (strcmp(operacao, "imprime") == 0)
            {
                imprimirArvore(raiz);
            }
            else if (strcmp(operacao, "fim") == 0)
            {
                finalizaArvore(raiz, saida);
                break;
            }
        }
    }
    return 0;
}