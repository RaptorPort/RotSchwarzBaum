#pragma once

#ifndef MYNODE_H
#define MYNODE_H
enum color { red, black };

struct node {
	color color;
	int *data;
	node *parent;
	node *left;
	node *right;
	node* getgrandparent() {
		//check if grandparent exists and return pointer
		if (parent != 0 && parent->parent != 0)
			return parent->parent;
		return 0;
	}
	node* getuncle() {
		if (getgrandparent() == 0)
			return 0;
		if (getgrandparent()->left == parent) {
			return getgrandparent()->right;
		}
		else {
			return getgrandparent()->left;
		}
	}
	int getside() {
		if (parent == 0)
			return 0;
		if (parent->right == this) {
			return 1;
		}
		else {
			return -1;
		}
	}
};

#endif // !NODE