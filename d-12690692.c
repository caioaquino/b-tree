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

void BTreeDeleteKey(ArvoreB **raiz, ArvoreB *no, int chave)
{ // DELETA UMA CHAVE DA ARVORE B
    int position = BtreeFindKey(no, chave);
    if (position != VALOR_INVALIDO)
    {
        if (no->folha)
        {
            int contador = 0;
            for (contador = 0; contador < no->qtdChaves && no->chaves[contador] != chave; contador++)
            {
            };
            for (; contador < no->qtdChaves; contador++)
            {
                if (contador != (2 * t) - 2)
                {
                    no->chaves[contador] = no->chaves[contador + 1];
                }
            }
            no->qtdChaves--;
            return;
        }
        if (!no->folha)
        {

            ArvoreB *prd = no->filhos[position];
            int predKey = 0;
            if (prd->qtdChaves >= t)
            {
                for (;;)
                {
                    if (prd->folha)
                    {
                        predKey = prd->chaves[prd->qtdChaves - 1];
                        break;
                    }
                    else
                    {
                        prd = prd->filhos[prd->qtdChaves];
                    }
                }
                BTreeDeleteKey(raiz, prd, predKey);
                no->chaves[position] = predKey;
                return;
            }

            ArvoreB *proximoNo = no->filhos[position + 1];
            if (proximoNo->qtdChaves >= t)
            {
                int proximaChave = proximoNo->chaves[0];
                if (!proximoNo->folha)
                {
                    proximoNo = proximoNo->filhos[0];
                    for (;;)
                    {
                        if (proximoNo->folha)
                        {
                            proximaChave = proximoNo->chaves[proximoNo->qtdChaves - 1];
                            break;
                        }
                        else
                        {
                            proximoNo = proximoNo->filhos[proximoNo->qtdChaves];
                        }
                    }
                }
                BTreeDeleteKey(raiz, proximoNo, proximaChave);
                no->chaves[position] = proximaChave;
                return;
            }

            int temp = prd->qtdChaves + 1;
            prd->chaves[prd->qtdChaves++] = no->chaves[position];
            for (int contador = 0, contadorJ = prd->qtdChaves; contador < proximoNo->qtdChaves; contador++)
            {
                prd->chaves[contadorJ++] = proximoNo->chaves[contador];
                prd->qtdChaves++;
            }
            for (int contador = 0; contador < proximoNo->qtdChaves + 1; contador++)
            {
                prd->filhos[temp++] = proximoNo->filhos[contador];
            }

            no->filhos[position] = prd;
            for (int contador = position; contador < no->qtdChaves; contador++)
            {
                if (contador != 2 * t - 2)
                {
                    no->chaves[contador] = no->chaves[contador + 1];
                }
            }
            for (int contador = position + 1; contador < no->qtdChaves + 1; contador++)
            {
                if (contador != 2 * t - 1)
                {
                    no->filhos[contador] = no->filhos[contador + 1];
                }
            }
            no->qtdChaves--;
            if (no->qtdChaves == 0)
            {
                if (no == (*raiz))
                {
                    *raiz = no->filhos[0];
                }
                no = no->filhos[0];
            }
            BTreeDeleteKey(raiz, prd, chave);
            return;
        }
    }
    else
    {
        for (position = 0; position < no->qtdChaves; position++)
        {
            if (no->chaves[position] > chave)
            {
                break;
            }
        }
        ArvoreB *temp = no->filhos[position];
        if (temp->qtdChaves >= t)
        {
            BTreeDeleteKey(raiz, temp, chave);
            return;
        }
        if (true)
        {
            ArvoreB *nd = NULL;
            int divider = -1;

            if (position != no->qtdChaves && no->filhos[position + 1]->qtdChaves >= t)
            {
                divider = no->chaves[position];
                nd = no->filhos[position + 1];
                no->chaves[position] = nd->chaves[0];
                temp->chaves[temp->qtdChaves++] = divider;
                temp->filhos[temp->qtdChaves] = nd->filhos[0];
                for (int contador = 1; contador < nd->qtdChaves; contador++)
                {
                    nd->chaves[contador - 1] = nd->chaves[contador];
                }
                for (int contador = 1; contador <= nd->qtdChaves; contador++)
                {
                    nd->filhos[contador - 1] = nd->filhos[contador];
                }
                nd->qtdChaves--;
                BTreeDeleteKey(raiz, temp, chave);
                return;
            }
            else if (position != 0 && no->filhos[position - 1]->qtdChaves >= t)
            {

                divider = no->chaves[position - 1];
                nd = no->filhos[position - 1];
                no->chaves[position - 1] = nd->chaves[nd->qtdChaves - 1];
                ArvoreB *child = nd->filhos[nd->qtdChaves];
                nd->qtdChaves--;

                for (int contador = temp->qtdChaves; contador > 0; contador--)
                {
                    temp->chaves[contador] = temp->chaves[contador - 1];
                }
                temp->chaves[0] = divider;
                for (int contador = temp->qtdChaves + 1; contador > 0; contador--)
                {
                    temp->filhos[contador] = temp->filhos[contador - 1];
                }
                temp->filhos[0] = child;
                temp->qtdChaves++;
                BTreeDeleteKey(raiz, temp, chave);
                return;
            }
            else
            {
                ArvoreB *left = NULL;
                ArvoreB *right = NULL;
                bool ehUltimo = false;
                if (position != no->qtdChaves)
                {
                    divider = no->chaves[position];
                    left = no->filhos[position];
                    right = no->filhos[position + 1];
                }
                else
                {
                    divider = no->chaves[position - 1];
                    right = no->filhos[position];
                    left = no->filhos[position - 1];
                    ehUltimo = true;
                    position--;
                }
                for (int contador = position; contador < no->qtdChaves - 1; contador++)
                {
                    no->chaves[contador] = no->chaves[contador + 1];
                }
                for (int contador = position + 1; contador < no->qtdChaves; contador++)
                {
                    no->filhos[contador] = no->filhos[contador + 1];
                }
                no->qtdChaves--;
                left->chaves[left->qtdChaves++] = divider;

                for (int contador = 0, contadorJ = left->qtdChaves; contador < right->qtdChaves + 1; contador++, contadorJ++)
                {
                    if (contador < right->qtdChaves)
                    {
                        left->chaves[contadorJ] = right->chaves[contador];
                    }
                    left->filhos[contadorJ] = right->filhos[contador];
                }
                left->qtdChaves += right->qtdChaves;
                if (no->qtdChaves == 0)
                {
                    if (no == (*raiz))
                    {
                        *raiz = no->filhos[0];
                    }
                    no = no->filhos[0];
                }
                BTreeDeleteKey(raiz, left, chave);
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