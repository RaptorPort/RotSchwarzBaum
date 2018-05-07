//tools.cpp Helper funktions for printing and input to the tree

#include "mynode.h"

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
//#include <string>

CONSOLE_SCREEN_BUFFER_INFO csbi;
HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);


#define AUTO_HIDE_NIL_CHILDS 6 //set max tree height up until which NIL childs should be drawn! -> -1 to always show!
using namespace std;


//just copy the data and color, the rest are pointers which will be different 
node* cloneNode(node *p) {
	if (p) {
		node* cpy = (struct node *)malloc(sizeof(struct node));
		if (p->data) {
			int* data = (int*)malloc(sizeof(int));
			*data = *p->data;
			cpy->data = data;
		}
		else {
			cpy->data = NULL;
		}
		cpy->color = p->color;
		return cpy;
	}
	return NULL;
}

node* copyTree(node* orgTree, node *parent) {
	if (orgTree == NULL)
		return NULL;
	//create node and copy data
	node* cpy = cloneNode(orgTree);
	cpy->parent = parent;

	cpy->left = copyTree(orgTree->left, cpy);
	cpy->right = copyTree(orgTree->right, cpy);

	return cpy;
}

void freeTree(node* p) {
	//free left subtree
	if (p->left) {
		freeTree(p->left);
	}
	//free right subtree
	if (p->right) {
		freeTree(p->right);
	}
	//free current node
	if (p->data)
		free(p->data); //free data
	free(p); //free node
}
/*
//returns the successor of the given node
node* successor(node *p) {
	node *y = NULL;
	if (p->left && p->left->data) {
		y = p->left;
		while (y->right->data)
			y = y->right;
	}
	else if (p->right && p->right->data)
	{
		y = p->right;
		while (y->left && y->left->data)
			y = y->left;
	}
	return y;
}
*/
node* successor(node *p) {
	node *y = NULL;
	if (p->right && p->right->data) {
		y = p->right;
		while (y->left->data)
			y = y->left;
	}
	else if (p->left && p->left->data)
	{
		y = p->left;
		while (y->right && y->right->data)
			y = y->right;
	}
	return y;
}

int checkBlackNodeProperty(node *p) {
	//abort on nullptr
	if (!p)
		return 0;

	if (!p->data && p->color == black)
		return 1;

	//left node
	int l = p->color == black ? 1 + checkBlackNodeProperty(p->left) : checkBlackNodeProperty(p->left);

	//right node
	int r = p->color == black ? 1 + checkBlackNodeProperty(p->right) : checkBlackNodeProperty(p->right);

	//check if it is the same
	if (r == l) {
		return r;
	}
	else {
		return -100;
	}
}

int countDataNodes(node *p) {
	//abort on nullptr
	if (!p)
		return 0;

	if (!p->data)
		return 0;

	return countDataNodes(p->left) + countDataNodes(p->right) + 1;
}

#define COUNT 10 // for tree output
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

//------------------------------------------------------------------OUTPUT-----------------------------------------------------------------------------------------
//https://articles.leetcode.com/how-to-pretty-print-binary-tree/# with modifications to our tree and coloring

void removeNilNodes(node* p) {
	if (p == nullptr)
		return;
	//stop at NILchild
	if (p->data == NULL) {
		if (p->parent->left == p) {
			p->parent->left = NULL;
		}
		else {
			p->parent->right = NULL;
		}
		free(p);
	}

	removeNilNodes(p->left);
	removeNilNodes(p->right);
}

int maxHeight(node* p) {
	if (p == NULL)
		return 0;
	//stop at NILchild
	//if (p->data == NULL)
	//	return 1;

	//get deeper subtree
	int l = maxHeight(p->left);
	int r = maxHeight(p->right);

	return l >= r ? l + 1 : r + 1;
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
		//reset to previous settings
		SetConsoleTextAttribute(hstdout, csbi.wAttributes);

	}
	out << endl;
}

// Print the leaves only (just for the bottom row)
void printLeaves(int indentSpace, int level, int nodesInThisLevel, const deque<node*>& nodesQueue, ostream& out) {
	GetConsoleScreenBufferInfo(hstdout, &csbi);

	deque<node*>::const_iterator iter = nodesQueue.begin();
	for (int i = 0; i < nodesInThisLevel; i++, iter++) {
		out << ((i == 0) ? setw(indentSpace + 2) : setw(2 * level + 2));
		//make text red
		if ((*iter) && (*iter)->color == red)
			SetConsoleTextAttribute(hstdout, 0x04);
		out << ((*iter) ? (*iter)->data ? intToString(*(*iter)->data) : "N" : "");
		//reset to previous settings
		SetConsoleTextAttribute(hstdout, csbi.wAttributes);
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

//Default RB-Print - Wrapper over printPretty
void printDefault(node *p) {
	if (maxHeight(p) <= AUTO_HIDE_NIL_CHILDS && AUTO_HIDE_NIL_CHILDS != -1) {
		//print tree with NIL childs
		printPretty(p, 1, 0, cout);
	}
	else {
		//make a copy
		node* cpy = copyTree(p, NULL);
		//delete NIL childs in copy
		removeNilNodes(cpy);
		printPretty(cpy, 1, 0, cout);
		freeTree(cpy);
	}
}
//------------------------------------------------------------------OUTPUT-----------------------------------------------------------------------------------------

void updateFont() {
	CONSOLE_FONT_INFOEX cfi;
	GetCurrentConsoleFontEx(hstdout, false, &cfi);

	SHORT keyStateP = GetKeyState('P');
	SHORT keyStateO = GetKeyState('O');
	if (keyStateP & 0x8000) {
		cfi.cbSize = sizeof(cfi);
		cfi.dwFontSize.Y++;
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	}
	else if (keyStateO & 0x8000) {

		cfi.cbSize = sizeof(cfi);
		if (cfi.dwFontSize.Y > 1)
			cfi.dwFontSize.Y--;
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	}
}

CONSOLE_FONT_INFOEX initFont() {
	//set FONT
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   // Width of each character in the font
	cfi.dwFontSize.Y = 20;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	return cfi;
}