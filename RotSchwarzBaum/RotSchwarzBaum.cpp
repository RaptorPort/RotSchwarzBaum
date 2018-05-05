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
			//copy color and data of successor to our node
			temp->color = delNode->color;
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
			if (child->data)
				cout << "Deleted black node! Returned child: " << *child->data << endl;
			else
				cout << "Deleted black node! Returned child: NIL" << endl;
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

node* rb_delete(node *T, int *data) {
	//cases and explanation http://www.stolerman.net/studies/cs521/red_black_trees.pdf
	//perform basic AVL-Tree deletion
	node* p = AVLdelete(T, data);
	//returns child if it is double black or NULL if nothing is to be fixed
	
	if (p) {
		node *s; //sibling
		p->color = black;
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
				}
				//sibling is now black
				if (s->right && s->left && s->right->color == s->left->color == black )
				{	//Case 2: sibling and childs are black
					//color sibling red
					s->color = red;
					//move double black problem up the tree
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
					}
					//Case 4: sibling is black, with right child red
					s->color = p->parent->color;
					//move double black up
					p->parent->color = black;
					//color siblings right child black (whill be p's uncle after rotation)
					s->right->color = black;
					//rotate around father
					rotateLeft(p->parent);
					//rb Tree properties restored! -> set p to root
					while (p->parent)
						p = p->parent;
				}
			}
			else
			{	//same as above, just switched side
				s = p->parent->left;
				if (s->color == red)
				{
					s->color = black;
					p->parent->color = red;
					rotateRight(p->parent);
					s = p->parent->left;
				}
				if (s->left->color == s->right->color == black)
				{
					s->color = red;
					p = p->parent;
				}
				else
				{
					if (s->left->color == black)
					{
						s->right->color = black;
						s->color = red;
						rotateLeft(s);
						s = p->parent->left;
					}
					s->color = p->parent->color;
					p->parent->color = black;
					s->left->color = black;
					rotateRight(p->parent);
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

/*
//normal AVL-Tree deletion with coloring
node* AVLdelete(node *T, int *data) {
	node* temp = T;
	node* deletedPos;
	//search for the node, until it is found or a NIL-child is reached
	while (temp->data || *temp->data == *data) {
		if (*temp->data < *data) {
			temp = temp->right;
		} else if (*temp->data > *data) {
			temp = temp->left;
		}
	}
	//delete node temp
	if (temp->data) {
		//does left child exist?
		if (temp->left && temp->left->data) {
			//does right child also exist?
			if (temp->right && temp->right->data) {
				//CASE 3: node has 2 children


			}
			else {
				//CASE 2: node only has a left child

				//set our left childs parent to our parent
				temp->left->parent = temp->parent;
				//set our parents child to our left child
				if (temp->parent->left == temp) {
					temp->parent->left = temp->left;
				}
				else {
					temp->parent->right = temp->left;
				}
				deletedPos = temp->left;
				//fix coloring if the node we delete / repalce is black
				if (temp->color == black) {
					if (deletedPos->color == black) {
						//both were black -> doubleBlack
						deletedPos->color = doubleBlack;
					}
					else {
						//temp was black -> color black to keep black property of tree
						deletedPos->color = black;
					}
				} //else deletedPos color does not have to be changed

				//free temp node
				if (temp->data)
					free(temp->data); //free data
				free(temp); //free node
			}
		}
		else {
			//left child does not exist -> check right child
			if (temp->right && temp->right->data) {
				//CASE 2: node only has a right child

				//set our right childs parent to our parent
				temp->right->parent = temp->parent;
				//set our parents child to our right child
				if (temp->parent->left == temp) {
					temp->parent->left = temp->right;
				}
				else {
					temp->parent->right = temp->right;
				}
				deletedPos = temp->right;
				//fix coloring if the node we delete / repalce is black
				if (temp->color == black) {
					if (deletedPos->color == black) {
						//both were black -> doubleBlack
						deletedPos->color = doubleBlack;
					}
					else {
						//temp was black -> color black to keep black property of tree
						deletedPos->color = black;
					}
				} //else deletedPos color does not have to be changed

				  //free temp node
				if (temp->data)
					free(temp->data); //free data
				free(temp); //free node
			}
			else {
				//CASE 1: node has no childs

				//check if we are a right or left child and set a NIL child in the parents pointer
				if (temp->parent->left == temp) {
					temp->parent->left = createNILchild(temp->parent);
					deletedPos = temp->parent->left;
				}
				else {
					temp->parent->right = createNILchild(temp->parent);
					deletedPos = temp->parent->right;
				}
				//if deleted node is black, the new NIL-child has to be double black: node(black)->NIL(black) to NIL(doubleblack)
				if (temp->color == black)
					deletedPos->color = doubleBlack;
		
				//delete temp with NIL childs
				freeTree(temp);
				return deletedPos;
			}
		}
	}
}
*/

void printAfterEachInsert(int amount, int sleep) {
	time_t t;
	/* Intializes random number generator */
	srand((unsigned)time(&t));

	//create root
	node* root = createNILchild(NULL);

	int array[10] = {76, 21, 75, 56, 57, 18, 92, 31, 27, 49};
	//fill tree with 'n' random numbers
	for (int i = 0; i < amount; i++) {
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
	Sleep(5000);
	for (int i = 0; i < amount; i++) {
		cout << "Delete: " << array[i] << endl;

		root = rb_delete(root, &array[i]);

		//print tree after each delete
		printDefault(root);
		cout << endl;
		Sleep(2000);
	}

	int blackNodeDepth = checkBlackNodeProperty(root);
	cout << "#ofBlackNodes: " << blackNodeDepth;

}


int main()
{
	printAfterEachInsert(10, 1);
	
	while (true) {
		updateFont();
		Sleep(100);
	};
}