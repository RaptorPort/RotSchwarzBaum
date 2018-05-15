//tools.cpp Helper funktions for printing and input to the tree

#include "RotSchwarzBaum.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <chrono>

//OUTPUT STUFF
#include <Windows.h>
#include <string>
#include <cwchar>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ostream>
#include <deque>

CONSOLE_SCREEN_BUFFER_INFO csbi;
HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

typedef std::chrono::high_resolution_clock Clock;


#define AUTO_HIDE_NIL_CHILDS 6 //set max tree height up until which NIL childs should be drawn! -> -1 to always show!

//using namespace std;


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

void printStats(node *p) {
	std::cout << "#blackNodeHeight: " << checkBlackNodeProperty(p) << "   #treeHeight" << maxHeight(p) << "   #nodes: " << countDataNodes(p) << std::endl;
}

void printStats(node *p, long time) {
	std::cout << "#blackNodeHeight: " << checkBlackNodeProperty(p) << "   #treeHeight" << maxHeight(p) << "   #nodes: " << countDataNodes(p);
	if (time > 1000) {
		time /= 1000;
		if (time > 1000) {
			time /= 1000;
			std::cout << "   #time: " << time << " milliseconds" << std::endl;
		}
		else {
			std::cout << "   #time: " << time << " microseconds" << std::endl;
		}
	}
	else {
		std::cout << "   #time: " << time << " nanoseconds" << std::endl;
	}
}

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

//find node to insert into, for testing when data contains a pointer on int
node* findNodeToInsertInt(node* T, int *data) {
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
	}
	return temp;
}

//find node to delete, for testing when data contains a pointer on int
node* findNodeToDeleteInt(node* T, int *data) {
	node* temp = T;
	//search for the node, until it is found or a NIL-child is reached
	while (temp && temp->data && *temp->data != *data) {
		if (*temp->data < *data) {
			temp = temp->right;
		}
		else if (*temp->data > *data) {
			temp = temp->left;
		}
	}
}


//------------------------------------------------------------------OUTPUT-----------------------------------------------------------------------------------------
//https://articles.leetcode.com/how-to-pretty-print-binary-tree/# with modifications to our tree and coloring

// Convert an integer value to string
std::string intToString(int val) {
	return std::to_string(val);
}


// Print the arm branches (eg, /    \ ) on a line
void printBranches(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const std::deque<node*>& nodesQueue, std::ostream& out) {
	std::deque<node*>::const_iterator iter = nodesQueue.begin();
	for (int i = 0; i < nodesInThisLevel / 2; i++) {
		out << ((i == 0) ? std::setw(startLen - 1) : std::setw(nodeSpaceLen - 2)) << "" << ((*iter++) ? "/" : " ");
		out << std::setw(2 * branchLen + 2) << "" << ((*iter++) ? "\\" : " ");
	}
	out << std::endl;
}

// Print the branches and node (eg, ___10___ )
void printNodes(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const std::deque<node*>& nodesQueue, std::ostream& out) {
	GetConsoleScreenBufferInfo(hstdout, &csbi);

	std::deque<node*>::const_iterator iter = nodesQueue.begin();
	for (int i = 0; i < nodesInThisLevel; i++, iter++) {
		out << ((i == 0) ? std::setw(startLen) : std::setw(nodeSpaceLen)) << "" << ((*iter && (*iter)->left) ? std::setfill('_') : std::setfill(' '));
		out << std::setw(branchLen + 2);
		//make text red
		if ((*iter) && (*iter)->color == red)
			SetConsoleTextAttribute(hstdout, 0x04);
		out << ((*iter) ? (*iter)->data ? intToString(*(*iter)->data) : "N" : "");
		out << ((*iter && (*iter)->right) ? std::setfill('_') : std::setfill(' ')) << std::setw(branchLen) << "" << std::setfill(' ');
		//reset to previous settings
		SetConsoleTextAttribute(hstdout, csbi.wAttributes);

	}
	out << std::endl;
}

// Print the leaves only (just for the bottom row)
void printLeaves(int indentSpace, int level, int nodesInThisLevel, const std::deque<node*>& nodesQueue, std::ostream& out) {
	GetConsoleScreenBufferInfo(hstdout, &csbi);

	std::deque<node*>::const_iterator iter = nodesQueue.begin();
	for (int i = 0; i < nodesInThisLevel; i++, iter++) {
		out << ((i == 0) ? std::setw(indentSpace + 2) : std::setw(2 * level + 2));
		//make text red
		if ((*iter) && (*iter)->color == red)
			SetConsoleTextAttribute(hstdout, 0x04);
		out << ((*iter) ? (*iter)->data ? intToString(*(*iter)->data) : "N" : "");
		//reset to previous settings
		SetConsoleTextAttribute(hstdout, csbi.wAttributes);
	}
	out << std::endl;
}

// Pretty formatting of a binary tree to the output stream
// @ param
// level  Control how wide you want the tree to sparse (eg, level 1 has the minimum space between nodes, while level 2 has a larger space between nodes)
// indentSpace  Change this to add some indent space to the left (eg, indentSpace of 0 means the lowest level of the left node will stick to the left margin)
void printPretty(node *root, int level, int indentSpace, std::ostream& out) {

	int h = maxHeight(root);
	int nodesInThisLevel = 1;

	int branchLen = 2 * ((int)pow(2.0, h) - 1) - (3 - level)*(int)pow(2.0, h - 1);  // eq of the length of branch for each node of each level
	int nodeSpaceLen = 2 + (level + 1)*(int)pow(2.0, h);  // distance between left neighbor node's right arm and right neighbor node's left arm
	int startLen = branchLen + (3 - level) + indentSpace;  // starting space to the first node to print of each level (for the left most node of each level only)

	std::deque<node*> nodesQueue;
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
void printDefault(node *p, int show_nil_childs, int maxHeightToPrint) {
	int height = maxHeight(p);
	if (height <= maxHeightToPrint || maxHeightToPrint == -1) {
		if (height <= show_nil_childs && show_nil_childs != -1) {
			//print tree with NIL childs
			printPretty(p, 1, 0, std::cout);
		}
		else {
			//make a copy
			node* cpy = copyTree(p, NULL);
			//delete NIL childs in copy
			removeNilNodes(cpy);
			printPretty(cpy, 1, 0, std::cout);
			freeTree(cpy);
		}
	}
}

void printDefault(node *p) {
	printDefault(p, 6, 7);
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

void updateFont(int i) {
	CONSOLE_FONT_INFOEX cfi;
	GetCurrentConsoleFontEx(hstdout, false, &cfi);
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.Y = i;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
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
	std::vector<int> in;
	std::vector<int> del;
	//create root
	node* root = createNILchild(NULL);

	std::cout << "Commands: " << std::endl;
	std::cout << "stats                                - show/hide stats" << std::endl;
	std::cout << "maxPrintHeight <amount>              - maximum height up until which the tree will be printed" << std::endl;
	std::cout << "maxNILHeight <amount>                - maximum height up until which NIL nodes will be printed" << std::endl;
	std::cout << "print <number>                       - <0> print deletion steps <1> print after each action <2> print once when done" << std::endl;
	std::cout << "fontSize <amount>                    - set the font size" << std::endl;
	std::cout << "in <int> <int> ... <int>             - insert <numbers> into RB-Tree" << std::endl;
	std::cout << "del <int> <int> ... <int>            - delete <numbers> from RB-Tree" << std::endl;
	std::cout << "ranIn <amount> <range>               - insert #amount of random numbers, ranging from 0 to #range" << std::endl;
	std::cout << "ranDel <amount> <range>              - delete #amount of random numbers, ranging from 0 to #range" << std::endl;

	std::cout << std::endl;

	while (true) {
		//cin.ignore();
		std::string mystring;
		std::cin >> mystring;
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
			std::cin >> amount >> size;
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
			std::cin >> amount >> size;
			//fill array
			for (int i = 0; i < amount; i++) {
				del.push_back(rand() % size);
			}
		}
		else if (mystring == "stats") {
			if (stats) {
				stats = false;
				std::cout << "Stats are now hidden!" << std::endl;
			}
			else {
				stats = true;
				std::cout << "Showing stats!" << std::endl;
			}
		}
		else if (mystring == "maxPrintHeight") {
			std::cin >> maxPrintHeight;
		}
		else if (mystring == "maxNILHeight") {
			std::cin >> maxNILHeight;
		}
		else if (mystring == "print") {
			int x;
			std::cin >> x;
			if (0 <= x <= 2) {
				printSetting = 2;
				switch (x) {
				case 0:
					std::cout << "Printing after each action and inbetween deletion steps." << std::endl;
					break;
				case 1:
					std::cout << "Printing after each action." << std::endl;
					break;
				case 2:
					std::cout << "Printing 1 time when done." << std::endl;
					break;
				}
			}
		}
		else if (mystring == "fontSize") {
			int x;
			std::cin >> x;
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
			root = rb_fix_insert(findNodeToInsertInt(root, newNumber));
			auto t2 = Clock::now();
			if (printSetting <= 1) {
				std::cout << "Insert " << *newNumber << std::endl;
				printDefault(root, maxNILHeight, maxPrintHeight);
				if (stats)
					printStats(root, std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count());
				std::cout << std::endl;
			}

		}

		while (!del.empty()) {
			int *newNumber = (int*)malloc(sizeof(int));
			*newNumber = del.back();
			del.pop_back();

			auto t1 = Clock::now();
			root = rb_fix_delete(findNodeToInsertInt(root, newNumber));
			auto t2 = Clock::now();
			free(newNumber);
			if (printSetting <= 1) {
				std::cout << "Delete " << *newNumber << std::endl;
				printDefault(root, maxNILHeight, maxPrintHeight);
				if (stats)
					printStats(root, std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count());
				std::cout << std::endl;
			}

		}

		if (printSetting == 2) {
			auto t2 = Clock::now();
			printDefault(root, maxNILHeight, maxPrintHeight);
			if (stats)
				printStats(root, std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count());
			std::cout << std::endl;
		}
	}
}




int main()
{
	manual();
}