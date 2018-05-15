// RedBlackTree.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "mynode.h"
#include "tools.h"

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <time.h> 
#include <string>
#include <Windows.h>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;

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

string outP(node* p) {
	if (!p)
		return "";
	if (p->data)
		return std::to_string(*p->data);
	return "NIL";
}

node* rb_delete(node *T, int *data) {
	//cases and explanation http://www.stolerman.net/studies/cs521/red_black_trees.pdf
	//perform basic AVL-Tree deletion
	node* p = AVLdelete(T, data);
	//returns child if it is double black or NULL if nothing is to be fixed
	
	
	if (p) {
		#ifdef DEBUG
		cout << "FIX double black at " << outP(p) << ", in Tree: "<< endl;
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
					cout << "Double Black: " << outP(p) << endl;
					cout << "Our sibling " << outP(p->getgrandparent()) << " is red -> Case 1: ";
					cout << "Rotate left around " << outP(p->parent) << " . Switch colors of father " << outP(p->parent) <<" and grandfather " << outP(p->getgrandparent()) << ":" << endl;
					debugPrint(p);
					#endif
					
	
				}
				//sibling is now black
				if (s->right && s->left && s->right->color == black && s->left->color == black)
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
					cout << "Double Black: " << outP(p) << endl;
					cout << "Sibling " << outP(s) << " and its childs are black -> Case 2: Color sibling " << outP(s) << " red. Move problem up the Tree:" << endl;
					debugPrint(p);					
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
						cout << "Double Black: " << outP(p) << endl;
						cout << "Sibling " << outP(s->right) << " is black, with left child " << outP(s) << " red and right child " << outP(s->right->right) << " black -> Case 3: ";
						cout << "Rotate right around sibling " << outP(s->right) << ". Switch colors between new " << outP(s) << " and old " << outP(s->right) << " siblings." << endl;
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
					cout << "Double Black: " << outP(p) << endl;
					cout << "Sibling " << outP(p->parent->right) << " is black, with right child " << outP(p->parent->right->right) << " red -> Case 4: ";
					cout << "Rotate left around father " << outP(p->parent) << ". Color father " << outP(p->parent) << " with extra black. Color uncle " << outP(p->getuncle()) << " black -> DONE";
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
					cout << "Double Black: " << outP(p) << endl;
					cout << "Our sibling " << outP(p->getgrandparent()) << " is red -> Case 1: ";
					cout << "Rotate right around " << outP(p->parent) << " . Switch colors of father " << outP(p->parent) << " and grandfather " << outP(p->getgrandparent()) << ":" << endl;
					debugPrint(p);
					#endif
				}
				if (s->left->color == black && s->right->color == black)
				{	//case parent red -> simple fix
					if (p->parent->color == red) {
						p->parent->color = black;
						s->color = red;
						#ifdef DEBUG
						cout << "Double Black: " << outP(p) << endl;
						cout << "Parent " << outP(p->parent) << " red, with black  -> Case 2: Color sibling " << outP(s) << " red. Move problem up the Tree:" << endl;
						debugPrint(p);
						#endif
						p = p->parent;
						break;
					}	
					//Case 2
					s->color = red;
					#ifdef DEBUG
					cout << "Double Black: " << outP(p) << endl;
					cout << "Sibling " << outP(s) << " and its childs are black -> Case 2: Color sibling " << outP(s) << " red. Move problem up the Tree:" << endl;
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
						cout << "Double Black: " << outP(p) << endl;
						cout << "Sibling " << outP(s->left) << " is black, with right child " << outP(s) << " red and left child " << outP(s->left->left) << " black -> Case 3: ";
						cout << "Rotate left around sibling " << outP(s->left) << ". Switch colors between new " << outP(s) << " and old " << outP(s->left) << " siblings: " << endl;
						debugPrint(p);
						#endif
					}
					//Case 4
					
					s->color = p->parent->color;
					p->parent->color = black;
					s->left->color = black;
					rotateRight(p->parent);
					#ifdef DEBUG
					cout << "Double Black: " << outP(p) << endl;
					cout << "Sibling " << outP(p->parent->right) << " is black, with right child " << outP(p->parent->right->right) << " red -> Case 4: ";
					cout << "Rotate right around father " << outP(p->parent) << ". Color father " << outP(p->parent) << " with extra black. Color uncle " << outP(p->getuncle()) << " black -> DONE";
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

void stressTestTree() {
	time_t t;
	/* Intializes random number generator */
	srand((unsigned)time(&t));

	
	while (true) {
		//create root
		node* root = createNILchild(NULL);

		int blackNodeDepth = 0;
		//fill tree with 'n' random numbers
		//cout << "INSERT: ";
		while (blackNodeDepth < 13) {
			blackNodeDepth = checkBlackNodeProperty(root);

			int *newNumber = (int*)malloc(sizeof(int));

			*newNumber = rand() % 100; //testarray[i];

			root = rb_insert(root, newNumber);

			//cout << *newNumber << ", ";

			if (blackNodeDepth < 0) {
				printDefault(root);
				cout << "#ofBlackNodes: " << blackNodeDepth << endl;
				int var;
				cin >> var;
			}
		}
		//cout << endl << "DELETE: ";
		while (blackNodeDepth > 2) {
			blackNodeDepth = checkBlackNodeProperty(root);

			int *newNumber = (int*)malloc(sizeof(int));
			//*newNumber = array[i];
			*newNumber = rand() % 100;

			root = rb_delete(root, newNumber);

			//cout << *newNumber << ", ";

			if (blackNodeDepth < 0) {
				printDefault(root);
				cout << "#ofBlackNodes: " << blackNodeDepth << endl;
				int var;
				cin >> var;
			}
			free(newNumber);
		}
		cout << endl;
		//printDefault(root);
		blackNodeDepth = checkBlackNodeProperty(root);
		cout << "#ofBlackNodes: " << blackNodeDepth << endl;
		//freeTree(root);
	}
}

std::vector<int> ReadNumbers()
{
	std::vector<int> numbers;

	do
	{
		int input;
		if (std::cin >> input)
			numbers.push_back(input);
	} while (std::cin && std::cin.peek() != '\n');

	return numbers;
}

void manual() {
	int printSetting = 1;
	bool stats = true;
	int maxPrintHeight = 8;
	int maxNILHeight = 6;
	vector<int> in;
	vector<int> del;
	//create root
	node* root = createNILchild(NULL);

	cout << "Commands: " << endl;
	cout << "stats                                - show/hide stats" << endl;
	cout << "maxPrintHeight <amount>              - maximum height up until which the tree will be printed" << endl;
	cout << "maxNILHeight <amount>                - maximum height up until which NIL nodes will be printed" << endl;
	cout << "print <number>                       - <0> print deletion steps <1> print after each action <2> print once when done" << endl;
	cout << "fontSize <amount>                    - set the font size" << endl;
	cout << "in <int> <int> ... <int>             - insert <numbers> into RB-Tree" << endl;
	cout << "del <int> <int> ... <int>            - delete <numbers> from RB-Tree" << endl;
	cout << "ranIn <amount> <range>               - insert #amount of random numbers, ranging from 0 to #range" << endl;
	cout << "ranDel <amount> <range>              - delete #amount of random numbers, ranging from 0 to #range" << endl;
	
	cout << endl;

	while (true) {
		//cin.ignore();
		string mystring;
			cin >> mystring;
			if (mystring == "in")
				in = ReadNumbers();
			else if (mystring == "del")
				del = ReadNumbers();
			else if (mystring == "ranIn") {
				int size = 100;
				int amount = 10;
				time_t t;
				/* Intializes random number generator */
				srand((unsigned)time(&t));
				//get parameters
				cin >> amount >> size;
				//fill array
				for (int i = 0; i < amount; i++) {
					in.push_back(rand() % size);
				}
			}
			else if (mystring == "ranDel") {
				int size = 100;
				int amount = 10;
				time_t t;
				/* Intializes random number generator */
				srand((unsigned)time(&t));
				//get parameters
				cin >> amount >> size;
				//fill array
				for (int i = 0; i < amount; i++) {
					del.push_back(rand() % size);
				}
			}
			else if (mystring == "stats") {
				if (stats) {
					stats = false;
					cout << "Stats are now hidden!" << endl;
				}
				else {
					stats = true;
					cout << "Showing stats!" << endl;
				}
			}
			else if (mystring == "maxPrintHeight") {
				cin >> maxPrintHeight;
			}
			else if (mystring == "maxNILHeight") {
				cin >> maxNILHeight;
			}
			else if (mystring == "print") {
				int x;
				cin >> x;
				if (0 <= x <= 2) {
					printSetting = 2;
					switch (x) {
					case 0:
						cout << "Printing after each action and inbetween deletion steps." << endl;
						break;
					case 1:
						cout << "Printing after each action." << endl;
						break;
					case 2:
						cout << "Printing 1 time when done." << endl;
						break;
					}
				}
			}
			else if (mystring == "fontSize") {
				int x;
				cin >> x;
				if (0 < x < 40)
					updateFont(x);
			}

			std::chrono::time_point<std::chrono::steady_clock> t1;
			if (printSetting == 2)
				t1 = Clock::now();

			while (!in.empty()) {
				int *newNumber = (int*)malloc(sizeof(int));
				*newNumber = in.back();
				in.pop_back();

				auto t1 = Clock::now();
				root = rb_insert(root, newNumber);
				auto t2 = Clock::now();
				if (printSetting <= 1) {
					cout << "Insert " << *newNumber << endl;
					printDefault(root, maxNILHeight, maxPrintHeight);
					if (stats)
						printStats(root, std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count());
					cout << endl;
				}
				
			}
			
			while (!del.empty()) {
				int *newNumber = (int*)malloc(sizeof(int));
				*newNumber = del.back();
				del.pop_back();

				auto t1 = Clock::now();
				root = rb_delete(root, newNumber);
				auto t2 = Clock::now();
				free(newNumber);
				if (printSetting <= 1) {
					cout << "Delete " << *newNumber << endl;
					printDefault(root, maxNILHeight, maxPrintHeight);
					if (stats)
						printStats(root, std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count());
					cout << endl;
				}
				
			}

			if (printSetting == 2) {
				auto t2 = Clock::now();
				printDefault(root, maxNILHeight, maxPrintHeight);
				if (stats)
					printStats(root, std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count());
				cout << endl;
			}
	}
}

int main()
{
	//printAfterEachInsert(100, 1);
	//stressTestTree();
	//printAfterEachInsertTestArray(1);

	manual();

	while (false) {
		updateFont();
		Sleep(100);
	};
}