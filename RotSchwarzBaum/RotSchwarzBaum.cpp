// RedBlackTree.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

//#include "stdafx.h"

#include "mynode.h"
#include "tools.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <math.h>


//OUTPUT STUFF
#include <Windows.h>
#include <cwchar>

#include <fstream>
#include <deque>
#include <iomanip>
#include <sstream>
#include <string>



using namespace std;


node* createNILchild(node *parent) {
	struct node *nilNode = (struct node *)malloc(sizeof(struct node));
	nilNode->color = black;
	nilNode->data = NULL;
	nilNode->parent = parent;
	nilNode->right = NULL;
	nilNode->left = NULL;
	return nilNode;
}

node* AVLinsert(node *node, int *data) {
	struct node *temp = node;
	//schauen ob Knoten NIL ist
	if (node->parent == NULL && node->left == NULL && node->right == NULL) {
		node->color = black;

		//create left NIL child
		node->left = createNILchild(node);

		//create right NIL child
		node->right = createNILchild(node);

		//set pointer for data
		node->data = data;

		return node;
	}
	else {
		//find right NIL child to insert node for data
		while (temp->data) {
			if (*temp->data < *data) {
				temp = temp->right;
			}
			else {
				temp = temp->left;
			}
		}
		temp->color = red;
		temp->data = data;
		temp->left = createNILchild(temp);
		temp->right = createNILchild(temp);
		node = temp;
	}
	return node;
}

/**	start:				after 1:				after 2:
*		    P				  P							  Q
*		  /  \			    /  \   temp:				/  \
*	  uncle   Q		    uncle   QL    Q				  P		QR
*     /  \   /  \	    /  \		/  \			/  \
*			QL   QR  			   QL   QR		uncle   QL
*												/  \
*/
void rotateLeft(node *p) {
	node *q = p->right;
	node* temp;

	p->right = q->left; // 1. set P's right child to be QL
	q->left->parent = p; //Set QL's parent to P

	q->left = p; // 2. Set Q's left child to be P
	if (p->parent) {
		if (p->parent->right == p) { //set parent of P to point at Q
			p->parent->right = q;
		}
		else {
			p->parent->left = q;
		}
	}

	q->parent = p->parent;
	p->parent = q;	//Set P's parent to Q
}

void rotateRight(node *q) {
	node *p = q->left;
	node* temp;

	q->left = p->right; // 1. set Q's left child to be QR
	p->right->parent = q; //Set QR's parent to Q

	p->right = q; // 2. Set Q's right child to be P
	if (q->parent) {
		if (q->parent->right == q) { //set parent of P to point at Q
			q->parent->right = p;
		}
		else {
			q->parent->left = p;
		}
	}
	
	p->parent = q->parent;
	q->parent = p;	//Set P's parent to Q
}

//              Tree , insertNode
node* rb_insert(node *T, int *data) {
	/* Insert in the tree in the usual way */
	struct node *temp = T;
	//schauen ob Knoten NIL ist
	if (T->parent == NULL && T->left == NULL && T->right == NULL) {
		T->color = black;

		//create left NIL child
		T->left = createNILchild(T);

		//create right NIL child
		T->right = createNILchild(T);

		//set pointer for data
		T->data = data;

		temp = T;
	}
	else {
		//find right NIL child to insert node for data
		while (temp->data) {
			if (*temp->data < *data) {
				temp = temp->right;
			}
			else {
				temp = temp->left;
			}
		}
		temp->color = red;
		temp->data = data;
		temp->left = createNILchild(temp);
		temp->right = createNILchild(temp);
	}
	node* x = temp;
	/* Now restore the red-black property */

	while ((x->parent != NULL) && (x->parent->color == red)) {
		if (x->parent == x->getgrandparent()->left) {
			/* If x's parent is a left, uncle is x's right 'uncle' */
			node* uncle = x->getgrandparent()->right;
			if (uncle->color == red) {
				/* case 1 - change the colours */
				x->parent->color = black;
				uncle->color = black;
				x->getgrandparent()->color = red;
				/* Move x up the tree */
				x = x->getgrandparent();
			}
			else {
				/* uncle is a black node */
				if (x == x->parent->right) {
					/* and x is to the right */
					/* case 2 - move x up and rotate */
					x = x->parent;
					rotateLeft(x);
				}
				/* case 3 */
				x->parent->color = black;
				x->getgrandparent()->color = red;
				rotateRight(x->getgrandparent());
			}
		}
		else {
			/* repeat the "if" part with right and left
			exchanged */
			/* If x's parent is a righty, uncle is x's left 'uncle' */
			node* uncle = x->getgrandparent()->left;
			if (uncle->color == red) {
				/* case 1 - change the colours */
				x->parent->color = black;
				uncle->color = black;
				x->getgrandparent()->color = red;
				/* Move x up the tree */
				x = x->getgrandparent();
			}
			else {
				/* uncle is a black node */
				if (x == x->parent->left) {
					/* and x is to the left */
					/* case 2 - move x up and rotate */
					x = x->parent;
					rotateRight(x);
				}
				/* case 3 */
				x->parent->color = black;
				x->getgrandparent()->color = red;
				//printf("%d", x->getgrandparent()->color);
				rotateLeft(x->getgrandparent());
			}
		}
	}
	/* Colour the root black */
	while (x->parent != NULL) {
		x = x->parent;
	}
	x->color = black;
	return x;
}



void printAfterEachInsert(int n) {
	time_t t;
	/* Intializes random number generator */
	srand((unsigned)time(&t));

	//create root
	node* root = createNILchild(NULL);


	//fill tree with 'n' random numbers
	for (int i = 0; i < n; i++) {
		int *newNumber = (int*)malloc(sizeof(int));
		*newNumber = rand() % 100; //testarray[i];

		cout << "Insert: " << *newNumber << endl;
		root = rb_insert(root, newNumber);

		//print tree after each insert
		printDefault(root);
	}
	int blackNodeDepth = checkBlackNodeProperty(root);
	cout << "#ofBlackNodes: " << blackNodeDepth;

}


int main()
{
	time_t t;
	/* Intializes random number generator */
	srand((unsigned)time(&t));
	//int testarray [20] = { 3, 41, 23, 57, 17, 65, 88, 20, 51, 92, 13, 80, 95, 17, 58, 24, 35, 6, 76, 1};
	//create root
	node* root = createNILchild(NULL);

	cout << "Insert";
	//fill tree with numbers
	for (int i = 0; i < 40; i++) {
		int *newNumber = (int*)malloc(sizeof(int));
		*newNumber = rand() % 100; //testarray[i];

		cout << "Insert: " << *newNumber << endl;
		root = rb_insert(root, newNumber);

		printDefault(root);
	}
	freeTree(root);
		
	cout << "DONE" << endl;
	while (true) {};
	
	
	//set FONT
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   // Width of each character in the font
	cfi.dwFontSize.Y = 20 - maxHeight(root);                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	while (true) {
		//keep window open
		
		SHORT keyStateP = GetKeyState('P');
		SHORT keyStateO = GetKeyState('O');
		if (keyStateP & 0x8000) {
			cfi.cbSize = sizeof(cfi);
			cfi.dwFontSize.Y++;
			SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
		} else if (keyStateO & 0x8000) {
			
			cfi.cbSize = sizeof(cfi);
			cfi.dwFontSize.Y--;
			SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
		}
		Sleep(100);
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	}

	
}