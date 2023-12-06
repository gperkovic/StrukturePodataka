#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOCATION_FAILURE -1
#define FILE_OPEN_FAILURE -2B
#define MAX 1024

struct _List;
typedef struct _List* Position;
typedef struct _List {
    double number;
    Position next;
} List;

// Function prototypes
int pushToStart(Position Head, double number);
int readFromFile(char fileName[], Position Stack);
int pushAfter(Position List, Position newElement);
Position createNewElement(double number);
int deleteAllElements(Position Stack);
int deleteAfter(Position Head);
int calculatePostfix(Position Stack, char operator);
int pop(Position Stack, double* operator);
int printList(Position Head);

int main() {
    char fileName[50];
    List Stack = { .number = 0, .next = NULL };

    printf("Enter the file name to read the postfix expression from: \n");
    scanf(" %s", fileName);

    readFromFile(fileName, &Stack);

    return 0;
}

int readFromFile(char fileName[], Position Stack) {
    FILE* fp = NULL;
    double readNumber = 0;
    int byteCount = 0;
    char* line = NULL;
    int functionValue = 0;
    char operator = 0, temporaryLine[MAX] = { 0 };

    fp = fopen(fileName, "r");

    line = (char*)malloc(1024 * sizeof(char));

    if (!line) {
        printf("Memory allocation failed.\n");
        return ALLOCATION_FAILURE;
    }

    if (!fp) {
        printf("Unable to open the file!\n");
        return FILE_OPEN_FAILURE;
    }

    fgets(temporaryLine, 1024, fp);
    printf("\nYour postfix expression: %s\n", temporaryLine);

    line = temporaryLine;

    while (strlen(line) > 0) {
        functionValue = sscanf(line, "%lf %n", &readNumber, &byteCount);

        if (functionValue == 1) {
            functionValue = pushToStart(Stack, readNumber);
            if (functionValue != 0)
                deleteAllElements(Stack);
        }
        else {
            sscanf(line, " %c %n", &operator, &byteCount);
            functionValue = calculatePostfix(Stack, operator);
            if (functionValue != 0)
                deleteAllElements(Stack);
        }
        line += byteCount;
    }

    printf("Stack Result:\n");
    printList(Stack);

    fclose(fp);
    return 0;
}

Position createNewElement(double number) {
    Position newElement = NULL;

    newElement = (Position)malloc(sizeof(List));
    if (!newElement) {
        printf("Unable to allocate memory!\n");
        return NULL;
    }

    newElement->number = number;
    newElement->next = NULL;

    return newElement;
}

int pushToStart(Position Head, double number) {
    Position newElement = NULL;

    newElement = createNewElement(number);
    if (!newElement) {
        printf("Unable to create a new element!\n");
        return EXIT_FAILURE;
    }

    pushAfter(Head, newElement);
    return 0;
}

int pushAfter(Position List, Position newElement) {
    newElement->next = List->next;
    List->next = newElement;
    return 0;
}

int deleteAfter(Position Head) {
    Position temporary = NULL;
    temporary = Head->next;

    if (!temporary)
        return EXIT_FAILURE;

    Head->next = temporary->next;
    free(temporary);
    return EXIT_SUCCESS;
}

int deleteAllElements(Position Stack) {
    while (Stack->next != NULL) {
        deleteAfter(Stack);
    }
    return EXIT_SUCCESS;
}

int pop(Position Stack, double* operator) {
    if (Stack->next == NULL) {
        printf("Invalid postfix expression!\n");
        return EXIT_FAILURE;
    }

    *operator = Stack->next->number;
    deleteAfter(Stack);
    return 0;
}

int printList(Position Head) {
    while (Head->next != NULL) {
        Head = Head->next;
        printf("%.2f\n", Head->number);
    }
    return 0;
}

int calculatePostfix(Position Stack, char operator) {
    double operator1 = 0;
    double operator2 = 0;
    int status = 0;
    double result = 0;

    status = pop(Stack, &operator2);
    if (status != 0)
        return -1;

    status = pop(Stack, &operator1);
    if (status != 0)
        return -1;

    switch (operator) {
    case '+':
        result = operator1 + operator2;
        break;
    case '-':
        result = operator1 - operator2;
        break;
    case '*':
        result = operator1 * operator2;
        break;
    case '/':
        if (operator2 == 0) {
            printf("Cannot divide by 0!\n");
            return EXIT_FAILURE;
        }
        else {
            result = operator1 / operator2;
            break;
        }
    default:
        break;
    }

    pushToStart(Stack, result);
    return EXIT_SUCCESS;
}
