///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//Author: 	Brian Dunn
//Filename:	dif.cabs
//Date:		2/27/2017
//Purpose:	Homework 3, Symbolic Differentiation
//			THis program generates and prints the first derivative (f'(x))of any common arithmetic function	f(x).
//Included:	No other files included.
///////////////////////////////////////////////////////////////////////////////	

#include <stdio.h>	// printf()
#include <malloc.h>	// malloc()	
#include <ctype.h>	// tolower()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES AND CONSTANTS
////////////////////////////////////////////////////////////////////////////////
#define NULL 0
#define FALSE 0
#define TRUE 1
#define VERIFY(cond, msg) \
	if (!cond){\
		printf(" <><> ERROR. %s\n", msg);\
	}
typedef enum {Literal, Identifier, Operator} NodeClass;

typedef struct NodeType * NodePtr;	//Forward declaration of node to be used in struct 
// Struct Definition
// each node has 1 of these class states:
// a Literal, an Identifier (for variable), or an Operator.
// Parenthesized expressions have been reduced

typedef struct NodeType
{
	NodeClass Class;
	char 	Symbol;
	int 	LitVal;
	NodePtr	Left;
	NodePtr	Right;
}	s_node_tp;

char	NextChar;
// Commononly occuring NodeTypes defined
s_node_tp	OneNode = { Literal, '1', 1, NULL, NULL };
s_node_tp	NullNode = {Literal, '0', 0, NULL, NULL };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// malloc() new node from heap. All fields are passed in;
// return the pointer to the new node to caller

NodePtr Make( NodeClass Class, char Symbol, int value, NodePtr Left, NodePtr Right)
{
	NodePtr Node = (NodePtr)malloc(sizeof(struct NodeType));
	Node->Class = Class;
	Node->Symbol = Symbol;
	Node->LitVal = value;	//if literal this is its value.
	Node->Left = Left;
	Node->Right = Right;
	return Node;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Recursively copy tree pointed to by Root.
// Pass pointer to the copy to its caller

NodePtr Copy(NodePtr Root)
{
	if(Root==NULL){
		return NULL;
	}else{
		return Make(Root->Class, Root->Symbol, Root->LitVal, Copy(Root->Left), Copy(Root->Right));
	}
}	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
NodePtr Derive(NodePtr Root)
{
	if(Root==NULL){
		return NULL;
	}else{
		switch(Root->Class){	// Switch on Class{Literal, Identifier, or Operator}
			case Literal:
				return Make(Literal, '0', 0, NULL, NULL);
			case Identifier:
				if((Root->Symbol == 'x') || (Root->Symbol == 'X')){	//Case for 'x'
					return Make(Literal, '1', 1, NULL, NULL);
				}else{												//Case for any other identifier that is not 'x'
					return Make(Literal, '0', 0, NULL, NULL);
				}
			case Operator:
				switch(Root->Symbol){
				// Case for +. Derives both operands left to right
					case '+': case '-':
						return Make(Operator, Root->Symbol, 0, Derive(Root->Left), Derive(Root->Right));
				//Case for *.	derive( u * v )  = u' * v + u * v'		
					case '*':
						return Make(Operator, '+', 0, Make(Operator, '*', 0, Derive(Root->Left), Copy(Root->Right) ), Make(Operator, '*', 0, Copy(Root->Left), Derive(Root->Right)));
				// Case for /.	derive( u / v )  = ( u' * v - u * v' ) / ( v * v)
					case	'/':
						return Make(Operator, '/', 0, Make(Operator, '-', 0, Make(Operator, '*', 0, Derive(Root->Left), Copy(Root->Right) ), Make(Operator, '*', 0, Copy(Root->Left), Derive(Root->Right))), Make(Operator, '*', 0, Copy(Root->Right), Copy(Root->Right)));
				// Case for ^. 	derive( u ^ v )  = u' * v * u ^ ( v - 1 ) + & u * v' * u ^ v
					case '^':
						return Make(Operator, '+', 0, Make(Operator, '*', 0, Derive(Root->Left), Make(Operator, '*', 0, Copy(Root->Right), Make(Operator, '^', 0, Copy(Root->Left), Make(Operator, '-', 0, Copy(Root->Right), Copy(&OneNode))))), Make (Operator, '*', 0, Make(Operator, '*', 0, Make(Operator, '&', 0, NULL, Copy(Root->Left)), Derive(Root->Right)), Make(Operator, '^', 0, Copy(Root->Left), Copy(Root->Right))));
				// Case for &(natural log(ln)).  derive( & u )    = u' / u
					case '&':
						if ( Root->Left != NULL ) {
							printf( "ln has only one operand.\n" );
						} //end if
						return Make(Operator, '/', 0, Derive(Root->Right), Copy(Root->Right));
				// Default Case	
				default:
						printf( "Impossible Operator!\n");
						return NULL;
				}
			default:
				printf("Unknown Root->Class\n");
				return NULL;
		}		
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Checks if it is a Literal and that it has a value
unsigned IsLit(char value, NodePtr Root)
{
		if(!Root){
			return FALSE;
		}else{ 
			return	(Root->Class == Literal) && (Root->Symbol == value);
		}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Checks both subtrees for literals	
unsigned BothLit(NodePtr Left, NodePtr Right)
{
	if(!Left){
		return FALSE;
	}else if(!Right){
		return FALSE;
	}else{
		return (Left->Class == Literal) && (Right->Class == Literal);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Checks if subtrees are equal
unsigned IsEqual(NodePtr Left, NodePtr Right)
{
	if((Left==NULL) && (Right==NULL)){
		return TRUE;
	}else if(Left == NULL){
		return FALSE;
	}else if(Right == NULL){
		return FALSE;
	}else if((Left->Class == Literal) && (Right->Class == Literal)){
		return (Left->LitVal) == (Right->LitVal);
	}else if((Left->Class == Identifier) && (Right->Class == Identifier)){
		return (Left->Symbol) == (Right->Symbol);
	}else{
			if((Left->Symbol) == (Right->Symbol)){
				return IsEqual(Left->Left, Right->Left) && IsEqual(Left->Right, Right->Right);
			}else{
				return FALSE;
			}
	}
	printf( "How did you get here?\n");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simplifies tree pointed to by root.
// Node_ptr_tp simplify( Node_ptr_tp root )
//  Many subtrees are redundant. Simplify! Obvious candidates:
//    x + 0   = x
//    0 + x   = x
//    x - 0   = x
//    x - x   = 0
//    x * 0   = 0
//    0 * x   = 0
//    x * 1   = x
//    1 * x   = x
//    x / x   = 1
//    x ^ 1   = x
//    x ^ 0   = 1

NodePtr Simplify( NodePtr Root ) 
{ // Simplify
	int val = 0;			// accumulate integer values from + - * etc.
	if ( !Root ) {
		  return Root;
	}else{
       switch ( Root->Class ) {
      	case Literal:
      	case Identifier:
				return Root;
      	case Operator:
       		Root->Left  = Simplify( Root->Left );
       		Root->Right = Simplify( Root->Right );
       		switch ( Root->Symbol ) {
        			case '+':
         			if ( IsLit( '0', Root->Left ) ) { // 0+x=x
							return Root->Right;
         			}else if ( IsLit( '0', Root->Right ) ) { // x+0=x 
							return Root->Left;
						}else if ( BothLit( Root->Left, Root->Right ) ) {// both trees are literals add them
							val = Root->Left->LitVal + Root->Right->LitVal;
							return Make( Literal, (char)( val + '0' ), val,
								NULL, NULL );
						}else{
							return Root; // no other simplifiction for ‘+’
						} //end if
					case '-':
						if ( IsLit( '0', Root->Right ) ) {	//x-0=x
							return Root->Left;
						}else if ( BothLit( Root->Left, Root->Right ) ) { // both trees are literals, sub right from left.
						val = Root->Left->LitVal - Root->Right->LitVal;
						return Make( Literal, (char)( val + '0' ), val, NULL, NULL );
					  }else if ( IsEqual( Root->Left, Root->Right ) ) { //if equal then result is 0(NullNode) z-z=0
						return & NullNode;
					  }else{
						return Root;
					  } //end if
					case '*':
					  if ( IsLit( '1', Root->Left ) ) { //1*x=x
						return Root->Right;
					  }else if ( IsLit( '1', Root->Right ) ) {//x*1=x
						return Root->Left;
					  }else if ( IsLit( '0', Root->Left ) || IsLit( '0', Root->Right ) ) {// 0*x or x*0=0
						return & NullNode;
					  }else{
						return Root;
					  }//end if
					case '/':
					   if ( IsLit( '1', Root->Right ) ) {// x/1=x
						 return Root->Left;
					   }else if ( IsLit( '0', Root->Left ) ) {// 0/z=0
						 return & NullNode;
					   }else if ( IsEqual( Root->Left, Root->Right ) ) {// z/z=1
						 return & OneNode;
					   }else{
						 return Root;
					   } //end if
					case '^':
						if ( IsLit( '0', Root->Right ) ) {			// x^0 = 1
						  return & OneNode;
						}else if ( IsLit( '1', Root->Right ) ) {		// x^1 = x
						  return Root->Left;
						}else{
						  return Root;
						} //end if
					case '&':
						if(IsLit('1', Root->Right)){ //ln(1)=0
							return &NullNode;
						}else{
							return Root;
						}
			}
	   }
	}
	printf("If you are here then I should get an A++!\n");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PrintTree( NodePtr Root )
{ // PrintTree
   if ( Root != NULL ) {
			if ( Root->Class == Operator ) {
				printf( "(" );
			} //end if
       PrintTree( Root->Left );
			if ( Root->Class == Literal ) {
				printf( "%d", Root->LitVal ); // prints ints > 9
			}else{
				printf( "%c", Root->Symbol );
			} //end if
			PrintTree( Root->Right );
			if ( Root->Class == Operator ) {
				printf( ")" );
			} //end if
   } //end if
} //end PrintTree
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gets next char from stream
void GetNextChar()
{
	if(NextChar != '$'){
		NextChar = getchar();
		if(NextChar == ' ')
			GetNextChar();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Checks if input is a digit 0 - 9

unsigned IsDigit(char c)
{
	return (c>= '0' ) && (c <= '9');
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Checks if input is a letter a-z or A-Z
unsigned IsLetter(char c)
{
	return( c >= 'a' && c <='z') || (c >= 'A' && c <= 'Z');
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
NodePtr Expression(); //Forward Declaration
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
NodePtr Factor()
{
	char Symbol;
	NodePtr Temp;
	
	Symbol = NextChar;
	GetNextChar();
	if (IsDigit(Symbol))
		return Make(Literal, Symbol, (int)(Symbol - '0'), NULL, NULL);
	else if(IsLetter(Symbol))
		return Make(Identifier, tolower(Symbol), 0, NULL, NULL);
	else if(Symbol == '('){
		Temp = Expression();
		if(NextChar != ')')
			printf(") expected.\n");
		GetNextChar();
		return Temp;
	}else if(Symbol == '&'){
		return Make(Operator, '&', 0, NULL, Factor() );
	}else{
		printf("Illegal character '%c'.\n", Symbol);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
NodePtr Primary()
{
	NodePtr Left = Factor();
	if( NextChar == '^' ) {
			GetNextChar(); 		// skip over ‘^’
		Left = Make( Operator, '^', 0, Left, Factor() );
    } //end if
    return Left;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
NodePtr Term()
{
	NodePtr Left;
	char Op;
	Left = Primary();
	while ( NextChar == '*' || NextChar == '/' ) {
     	Op = NextChar;			// remember ‘*’ or ‘/’
     	GetNextChar(); 		// skip over Op
			// note 0 below for LitVal is just a dummy
     	Left = Make( Operator, Op, 0, Left, Primary() );
    } //end while
    return Left;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
NodePtr Expression()
{
	char Op;						// remember ‘+’ or ‘-’
	NodePtr Left = Term();	// handle all higher prior.
	while (( NextChar == '+') || ( NextChar == '-' )) {
			Op = NextChar;			// remember ‘+’ or ‘-’
			GetNextChar();			// skip Op
			// note 0 below for LitVal is just a dummy
			Left = Make( Operator, Op, 0, Left, Term() );
	} //end while
	return Left;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Initialize()
{
	printf( " Enter f(x), ended with $:");
	NextChar = ' ';
	GetNextChar();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SHOW(msg, r)    \
	printf("%s", msg);  \
	PrintTree(r);		\
	printf("\n")
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{ // main: Differentiation
	NodePtr root = NULL;
 	Initialize();

	root = Expression();
	VERIFY( ( NextChar == '$' ), "$ expected, not found\n" );
	SHOW( " original   f(x) = ", root );
	root = Simplify( root );
	SHOW( " Simplified f(x) = ", root );
	root = Derive( root );
	SHOW( " derived   f'(x) = ", root );
 	root = Simplify( root );
	SHOW( " reduced   f'(x) = ", root );

}	//The END. Thank You for Reading this! Have A Wonderful Day!!!