#include "9cc.h"

// 次のトークンが期待している記号のときは
// トークンを 1 つ進めて true を返す
// そうでないときには false を返す
bool consume(char *op) {
    if (token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len) != 0)    // 2 つのメモリ領域を先頭から指定したバイト数比較
        return false;
    token = token->next;
    return true;
}

// 次のトークンが期待している記号のときは
// トークンを 1 つ進める
// そうでないときにはエラーを報告する
void expect(char *op) {
    if (token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len) != 0)
        error_at(token->str, "'%s'ではありません", op);
    token = token->next;
}

// 整数トークン限定の expect 関数
int expect_number(void) {
    if (token->kind != TK_NUM)
        error_at(token->str, "数ではありません");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof(void) {
    return token->kind == TK_EOF;
}

// 新しいトークンを作成して cur に繋げる
static Token *new_token(TokenKind kind, Token *cur, char *str, size_t len) {
    Token *tok = calloc(1, sizeof(Token));  // Token 1 つぶんの領域を確保し初期化
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}
    
static bool startwith(char *p, char *q) {
    return memcmp(p, q, strlen(q)) == 0;
}

// 入力文字列 p をトークナイズし, それを返す
Token *tokenize(void) {
    char *p = user_input;
    Token head;         // ダミーヘッド
    head.next = NULL;
    Token *cur = &head; // head のメモリアドレス

    while(*p) {
        // 空白文字をスキップ
        if (isspace(*p)) {
            p++;
            continue;
        }


        // 2 文字以上用
        if(startwith(p, "==") ||
            startwith(p, "!=") ||
            startwith(p, "<=") ||
            startwith(p, ">=")) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        // 1 文字用
        if (strchr("+-*/()<>", *p)) {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p - q;
            continue;
        }

        error_at(p, "トークナイズできません");
    }
    cur = new_token(TK_EOF, cur, p, 0);
    return head.next;
}
