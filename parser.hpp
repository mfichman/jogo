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
    void translation_unit_class();
    void translation_unit_interface();
    void translation_unit_struct();
    void translation_unit_module();

    void class_member_list_import();
    void class_member_list_def();
    void class_member_list_variable();
    void class_member_list_constructor();
    void class_member_list_destructor();
    void class_member_list_function();

    void interface_member_list_import();
    void interface_member_list_def();
    void interface_member_list_prototype();
    
    void struct_member_list_import();
    void struct_member_list_def();
    void struct_member_list_variable();
    void struct_member_list_constructor();
    void struct_member_list_function();

    void module_member_list_import();
    void module_member_list_def();
    void module_member_list_function();

    void import();
    void def();
    void variable();
    void constructor();
    void destructor();
    void function();
    void prototype();

    void access_public();
    void access_private();
    void access_protected();
    void storage();
    void native();
    void type_uint();
    void type_int();
    void type_qualified_name();

    void initializer_expression();
    void initializer_zero();
    void qualified_name();
    void qualified_name_tail();
    void compound_statement();

    void empty();
    
    void print_ast(Node::Ptr node, int level);
    void push(Node::Ptr node);

    std::stack<Token> sstack_;
    std::stack<Node::Ptr> nstack_;
    Node::Ptr node_;
};
