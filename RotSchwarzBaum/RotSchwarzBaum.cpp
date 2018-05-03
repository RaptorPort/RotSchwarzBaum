// RedBlackTree.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

//#include "stdafx.h"

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

#define COUNT 10 // for tree output

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

char* print2DUtil(node *root) {
	//stop at NILchild
	if (root->data == NULL)
		return 0;

	//process left child
	if (root->left)
		print2DUtil(root->left);

	//process right child
	if (root->right)
		print2DUtil(root->right);

	//process current node

}

// Wrapper over print2DUtil()
void print2D(node *root)
{
	// Pass initial space count as 0
	print2DUtil(root, 0);
}



//------------------------------------------------------------------OUTPUT-----------------------------------------------------------------------------------------
//https://articles.leetcode.com/how-to-pretty-print-binary-tree/#

void removeNilNodes(node* p) {
	if (p == nullptr)
		return;
	//stop at NILchild
	if (p->data == NULL)
		p->parent->left == p ? p->parent->left = NULL : p->parent->right = NULL;

	removeNilNodes(p->left);
	removeNilNodes(p->right);
}

int maxHeight(node* p) {
	if (p == nullptr)
		return 0;
	//stop at NILchild
	if (p->data == NULL)
		return 1;

	//get deeper subtree
	int l = maxHeight(p->left);
	int r = maxHeight(p->right);
	
	return l >= r ? l+1 : r+1;
}

// Convert an integer value to string
string intToString(int val) {
	return std::to_string(val);
}

// Print the arm branches (eg, /    \ ) on a line
void printBranches(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<node*>& nodesQueue, ostream& out) {
	deque<node*>::const_iterator iter = nodesQueue.begin();
	for (int i = 0; i < nodesInThisLevel / 2; i++) {
		out << ((i == 0) ? setw(startLen - 1) : setw(nodeSpaceLen - 2)) << "" << ((*iter++) ? "/" : " ");
		out << setw(2 * branchLen + 2) << "" << ((*iter++) ? "\\" : " ");
	}
	out << endl;
}

// Print the branches and node (eg, ___10___ )
void printNodes(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<node*>& nodesQueue, ostream& out) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hstdout, &csbi);

	deque<node*>::const_iterator iter = nodesQueue.begin();
	for (int i = 0; i < nodesInThisLevel; i++, iter++) {
		out << ((i == 0) ? setw(startLen) : setw(nodeSpaceLen)) << "" << ((*iter && (*iter)->left) ? setfill('_') : setfill(' '));
		out << setw(branchLen + 2);
		//make text red
		if ((*iter) && (*iter)->color == red)
			SetConsoleTextAttribute(hstdout, 0x04);
        out << ((*iter) ? (*iter)->data ? intToString(*(*iter)->data) : "N" : "");
		out << ((*iter && (*iter)->right) ? setfill('_') : setfill(' ')) << setw(branchLen) << "" << setfill(' ');
		SetConsoleTextAttribute(hstdout, csbi.wAttributes);

	}
	out << endl;
}

// Print the leaves only (just for the bottom row)
void printLeaves(int indentSpace, int level, int nodesInThisLevel, const deque<node*>& nodesQueue, ostream& out) {
	deque<node*>::const_iterator iter = nodesQueue.begin();
	for (int i = 0; i < nodesInThisLevel; i++, iter++) {
		out << ((i == 0) ? setw(indentSpace + 2) : setw(2 * level + 2)) << ((*iter) ? (*iter)->data ? intToString(*(*iter)->data) : "N" : "");
	}
	out << endl;
}

// Pretty formatting of a binary tree to the output stream
// @ param
// level  Control how wide you want the tree to sparse (eg, level 1 has the minimum space between nodes, while level 2 has a larger space between nodes)
// indentSpace  Change this to add some indent space to the left (eg, indentSpace of 0 means the lowest level of the left node will stick to the left margin)
void printPretty(node *root, int level, int indentSpace, ostream& out) {

	int h = maxHeight(root);
	int nodesInThisLevel = 1;

	int branchLen = 2 * ((int)pow(2.0, h) - 1) - (3 - level)*(int)pow(2.0, h - 1);  // eq of the length of branch for each node of each level
	int nodeSpaceLen = 2 + (level + 1)*(int)pow(2.0, h);  // distance between left neighbor node's right arm and right neighbor node's left arm
	int startLen = branchLen + (3 - level) + indentSpace;  // starting space to the first node to print of each level (for the left most node of each level only)

	deque<node*> nodesQueue;
	nodesQueue.push_back(root);
	for (int r = 1; r < h; r++) {
		printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
		branchLen = branchLen / 2 - 1;
		nodeSpaceLen = nodeSpaceLen / 2 + 1;
		startLen = branchLen + (3 - level) + indentSpace;
		printNodes(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);

		for (int i = 0; i < nodesInThisLevel; i++) {
			node *currNode = nodesQueue.front();
			nodesQueue.pop_front();
			if (currNode) {
				nodesQueue.push_back(currNode->left);
				nodesQueue.push_back(currNode->right);
			}
			else {
				nodesQueue.push_back(NULL);
				nodesQueue.push_back(NULL);
			}
		}
		nodesInThisLevel *= 2;
	}
	printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
	printLeaves(indentSpace, level, nodesInThisLevel, nodesQueue, out);
}

//------------------------------------------------------------------OUTPUT-----------------------------------------------------------------------------------------

int main()
{
	time_t t;
	/* Intializes random number generator */
	srand((unsigned)time(&t));

	//create root
	node* root = createNILchild(NULL);

	cout << "Insert";
	//fill tree with numbers
	for (int i = 0; i < 20; i++) {
		int *newNumber = (int*)malloc(sizeof(int));
		*newNumber = rand() % 100;
		//cout << " " << *newNumber;
		cout << "Insert: " << *newNumber << endl;
		rb_insert(root, newNumber);
		printPretty(root, 1, 0, cout);
		
		//printPretty(root, 1, 0, cout);
	}
	cout << ": " << endl;

	//print tree
	removeNilNodes(root);
	printPretty(root, 1, 0, cout);

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