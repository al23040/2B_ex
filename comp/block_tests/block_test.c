#include <stdio.h>
#include <stdlib.h>
#include "csua.h" // コンパイラ構造体とユーティリティ関数をインクルード

// 外部宣言 (y.tab.c と scanner.c からリンクされる)
extern int yyparse(void);
extern FILE *yyin;

// Yacc/Lex が参照するグローバル変数 (通常、scanner.c にある)
extern int current_line; 

int main(void) {
    CS_Compiler *compiler;
    
    // 1. コンパイラ構造体の初期化と設定 (メモリ管理の初期化)
    // これがセグメンテーション違反を防ぐための必須ステップです。
    compiler = CS_create_compiler(); // MEM_Storage もここで初期化されるはず
    cs_set_current_compiler(compiler); 
    current_line = 1;

    // 2. 入力ファイルの設定
    // テストしたいブロック構文を含むファイルを使用
    yyin = fopen("comp/tests/block_test.cs", "r"); 
    if (yyin == NULL) {
        fprintf(stderr, "cannot open file\n");
        exit(1);
    }
    
    fprintf(stdout, "--- Starting Parsing Test ---\n");
    
    // 3. 構文解析の実行
    if (yyparse() != 0) { 
        fprintf(stderr, "\n--- !!! Parse Error Detected !!! ---\n");
        // cs_delete_compiler(compiler); // エラー時はメモリを解放する
        fclose(yyin);
        exit(1);
    }

    fprintf(stdout, "\n--- Parsing Successful! No Syntax Errors Found ---\n");

    // 4. クリーンアップ
    fclose(yyin);
    CS_delete_compiler(compiler);
    return 0;
}