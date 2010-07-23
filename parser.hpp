#pragma once

#include <tr1/memory>
#include <stack>
#include <lexer.hpp>
#include <token.hpp>
#include <node.hpp>

class Parser {
public:
    typedef std::tr1::shared_ptr<Parser> Ptr;
    typedef void (Parser::*Rule)(void);

    /* Creates a new parser that reads tokens from the given lexer. */
    Parser(Lexer::Ptr lexer);

    /* File structure and layout */
    void rule_translation_unit_class();
    void rule_translation_unit_struct();
    void rule_translation_unit_module();
    void rule_import_statement_list();
    void rule_import_statement();
    void rule_member_definition_list();
    void rule_member_definition();
    void rule_member_definition_tail();
    void rule_access_specifier_public();

    void rule_empty();
    void rule_file_class();
    void rule_file_struct();
    void rule_imports();
    void rule_import();
    void rule_defs_public();
    void rule_defs_private();
    void rule_def_member();
    void rule_def_ctor();
    void rule_def_dtor();

    /* Type names and structure */
    void rule_typename();
    void rule_typetoken();
    void rule_type_uint();
    void rule_type_int();

    /* Statements */
    void rule_init_assign();
    void rule_init_zero();
    void rule_init_function();

    /* Source code body */
    void rule_function();
    void rule_expr();

    void print_ast(Node::Ptr node, int level);
    void push(Node::Ptr node);

    std::stack<Token> sstack_;
    std::stack<Node::Ptr> nstack_;
    Node::Ptr node_;
};
