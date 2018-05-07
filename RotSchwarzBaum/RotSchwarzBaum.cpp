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

//#define DEBUG

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

//insert data in Tree T and return pointer to new node
node* AVLinsert(node *T, int *data) {
	node *temp = T;
	//schauen ob Knoten NIL ist
	if (T->parent == NULL && T->left == NULL && T->right == NULL) {
		T->color = black;

		//create left NIL child
		T->left = createNILchild(T);

		//create right NIL child
		T->right = createNILchild(T);

		//set pointer for data
		T->data = data;

		return T;
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
		T = temp;
	}
	return T;
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
	node *temp;

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
	node *temp;

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

//normal AVL-Tree deletion
node* AVLdelete(node *T, int *data) {
	node* temp = T;
	node* delNode; //node that has NIL childs - will be deleted
	node* child; //child of delNode

	//search for the node, until it is found or a NIL-child is reached
	while (temp && temp->data && *temp->data != *data) {
		if (*temp->data < *data) {
			temp = temp->right;
		}
		else if (*temp->data > *data) {
			temp = temp->left;
		}
	}
	//delete node temp
	if (temp && temp->data) 
	{
		//do we have 1 or more NIL child/s
		if ((temp->left && !temp->left->data) || (temp->right && !temp->right->data)) 
		{	//node with 1 or 2 NIL childs -> can be deleted
			delNode = temp; 
		}
		else {	//we have no NIL childs -> get successor which can be deleted			
			delNode = successor(temp);
		}

		//find child of delNode
		if (delNode->left && delNode->left->data) 
		{	//delNode can just have 1 child thats not NIL
			child = delNode->left;
			delNode->left = NULL; //set to NULL to avoid deletion
		}
		else {
			if (delNode->right && delNode->right->data) {
				child = delNode->right;
				delNode->right = NULL; //set to NULL to avoid deletion
			}
			else {
				child = NULL;
			}
		}

		//replace delNode with child in the Tree
		if (child)
		{
			child->parent = delNode->parent;
		}
		else { //no child found -> create NIL-child
			child = createNILchild(delNode->parent);
		}

		if (delNode->parent) {
			if (delNode == delNode->parent->left)
			{	//delNode is left child
				delNode->parent->left = child;
			}
			else {	//delNode is right child
				delNode->parent->right = child;
			}
		}

		//if we had to find a successor
		if (delNode != temp)
		{
			//copy color if the successor wasn't temps right child
				//temp->color = delNode->color;
			//copy pointer on data of successor to our node
			if (temp->data)
				free(temp->data);
			temp->data = delNode->data;
			//set pointer to NULL, so we don't free it later
			delNode->data = NULL;
		}
		
		//all pointers of delNode to data we wanted to keep, have been set to NULL
		if (delNode->color == child->color == black) { //
			//when we delete a black node we have to fix the tree (double black)
			freeTree(delNode);
			#ifdef DEBUG
			if (child->data)
				cout << "Deleted black node! Returned child: " << *child->data << endl;
			else
				cout << "Deleted black node! Returned child: NIL" << endl;
			#endif
			return child;
		}
		else {
			child->color = black;
			//done
			freeTree(delNode);
			return NULL;
		}
	}
	return NULL;
}

void debugPrint(node* p) {
	#ifdef DEBUG
	if (p->data)
		cout << "p: " << *p->data << endl;
	while (p->parent)
		p = p->parent;
	printDefault(p);
	#endif
}

node* rb_delete(node *T, int *data) {
	//cases and explanation http://www.stolerman.net/studies/cs521/red_black_trees.pdf
	//perform basic AVL-Tree deletion
	node* p = AVLdelete(T, data);
	//returns child if it is double black or NULL if nothing is to be fixed
	
	
	if (p) {
		#ifdef DEBUG
		cout << "IN: " << endl;
		debugPrint(p);
		#endif
		node *s; //sibling
		//p->color = black;
		//work up the tree, until we reach the root
		while (p->parent && p->color == black)
		{
			if (p->parent->left == p)
			{	//we are a left child
				s = p->parent->right;
				if (s->color == red)
				{	//Case 1: our sibling is red
					//switch colors of sibling and parent
					s->color = black;
					p->parent->color = red;
					//rotate left around parent
					rotateLeft(p->parent);
					//set s to new sibling
					s = p->parent->right;
					#ifdef DEBUG
					cout << "Case 1" << endl;
					debugPrint(p);
					#endif
					
	
				}
				//sibling is now black
				if (s->right && s->left && s->right->color == s->left->color == black)
				{	//case parent red -> simple fix
					if (p->parent->color == red) {
						p->parent->color = black;
						s->color = red;
						break;
					}					
					//Case 2: sibling and childs are black
					//color sibling red
					s->color = red;
					//move double black problem up the tree
					#ifdef DEBUG
					debugPrint(p);
					cout << "Case 2" << endl;
					#endif
					p = p->parent;					
				}
				else {
					if (s->right->color == black)
					{	//Case 3: siblings left child is red and right child is black
						
						//Switch colors between new (sibling after rotation) and old siblings 
						s->left->color = black;
						s->color = red;
						//rotate right around sibling
						rotateRight(s);
						//set s to new sibling
						s = p->parent->right;
						#ifdef DEBUG
						cout << "Case 3" << endl;
						debugPrint(p);
						#endif
					}
					//Case 4: sibling is black, with right child red
					s->color = p->parent->color;
					//move double black up
					p->parent->color = black;
					//color siblings right child black (whill be p's uncle after rotation)
					s->right->color = black;
					//rotate around father
					rotateLeft(p->parent);
					#ifdef DEBUG
					cout << "Case 4" << endl;
					debugPrint(p);
					#endif

					//rb Tree properties restored! -> set p to root
					while (p->parent)
						p = p->parent;
				}
			}
			else
			{	//same as above, just switched side
				s = p->parent->left;
				if (s->color == red)
				{	//Case 1					
					s->color = black;
					p->parent->color = red;
					rotateRight(p->parent);
					s = p->parent->left;
					#ifdef DEBUG
					cout << "Case 1" << endl;
					debugPrint(p);
					#endif
				}
				if (s->left->color == s->right->color == black)
				{	//case parent red -> simple fix
					if (p->parent->color == red) {
						p->parent->color = black;
						s->color = red;
						break;
					}	
					//Case 2
					s->color = red;
					#ifdef DEBUG
					cout << "Case 2" << endl;
					debugPrint(p);
					#endif
					p = p->parent;
					
				}
				else
				{
					if (s->left->color == black)
					{	//Case 3
						s->right->color = black;
						s->color = red;
						rotateLeft(s);
						s = p->parent->left;
						#ifdef DEBUG
						cout << "Case 3" << endl;
						debugPrint(p);
						#endif
					}
					//Case 4
					
					s->color = p->parent->color;
					p->parent->color = black;
					s->left->color = black;
					rotateRight(p->parent);
					#ifdef DEBUG
					cout << "Case 4" << endl;
					debugPrint(p);
					#endif
					while (p->parent)
						p = p->parent;
				}
			}
			p->color = black;
		}
	}
	else {
		return T;
	}
	while (p->parent)
		p = p->parent;
	return p;
}


void printAfterEachInsertTestArray(int sleep) {
	time_t t;
	/* Intializes random number generator */
	srand((unsigned)time(&t));

	//create root
	node* root = createNILchild(NULL);

	int array[] = {64, 23, 93, 73, 31, 69, 14, 1, 63, 51, 2, 96, 59, 47, 91, 10, 25, 68, 55, 45, 16, 13, 85, 64, 80, 74, 83, 78, 78, 3, 72, 20, 14, 48, 72, 56, 94, 45, 2};
	int delarray[] = {3, 41, 67, 45, 87, 47, 14, 54, 8, 47, 93, 3, 23, 58, 47, 17, 9, 62, 47, 19, 75, 7, 50, 96, 46, 60, 27, 13, 85, 29, 76, 89, 28, 84, 3, 44, 43, 26, 91, 43};
	//fill tree with 'n' random numbers
	for (int i = 0; i < 39; i++) {
		int *newNumber = (int*)malloc(sizeof(int));
		*newNumber = array[i];
		//*newNumber = rand() % 100; //testarray[i];
		//array[i] = *newNumber;

		cout << "Insert: " << *newNumber << endl;
		root = rb_insert(root, newNumber);

		//print tree after each insert
		printDefault(root);
		cout << endl;
		Sleep(sleep);
	}
	Sleep(1000);
	for (int i = 0; i < 40; i++) {
		
		int *newNumber = (int*)malloc(sizeof(int));
		
		*newNumber = delarray[i];

		cout << "Delete: " << *newNumber << endl;

		root = rb_delete(root, newNumber);

		//print tree after each delete
		printDefault(root);
		int blackNodeDepth = checkBlackNodeProperty(root);
		cout << "#ofBlackNodes: " << blackNodeDepth << endl;
		cout << endl;
		if (blackNodeDepth < 0) {
			int var;
			cin >> var;
		}
		Sleep(sleep);
	}

	int blackNodeDepth = checkBlackNodeProperty(root);
	cout << "#ofBlackNodes: " << blackNodeDepth;

}

void printAfterEachInsert(int amount, int sleep) {
	time_t t;
	/* Intializes random number generator */
	srand((unsigned)time(&t));

	//create root
	node* root = createNILchild(NULL);

	//fill tree with 'n' random numbers
	for (int i = 0; i < amount; i++) {
		int *newNumber = (int*)malloc(sizeof(int));

		*newNumber = rand() % 100; //testarray[i];

		cout << "Insert: " << *newNumber << endl;
		root = rb_insert(root, newNumber);

		//print tree after each insert
		printDefault(root);
		cout << endl;
		Sleep(sleep);
	}
	Sleep(1000);
	for (int i = 0; i < amount; i++) {

		int *newNumber = (int*)malloc(sizeof(int));

		*newNumber = rand() % 100;
		cout << "Delete: " << *newNumber << endl;

		root = rb_delete(root, newNumber);

		//print tree after each delete
		printDefault(root);
		int blackNodeDepth = checkBlackNodeProperty(root);
		cout << "#ofBlackNodes: " << blackNodeDepth << endl;
		cout << endl;
		if (blackNodeDepth < 0) {
			int var;
			cin >> var;
		}
		Sleep(sleep);
	}

	int blackNodeDepth = checkBlackNodeProperty(root);
	cout << "#ofBlackNodes: " << blackNodeDepth;

}

void printAtEnd(int amount, int sleep) {
	time_t t;
	/* Intializes random number generator */
	srand((unsigned)time(&t));

	//create root
	node* root = createNILchild(NULL);

	//fill tree with 'n' random numbers
	for (int i = 0; i < amount; i++) {
		int *newNumber = (int*)malloc(sizeof(int));

		*newNumber = rand() % 100; //testarray[i];
		
		root = rb_insert(root, newNumber);
	}

	for (int i = 0; i < amount; i++) {

		int *newNumber = (int*)malloc(sizeof(int));
		//*newNumber = array[i];
		*newNumber = rand() % 100;

		root = rb_delete(root, newNumber);
	}

	int blackNodeDepth = checkBlackNodeProperty(root);
	cout << "#ofBlackNodes: " << blackNodeDepth;

}

void stressTestTree() {
	time_t t;
	/* Intializes random number generator */
	srand((unsigned)time(&t));

	
	while (true) {
		//create root
		node* root = createNILchild(NULL);

		int blackNodeDepth = 0;
		//fill tree with 'n' random numbers
		cout << "INSERT: ";
		while (blackNodeDepth < 5) {
			blackNodeDepth = checkBlackNodeProperty(root);

			int *newNumber = (int*)malloc(sizeof(int));

			*newNumber = rand() % 100; //testarray[i];

			root = rb_insert(root, newNumber);

			cout << *newNumber << ", ";

			if (blackNodeDepth < 0) {
				printDefault(root);
				cout << "#ofBlackNodes: " << blackNodeDepth << endl;
				int var;
				cin >> var;
			}
		}
		cout << endl << "DELETE: ";
		while (blackNodeDepth > 2) {
			blackNodeDepth = checkBlackNodeProperty(root);

			int *newNumber = (int*)malloc(sizeof(int));
			//*newNumber = array[i];
			*newNumber = rand() % 100;

			root = rb_delete(root, newNumber);

			cout << *newNumber << ", ";

			if (blackNodeDepth < 0) {
				printDefault(root);
				cout << "#ofBlackNodes: " << blackNodeDepth << endl;
				int var;
				cin >> var;
			}
		}
		cout << endl;
		printDefault(root);
		blackNodeDepth = checkBlackNodeProperty(root);
		cout << "#ofBlackNodes: " << blackNodeDepth << endl;
		freeTree(root);
	}
	
}
//INSERT: 64, 23, 93, 73, 31, 69, 14, 1, 63, 51, 2, 96, 59, 47, 91, 10, 25, 68, 55, 45, 16, 13, 85, 64, 80, 74, 83, 78, 78, 3, 72, 20, 14, 48, 72, 56, 94, 45, 2,
//DELETE : 3, 41, 67, 45, 87, 47, 14, 54, 8, 47, 93, 3, 23, 58, 47, 17, 9, 62, 47, 19, 75, 7, 50, 96, 46, 60, 27, 13, 85, 29, 76, 89, 28, 84, 3, 44, 43, 26, 91, 43,

int main()
{
	//printAfterEachInsert(100, 1);
	//stressTestTree();
	printAfterEachInsertTestArray(1);

	while (true) {
		updateFont();
		Sleep(100);
	};
}