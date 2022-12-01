#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BTree.h"


void errorB(char *s){
	fprintf(stderr, "%s", s);
	exit(EXIT_FAILURE);
}

/*****************************************************************************/
/**************************** fonctions de base ******************************/
/*****************************************************************************/

BTree makeEmptyBTree(void){
	return (BTree)NULL;
}

BTree makeNode(Element e, BTree l,  BTree r){
	BTree new;
	if ((new=(BTree)malloc(sizeof(Node)))==NULL) errorB("Allocation ratï¿½e!");
	new->elem=e;
	new->left=l;
	new->right=r;
	return new;
}

BTree makeLeaf(Element e){
	return makeNode(e,makeEmptyBTree(),makeEmptyBTree());
}


int isEmptyBTree(BTree bt){
	return (bt==NULL);
}

Element root(BTree bt){
	if(isEmptyBTree(bt)) errorB("Pas de noeud ï¿½ la racine d'un arbre vide!!!");
	return bt->elem;
}


BTree leftChild(BTree bt){
	if(isEmptyBTree(bt)) errorB("Pas de fils gauche dans un arbre vide!!!");
	return bt->left;
}

BTree rightChild(BTree bt){
	if(isEmptyBTree(bt)) errorB("Pas de fils droit dans un arbre vide!!!");
	return bt->right;
}

int isLeaf(BTree bt){
	return !isEmptyBTree(bt) && isEmptyBTree(leftChild(bt)) && isEmptyBTree(rightChild(bt));
}

void freeNode(Node *c){
	free(c);
}

/*****************************************************************************/
/*************************** fonctions bas niveau ****************************/
/*****************************************************************************/

void insertRight(Node *n, Element e){
	if(!isEmptyBTree(n) && isEmptyBTree(rightChild(n)))
		n->right=makeLeaf(e);
	else errorB("insertRight impossible!");
}

void insertLeft(Node *n, Element e){
	if(!isEmptyBTree(n) && isEmptyBTree(leftChild(n)))
		n->left=makeLeaf(e);
	else errorB("insertLeft impossible!");
}

Element deleteRight(Node *n){
	if(isEmptyBTree(n) || !isLeaf(rightChild(n)))
		errorB("deleteRight imossible!");

	Element res=root(n->right);
	n->right=makeEmptyBTree();
	return res;
}

Element deleteLeft(Node *n){
	if(isEmptyBTree(n)  || !isLeaf(leftChild(n)))
		errorB("deleteLeft imossible!");

	Element res=root(n->left);
	n->left=makeEmptyBTree();
	return res;
}

void insertRightmostNode(BTree *bt, Element e){
	if(isEmptyBTree(*bt))
		*bt=makeLeaf(e);
	else{
		BTree tmp=*bt;
		while(!isEmptyBTree(rightChild(tmp)))
			tmp=rightChild(tmp);
		insertRight(tmp,e);
	}
}

Element deleteLeftmostNode(BTree *bt){
	Element res;
	if(isEmptyBTree(*bt))
		errorB("deleteLeftmostNode imossible!");
	if(isEmptyBTree(leftChild(*bt))){
		res=root(*bt);
		*bt=rightChild(*bt);
	}
	else{
		BTree tmp=*bt;
		while(!isEmptyBTree(leftChild(leftChild(tmp))))
			tmp=leftChild(tmp);
		res=root(leftChild(tmp));
		tmp->left=(tmp->left)->right;
	}
	return res;
}

//__________________________________________________________________________

// renvoie le min
int min(int a, int b){
	return (a > b)? b : a;
}


// la creation d un arbre
void addNode(Node **tree, unsigned int key)
{
    Node *tmpNode;
    Node *tmpTree = *tree;

    //la creation de l element a ajouter
    Node *elem = malloc(sizeof(Node));
    elem->elem = key;
    elem->left = NULL;
    elem->right = NULL;

    if(tmpTree)  //TQ l arbre n est pas vide
    do
    {
        tmpNode = tmpTree;
        if(key > tmpTree->elem )
        {
            tmpTree = tmpTree->right;
            if(!tmpTree) tmpNode->right = elem;
        }
        else
        {
            tmpTree = tmpTree->left;
            if(!tmpTree) tmpNode->left = elem;
        }
    }
    while(tmpTree);
    else  *tree = elem;
}


//l affichage d un arbre parcour infexe
void printTree(Node *tree)
{
    if(!tree) return;

    if(tree->left)  printTree(tree->left);

    printf("key => %d\n", tree->elem); 

    if(tree->right) printTree(tree->right);
	
}


//le nettoyage de l arbre
void clearTree(Node **tree)
{
    Node *tmpTree = *tree;

    if(!tree) return;

    if(tmpTree->left)  clearTree(&tmpTree->left);

    if(tmpTree->right) clearTree(&tmpTree->right);

    free(tmpTree);      

    *tree = NULL;
}




/***************************  (pile) ****************************/
/*****************************************************************************/
//stucture de la pile
struct stack
{
	int maxsize;	// define max de la pile
	int top; //sommet
	Node* *items; 
};

// initialisation de la pile
struct stack* newStack(int capacity)
{
	struct stack *pt = (struct stack*)malloc(sizeof(struct stack));  //allocation dynamique

	pt->maxsize = capacity;
	pt->top = -1;
	pt->items = (Node**)malloc(sizeof(int) * capacity);

	return pt;
}

// size de la pile
int size(struct stack *pt) {
	return pt->top + 1;
}

// verifier si la pile est vide
int isEmpty(struct stack *pt) {
	return pt->top == -1;   				// or return size(pt) == 0;
}

// verifier si pile pleine
int isFull(struct stack *pt) {
	return pt->top == pt->maxsize - 1;  	// or return size(pt) == pt->maxsize;
}

// fonction pour ajout d'un element dans la pile
void push(struct stack *pt, Node* x)
{
	// verifier si pile pleine
	if (isFull(pt))
	{
		printf("Overflow\nProgram Terminated\n");//fin
		exit(EXIT_FAILURE);
	}

	//printf("Inserting %d\n", x->elem);

	// add an element and increment the top's index
	pt->items[++pt->top] = x;
}

// fct pour afficher le sommet de pile
Node* peek(struct stack *pt)
{
	// verifier si la pile est vide
	if (!isEmpty(pt)) {
		return pt->items[pt->top];
	}
	else {
		exit(EXIT_FAILURE);
	}
}

// empiler les elements dans notre pile
Node* pop(struct stack *pt)
{
	// si pile vide
	if (isEmpty(pt))
	{
		printf("Underflow\nProgram Terminated\n");//pas d'elemnt donc fin
		exit(EXIT_FAILURE);
	}

	//printf("Removing %d\n", peek(pt));

	//desempiler et decrementer la pile d'un element
	return pt->items[pt->top--];
}

/*****************************************file*/
typedef struct tmaillon{
	BTree val; struct tmaillon *suiv ;
}tmaillon;
typedef struct file{tmaillon *tete ; tmaillon *queuef;}file;


void initfile(file *f){
	( *f ).tete=NULL;
	}

int filevide(file *f){
	if (( *f ).tete == NULL) return 1;
	else return 0;
}


void enfiler(file *f , BTree x){
	tmaillon *nouv;
	nouv=malloc(sizeof(tmaillon));
	nouv->val =x;
	nouv->suiv=NULL;
	if(filevide(f)==1){
		(*f).tete=nouv;
	}
	else(((*f).queuef)->suiv=nouv); 
    (*f).queuef=nouv;
}


void defiler(file *f ,  BTree *x){
	tmaillon *sauv;
	if(filevide(f)==1){printf("la file est vide \n");}
	else {
	*x =((*f).tete)->val; 
	sauv=(*f).tete; 
	(*f).tete = ((*f).tete)->suiv;
	 free(sauv);}
}

 
/* A binary tree nodee has data,
   pointer to left child
   and a pointer to right child */
/*struct nodee {
    int data;
    struct nodee *left, *right;
}; 
*/
 
/* Function prototypes */
/*void printCurrentLevel(struct nodee* root, int level);
int height(struct nodee* nodee);
struct nodee* newnodee(int data);*/
 
/* Function to print level order traversal a tree*/
void printLevelOrder( Node *root)
{
    int h = height(root);
    int i;
    for (i = 1; i <= h; i++)
        printCurrentLevel(root, i);
}
 
/* Print nodees at a current level */
void printCurrentLevel(Node *root, int level)
{
    if (root == NULL)
        return;
    if (level == 1)
        printf("%d ", root->elem); 
    else if (level > 1) {
        printCurrentLevel(root->left, level - 1);
        printCurrentLevel(root->right, level - 1);
    }
}
 
/* Compute the "height" of a tree -- the number of
    nodees along the longest path from the root nodee
    down to the farthest leaf nodee.*/
int height(Node* nodee)
{
    if (nodee == NULL)
        return 0;
    else {
        /* compute the height of each subtree */
        int lheight = height(nodee->left);
        int rheight = height(nodee->right);
 
        /* use the larger one */
         if (lheight > rheight)
            return (lheight + 1);
        else
            return (rheight + 1);
    }
}
 
/* Helper function that allocates a new nodee with the
   given data and NULL left and right pointers. */
/*struct nodee* newnodee(int data)
{
    struct nodee* nodee
        = (struct nodee*)malloc(sizeof(struct nodee));
    nodee->data = data;
    nodee->left = NULL;
    nodee->right = NULL;
 
    return (nodee);
}*/
 




























