#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

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

typedef struct fila{
    struct calculadora *elementos;
    unsigned int inicio;
    unsigned int fim;
    unsigned int capacidade;
} FilaCircularInt;


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
    }
    else{
        return INT_MAX;
    }
}


FilaCircularInt *fila_circular_criar(unsigned int capacidade) {
    FilaCircularInt *nova_fila = malloc(sizeof(FilaCircularInt));

    nova_fila->elementos = malloc(sizeof(int)*capacidade);
    nova_fila->capacidade = capacidade;
    nova_fila->inicio = 0;
    nova_fila->fim = 0;

    return nova_fila;
}

void fila_circular_devolver(FilaCircularInt *fila) {
    // ! IMPORTANTE: fizemos **duas** alocações para criar. A ordem de desalocação **sempre** importa.
    free(fila->elementos);
    free(fila);
}

bool fila_circular_cheia(FilaCircularInt *fila) {
    unsigned int novo_fim = (fila->fim + 1) % fila->capacidade;

    return fila->inicio == novo_fim;
}

bool fila_circular_vazia(FilaCircularInt *fila) {
    return fila->inicio == fila->fim;
}

bool fila_circular_enfileirar(FilaCircularInt *fila, struct calculadora c) {
    if(fila_circular_cheia(fila)) {
        return false;
    }

    // fim marca a posição onde inserir
    fila->elementos[fila->fim] = c;
    fila->fim = (fila->fim + 1) % fila->capacidade;

    return true;
}

// esta função não serve para informar quem foi removido
bool fila_circular_desenfileirar(FilaCircularInt *f) {
    if(fila_circular_vazia(f)) {
        return false;
    }

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
    }
    else{
        a[0] = f->elementos[f->inicio].numero;
        a[1] = f->elementos[f->inicio + 1].numero; //revisar se f->inicio + 1 ta certo.
        aux.numero = calcula(a[0], a[1], b);
        aux.tag = 0;
        f->inicio = (f->inicio + 1) % f->capacidade;
        f->inicio = (f->inicio + 1) % f->capacidade;
        f->elementos[f->inicio] = aux; // o elemento deve continuar no começo da fila
    }

    return true;
}

int retornar_elementos_fila(FilaCircularInt *f){
    return f->elementos[f->fim].numero;
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
    }else if(p->elementos[p->fim].precedencia > c.precedencia){
        p->elementos[p->fim] = c;
        p->fim++;
        return true;
    }
    else if(c.operando == ')'){
        while(p->elementos[p->fim].operando != '('){
            p->elementos[p->fim].tag = 1;
            pop(p, f);
        }
        p->fim--;
        return true;
    }
    else{
       while (p->elementos[p->fim].precedencia <= c.precedencia){
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




int main(){

    Pilha *p = pilha_criar();
    FilaCircularInt *f =  fila_circular_criar(100);
    
    char aux;
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

    }
    while (pilha_vazia(p)){
        pop(p, f);
    }
    while(f->fim != 1){
        fila_circular_desenfileirar(f);
    }
    printf("%d", retornar_elementos_fila(f));
    fila_circular_devolver(f);
    pilha_libera(p);
    
    
    
}



