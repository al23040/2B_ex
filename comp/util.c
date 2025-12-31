
#include <stdio.h>
#include <string.h>

#include "csua.h"

static CS_Compiler* current_compiler = NULL;

void cs_set_current_compiler(CS_Compiler* compiler) {
    current_compiler = compiler;
}

CS_Compiler* cs_get_current_compiler() { return current_compiler; }

DeclarationList* cs_chain_declaration(DeclarationList* decl_list,
                                      Declaration* decl) {
    DeclarationList* p;
    DeclarationList* list = cs_create_declaration_list(decl);
    if (decl_list == NULL) return list;
    for (p = decl_list; p->next; p = p->next)
        ;
    p->next = list;
    return decl_list;
}

StatementList* cs_chain_statement_list(StatementList* stmt_list,
                                       Statement* stmt) {
    StatementList* p = NULL;
    StatementList* nstmt_list = cs_create_statement_list(stmt);
    if (stmt_list == NULL) {
        return nstmt_list;
    }
    for (p = stmt_list; p->next; p = p->next)
        ;
    p->next = nstmt_list;

    return stmt_list;
}

FunctionDeclarationList* cs_chain_function_declaration_list(
    FunctionDeclarationList* func_list, FunctionDeclaration* func) {
    FunctionDeclarationList* p = NULL;
    FunctionDeclarationList* nfunc_list =
        cs_create_function_declaration_list(func);
    if (func_list == NULL) {
        return nfunc_list;
    }
    for (p = func_list; p->next; p = p->next)
        ;
    p->next = nfunc_list;
    return func_list;
}

ParameterList* cs_chain_parameter_list(ParameterList* list, CS_BasicType type,
                                       char* name) {
    ParameterList* p = NULL;
    ParameterList* current = cs_create_parameter(type, name);
    for (p = list; p->next; p = p->next)
        ;
    p->next = current;
    return list;
}

ArgumentList* cs_chain_argument_list(ArgumentList* list, Expression* expr) {
    ArgumentList* p;
    ArgumentList* current = cs_create_argument(expr);
    for (p = list; p->next; p = p->next)
        ;
    p->next = current;
    return list;
}

static Declaration* search_decls_from_list(DeclarationList* list,
                                           const char* name) {
    for (; list; list = list->next) {
        if (!strcmp(list->decl->name, name)) {
            return list->decl;
        }
    }
    return NULL;
}

Declaration* cs_search_decl_global(const char* name) {
    CS_Compiler* compiler = cs_get_current_compiler();
    return search_decls_from_list(compiler->decl_list, name);
}

static FunctionDeclaration* search_function_from_list(
    FunctionDeclarationList* list, const char* name) {
    for (; list; list = list->next) {
        if (!strcmp(list->func->name, name)) {
            return list->func;
        }
    }
    return NULL;
}

FunctionDeclaration* cs_search_function(const char* name) {
    CS_Compiler* compiler = cs_get_current_compiler();
    return search_function_from_list(compiler->func_list, name);
}

void cs_push_scope() {
    CS_Compiler* compiler = cs_get_current_compiler();
    LocalScope* new_scope = (LocalScope*)cs_malloc(sizeof(LocalScope));

    new_scope->decl_list = NULL;
    new_scope->outer = compiler->current_scope;
    if (compiler->current_scope) {
        new_scope->variable_count = compiler->current_scope->variable_count;
    } else {
        new_scope->variable_count = 0;
    }
    compiler->current_scope = new_scope;
}

void cs_pop_scope() {
    CS_Compiler* compiler = cs_get_current_compiler();
    if (compiler->current_scope == NULL) {
        return;
    }
    compiler->current_scope = compiler->current_scope->outer;
}

Declaration* cs_search_decl_in_block(char* name) {
    CS_Compiler* compiler = cs_get_current_compiler();
    LocalScope* scope = compiler->current_scope;
    int depth = 0;

    while (scope) {
        depth++;
        DeclarationList* dl = scope->decl_list;
        for (; dl != NULL; dl = dl->next) {
            if (strcmp(dl->decl->name, name) == 0) {
                //fprintf(stderr, "TRACE: Found [%s] in LOCAL scope (depth %d)\n", name, depth);
                return dl->decl;
            }
        }
        scope = scope->outer;
    }
    return NULL;
}