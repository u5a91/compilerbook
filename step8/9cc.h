// インクルードガード
#ifndef NINECC_H
#define NINECC_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// トークンの種類
typedef enum {
    TK_RESERVED,    // 記号
    TK_NUM,         // 整数トークン
    TK_EOF,         // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

// トークン型
struct Token {
    TokenKind kind; // トークンの型
    Token *next;    // 次の入力トークン (Token はすでに宣言済み)
    int val;        // kind が TK_NUMの場合、その数値
    char *str;      // トークン文字列
    size_t len;        // トークンの長さ
};

// パーサ
typedef enum {
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
    ND_EQ,  // ==
    ND_NE,  // !=
    ND_LT,  // < or >
    ND_LE,  // <=
    ND_NUM, // 整数
} NodeKind;

typedef struct Node Node;

// 抽象構文木 (AST) のノードの型
struct Node {
    NodeKind kind;  // ノードの型
    Node *lhs;      // 左辺
    Node *rhs;      // 右辺
    int val;        // kind が ND_NUM の場合のみ使う
};

// parse.c, tokenize.c, main.c などの中でしか使わないものは書かない

void error_at(char *loc, char *fmt, ...);

bool consume(char *op);
void expect(char *op);
int expect_number(void);
bool at_eof(void);

void gen(Node *node);

// main.c で使う関数の宣言
Token *tokenize(void);
Node *expr(void);

extern char *user_input;
extern Token *token;

#endif
