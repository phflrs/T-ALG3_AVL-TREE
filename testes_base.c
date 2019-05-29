#include <stdio.h>
#include <stdlib.h>
#include "avl-tree.h"




int calcula_altura(nodo_t *nodo)
{
    int alt_dir, alt_esq;

    if(nodo != NULL) {
        if(nodo->esq == NULL) {
            alt_esq = 1;
        }
        else {
            alt_esq = nodo->esq->altura + 1;
        }

        if(nodo->dir == NULL) {
            alt_dir = 1;
        }
        else {
            alt_dir = nodo->dir->altura + 1;
        }

        if(alt_esq >= alt_dir) {
            return(alt_esq);
        }
        return(alt_dir);
    }
    return 0; //ver isso aqui    
}

int calcula_fator(nodo_t *nodo)
{
    int alt_dir, alt_esq;

    if(nodo != NULL) {
        if(nodo->esq == NULL) {
            alt_esq = 1;
        }
        else {
            alt_esq = nodo->esq->altura + 1;
        }

        if(nodo->dir == NULL) {
            alt_dir = 1;
        }
        else {
            alt_dir = nodo->dir->altura + 1;
        }

        return(alt_esq - alt_dir);
    }
    return 0; //ver isso aqui    
}

nodo_t * cria_nodo(int chave) 
{
    nodo_t * novo = (nodo_t*)malloc(sizeof(nodo_t));
    novo->dir = NULL;
    novo->esq = NULL;
    novo->chave = chave;
    novo->altura = 1;
    return novo;
}

nodo_t * rotaciona_esquerda(nodo_t *nodo)
{
	nodo_t *temp;
	temp = nodo->dir;
	nodo->dir = temp->esq;
	temp->esq = nodo;
	nodo->altura = calcula_altura(nodo); //recalcula a altura
	temp->altura = calcula_altura(temp); //recalcula a altura
	
	return(temp);
}

nodo_t * rotaciona_direta(nodo_t *nodo)
{
    nodo_t *temp;
	temp = nodo->esq;
	nodo->esq = temp->dir;
	temp->dir = nodo;
	nodo->altura = calcula_altura(nodo);
	temp->altura = calcula_altura(temp);
	return(temp);
}

nodo_t * DD(nodo_t *nodo)
{
	nodo = rotaciona_esquerda(nodo);
	return(nodo);
}
 
nodo_t * EE(nodo_t *nodo)
{
	nodo = rotaciona_direta(nodo);
	return(nodo);
}
 
nodo_t * ED(nodo_t *nodo)
{
	nodo->esq = rotaciona_esquerda(nodo->esq);
	nodo = rotaciona_direta(nodo);	
	return(nodo);
}
 
nodo_t * DE(nodo_t *nodo)
{
	nodo->dir = rotaciona_direta(nodo->dir);
	nodo = rotaciona_esquerda(nodo);
	return(nodo);
}

void imprime(nodo_t *nodo) 
{       
	if(nodo!=NULL) {	
		imprime(nodo->esq);
		printf("chave: %d, altura: %d\n",nodo->chave, nodo->altura);
		imprime(nodo->dir);
	}
}

nodo_t * insere(nodo_t *nodo, int nova_chave) //começa passando a raiz
{
    if (nodo == NULL) {
        nodo = cria_nodo(nova_chave);
    } 
    else {
        if(nova_chave > nodo->chave) {	
			nodo->dir = insere(nodo->dir,nova_chave);
            if(calcula_fator(nodo) == -2) {   //depois vê o balanceamento
                if(nova_chave > nodo->dir->chave)  
					nodo = DD(nodo);
				else
					nodo = DE(nodo); 
            }
		}
        else {            
				nodo->esq = insere(nodo->esq,nova_chave);
                if(calcula_fator(nodo) == 2) { //depois vê o balanceamento
					if(nova_chave < nodo->esq->chave)
						nodo = EE(nodo);
					else
						nodo = ED(nodo);
                }
        }
    }
    nodo->altura = calcula_altura(nodo);
		
    return(nodo);
}

nodo_t * deleta(nodo_t* nodo,int deleta_chave)
{
	nodo_t *temp;
	
	if(nodo==NULL)
	{
		return NULL;
	}
	else
		if(deleta_chave > nodo->chave)		// insert in right subtree
		{
			nodo->dir = deleta(nodo->dir,deleta_chave);
			if(calcula_fator(nodo) == 2) {
				if(calcula_fator(nodo->esq) >= 0)
					nodo = EE(nodo);
				else
					nodo = ED(nodo);
            }
		}
		else
			if(deleta_chave < nodo->chave)
			{
				nodo->esq = deleta(nodo->esq,deleta_chave);
				if(calcula_fator(nodo) == -2) {	//Rebalance during windup
					if(calcula_fator(nodo->dir) <= 0)
						nodo = DD(nodo);
					else
						nodo = DE(nodo);
                }
			}
			else
			{
				//data to be deleted is found
				if(nodo -> dir != NULL)
				{	//delete its inorder succesor
					temp = nodo->dir;
					
					while(temp->esq != NULL)
						temp = temp ->esq;
					
					nodo->chave = temp->chave;
					nodo->dir = deleta(nodo->dir,temp->chave);
					
					if(calcula_fator(nodo) == 2) { //Rebalance during windup
						if(calcula_fator(nodo->esq) >= 0)
						    nodo = EE(nodo);
						else
							nodo = ED(nodo);
                    }
				}
				else
					return(nodo->esq);
			}
	nodo->altura = calcula_altura(nodo);
	return(nodo);
}



int main() {
    
    nodo_t *raiz = NULL;    
     
    raiz = insere(raiz, 5);    
    raiz = insere(raiz, 4);
    raiz = insere(raiz, 3);
    raiz = insere(raiz, 2);
    raiz = insere(raiz, 1);
    imprime(raiz);

    printf("\n");
    raiz = deleta(raiz, 5);
    imprime(raiz);
    return 0;
}
