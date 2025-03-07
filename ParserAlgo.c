#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global pointer to traverse the input string.
char *input;

// Helper function to skip whitespace.
void skipWhitespace() {
    while (isspace(*input)) {
        input++;
    }
}

// Error handling: prints a message and exits.
void error(const char *msg) {
    printf("Error: %s\n", msg);
    exit(1);
}

// Matches a specific single-character token.
void match(char expected) {
    skipWhitespace();
    if (*input == expected) {
        input++;  // consume the character
        skipWhitespace();
    } else {
        char buf[50];
        sprintf(buf, "Expected '%c' but found '%c'", expected, *input);
        error(buf);
    }
}

// Matches the terminal "id".
void matchId() {
    skipWhitespace();
    if (strncmp(input, "id", 2) == 0) {
        input += 2;  // consume "id"
        skipWhitespace();
    } else {
        error("Expected 'id'");
    }
}

// Forward declarations
void E();
void T();

// E -> T | T + E
void E() {
    T();
    skipWhitespace();
    // If a '+' is encountered, match it and then parse the following E.
    if (*input == '+') {
        match('+');
        E();
    }
}

// T -> id | id * T | (E)
void T() {
    skipWhitespace();
    if (*input == '(') {
        // Production: T -> (E)
        match('(');
        E();
        match(')');
    } else if (strncmp(input, "id", 2) == 0) {
        // Production: T -> id or T -> id * T
        matchId();
        skipWhitespace();
        if (*input == '*') {
            match('*');
            T();
        }
    } else {
        error("Unexpected token in T");
    }
}

int main() {
    // Buffer to hold input. For example, enter: (id)$
    char buffer[100];
    printf("Enter expression (end with $): ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        error("Input error");
    }
    
    // Set the global input pointer and skip initial whitespace.
    input = buffer;
    skipWhitespace();

    // Begin parsing from the start symbol E.
    E();
    
    // After parsing, check for the end-of-input marker '$'.
    skipWhitespace();
    if (*input == '$') {
        printf("Parsing successful.\n");
    } else {
        error("Extra symbols at end of input");
    }
    return 0;
}
