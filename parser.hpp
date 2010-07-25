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

    void assignment();
    void assignment_list_assign();
    void assignment_list_mulassign();   
    void assignment_list_divassign();
    void assignment_list_modassign();
    void assignment_list_subassign();
    void assignment_list_addassign();
    void assignment_list_lshiftassign();    
    void assignment_list_rshiftassign();
    void assignment_list_bitandassign();
    void assignment_list_bitxorassign();
    void assignment_list_bitorassign();

    void logical_or();
    void logical_or_list();
    void logical_and();
    void logical_and_list();

    void bitwise_or();  
    void bitwise_or_list_or();
    void bitwise_or_list_xor();
    void bitwise_and();
    void bitwise_and_list();

    void equality();
    void equality_list_equal();
    void equality_list_nequal();

    void relation();
    void relation_list_gt();
    void relation_list_lt();    
    void relation_list_gteq();
    void relation_list_lteq();

    void shift();
    void shift_list_lshift();
    void shift_list_rshift();

    void addition();
    void addition_list_plus();
    void addition_list_minus();

    void multiplication();
    void multiplication_list_mul();
    void multiplication_list_div();
    void multiplication_list_mod();

    void unary_postfix();
    void unary_inc();
    void unary_dec();
    void unary_plus();
    void unary_minus();
    void unary_bang();
    void unary_tilde();
    void unary_star();

    void postfix();
    void postfix_list_index();
    void postfix_list_call();
    void postfix_list_dot();
    void postfix_list_inc();
    void postfix_list_dec();
    
    void primary_string();
    void primary_number();
    void primary_ident();
    void primary_group();    

    void empty();
    
    void print_ast(Node::Ptr node, int level);
    void push(Node::Ptr node);

    std::stack<Token> sstack_;
    std::stack<Node::Ptr> nstack_;
    Node::Ptr node_;
};
