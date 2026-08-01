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
};

Token *token;   // 現在着目しているトークン

// エラーを報告するための関数
// printf と同じ引数
void error(char *fmt, ...) {
    va_list ap;                 // va_list: 可変長引数を扱うための型
    va_start(ap, fmt);          // ap を fmt より後ろにある可変長引数列のイテレータとする
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// 次のトークンが期待している記号のときは
// トークンを 1 つ進めて true を返す
// そうでないときには false を返す
bool consume(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op)
        return false;
    token = token->next;
    return true;
}

// 次のトークンが期待している記号のときは
// トークンを 1 つ進める
// そうでないときにはエラーを報告する
void expect(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op)
        error("'%c'ではありません", op);
    token = token->next;
}

// 整数トークン限定の expect 関数
int expect_number() {
    if (token->kind != TK_NUM)
        error("数ではありません");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

// 新しいトークンを作成して cur に繋げる
Token *new_token(TokenKind kind, Token *cur, char *str) {
    Token *tok = calloc(1, sizeof(Token));  // Token 1 つぶんの領域を確保し初期化
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

// 入力文字列 p をトークナイズし, それを返す
Token *tokenize(char *p) {
    Token head;         // ダミーヘッド
    head.next = NULL;
    Token *cur = &head; // head のメモリアドレス

    while(*p) {
        // 空白文字をスキップ
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-') {
            cur = new_token(TK_RESERVED, cur, p++);
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        error("トークナイズできません");
    }
    cur = new_token(TK_EOF, cur, p);
    return head.next;
}

int main(int argc, char **argv) {   // 整数, 文字列配列
    if (argc != 2) {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }
    
    // トークナイズする
    token = tokenize(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    // 式の最初が整数かどうか
    printf("    mov rax, %ld\n", expect_number());

    while(!at_eof()) {
        if (consume('+')) {
            printf("    add rax, %ld\n", expect_number());
            continue;
        }

        expect('-');
        printf("    sub rax, %ld\n", expect_number());
    }

    printf("    ret\n");
    return 0;
}
