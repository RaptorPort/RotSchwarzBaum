#pragma once

// tools.h
#include <ostream>

#ifndef TOOLS
#define TOOLS

void printPretty(node *root, int level, int indentSpace, std::ostream& out);
void printDefault(node *p);
int maxHeight(node* p);
int checkBlackNodeProperty(node*p);
void updateFont();
node* successor(node *p);

void removeNilNodes(node* p);
node* copyTree(node* orgTree, node* parent);
void freeTree(node* p);

#endif