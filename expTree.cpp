#include "expTree.h"
#include <iostream>
using namespace std;
#include<bits/stdc++.h> 

ExpTree::ExpTree()
{
	payload = "";
	leftSubTree = NULL;
	rightSubTree = NULL;
}

/* 
@brief Function that duplicates the expression tree recursivley and 
returns a pointer to the duplicate

*/
ExpTree* ExpTree::duplicate()
{
	//Dynamically allocating duplicate tree
	ExpTree *duplicateTree = new ExpTree();
	//Root node payload of duplicate tree is equal to root node payload of referenced tree
	duplicateTree->payload = payload;					
	//Copying left and right trees recursively 
	if (leftSubTree != NULL){
		duplicateTree->leftSubTree = leftSubTree->duplicate();
	}  
	if (rightSubTree != NULL){
		duplicateTree->rightSubTree = rightSubTree->duplicate();
	} 
	//Returning pointer to duplicate tree
	return duplicateTree;
}


/*
@brief Function that constructs the expression tree using the postfix form of
inputted equation
*/
void ExpTree::construct(string postFix)
{
	stack<ExpTree*> treeStack;		//will help to obey the order of operations
	ExpTree *tree1, *tree2, *tree3;				//	specified by the postfix string
	string val = "";

	while (postFix.length() > 0){
		//Removing whitespace
		if (postFix[0] == ' '){
			postFix.erase(0, 1);				
		}
		else if (this->isOperand(postFix))
		{
			while (this->isOperand(postFix))
			{
				val += postFix[0];			
				postFix.erase(0, 1);
			}

			ExpTree *t;			//make a new expression tree with val
			t = new ExpTree();
			t->payload = val;
			treeStack.push(t);
			val = "";
		}
		else if (this->isOperator(postFix[0]))
		{
			val += postFix[0];
			postFix.erase(0, 1);
			tree1 = treeStack.top();
			treeStack.pop();
			tree2 = treeStack.top();
			treeStack.pop();

			ExpTree *t;
			t = new ExpTree();		//take two trees off stack and make a new tree 
			t->payload = val;					//with operator payload, then push its ptr on the stack
			t->leftSubTree = tree2;
			t->rightSubTree = tree1;
			treeStack.push(t);
			val = "";
		}
	}	//END LOOP

	payload = treeStack.top()->payload;
	leftSubTree = treeStack.top()->leftSubTree;
	rightSubTree = treeStack.top()->rightSubTree;
	treeStack.pop();
}

void ExpTree::inorder()
{
	if (leftSubTree != NULL)	cout << "(";
	if (leftSubTree != NULL)	leftSubTree->inorder();
	cout << ' ' << payload << ' ';
	if (rightSubTree != NULL)	rightSubTree->inorder();
	if (rightSubTree != NULL)	cout << ")";
}

void ExpTree::display()
{
	if (leftSubTree != NULL)	leftSubTree->inorder();
	cout << ' ' << payload << ' ';
	if (rightSubTree != NULL)	rightSubTree->inorder();
	cout << endl;
}

/*
@brief recursively deleting a subtree 
*/
void ExpTree::remove()
{
	if (leftSubTree != NULL){
		leftSubTree->remove();
	}
	if (rightSubTree != NULL){
		rightSubTree->remove();
	}	
	delete this;
}

// A utility function to check if 'c'
// is an operator
bool ExpTree::isOperator(char c)
{
    if (c == '+' || c == '-' ||
            c == '*' || c == '/' ||
            c == '^')
        return true;
    return false;
}

/*
@brief Takes in a character and checks if it is x. If so, return true, else return false
@param c: character that we check 
*/
bool ExpTree::isX(char c)
{
	if (c == 'x') return true;
	else{
		return false;
	}
}

bool ExpTree::isNumber(char c)
{
	if (c >= '0' && c <= '9'){
		return true;
	} 
	return false;
}

bool ExpTree::isOperand(char c)
{
	if (c >= 'a' && c <= 'z') return true;
	if (c >= '0' && c <= '9') return true;
	return false;
}
bool ExpTree::isOperand(string s)
{
	if (isNumber(s[0]))	return true;
	if (isX(s[0]))	return true;
	return false;
}

/*
@brief Convert the expression tree of postfixed user input into its derivative 
*/
void ExpTree::derive()
{
	//Declare 5 expression trees 
	ExpTree *tree1, *tree2, *tree3, *tree4, *tree5;
	//Base Case: if the payload of the root node of subtree is x, then the derivative is 1 
	if (this->isX(payload[0])){
		payload = "1";
	}
	//Base Case: if the payload at the root is a constant, then the derivative is 0 
	else if (this->isNumber(payload[0])){
		payload = "0";
	}
	// The payload is an operator
	else
	{
		switch (payload[0]){	
			//For additions, you add the derivatives of the left and right subtrees
			case '+':
				leftSubTree->derive();
				rightSubTree->derive();
				break;	
			//For subtraction, you subtract the derivatives of the left and right subtrees
			case '-':
				leftSubTree->derive();
				rightSubTree->derive();
				break;	
			//Product rule, you use four of the subtrees declared above and do: tree1 * (tree2)' + tree3 * (tree4)'
			case '*':	
				//duplicates of leftSubTree and rightSubTree to use for T1*T2' + T3*T4' (product rule)
				tree1 = leftSubTree->duplicate();
				tree2 = rightSubTree->duplicate();

				tree3 = rightSubTree->duplicate();
				tree4 = leftSubTree->duplicate();

				//Parent nodes will be the multiplication operator
				leftSubTree->payload = "*";	
				rightSubTree->payload = "*";

				//Grandparent node will be the addition operator since we add both products
				payload = "+";

				//removeing children of the multiplication signs, since we will repopulate
				if (leftSubTree->leftSubTree != NULL){
					leftSubTree->leftSubTree->remove();
				}		
				if (leftSubTree->rightSubTree != NULL){
					leftSubTree->rightSubTree->remove();
				}  

				//Apply the product rule
				leftSubTree->leftSubTree = tree1;
				leftSubTree-> rightSubTree = tree2;

				//Derive tree2
				tree2->derive();

				if(rightSubTree->leftSubTree != NULL){
					rightSubTree->leftSubTree->remove();
				}
				if(rightSubTree->rightSubTree != NULL){
					rightSubTree->rightSubTree->remove();
				}

				rightSubTree->leftSubTree = tree3;
				rightSubTree->rightSubTree = tree4;
				tree4->derive();
				break;

			//Quotient rule: (tree1 * tree2' - tree3 * tree4')/(tree5^2)
			case '/':	
				tree1 = leftSubTree->duplicate();
				tree2 = rightSubTree->duplicate();
				tree3 = rightSubTree->duplicate();
				tree4 = leftSubTree->duplicate();
				tree5 = rightSubTree->duplicate();

				//Instead of addition between products, we will subtract 
				leftSubTree->payload = "-";

				//Setting up the expression tree:        /
				//                                  -         ^
				//								*      *
				if (leftSubTree->leftSubTree == NULL){
					leftSubTree->leftSubTree = new ExpTree();
					leftSubTree->leftSubTree->payload = "*";
				}
				//Cleaning children to avoid orphans
				if (leftSubTree->leftSubTree->leftSubTree != NULL){
					leftSubTree->leftSubTree->leftSubTree->remove();
				}
				if (leftSubTree->leftSubTree->rightSubTree != NULL){
					leftSubTree->leftSubTree->rightSubTree->remove();
				}
				//Setting up the numerator
				leftSubTree->leftSubTree->leftSubTree = tree1;
				leftSubTree->leftSubTree->rightSubTree = tree2;
				tree1->derive();
				if (leftSubTree->rightSubTree == NULL){
					leftSubTree->rightSubTree = new ExpTree();
					leftSubTree->rightSubTree->payload = "*";
				}
				//Remove already existing branches??
				if (leftSubTree->rightSubTree->leftSubTree != NULL){
					leftSubTree->rightSubTree->leftSubTree->remove();
				}
				if (leftSubTree->rightSubTree->rightSubTree != NULL){
					leftSubTree->rightSubTree->rightSubTree->remove();
				}
				leftSubTree->rightSubTree->leftSubTree = tree3;	
				leftSubTree->rightSubTree->rightSubTree = tree4;	
				tree3->derive();

				//Denominator
				rightSubTree->payload = "^";
				rightSubTree->leftSubTree = tree5;
				if (rightSubTree->rightSubTree == NULL){
					rightSubTree->rightSubTree = new ExpTree();
					rightSubTree->rightSubTree->payload = "2";
				}

				if (rightSubTree->rightSubTree->leftSubTree != NULL){
					rightSubTree->rightSubTree->leftSubTree->remove();
					rightSubTree->rightSubTree->leftSubTree = NULL;
				}
				if (rightSubTree->rightSubTree->rightSubTree != NULL)
				{
					rightSubTree->rightSubTree->rightSubTree->remove();
					rightSubTree->rightSubTree->rightSubTree = NULL;
				}

				break;
			// //Chain rule
			// case '^':
			// 	ExpTree * base = leftSubTree->duplicate();	//allows for chain rule if enter for ex (x+2)^2
			// 	ExpTree* power = rightSubTree->duplicate();
			// 	payload = "*";
			// 	leftSubTree->remove();
			// 	rightSubTree->remove();
			// 	leftSubTree = NULL;
			// 	rightSubTree = NULL;

			// 	if (isNum(power->payload) && isNum(base->payload))
			// 	{
			// 		payload = "0";
			// 	}
			// 	else if (isNum(power->payload))
			// 	{
			// 		rightSubTree = new ExpTree();
			// 		rightSubTree->payload = "^";

			// 		double powerDouble = stod(power->payload);
			// 		int decIndex;

			// 		rightSubTree->rightSubTree = new ExpTree();
			// 		rightSubTree->rightSubTree->payload = to_string((powerDouble - 1));
			// 		decIndex = rightSubTree->rightSubTree->payload.find('.');
			// 		rightSubTree->rightSubTree->payload = rightSubTree->rightSubTree->payload.substr(0, decIndex + 2);	//set number of decimal places for number payloads that have been altered

			// 		rightSubTree->leftSubTree = base->duplicate();


			// 		if (!isOperand(base->payload))
			// 		{
			// 			ExpTree * toDeriv;
			// 			toDeriv = base->duplicate();
			// 			toDeriv->deriv();
			// 			leftSubTree = new ExpTree();
			// 			leftSubTree->payload = "*";
			// 			leftSubTree->leftSubTree = toDeriv;
			// 			leftSubTree->rightSubTree = power->duplicate();
			// 		}
			// 		else
			// 		{
			// 			leftSubTree = power->duplicate();
			// 		}


			// 	}
			// 	else					//	TODO: exponential functions
			// 	{


			// 		leftSubTree = new ExpTree();
			// 		leftSubTree->payload = "^";

			// 		leftSubTree->leftSubTree = base->duplicate();
			// 		leftSubTree->rightSubTree = power->duplicate();

			// 		if (power->leftSubTree != NULL && power->rightSubTree != NULL)		//chain rule if there is more than a single numeric payload in power
			// 		{
			// 			rightSubTree = new ExpTree();
			// 			rightSubTree->payload = "*";
			// 			rightSubTree->leftSubTree = power->duplicate();
			// 			rightSubTree->leftSubTree->deriv();
			// 			rightSubTree->rightSubTree = new FunctionLeaf();
			// 			rightSubTree->rightSubTree->leftSubTree = new ExpTree();
			// 			rightSubTree->rightSubTree->leftSubTree->payload = "ln";
			// 			rightSubTree->rightSubTree->rightSubTree = base->duplicate();

			// 		}
			// 		else											//do exponential function differentiation rule w/o chain rule
			// 		{
			// 			rightSubTree = new FunctionLeaf();

			// 			rightSubTree->leftSubTree = new ExpTree();
			// 			rightSubTree->leftSubTree->payload = "ln";

			// 			rightSubTree->rightSubTree = new ExpTree();
			// 			rightSubTree->rightSubTree = base->duplicate();
			// 		}

			// 	}


			// 	power->remove();
			// 	base->remove();

			// 	break;
		}
	}
}

void ExpTree::simplify()
{
	if (this->isNumber(payload[0]))	return;
	if (this->isX(payload[0]))	return;

	switch (payload[0])
	{
	case '*':
		if (leftSubTree->payload == "0" || rightSubTree->payload == "0")
		{
			payload = "0";
			leftSubTree->remove();
			rightSubTree->remove();
			leftSubTree = NULL;
			rightSubTree = NULL;
		}
		else if (this->isNumber(leftSubTree->payload[0]) && this->isNumber(rightSubTree->payload[0]))
		{
			double a, b;
			int decIndex;
			a = stod(leftSubTree->payload);
			b = stod(rightSubTree->payload);
			payload = to_string(a*b);
			decIndex = payload.find('.');
			payload = payload.substr(0, decIndex + 2);
			leftSubTree->remove();
			rightSubTree->remove();
			leftSubTree = NULL;
			rightSubTree = NULL;
		}
		else if (leftSubTree->payload == "1" || leftSubTree->payload == "1.0")
		{
			ExpTree * T1;
			T1 = rightSubTree->duplicate();
			leftSubTree->remove();
			rightSubTree->remove();
			payload = T1->payload;
			leftSubTree = T1->leftSubTree;
			rightSubTree = T1->rightSubTree;
		}
		else if (rightSubTree->payload == "*")
		{
			if (this->isNumber(leftSubTree->payload[0]) && this->isNumber(rightSubTree->leftSubTree->payload[0]))
			{
				ExpTree *T1;
				T1 = rightSubTree->rightSubTree->duplicate();
				double num1 = stod(leftSubTree->payload);
				double num2 = stod(rightSubTree->leftSubTree->payload);
				rightSubTree->leftSubTree->remove();
				rightSubTree->rightSubTree->remove();
				double prod = num1 * num2;
				int decIndex;

				leftSubTree->payload = to_string(prod);
				decIndex = leftSubTree->payload.find('.');
				leftSubTree->payload = leftSubTree->payload.substr(0, decIndex + 2);
				rightSubTree = T1;
			}

		}
		break;
	case '+':
		if (leftSubTree->payload == "0")
		{
			ExpTree *T1;
			T1 = rightSubTree->duplicate();		//duplicate rightSubTree
			rightSubTree->remove();				// delete it
			leftSubTree->remove();
			payload = T1->payload;		//	take payload and ptrs 
			leftSubTree = T1->leftSubTree;			//	from T1
			rightSubTree = T1->rightSubTree;
		}
		else if (rightSubTree->payload == "0")
		{
			ExpTree *T1;
			T1 = leftSubTree->duplicate();
			rightSubTree->remove();
			leftSubTree->remove();
			payload = T1->payload;
			leftSubTree = T1->leftSubTree;
			rightSubTree = T1->rightSubTree;
		}
		break;
	case '-':
		if (rightSubTree->payload == "0")
		{
			ExpTree *T1;
			T1 = leftSubTree->duplicate();
			rightSubTree->remove();
			leftSubTree->remove();
			payload = T1->payload;
			leftSubTree = T1->leftSubTree;
			rightSubTree = T1->rightSubTree;

		}

		break;
	case '^':
		if (rightSubTree->payload == "1" || rightSubTree->payload == "1.0")	//if it's to the power of 1, make this tree
		{													//remove the power and make this tree the base tree
			ExpTree *T1;
			T1 = leftSubTree->duplicate();
			rightSubTree->remove();
			leftSubTree->remove();
			payload = T1->payload;
			leftSubTree = T1->leftSubTree;
			rightSubTree = T1->rightSubTree;
		}
		break;
	}
	if (leftSubTree != NULL)	leftSubTree->simplify();
	if (rightSubTree != NULL)	rightSubTree->simplify();
}

void ExpTree::postfix(string& pf)
{
	if (leftSubTree != NULL)		leftSubTree->postfix(pf);
	if (rightSubTree != NULL)		rightSubTree->postfix(pf);
	pf += ' ' + payload;
}



int main(){
	printf("HIII!!");
    ExpTree *exp = new ExpTree();
	string postFix = "xx2+*";
	exp->construct(postFix);
	printf("\nExpression Tree\n: ");
	//exp->display();
	// printf("\nDerivative Expression: \n");
	// exp->derive();
	// exp->display();
    return 0;
}