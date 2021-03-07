#ifndef EXP_TREE
#define EXP_TREE

#include <string>
#include <stack>
#include <iostream>
#include <cmath>
#include<bits/stdc++.h> 

using namespace std;

class ExpTree
{
protected:
	string payload;
	ExpTree *leftSubTree, *rightSubTree;
public:
	//constructor
	ExpTree();
	//clones the ExpTree recursively and returns a pointer to the clone
	ExpTree * duplicate();
	//constructs an ExpTree from a given string formatted in postfix
	void construct(string PF);
	//recursively destroys tree and all its children
	void remove();
	//recursively displays tree inorder showing order of operations with parantheses
	virtual void inorder();
	//recursively tranforms the ExpTree into an ExpTree representing its derivative 
	void derive();
	//performs routine simplifications on the Expression tree where possible
	void simplify();
	//takes a given string variable by reference and appends the ExpTree in postfix notation to it recursively
	void postfix(string& pf);
	//displays an ExpTree without outer parans and ends line
	void display();
    bool isNumber(char c);
    bool isOperand(char c);
    bool isOperand(string s);
    bool isOperator(char c);
    bool isX(char c);

};

#endif // EXP_TREE