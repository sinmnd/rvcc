#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef enum {
    TK_OP,
    TK_NUM,
    TK_EOF
} TokenKind;

struct Token{
    TokenKind kind;
    struct Token *next;
    int val;
    char *loc;
    int len;
};

typedef struct Token *Token;
static void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    exit(1);
}
static Token newToken(TokenKind kind, char *start, char *end) {
    Token tok = calloc(1, sizeof(struct Token));
    tok->kind = kind;
    tok->loc = start;
    tok->len = end - start;
    tok->next = NULL;
    return tok;
}


static Token tokenize(char *p) {
    Token head = calloc(1, sizeof(struct Token));

    Token cur = head;

    while(*p) {
        if(isspace(*p)) {
            ++p;
            continue;
        }

        if(isdigit(*p)) {
            char *start = p;
            int val = strtoul(p, &p, 10);
            cur->next = newToken(TK_NUM, start, p);
            cur = cur->next;
            cur->val = val;
            continue;
        }

        if(*p == '+' || *p == '-') {
            cur->next = newToken(TK_OP, p, p + 1);
            cur = cur->next;
            ++p;
            continue;
        }

        error("invalid token: %c", *p);
    }
    cur->next = newToken(TK_EOF, p, p);

    return head->next;
}

static int getNumber(Token tok) {
    if(tok->kind != TK_NUM) {
        error("expected a number, got %u", tok->kind);
    }
    return tok->val;
}

static bool equal(Token tok, char *op) {
    return memcmp(tok->loc, op, tok->len) == 0 && op[tok->len] == '\0';
}

static Token skip(Token tok, char *op) {
    if(!equal(tok, op)) {
        error("expected '%s'", op);
    }
    return tok->next;
}

int main(int argc, char *argv[]) {
    
    if(argc != 2) {

        error("%s: invalid number of arguments", argv[0]);
    }

    Token tok = tokenize(argv[1]);


    printf("  .globl main\n");
    printf("main:\n");
    printf("  li a0, %d\n", getNumber(tok));
    tok = tok->next;

    while(tok->kind != TK_EOF) {
        if(equal(tok, "+")) {
            tok = tok->next;
            printf("  addi a0, a0, %d\n", getNumber(tok));
            tok = tok->next;
            continue;
        }
        tok = skip(tok, "-");
        printf("  addi a0, a0, -%d\n", getNumber(tok));
        tok = tok->next;
    }
    
    printf("  ret\n");

    return 0;
}

