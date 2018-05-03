// RedBlackTree.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <math.h>
#include <Windows.h>

#define COUNT 5 // for tree output

enum color { red, black };

struct node {
	color color;
	int *data;
	node *parent;
	node *left;
	node *right;
	node* getgrandparent() {
		//check if grandparent exists and return pointer
		if (parent != NULL && parent->parent != NULL)
			return parent->parent;
		return NULL;
	}
	node* getuncle() {
		if (getgrandparent() == NULL)
			return NULL;
		if (getgrandparent()->left == parent) {
			return getgrandparent()->right;
		}
		else {
			return getgrandparent()->left;
		}
	}
	int getside() {
		if (parent == NULL)
			return 0;
		if (parent->right == this) {
			return 1;
		}
		else {
			return -1;
		}
	}
};


node* createNILchild(node *parent) {
	struct node *nilNode = (struct node *)malloc(sizeof(struct node));
	nilNode->color = black;
	nilNode->data = NULL;
	nilNode->parent = parent;
	nilNode->right = NULL;
	nilNode->left = NULL;
	return nilNode;
}

void printTree(node *x)
{
	using std::cout;
	if (!x->data) return;

	printTree(x->left);

	cout << ' ' << *x->data << '\n';

	printTree(x->right);
}


int getCase(node *temp) {
	//do we have a uncle? is our uncle a nigga?
	if (temp->getuncle() != NULL && temp->getuncle()->color == black) {
		//do we have a triangle or line???
		if (temp->getside() == temp->parent->getside()) {
			//we have a LINE
			if (temp->getside() == 1) {
				return 31; // right side line
			}
			else {
				return 30; // left side line
			}
		}
		else {
			//we have a TRIANGLE
			if (temp->getside() == 1 && temp->parent->getside() == -1) {
				return 20; //triangle points left
			}
			else {
				return 21; // triangle points right
			}
		}
	}
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
	if (p->getside() == 1) { //set parent of P to point at Q
		p->parent->right = q;
	}
	else if (p->getside() == -1) {
		p->parent->left = q;
	}
	else {
		//?
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
	if (q->getside() == 1) { //set parent of P to point at Q
		q->parent->right = p;
	}
	else if (q->getside() == -1) {
		q->parent->left = p;
	}
	else {
		//?
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

// Function to print binary tree in 2D
// It does reverse inorder traversal
void print2DUtil(node *root, int space)
{
	// Base case
	if (root == NULL)
		return;

	// Increase distance between levels
	space += COUNT;

	// Process right child first
	if (root->right != NULL)
		print2DUtil(root->right, space);

	// Print current node after space
	// count
	printf("\n");
	for (int i = COUNT; i < space; i++)
		printf(" ");
	if (root->data != NULL) {
		if (root->color == black) {
			printf("%d\n", *root->data);
		}
		else {
			HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
			HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(hstdout, &csbi);
			SetConsoleTextAttribute(hstdout, 0x04);
			std::cout << *root->data << std::endl;
			SetConsoleTextAttribute(hstdout, csbi.wAttributes);
		}
	}
	else {
		printf("NULL");
	}

	// Process left child
	if (root->left != NULL)
		print2DUtil(root->left, space);
}

// Wrapper over print2DUtil()
void print2D(node *root)
{
	// Pass initial space count as 0
	print2DUtil(root, 0);
}

int main()
{
	time_t t;
	/* Intializes random number generator */
	srand((unsigned)time(&t));

	node* root = createNILchild(NULL);
	for (int i = 0; i < 100; i++) {
		int *newNumber = (int*)malloc(sizeof(int));
		*newNumber = rand() % 100;
		rb_insert(root, newNumber);
	}
	print2D(root);
	std::cin.get();
	system("pause");
}