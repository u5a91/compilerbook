#include "9cc.h"

Token *token;       // 現在着目しているトークン
char *user_input;   // 入力プログラム

// エラーを報告するための関数
// printf と同じ引数
static void error(char *fmt, ...) {
    va_list ap;                 // va_list: 可変長引数を扱うための型
    va_start(ap, fmt);          // ap を fmt より後ろにある可変長引数列のイテレータとする
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// エラー位置を報告するための関数
// printf と同じ引数
void error_at(char *loc, char *fmt, ...) {
    va_list ap;                 // va_list: 可変長引数を扱うための型
    va_start(ap, fmt);          // ap を fmt より後ろにある可変長引数列のイテレータとする

    int pos = loc - user_input;  // エラー箇所の位置を計算
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, ""); // pos ぶんの空白を出力
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

int main(int argc, char **argv) {   // 整数, 文字列配列
    if (argc != 2)
        error("%s: 引数の個数が正しくありません", argv[0]);

    user_input = argv[1];
    
    // トークナイズする
    token = tokenize();
    // パースする
    Node *node = expr();

    if (!at_eof())
        error_at(token->str, "余分なトークンです");

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    gen(node);

    printf("    pop rax\n");
    printf("    ret\n");
    return 0;
}
