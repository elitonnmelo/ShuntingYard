#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct pilha{
    int inicio;
    int fim;
    int *elementos;
    int capacidade;
}Pilha;

Pilha *pilha_criar(int capacidade){
    Pilha *nova_pilha = (Pilha*) malloc(sizeof(Pilha));

    nova_pilha->inicio = 0;
    nova_pilha->fim = 0;
    nova_pilha->capacidade = capacidade;

    return nova_pilha;
};

bool pilha_vazia(Pilha *p){
    return p->fim == 0;
};

void push(int elemento, Pilha *p){

    if(p->fim == p->capacidade){
        return;
    }else{
        p->elementos[p->fim] = elemento;
        p->fim++;
    }
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

int main(){

    Pilha *p = pilha_criar(5);

    push(10, p);
    push(35, p);
    pop(p);
    push(23, p);
    push(15, p);
    push(2, p);
    pop(p);
    push(11, p);
    push(545, p);
    push(1, p);
    
    for(int i = 0; i < p->fim; i++){
        printf("%d ", p->elementos[i]);
    }

    pilha_libera(p);
    return 0;
}


