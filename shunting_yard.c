#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

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

Pilha *pilha_criar(){
    Pilha *nova_pilha = (Pilha*) malloc(sizeof(Pilha));

    nova_pilha->inicio = 0;
    nova_pilha->fim = 0;

    return nova_pilha;
};

bool push(Pilha *p, Calculadora c, FilaCircularInt *f){
    if(c.precedencia == -1){
        exit; //operando inválido
    }else if(pilha_vazia(p)){
        p->elementos[p->fim] = c;
        p->fim++;
    }else if(p->elementos[p->fim].precedencia > c.precedencia){
        p->elementos[p->fim] = c;
        p->fim++;
    }else{
        p->elementos[p->fim].tag = 1;
        fila_circular_enfileirar(f, p->elementos[p->fim]);
        pop(p);
        p->elementos[p->fim] = c;
        p->fim++;
    }
}

bool pilha_vazia(Pilha *p){
    return p->fim == 0;
};

bool pop(Pilha *p){
    if(pilha_vazia(p))
        return false;
    else{
        p->fim--;
        return true;
    }
};

void pilha_libera(Pilha *p){
    free(p);
};

FilaCircularInt *fila_circular_criar(unsigned int capacidade) {
    FilaCircularInt *nova_fila = malloc(sizeof(FilaCircularInt));

    nova_fila->elementos = malloc(sizeof(int)*capacidade);
    nova_fila->capacidade = capacidade;
    nova_fila->inicio = 0;
    nova_fila->fim = 0;
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
bool fila_circular_desenfileirar(FilaCircularInt *fila, Pilha p) {
    if(fila_circular_vazia(fila)) {
        return false;
    }

    int *a;
    int i = 0;

    while(fila->elementos[fila->inicio].tag == 0){
        a[i] = fila->elementos[fila->inicio].numero;
        fila->inicio = (fila->inicio + 1) % fila->capacidade;
        i++;
    }

    char b;
    if(fila->elementos[fila->inicio].tag == 1){
        b = fila->elementos[fila->inicio].operando;
        fila->inicio = (fila->inicio + 1) % fila->capacidade;
    }
    
    Calculadora aux;

    if(i == 1){
        aux.numero = calcula(a[0], 10, b);
        aux.tag = 0;
    }else if(i == 2){
        aux.numero = calcula(a[0], a[1], b);
        aux.tag = 0;
    }else{
        exit; //equação inválida
    }

    fila_circular_enfileirar(fila, aux);
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
        exit;
    }
}


int main(){

    Pilha *p = pilha_criar();
    FilaCircularInt *f =  fila_circular_criar(100);
    
    int aux;
    while(aux != '\n'){
        scanf("%c", &aux);
        if(aux >= 0){

        }
    }
    
}



