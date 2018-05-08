#pragma once

// tools.h
#include <ostream>

#ifndef TOOLS
#define TOOLS

void printPretty(node *root, int level, int indentSpace, std::ostream& out);
void printDefault(node *p, int show_nil_childs, int maxHeightToPrint);
void printDefault(node *p);
void printStats(node *p, long time);
void printStats(node *p);
void updateFont(int i);
void updateFont();
void updateFont(int i);
int countDataNodes(node *p);

int maxHeight(node* p);
int checkBlackNodeProperty(node*p);
node* successor(node *p);

void removeNilNodes(node* p);
node* copyTree(node* orgTree, node* parent);
void freeTree(node* p);

#endif