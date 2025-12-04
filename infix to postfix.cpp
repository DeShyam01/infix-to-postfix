#include<stdio.h>
#include<stdlib.h>

typedef struct node{
	int type;	// for operand 0 & for operator 1
	
	union{
		int opd;
		struct{
			int precedence;
			char symbol;
		}opt;
	}element;
	
	struct node* next;
}node;

/* --- precedence calculator --- */

int precedence(int opt){
	switch(opt){
		case '(':
			return 0;
		case ')':
			return 0;
		case '+':
			return 1;
		case '-':
			return 1;
		case '*':
			return 2;
		case '/':
			return 3;
		case '^':
			return 4;
	}
	
	return -1;
}

/* --- Node creation functions --- */

node* createIntNode(int data){
	node* newNode = (node*)malloc(sizeof(node));
	newNode->type = 0;
	newNode->element.opd = data;
	newNode->next = NULL;
	
	return newNode;
}

node* createCharNode(char opt){
	node* newNode = (node*)malloc(sizeof(node));
	newNode->type = 1;
	newNode->element.opt.symbol = opt;
	newNode->element.opt.precedence = precedence(opt);
	newNode->next = NULL;
	
	return newNode;
}

/* --- stack operations functions definition --- */

int isEmpty(node* head){
	if(head==NULL){
		return 1;
	}
	
	return 0;
}

void push(node** top, node* newNode){
	newNode->next = (*top);
	(*top) = newNode;
}

node* pop(node** top){
	if(isEmpty(*top)){
		return NULL;
	}
	node* popedNode = (*top);
	(*top)=(*top)->next;
	
	return popedNode;
}

/* --- Linkedlist operations function definition --- */

void append(node** head, node* newNode){	
	if(*head==NULL){
		(*head) = newNode;
	}else{
		node* curr = (*head);
		
		while(curr->next!=NULL){
			curr=curr->next;
		}
		curr->next = newNode;
	}
}

/* --- Common display function for stack and linked list both --- */

void display(node* head){
	if(head==NULL){
		printf("Stack or Linkedlist is empty");
		return;
	}
	
	node* curr = head;
	
	while(curr!=NULL){
		if(curr->type==0){
			printf("%d ", curr->element.opd);
		}else if(curr->type==1){
			printf("%c ", curr->element.opt.symbol);
		}
		curr=curr->next;
	}
	printf("\n");
}

/* --- Infix input fucntion --- */

void infixInput(node** head){
	int turn=0;
	int opd;
	char opt;
	appendOpt(head, '(');
	while(opt!='\n'){
		if(turn==0){
			printf("opd: ");
			scanf("%d", &opd);
			append(head, createIntNode(opd));
			turn=1;
		}else{
			printf("opt: ");
			fflush(stdin);
			opt=getchar();
			if(opt=='+' || opt=='-' || opt=='*' || opt=='/' || opt=='(' || opt==')'){
				append(head, createCharNode(opt));
				turn=0;
			}else{
				printf("\n%c is not a valid operator\n", opt);
			}
		}
		printf("\n");
	}
	appendOpt(head, ')');
}

/* --- postfix conversion function --- */

node* postfix(node* infixHead){
	node* curr = infixHead;	//holds infix expression	
	node* stackTop = NULL;	//holds operator temporarily
	node* expressionTop = NULL;	//holds the main postfix expression
	
	while(curr!=NULL){
		if(curr->type==0){
			push(&expressionTop, curr->element.opd);
		}else if(curr->type==1){
			if(curr->element.opt.symbol=='('){
				push(&stackTop, createCharNode(curr->element.opt.symbol));
			}else if(curr->element.opt.symbol==')'){
				node* temp = (*stackTop);
				while(temp->element.opt.precedence!='('){
					push(&expressionTop, pop(&stackTop));
					temp=temp->next;
				}
			}
		}
	}
}

int main(){
	node* infixHead = NULL;				/* --- for storing the main expression --- */
	node* postfixTop = NULL;			/* --- for storing the postfix expression --- */
	
	infixInput(&infixHead);				/* --- main expression input --- */
	
	display(infixHead);					/* --- displaying the infix expression --- */
	
	postfixTop = postfix(infixHead);	/* --- converting to postfix expression --- */
	
	return 0;
}