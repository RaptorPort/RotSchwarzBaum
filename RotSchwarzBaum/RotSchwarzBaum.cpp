// RotSchwarzBaum.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"

#include <time.h> 
#include "math.h"


struct node {
	bool black;
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

int main()
{
	time_t t;
	/* Intializes random number generator */
	srand((unsigned)time(&t));

    return 0;
}

void insert(node *node ,int *data) {
	struct node *temp = node;
	//schauen ob Knoten NIL ist
	if (node->data == NULL) {
		node->black = true;

		//create left NIL child
		node->left = createNILchild(node);

		//create right NIL child
		node->right = createNILchild(node);

		//set pointer for data
		node->data = data;
	}
	else {
		//find right NIL child to insert node for data
		while (temp->data) {
			if (*temp->data > *data) {
				temp = temp->right;
			}
			else {
				temp = temp->left;
			}
		}
		temp->black = false;
		temp->data = data;
		temp->left = createNILchild(temp);
		temp->right = createNILchild(temp);
	}
	//We inserted our node -> check which case violation we have and FIX THE TREE
	switch (getCase(temp)) {
	case 31: // right side line

		break;
	case 30: // left side line
		break;
	case 20: // triangle points left
		break;
	case 21: // triangle points right
		break;

	}
}

void rotateLeft(node *r) {

}

int getCase(node *temp) {
	//do we have a uncle? is our uncle a nigga?
	if (temp->getuncle() != NULL && temp->getuncle()->black) {
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

node* createNILchild(node *parent) {
	struct node *nilNode = (struct node *)malloc(sizeof(struct node));
	nilNode->black = true;
	nilNode->data = NULL;
	nilNode->parent = parent;
	nilNode->right = NULL;
	nilNode->left = NULL;
}
