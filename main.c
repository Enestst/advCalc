#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define MAX_INPUT_SIZE 256
#define MAX_TOKEN_SIZE 64
#define MAX_TOKENS 256

// created char input array with MAX_INPUT_SIZE, it declared in line 8 as 256
char input[MAX_INPUT_SIZE];
// created values array with MAX_INPUT_SIZE, it declared in line 8 as 256
long long int values[MAX_INPUT_SIZE];
// created char variables array with MAX_INPUT_SIZE, it declared in line 8 as 256
char *variables[MAX_INPUT_SIZE];
// created int numVariables for a counter
int numVariables = 0;
// created a tokentype for enums
typedef enum
{
    NULL_TOKEN,
    LONGLONG,
    INTEGER,
    VARIABLE,
    PLUS,
    MINUS,
    MULTIPLY,
    OR,
    AND,
    EQUALS,
    XOR,
    LEFT_SHIFT,
    RIGHT_SHIFT,
    LEFT_ROTATE,
    RIGHT_ROTATE,
    BITWISE_NOT,
    LEFT_PAREN,
    RIGHT_PAREN,
    COMMA,
} TokenType;

// created a struct for expressions
typedef struct
{
    TokenType type;
    long long int value;
    char *var;
    struct Expression *left;
    struct Expression *right;
} Expression;

// created a struct for tokens
typedef struct
{
    TokenType type;
    char *string;

} Token;

// created a struct for 5 which includes Token struct
typedef struct
{
    Token *tokens;
    int numTokens;
    int currentToken;
    bool hasError;
} TokenList;

// createExpression function, this function called to create expressions
Expression *createExpression(TokenType type, long long int value, char *var, Expression *left, Expression *right)
{
    Expression *expression = (Expression *)malloc(sizeof(Expression));
    if (type == EQUALS)
    {
        expression->type = type;
        expression->left = left;
        expression->right = right;
        expression->value = right->value;
        expression->var = left->var;
    }
    else
    {
        expression->type = type;
        expression->left = left;
        expression->right = right;
        expression->value = value;
        expression->var = var;
    }
    return expression;
}

// Declaring functions in the code
// checks is a varible already assigned
bool IsAlreadyVariable(char *var);
// getting the value of an assigned value
long long int getValue(char *var);
// checks if the input is valid or not. (Some \ ; chars.)
bool isStringValid(char *string);
// Parse Base functions. These functions parsing the tokenlist and make a Parse Tree.
Expression *parseBase(TokenList *tokenlist);
Expression *parseBase2(TokenList *tokenlist);
Expression *parseBase3(TokenList *tokenlist);
Expression *parseBase4(TokenList *tokenlist);
Expression *parseBase5(TokenList *tokenList);
// Parse Expression function. Make the expressions.
Expression *parseExpression(TokenList *tokenlist);

// createToken function, creating and return a token
Token *createToken(TokenType type, char *string, int startPos)
{
    Token *token = malloc(sizeof(Token));
    token->type = type;
    token->string = string;
    return token;
}

// createTokenList function, creating a token list of tokens
TokenList *createTokenList()
{
    TokenList *list = malloc(sizeof(TokenList));
    list->tokens = malloc(MAX_TOKENS * sizeof(Token));
    list->numTokens = 0;
    list->currentToken = 0;
    list->hasError = false;
    return list;
}

// void addToken function, add tokens to the tokenlist
void addToken(TokenList *list, Token *token)
{
    list->tokens[list->numTokens++] = *token;
}
// declaring what is multiply function
int multiply(int a, int b)
{
    return a * b;
}
// declaring what is minus function
int minus(int a, int b)
{
    return a - b;
}
// declaring what is plus function
int plus(int a, int b)
{
    return a + b;
}
// declaring what is or function
int or (int a, int b)
{
    return a | b;
}
// declaring what is and function
int and (int a, int b)
{
    return a & b;
}
// declaring what is xor function
int xor (int a, int b) {
    return a ^ b;
}
    // declaring what is leftShift function
    int leftShift(int a, int b)
{
    return a << b;
}
// declaring what is rightShift function
int rightShift(int a, int b)
{
    return a >> b;
}
// declaring what is leftRotate function
int leftRotate(int a, int b)
{
    return (a << b) | (a >> (64 - b));
}
// declaring what is rightRotate function
int rightRotate(int a, int b)
{
    return (a >> b) | (a << (64 - b));
}
// declaring what is bitwiseNot function
int bitwiseNot(int a)
{
    return ~a;
}
// LEXER function, this function called to check the input and add tokens to the tokenlist. This function is the one of the main functions in the program.
void lexer(char *input, TokenList *tokenList)
{
    int i = 0;
    int startPos = 0;
    // if the input[i] is '\0' code stops the working
    while (input[i] != '\0')
    {
        // checking if the input[i] is a space
        if (isspace(input[i]))
        {
            i++;
            continue;
        }
        // checking if the input[i] is a digit
        if (isdigit(input[i]))
        {
            int j = i;
            while (isdigit(input[j]))
            {
                j++;
            }
            char *num = malloc((j - i + 1) * sizeof(char));
            memcpy(num, input + i, j - i);
            num[j - i] = '\0';
            addToken(tokenList, createToken(LONGLONG, num, i));
            i = j;
            continue;
        }
        // checking if the input[i] is a EQUALS and create and add a EQUALS token to the tokenlist
        if (input[i] == '=')
        {
            addToken(tokenList, createToken(EQUALS, "=", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a | and create and add a OR token to the tokenlist
        if (input[i] == '|')
        {
            addToken(tokenList, createToken(OR, "|", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a % and break the loop because this sign is for the comments
        if (input[i] == '%')
        {
            break;
        }
        // checking if the input[i] is a + and create and add a PLUS token to the tokenlist
        if (input[i] == '+')
        {
            addToken(tokenList, createToken(PLUS, "+", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a - and create and add a MINUS token to the tokenlist
        if (input[i] == '-')
        {
            addToken(tokenList, createToken(MINUS, "-", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a * and create and add a MULTIPLY token to the tokenlist
        if (input[i] == '*')
        {
            addToken(tokenList, createToken(MULTIPLY, "*", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a , and create and add a COMMA token to the tokenlist
        if (input[i] == ',')
        {
            addToken(tokenList, createToken(COMMA, ",", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a ( and create and add a LEFT_PAREN token to the tokenlist
        if (input[i] == '(')
        {
            addToken(tokenList, createToken(LEFT_PAREN, "(", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a ) and create and add a RIGHT_PAREN token to the tokenlist
        if (input[i] == ')')
        {
            addToken(tokenList, createToken(RIGHT_PAREN, ")", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a & and create and add an AND token to the tokenlist
        if (input[i] == '&')
        {
            addToken(tokenList, createToken(AND, "&", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a alphanumeric
        if (isalpha(input[i]))
        {
            int j = i;
            while (isalpha(input[j]))
            {
                j++;
            }
            char *var = malloc((j - i + 1) * sizeof(char));
            memcpy(var, input + i, j - i);
            var[j - i] = '\0';
            // checking if it is a varible
            if (strcmp(var, "xor") != 0 && strcmp(var, "ls") != 0 && strcmp(var, "rs") != 0 && strcmp(var, "lr") != 0 &&
                strcmp(var, "rr") != 0 && strcmp(var, "not") != 0)
            {
                // adding a VARIABLE token with var to tokenlist
                addToken(tokenList, createToken(VARIABLE, var, startPos));
                i = j;
                continue;
            }
            else
            {
                // checking if the var is an xor and create and add a XOR token to the tokenlist
                if (strcmp(var, "xor") == 0)
                {
                    addToken(tokenList, createToken(XOR, "xor", startPos));
                    i = j;
                    continue;
                }
                // checking if the var is an ls and create and add a LEFT_SHIFT token to the tokenlist
                if (strcmp(var, "ls") == 0)
                {
                    addToken(tokenList, createToken(LEFT_SHIFT, "ls", startPos));
                    i = j;
                    continue;
                }
                // checking if the var is an rs and create and add a RIGHT_SHIFT token to the tokenlist
                if (strcmp(var, "rs") == 0)
                {
                    addToken(tokenList, createToken(RIGHT_SHIFT, "rs", startPos));
                    i = j;
                    continue;
                }
                // checking if the var is an lr and create and add a LEFT_ROTATE token to the tokenlist
                if (strcmp(var, "lr") == 0)
                {
                    addToken(tokenList, createToken(LEFT_ROTATE, "lr", startPos));
                    i = j;
                    continue;
                }
                // checking if the var is an lr and create and add a RIGHT_ROTATE token to the tokenlist
                if (strcmp(var, "rr") == 0)
                {
                    addToken(tokenList, createToken(RIGHT_ROTATE, "rr", startPos));
                    i = j;
                    continue;
                }
                // checking if the var is an not and create and add a BITWISE_NOT token to the tokenlist
                if (strcmp(var, "not") == 0)
                {
                    addToken(tokenList, createToken(BITWISE_NOT, "not", startPos));
                    i = j;
                    continue;
                }
            }
        }
    }
}

// these functions are parsing the tokens and make a tree with recursive calls.
// createExpression creates the expressions and make a tree with the expressions
// these functions parsed for the precedence of the operators with this way we make a true tree for a compiler

Expression *parseExpression(TokenList *tokenList)
{
    Expression *expression = parseBase5(tokenList);
    // checking if the current token is a EQUALS token
    while (tokenList->tokens[tokenList->currentToken].type == EQUALS)
    {
        // checking if the expression is not a VARIABLE if it is not then it will be an error
        if (expression->type != VARIABLE)
        {
            //printf("Error!\n");
            // exit(1);
            //  tokenlist hasError true
            tokenList->hasError = true;
        }
        // we will increment the current token and parse the next expression
        tokenList->currentToken++;
        // we created a new expression and call the parseBase5
        Expression *next_expr = parseBase5(tokenList);
        // we will create a new expression with the EQUALS type and the expression and the next expression
        expression = createExpression(EQUALS, 0, NULL, expression, next_expr);
    }
    // checking the error situations
    if (tokenList->tokens[tokenList->currentToken].type == VARIABLE || tokenList->tokens[tokenList->currentToken].type == INTEGER || tokenList->tokens[tokenList->currentToken].type == LONGLONG || tokenList->tokens[tokenList->currentToken].type == XOR || tokenList->tokens[tokenList->currentToken].type == LEFT_SHIFT || tokenList->tokens[tokenList->currentToken].type == RIGHT_SHIFT || tokenList->tokens[tokenList->currentToken].type == LEFT_ROTATE || tokenList->tokens[tokenList->currentToken].type == RIGHT_ROTATE || tokenList->tokens[tokenList->currentToken].type == BITWISE_NOT)
    {
        //printf("Error!\n");
        // exit(1);
        tokenList->hasError = true;
    }
    return expression;
}
// parseBase5 is parsing the tokens for the OR operator
Expression *parseBase5(TokenList *tokenList)
{
    // we will call the parseBase4 function
    Expression *expression = parseBase4(tokenList);
    // checking if the current token is an OR token
    while (tokenList->tokens[tokenList->currentToken].type == OR)
    {
        // we will increment the current token and parse the next expression
        tokenList->currentToken++;
        // we created a new expression and call the parseBase4
        Expression *next_expr = parseBase4(tokenList);
        // we will create a new expression with the OR type and the expression and the next expression
        expression = createExpression(OR, or (expression->value, next_expr->value), NULL, expression, next_expr);
    }
    return expression;
}
// parseBase4 is parsing the tokens for the AND operator
Expression *parseBase4(TokenList *tokenList)
{
    // we will call the parseBase3 function
    Expression *expression = parseBase3(tokenList);
    // checking if the current token is an AND token
    while (tokenList->tokens[tokenList->currentToken].type == AND)
    {
        // we will increment the current token and parse the next expression
        tokenList->currentToken++;
        // we created a new expression and call the parseBase3
        Expression *next_expr = parseBase3(tokenList);
        // we will create a new expression with the AND type and the expression and the next expression
        expression = createExpression(AND, and(expression->value, next_expr->value), NULL, expression, next_expr);
    }
    return expression;
}
// parseBase3 is parsing the tokens for the PLUS and MINUS operators
Expression *parseBase3(TokenList *tokenList)
{
    // we will call the parseBase2 function
    Expression *expression = parseBase2(tokenList);
    // checking if the current token is an PLUS or MINUS token
    while (tokenList->tokens[tokenList->currentToken].type == PLUS || tokenList->tokens[tokenList->currentToken].type == MINUS)
    {
        // we created a TokenType temp variable to find the type of the token
        TokenType temp = tokenList->tokens[tokenList->currentToken].type;
        // we will increment the current token and parse the next expression
        tokenList->currentToken++;
        // we created a new expression and call the parseBase2
        Expression *next_expr = parseBase2(tokenList);
        // we will create a new expression with the PLUS or MINUS type and the expression and the next expression
        if (temp == MINUS)
        {
            expression = createExpression(MINUS, minus(expression->value, next_expr->value), NULL, expression, next_expr);
        }
        else
        {
            expression = createExpression(PLUS, plus(expression->value, next_expr->value), NULL, expression, next_expr);
        }
    }
    return expression;
}
// parseBase2 is parsing the tokens for the MULTIPLY and DIVIDE operators
Expression *parseBase2(TokenList *tokenList)
{
    // we will call the parseBase function
    Expression *expression = parseBase(tokenList);
    // checking if the current token is an MULTIPLY or DIVIDE token
    while (tokenList->tokens[tokenList->currentToken].type == MULTIPLY)
    {
        // we will increment the current token and parse the next expression
        tokenList->currentToken++;
        // we created a new expression and call the parseBase
        Expression *next_expr = parseBase(tokenList);
        // we will create a new expression with the MULTIPLY type and the expression and the next expression
        expression = createExpression(MULTIPLY, multiply(expression->value, next_expr->value), NULL, expression, next_expr);
    }
    return expression;
}
// parseBase is parsing the tokens for the LONGLONG, VARIABLE, XOR, LEFT_SHIFT, RIGHT_SHIFT, LEFT_ROTATE, RIGHT_ROTATE, BITWISE_NOT operators
Expression *parseBase(TokenList *tokenList)
{
    // checking if the current token is a LONGLONG token
    if (tokenList->tokens[tokenList->currentToken].type == LONGLONG)
    {
        // we will create a new expression with the LONGLONG type and the value of the token with atoll function, we use atoll function to convert the string to long long
        Expression *expression = createExpression(LONGLONG, atoll(tokenList->tokens[tokenList->currentToken].string), NULL, NULL, NULL);
        // we will increment the current token
        tokenList->currentToken++;
        return expression;
    }
    // checking if the current token is a VARIABLE token
    if (tokenList->tokens[tokenList->currentToken].type == VARIABLE)
    {
        Expression *expression;
        // checks if it is already assigned
        if (IsAlreadyVariable(tokenList->tokens[tokenList->currentToken].string))
        {   
            // creating expression with a allready Assigned variable with getValue
            expression = createExpression(VARIABLE, getValue(tokenList->tokens[tokenList->currentToken].string), tokenList->tokens[tokenList->currentToken].string, NULL, NULL);
        }
        else
        {
            expression = createExpression(VARIABLE, 0, tokenList->tokens[tokenList->currentToken].string, NULL, NULL);
        }
        tokenList->currentToken++;
        if (tokenList->tokens[tokenList->currentToken].type == LEFT_PAREN)
        {
            tokenList->hasError = true;
        }
        return expression;
    }
    // checking if the current token is a XOR token
    if (tokenList->tokens[tokenList->currentToken].type == XOR)
    {
        tokenList->currentToken++;
        tokenList->currentToken++;
        Expression *expression = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != COMMA)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        Expression *expression2 = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != RIGHT_PAREN)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        return createExpression(XOR, xor(expression->value, expression2->value), NULL, expression, expression2);
    }
    // checking if the current token is a LEFT_ROTATE token
    if (tokenList->tokens[tokenList->currentToken].type == LEFT_ROTATE)
    {
        tokenList->currentToken++;
        tokenList->currentToken++;
        Expression *expression = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != COMMA)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        Expression *expression2 = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != RIGHT_PAREN)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        return createExpression(LEFT_ROTATE, leftRotate(expression->value, expression2->value), NULL, expression, expression2);
    }
    // checking if the current token is a RIGHT_ROTATE token
    if (tokenList->tokens[tokenList->currentToken].type == RIGHT_ROTATE)
    {
        tokenList->currentToken++;
        tokenList->currentToken++;
        Expression *expression = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != COMMA)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        Expression *expression2 = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != RIGHT_PAREN)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        return createExpression(RIGHT_ROTATE, rightRotate(expression->value, expression2->value), NULL, expression, expression2);
    }
    // checking if the current token is a BITWISE_NOT token
    if (tokenList->tokens[tokenList->currentToken].type == BITWISE_NOT)
    {
        tokenList->currentToken++;
        tokenList->currentToken++;
        Expression *expression = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != RIGHT_PAREN)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        return createExpression(BITWISE_NOT, bitwiseNot(expression->value), NULL, expression, NULL);
    }
    // checking if the current token is a RIGHT_SHIFT token
    if (tokenList->tokens[tokenList->currentToken].type == RIGHT_SHIFT)
    {
        tokenList->currentToken++;
        tokenList->currentToken++;
        Expression *expression = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != COMMA)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        Expression *expression2 = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != RIGHT_PAREN)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        return createExpression(RIGHT_SHIFT, rightShift(expression->value, expression2->value), NULL, expression, expression2);
    }
    // checking if the current token is a LEFT_SHIFT token
    if (tokenList->tokens[tokenList->currentToken].type == LEFT_SHIFT)
    {
        tokenList->currentToken++;
        tokenList->currentToken++;
        Expression *expression = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != COMMA)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        Expression *expression2 = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != RIGHT_PAREN)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        return createExpression(LEFT_SHIFT, leftShift(expression->value, expression2->value), NULL, expression, expression2);
    }
    // checking if the current token is a LEFT_PAREN token
    if (tokenList->tokens[tokenList->currentToken].type == LEFT_PAREN)
    {
        tokenList->currentToken++;
        Expression *expression = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != RIGHT_PAREN)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        return expression;
    }
    // logs for errors
    tokenList->hasError = true;
}
// checking if the variable is already assigned and returns boolean
bool IsAlreadyVariable(char *var)
{
    for (int i = 0; i < numVariables; i++)
    {
        if (strcmp(var, variables[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

// return the value of an assigned variable
long long int getValue(char *var) // checking if the var is an lr and create and add a LEFT_ROTATE token to the tokenlist
{
    for (int i = 0; i < numVariables; i++)
    {
        if (strcmp(var, variables[i]) == 0)
        {
            return values[i];
        }
    }
}

// when we take input, we will check if all chars are valid or not
bool isStringValid(char *string)
{
    for (int i = 0; i < strlen(string); i++)
    {
        // this is the end of the string
        if (string[i] == '\n' || string[i] == '%')
        {
            return true;
        }
        // checking the chars
        if (string[i] >= 'a' && string[i] <= 'z' || string[i] >= 'A' && string[i] <= 'Z' || string[i] >= '0' && string[i] <= '9' || string[i] == ' ' || string[i] == '(' || string[i] == ')' || string[i] == ',' || string[i] == '=' || string[i] == '+' || string[i] == '-' || string[i] == '*' || string[i] == '%' || string[i] == '&' || string[i] == '|' || string[i] == '^' || string[i] == '~' || string[i] == '<' || string[i] == '>')
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}

int main()
{
    printf("> ");
    // taking input
    while (fgets(input, MAX_INPUT_SIZE, stdin))
    {
        // if input is null, it will break the loop and stops the program
        if (input == NULL){
            break;
        }
        
        // deleting spaces front of the input
        int i = 0;
        while (input[i] == ' ')
        {
            i++;
        }
        strcpy(input, input + i);

        // if the first variable of input continued
        if (input[0] == '\n')
        {
            printf("> ");
            continue;
        }
        // if the all line is a comment line continued
        if (input[0] == '%')
        {
            printf("> ");
            continue;
        }

        // checking if the input is valid
        if (!isStringValid(input))
        {
            printf("Error!\n");
            printf("> ");
            continue;
        }

        // created a token list
        TokenList *tokenList = createTokenList();

        // lexed the input
        lexer(input, tokenList);

        // parsed the input
        Expression *expression = parseExpression(tokenList);

        // if we logged the error, program says error and back to the taking input
        if (tokenList->hasError == true)
        {
            printf("Error!\n");
            printf("> ");
            continue;
        }

        // if the expression is an assignment expression
        if (expression->type == EQUALS)
        {
            if (IsAlreadyVariable(expression->var))
            {
                for (int i = 0; i < numVariables; i++)
                {
                    if (strcmp(expression->var, variables[i]) == 0)
                    {
                        values[i] = expression->value;
                    }
                }
            }
            else
            {
                variables[numVariables] = expression->var;
                values[numVariables] = expression->value;
                numVariables++;
            }
            variables[numVariables] = expression->var;
            values[numVariables] = expression->value;
            numVariables++;
        }
        else
        {
            // if the expression is not an assignment expression, it will print it
            printf("%lld\n", expression->value);
        }
        printf("> ");
    }

    return 0;
}
