#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <string.h>
typedef struct calculadora{
    int numero;
    char operando;
    int precedencia;
    int tag; // saber se na fila tem um número ou um operando
            // 1 = operando, 0 = número 
}Calculadora; // informações globais
typedef struct pilha{
    int inicio;
    int fim;
    struct calculadora *elementos;
}Pilha;
typedef struct no{
    Calculadora elemento;
    struct no *proximo;
    struct no *anterior;
}No;

typedef struct fila{
    struct calculadora *elementos;
    No *primeiro;
} FilaCircularInt;

No *no_enc_criar(Calculadora elemento) {
  No *novo_no = malloc(sizeof(struct no));

  novo_no->elemento = elemento;
  novo_no->proximo = NULL;

  return novo_no;
}

void no_enc_apagar(No *no) { free(no); }

int calcula(int a, int b, char op){
    if(op == '+'){
        return a + b;
    }else if(op == '-'){
        return a - b;   
    }else if(op == '/'){
        return a / b;   
    }else if(op == '*'){
        return a * b;   
    }else if(op == '^'){
        return pow(a, b);   
    }else if(op == 'm'){
        return a % b;   
    }else if(op == 'l'){
        return log10(a);  
    }else{
        return INT_MAX; // operando inválido
    }
}

FilaCircularInt *fila_circular_criar() {
    FilaCircularInt *nova_fila = malloc(sizeof(struct fila));

    nova_fila->primeiro = NULL;
    nova_fila->elementos = malloc(sizeof(struct calculadora)*100);

    return nova_fila;
}

void fila_circular_devolver(FilaCircularInt *fila) {
    // ! IMPORTANTE: fizemos **duas** alocações para criar. A ordem de desalocação **sempre** importa.
    free(fila->elementos);
    free(fila);
}

bool fila_circular_vazia(FilaCircularInt *fila) {
    return fila->primeiro == NULL;
}

bool fila_circular_enfileirar(FilaCircularInt *fila, struct calculadora c){

    if(fila_circular_vazia(fila)) {
        fila->primeiro->elemento = c;
        fila->primeiro->proximo = NULL;
        return true;
    }else{
        No *atual = fila->primeiro;
        while(atual->proximo != NULL){
            atual = atual->proximo;
        }

        atual->elemento = c;
        atual->proximo = NULL;
    }
    return true;
}

// esta função não serve para informar quem foi removido
bool fila_circular_desenfileirar(FilaCircularInt *f) {
    if(fila_circular_vazia(f)) {
        return false;
    }

    No *atual = f->primeiro;
    char aux_char;
    int a[2];
    Calculadora aux;

    while(atual->proximo != NULL && atual->elemento.tag != 1){
        atual = atual->proximo;
    }// depois disso atual está no operando

    aux_char = atual->elemento.operando; // operando
    a[1] = atual->anterior->elemento.numero; //numero antes dele
    a[0] = atual->anterior->anterior->elemento.numero; //numero dois antes dele

    aux.numero = calcula(a[0], a[1], aux_char);
    aux.tag = 0;

    No *aux_no;
    aux_no->elemento = aux; //resultado da operação 
    aux_no->proximo = atual->proximo; //número depois do operando
    
    atual = atual->anterior->anterior;
    atual = aux_no;

    return true;
}

/*
    int *a;
    int i = 0;

    Calculadora aux;
    char b;
    
    while(f->elementos[i].tag != 1 ){
        i++;
    }

    b = f->elementos[i].operando;
    i = 0;

    if(b == 'l'){
        a[0] = f->elementos[f->inicio].numero; 
        aux.numero = calcula(a[0], 10, b);
        aux.tag = 0;
        f->inicio = (f->inicio + 1) % f->capacidade;
        f->elementos[f->inicio] = aux; // o elemento deve continuar no começo da fila
    }else{
        a[0] = f->elementos[f->inicio].numero;
        a[1] = f->elementos[f->inicio + 1].numero; //revisar se f->inicio + 1 ta certo.
        aux.numero = calcula(a[0], a[1], b);
        aux.tag = 0;
        f->inicio = (f->inicio + 1) % f->capacidade;
        f->inicio = (f->inicio + 1) % f->capacidade;
        f->elementos[f->inicio] = aux; // o elemento deve continuar no começo da fila
    }
*/

int retornar_elementos_fila(FilaCircularInt *f){
    if(f->primeiro != NULL){
        return f->primeiro->elemento.numero;
    }else{
        return INT_MAX;
    }
}

Pilha *pilha_criar(){
    Pilha *nova_pilha = (Pilha*) malloc(sizeof(Pilha));

    nova_pilha->inicio = 0;
    nova_pilha->fim = 0;

    return nova_pilha;
}

bool pilha_vazia(Pilha *p){
    return p->fim == 0;
}

bool pop(Pilha *p, FilaCircularInt *f){
    if(pilha_vazia(p))
        return false;
    else{
        fila_circular_enfileirar(f, p->elementos[p->fim]);
        p->fim--;
        return true;
    }
}

bool push(Pilha *p, Calculadora c, FilaCircularInt *f){
    if(c.precedencia == -1){
        return false;
    }else if(pilha_vazia(p)){
        p->elementos[p->fim] = c;
        p->fim++;
        return true;
    }else if(p->elementos[p->fim].precedencia < c.precedencia){
        p->elementos[p->fim] = c;
        p->fim++;
        return true;
    }else if(c.operando == ')'){
        while(p->elementos[p->fim].operando != '('){
            p->elementos[p->fim].tag = 1;
            pop(p, f);
        }
        p->fim--;
        return true;
    }else{
       while(p->elementos[p->fim].precedencia > c.precedencia){
            p->elementos[p->fim].tag = 1;
            pop(p, f);
        }
        p->elementos[p->fim] = c;
        p->fim++;
        return true;
    }
}

void pilha_libera(Pilha *p){
    free(p);
}

void criar_precedencia(struct calculadora c){
    if(c.operando == '+' || c.operando == '-'){
        c.precedencia = 0;
    }else if(c.operando == '*' || c.operando == '/' || c.operando == 'm'){
        c.precedencia = 1;
    }else if(c.operando == '^' || c.operando == 'l'){
        c.precedencia = 2;
    }else if(c.operando == '(' || c.operando == ')'){
        c.precedencia = 3;
    }else{
        c.precedencia = -1;
    }
}

int fila_circular_qnt_elementos(FilaCircularInt *f){
    int qnt = 0;
    No *atual;

    if(f->primeiro == NULL){
        return 0;
    }else{
        atual = f->primeiro;
        while(atual->proximo != NULL){
            atual = atual->proximo;
            qnt++;
        }
        return qnt;
    }
}

int main(){
    Calculadora aux;
    Pilha *p = pilha_criar();
    FilaCircularInt *f =  fila_circular_criar();
    
    char str[100];
    char *pt;
    scanf("%[^\n]", str);
    pt = strtok(str, " ");
    
    while(pt){
        if(pt == '+' || pt == '-' || pt == '*' || pt == '/' || pt == 'l' || pt == 'm' || pt == '^' || pt == ')' || pt == '('){
            aux.operando = pt;
            aux.tag = 1;
            criar_precedencia(aux);
            push(p, aux, f);
        }else{
            aux.numero = strtol(pt, NULL, 10);
            aux.tag = 0;
            fila_circular_enfileirar(f, aux);
        }
        pt = strtok(NULL, " ");
    }
    
    while(pilha_vazia(p) == false){
        pop(p, f);
    }
    
    No *atual;
    if(f->primeiro != NULL){
        atual = f->primeiro;
    }

    while(atual->proximo != NULL && fila_circular_qnt_elementos(f) > 1){
        fila_circular_desenfileirar(f);
        atual = atual->proximo;
    }

    printf("%d", retornar_elementos_fila(f));
    fila_circular_devolver(f);
    pilha_libera(p);

    return 0;
}

/*char aux = 1; // só para não armazenar lixo, já que será substituído depois
    Calculadora *vet;
    int i = 0;

    while(aux != '\n'){
        scanf("%c", &aux);
        if(i %2 == 0){
            vet[i].tag = 0;
            vet[i].numero = (int) aux;
            fila_circular_enfileirar(f, vet[i]);
        }

        else{
            vet[i].tag = 1;
            vet[i].operando = aux;
            push(p, vet[i], f);
        }
        i++;
    }*/