#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define NUM_NON_TERMINALS 3
#define NUM_TERMINALS 3
#define MAX_RULE_LENGTH 3 // Maximum length of a rule

char parsing_table[NUM_NON_TERMINALS][NUM_TERMINALS][MAX_RULE_LENGTH];

// Function to initialize the parsing table
void initializeParsingTable() {
    // Initialize all entries in the parsing table to empty rules
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        for (int j = 0; j < NUM_TERMINALS; j++) {
            strcpy(parsing_table[i][j], "");
        }
    }

    // Fill in the parsing table according to the LL(1) grammar rules
    // Example grammar:
    // S -> aAB
    // A -> b | ε
    // B -> c | ε

    // Non-terminal: S
    strcpy(parsing_table[0][0], "aAB"); // Terminal: 'a'
    strcpy(parsing_table[0][1], "");   // Terminal: 'b'

    // Non-terminal: A
    strcpy(parsing_table[1][1], "b"); // Terminal: 'b'
    strcpy(parsing_table[1][2], "");  // Terminal: 'c'

    // Non-terminal: B
    strcpy(parsing_table[2][2], "c"); // Terminal: 'c'
}

// Function to perform predictive parsing
bool predictiveParsing(const char *input) {
    int stack_top = 0; // Initialize stack pointer
    char stack[100];   // Stack to store non-terminals
    stack[stack_top] = 'S'; // Push start symbol onto the stack

    int input_index = 0; // Initialize input pointer

    while (input[input_index] != '\0') {
        char current_symbol = stack[stack_top];
        char next_input = input[input_index];

        if (current_symbol == next_input) {
            // Matched terminal symbol, pop from stack and move to next input symbol
            stack_top--;
            input_index++;
        } else if (current_symbol >= 'A' && current_symbol <= 'Z') {
            // Non-terminal symbol
            char *rule = parsing_table[current_symbol - 'A'][next_input - 'a'];
            if (strlen(rule) != 0) {
                // Non-empty entry in parsing table
                // Replace non-terminal symbol with the corresponding rule
                stack_top--;
                for (int i = strlen(rule) - 1; i >= 0; i--) {
                    if (rule[i] != 'E') {
                        if (stack_top < sizeof(stack) - 1) {
                            stack[++stack_top] = rule[i];
                        } else {
                            // Stack overflow
                            return false;
                        }
                    }
                }
            } else {
                // Empty entry in parsing table, reject
                return false;
            }
        } else {
            // Error: unexpected symbol
            return false;
        }
    }

    // If the input is fully parsed and the stack is empty, accept
    return stack_top == -1;
}

int main() {
    initializeParsingTable();
    const char *input1 = "abc";
    const char *input2 = "ab";

    printf("Input \"%s\" %s\n", input1, predictiveParsing(input1) ? "accepted" : "rejected");
    printf("Input \"%s\" %s\n", input2, predictiveParsing(input2) ? "accepted" : "rejected");

    return 0;
}
