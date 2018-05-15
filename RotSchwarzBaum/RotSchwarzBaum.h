#pragma once

#ifndef RBTREE
#define RBTREE

enum color { red, black };
struct node {
	color color;
	int *data;
	node *parent;
	node *left;
	node *right;
	node* getgrandparent();
	node* getuncle();
};

node* rb_fix_insert(node *x);
node* rb_fix_delete(node *x);

void freeTree(node* p);
node* successor(node *p);
node* createNILchild(node *parent);

#endif