#include <parser.hpp>
#include <stdexcept>
#include <iostream>
#include <map>
#include <cassert>

using namespace std;

typedef std::pair<Token, Token> TokenPair;
typedef std::map<TokenPair, Parser::Rule>::const_iterator RuleIterator;

static std::map<TokenPair, Parser::Rule> table_rule();
static const std::map<TokenPair, Parser::Rule> rule_ = table_rule();


Parser::Parser(Lexer::Ptr lexer) {
    sstack_.push(TOK_EOF);
    sstack_.push(SYM_TRANSLATION_UNIT);
    node_ = Node::Ptr(new Node);

    Token token = lexer->token();

    while (!sstack_.empty()) {
        if (token == sstack_.top()) {
            /* 
             * Terminal symbol matches the top of the stack, consume the token
             * and pop the corresponding symbol from the stack. 
             */
            /* Save the symbol node as a child of the current node */
            sstack_.pop();
            if (!lexer->value().empty()) {
                node_->child(Node::Ptr(new Node(lexer->value())));
            }
            token = lexer->token();
        } else if (SYM_POP == sstack_.top()) {
            assert(!nstack_.empty());
            node_ = nstack_.top();   
            nstack_.pop();
            sstack_.pop();
        } else {
            RuleIterator i = rule_.find(make_pair(sstack_.top(), token));
            if (i == rule_.end()) {
                /* 
                 * Could not find rule matching the symbol at the top of the
                 * stack and the input token.
                 */
                cerr << "Syntax error: line " << lexer->line_number();
                cerr << ":" << lexer->char_number() << endl;
                throw std::runtime_error("Syntax error");
            } else {

                /* Rule found; call the rule */
                /* Push special "POP" marker */
                /* Push node down on the stack, and save the current node */
                sstack_.pop();
                (this->*i->second)();
            }
        }  
    }
    cout << "File parsed" << endl;

    print_ast(node_, 0);
}

void Parser::push(Node::Ptr node) {
    nstack_.push(node_);
    node_->child(node);
    node_ = node;
    sstack_.push(SYM_POP);
}

void Parser::print_ast(Node::Ptr node, int level) {

    for (int i = 0; i < level; i++) {
        cout << "  ";
    }
    cout << node->value() << endl;
    for (size_t i = 0; i < node->nchild(); i++) {
        print_ast(node->child(i), level + 1);
    }
}

void Parser::translation_unit_class() {
    cout << "class" << endl;
    sstack_.push(SYM_CLASS_MEMBER_LIST);
    sstack_.push(TOK_SEMI);
    sstack_.push(SYM_QUALIFIED_NAME);
    sstack_.push(TOK_CLASS);
}

void Parser::translation_unit_interface() {
    sstack_.push(SYM_INTERFACE_MEMBER_LIST);
    sstack_.push(TOK_SEMI);
    sstack_.push(SYM_QUALIFIED_NAME);
    sstack_.push(TOK_INTERFACE);
}

void Parser::translation_unit_struct() {
    sstack_.push(SYM_STRUCT_MEMBER_LIST);
    sstack_.push(TOK_SEMI);
    sstack_.push(SYM_QUALIFIED_NAME);
    sstack_.push(TOK_STRUCT);
}

void Parser::translation_unit_module() {
    sstack_.push(SYM_MODULE_MEMBER_LIST);
    sstack_.push(TOK_SEMI);
    sstack_.push(SYM_QUALIFIED_NAME);
    sstack_.push(TOK_MODULE);
}


void Parser::class_member_list_import() {
    sstack_.push(SYM_CLASS_MEMBER_LIST);
    sstack_.push(SYM_IMPORT);
}

void Parser::class_member_list_def() {
    sstack_.push(SYM_CLASS_MEMBER_LIST);
    sstack_.push(SYM_DEF);
}

void Parser::class_member_list_variable() {
    sstack_.push(SYM_CLASS_MEMBER_LIST);
    sstack_.push(SYM_VARIABLE);
}

void Parser::class_member_list_constructor() {
    sstack_.push(SYM_CLASS_MEMBER_LIST);
    sstack_.push(SYM_CONSTRUCTOR);
}

void Parser::class_member_list_destructor() {
    sstack_.push(SYM_CLASS_MEMBER_LIST);
    sstack_.push(SYM_DESTRUCTOR);
}

void Parser::class_member_list_function() {
    sstack_.push(SYM_CLASS_MEMBER_LIST);
    sstack_.push(SYM_FUNCTION);
}


void Parser::interface_member_list_import() {
    sstack_.push(SYM_INTERFACE_MEMBER_LIST);
    sstack_.push(SYM_IMPORT);
}

void Parser::interface_member_list_def() {
    sstack_.push(SYM_INTERFACE_MEMBER_LIST);
    sstack_.push(SYM_DEF);
}

void Parser::interface_member_list_prototype() {
    sstack_.push(SYM_INTERFACE_MEMBER_LIST);
    sstack_.push(SYM_PROTOTYPE);
}

    
void Parser::struct_member_list_import() {
    sstack_.push(SYM_STRUCT_MEMBER_LIST);
    sstack_.push(SYM_IMPORT);
}

void Parser::struct_member_list_def() {
    sstack_.push(SYM_STRUCT_MEMBER_LIST);
    sstack_.push(SYM_IMPORT);
}

void Parser::struct_member_list_variable() {
    sstack_.push(SYM_STRUCT_MEMBER_LIST);
    sstack_.push(SYM_VARIABLE);
}

void Parser::struct_member_list_constructor() {
    sstack_.push(SYM_STRUCT_MEMBER_LIST);
    sstack_.push(SYM_CONSTRUCTOR);
}

void Parser::struct_member_list_function() {
    sstack_.push(SYM_STRUCT_MEMBER_LIST);
    sstack_.push(SYM_FUNCTION);
}


void Parser::module_member_list_import() {
    sstack_.push(SYM_MODULE_MEMBER_LIST);
    sstack_.push(SYM_IMPORT);
}

void Parser::module_member_list_def() {
    sstack_.push(SYM_MODULE_MEMBER_LIST);
    sstack_.push(SYM_DEF);
}

void Parser::module_member_list_function() {
    sstack_.push(SYM_MODULE_MEMBER_LIST);
    sstack_.push(SYM_FUNCTION);
}


void Parser::import() {
    sstack_.push(TOK_SEMI);
    sstack_.push(SYM_QUALIFIED_NAME);
    sstack_.push(TOK_IMPORT);
}

void Parser::def() {
    sstack_.push(TOK_SEMI);
    sstack_.push(TOK_TYPE);
    sstack_.push(SYM_TYPE);
    sstack_.push(TOK_DEF);
}

void Parser::variable() {
    sstack_.push(SYM_INITIALIZER);
    sstack_.push(TOK_IDENT);
    sstack_.push(SYM_TYPE);
    sstack_.push(SYM_STORAGE);
    sstack_.push(SYM_ACCESS);
}

void Parser::constructor() {
    sstack_.push(SYM_COMPOUND_STATEMENT);
    sstack_.push(SYM_ACCESS);

    // TODO: ADD ARGUMENT LIST
    sstack_.push(TOK_RPAREN);
    sstack_.push(TOK_LPAREN);

    sstack_.push(TOK_INIT);
}

void Parser::destructor() {
    sstack_.push(SYM_COMPOUND_STATEMENT);

    // TODO: ADD ARGUMENT LIST
    sstack_.push(TOK_RPAREN);
    sstack_.push(TOK_LPAREN);

    sstack_.push(TOK_DESTROY);
}

void Parser::function() {
    sstack_.push(SYM_COMPOUND_STATEMENT);
    sstack_.push(SYM_TYPE);
    sstack_.push(SYM_NATIVE);
    sstack_.push(SYM_STORAGE);
    sstack_.push(SYM_ACCESS);

    // TODO: ADD ARGUMENT LIST
    sstack_.push(TOK_RPAREN);
    sstack_.push(TOK_LPAREN);

    sstack_.push(TOK_IDENT);
}

void Parser::prototype() {
    sstack_.push(TOK_SEMI);
    sstack_.push(SYM_TYPE);

    // TODO: ADD ARGUMENT LIST
    sstack_.push(TOK_RPAREN);
    sstack_.push(TOK_LPAREN);

    sstack_.push(TOK_IDENT);
}


void Parser::access_public() {
    sstack_.push(TOK_PUBLIC);
}

void Parser::access_private() {
    sstack_.push(TOK_PRIVATE);
}

void Parser::access_protected() {
    sstack_.push(TOK_PROTECTED);
}

void Parser::storage() {
    sstack_.push(TOK_STATIC);
}

void Parser::native() {
    sstack_.push(TOK_NATIVE);
}

void Parser::type_uint() {
    sstack_.push(TOK_UINT);
}

void Parser::type_int() {
    sstack_.push(TOK_INT);
}

void Parser::type_qualified_name() {
    sstack_.push(SYM_QUALIFIED_NAME);
}

void Parser::initializer_expression() {
    sstack_.push(TOK_SEMI);
    sstack_.push(SYM_ASSIGNMENT); 
    sstack_.push(TOK_ASSIGN);
}

void Parser::initializer_zero() {
    sstack_.push(TOK_SEMI);
}


void Parser::qualified_name() {
    sstack_.push(SYM_QUALIFIED_NAME_TAIL);
    sstack_.push(TOK_TYPE);
}

void Parser::qualified_name_tail() {
    sstack_.push(SYM_QUALIFIED_NAME_TAIL);
    sstack_.push(TOK_TYPE);
    sstack_.push(TOK_SCOPE);
}

void Parser::assignment() {
cout << "assignment" << endl;
    sstack_.push(SYM_ASSIGNMENT_LIST);
    sstack_.push(SYM_LOGICAL_OR);
}

void Parser::assignment_list_assign() {
cout << "assignment LIST" << endl;
    sstack_.push(SYM_ASSIGNMENT);
    sstack_.push(TOK_ASSIGN);
}

void Parser::assignment_list_mulassign() {
    sstack_.push(SYM_ASSIGNMENT);
    sstack_.push(TOK_MULASSIGN);
}
   
void Parser::assignment_list_divassign() {
    sstack_.push(SYM_ASSIGNMENT);
    sstack_.push(TOK_DIVASSIGN);
}

void Parser::assignment_list_modassign() {
    sstack_.push(SYM_ASSIGNMENT);
    sstack_.push(TOK_MODASSIGN);
}

void Parser::assignment_list_subassign() {
    sstack_.push(SYM_ASSIGNMENT);
    sstack_.push(TOK_SUBASSIGN);
}

void Parser::assignment_list_addassign() {
    sstack_.push(SYM_ASSIGNMENT);
    sstack_.push(TOK_ADDASSIGN);
}

void Parser::assignment_list_lshiftassign() {
    sstack_.push(SYM_ASSIGNMENT);
    sstack_.push(TOK_LSHIFTASSIGN);
}
    
void Parser::assignment_list_rshiftassign() {
    sstack_.push(SYM_ASSIGNMENT);
    sstack_.push(TOK_RSHIFTASSIGN);
}

void Parser::assignment_list_bitandassign() {
    sstack_.push(SYM_ASSIGNMENT);
    sstack_.push(TOK_BITANDASSIGN);
}

void Parser::assignment_list_bitxorassign() {
    sstack_.push(SYM_ASSIGNMENT);
    sstack_.push(TOK_BITXORASSIGN);
}

void Parser::assignment_list_bitorassign() {
    sstack_.push(SYM_ASSIGNMENT);
    sstack_.push(TOK_BITORASSIGN);
}

void Parser::logical_or() {
    sstack_.push(SYM_LOGICAL_OR_LIST);
    sstack_.push(SYM_LOGICAL_AND);
}

void Parser::logical_or_list() {
    sstack_.push(SYM_LOGICAL_OR);
    sstack_.push(TOK_OR);
}

void Parser::logical_and() {
    sstack_.push(SYM_LOGICAL_AND_LIST);
    sstack_.push(SYM_BITWISE_OR);
}

void Parser::logical_and_list() {
    sstack_.push(SYM_LOGICAL_AND);
    sstack_.push(TOK_AND);
}

void Parser::bitwise_or() {
    sstack_.push(SYM_BITWISE_OR_LIST);  
    sstack_.push(SYM_BITWISE_AND);
}
  
void Parser::bitwise_or_list_or() {
    sstack_.push(SYM_BITWISE_OR);
    sstack_.push(TOK_BITOR);
}

void Parser::bitwise_or_list_xor() {
    sstack_.push(SYM_BITWISE_OR);
    sstack_.push(TOK_BITXOR);
}

void Parser::bitwise_and() {
    sstack_.push(SYM_BITWISE_AND_LIST);
    sstack_.push(SYM_EQUALITY);
}

void Parser::bitwise_and_list() {
    sstack_.push(SYM_BITWISE_AND);
    sstack_.push(TOK_BITAND);
}

void Parser::equality() {
    sstack_.push(SYM_EQUALITY_LIST);
    sstack_.push(SYM_RELATION);
}

void Parser::equality_list_equal() {
    sstack_.push(SYM_EQUALITY);
    sstack_.push(TOK_EQUAL);
}

void Parser::equality_list_nequal() {
    sstack_.push(SYM_EQUALITY);
    sstack_.push(TOK_NEQUAL);
}


void Parser::relation() {
    sstack_.push(SYM_RELATION_LIST);
    sstack_.push(SYM_SHIFT);
}

void Parser::relation_list_gt() {
    sstack_.push(SYM_RELATION);
    sstack_.push(TOK_GT);
}

void Parser::relation_list_lt() {
    sstack_.push(SYM_RELATION);
    sstack_.push(TOK_LT);
}
    
void Parser::relation_list_gteq() {
    sstack_.push(SYM_RELATION);
    sstack_.push(TOK_GTEQ);
}

void Parser::relation_list_lteq() {
    sstack_.push(SYM_RELATION);
    sstack_.push(TOK_LTEQ);
}


void Parser::shift() {
    sstack_.push(SYM_SHIFT_LIST);
    sstack_.push(SYM_ADDITION);
}

void Parser::shift_list_lshift() {
    sstack_.push(SYM_SHIFT);
    sstack_.push(TOK_LSHIFT);
}

void Parser::shift_list_rshift() {
    sstack_.push(SYM_SHIFT);
    sstack_.push(TOK_RSHIFT);
}


void Parser::addition() {
    sstack_.push(SYM_ADDITION_LIST);
    sstack_.push(SYM_MULTIPLICATION);
}

void Parser::addition_list_plus() {
    sstack_.push(SYM_ADDITION);
    sstack_.push(TOK_PLUS);
}

void Parser::addition_list_minus() {
    sstack_.push(SYM_ADDITION);
    sstack_.push(TOK_MINUS);
}


void Parser::multiplication() {
    sstack_.push(SYM_MULTIPLICATION_LIST);
    sstack_.push(SYM_UNARY);
}

void Parser::multiplication_list_mul() {
    sstack_.push(SYM_MULTIPLICATION);
    sstack_.push(TOK_STAR);
}

void Parser::multiplication_list_div() {
    sstack_.push(SYM_MULTIPLICATION);
    sstack_.push(TOK_SLASH);
}

void Parser::multiplication_list_mod() {
    sstack_.push(SYM_MULTIPLICATION);
    sstack_.push(TOK_MOD);
}


void Parser::unary_postfix() {
    sstack_.push(SYM_POSTFIX);
}

void Parser::unary_inc() {
    sstack_.push(SYM_UNARY);
    sstack_.push(TOK_INC);
}

void Parser::unary_dec() {
    sstack_.push(SYM_UNARY);
    sstack_.push(TOK_DEC);
}

void Parser::unary_plus() {
    sstack_.push(SYM_UNARY);
    sstack_.push(TOK_PLUS);
}

void Parser::unary_minus() {
    sstack_.push(SYM_UNARY);
    sstack_.push(TOK_MINUS);
}

void Parser::unary_bang() {
    sstack_.push(SYM_UNARY);
    sstack_.push(TOK_BANG);
}

void Parser::unary_tilde() {
    sstack_.push(SYM_UNARY);
    sstack_.push(TOK_TILDE);
}

void Parser::unary_star() {
    sstack_.push(SYM_UNARY);
    sstack_.push(TOK_STAR);
}


void Parser::postfix() {
cout << "Postfix" << endl;
    sstack_.push(SYM_POSTFIX_LIST);
    sstack_.push(SYM_PRIMARY);
}

void Parser::postfix_list_call() {
    sstack_.push(SYM_POSTFIX_LIST);
    sstack_.push(TOK_RPAREN);
    //sstack_.push(SYM_ARGUMENT_LIST);
    sstack_.push(TOK_LPAREN);
}

void Parser::postfix_list_index() {
    sstack_.push(SYM_POSTFIX_LIST);
    sstack_.push(TOK_RBRACKET);
    sstack_.push(SYM_ASSIGNMENT);
    sstack_.push(TOK_LBRACKET);
}

void Parser::postfix_list_dot() {
cout << "DOT" << endl;
    sstack_.push(SYM_POSTFIX_LIST);
    sstack_.push(TOK_IDENT);
    sstack_.push(TOK_DOT);
}

void Parser::postfix_list_inc() {
    sstack_.push(SYM_POSTFIX_LIST);
    sstack_.push(TOK_INC);
}

void Parser::postfix_list_dec() {
    sstack_.push(SYM_POSTFIX_LIST);
    sstack_.push(TOK_DEC);
}

    
void Parser::primary_string() {
    sstack_.push(TOK_STRING);
}

void Parser::primary_number() {
    sstack_.push(TOK_NUMBER);
}

void Parser::primary_ident() {
    sstack_.push(TOK_IDENT);
}

void Parser::primary_group() {
    sstack_.push(TOK_RPAREN);
    sstack_.push(SYM_ASSIGNMENT);
    sstack_.push(TOK_LPAREN);
}

void Parser::primary_qualified_name() {
    sstack_.push(SYM_QUALIFIED_NAME);
}

void Parser::compound_statement() {
    sstack_.push(TOK_RBRACE);
    sstack_.push(SYM_STATEMENT_LIST);
    sstack_.push(TOK_LBRACE);
}

void Parser::statement_list() {
    sstack_.push(SYM_STATEMENT_LIST); 
    sstack_.push(SYM_STATEMENT);
}

void Parser::statement_expression() {
    sstack_.push(TOK_SEMI);
    sstack_.push(SYM_ASSIGNMENT);
}

void Parser::statement_definition() {
    sstack_.push(SYM_INITIALIZER);
    sstack_.push(TOK_IDENT);
    sstack_.push(SYM_TYPE);
}

void Parser::empty() {
}

std::map<TokenPair, Parser::Rule> table_rule() {
    std::map<TokenPair, Parser::Rule> rule;

    rule[make_pair(SYM_TRANSLATION_UNIT, TOK_CLASS)] = &Parser::translation_unit_class; 
    rule[make_pair(SYM_TRANSLATION_UNIT, TOK_INTERFACE)] = &Parser::translation_unit_interface;
    rule[make_pair(SYM_TRANSLATION_UNIT, TOK_STRUCT)] = &Parser::translation_unit_struct;
    rule[make_pair(SYM_TRANSLATION_UNIT, TOK_MODULE)] = &Parser::translation_unit_module;

    rule[make_pair(SYM_CLASS_MEMBER_LIST, TOK_IMPORT)] = &Parser::class_member_list_import;
    rule[make_pair(SYM_CLASS_MEMBER_LIST, TOK_DEF)] = &Parser::class_member_list_def;
    rule[make_pair(SYM_CLASS_MEMBER_LIST, TOK_PUBLIC)] = &Parser::class_member_list_variable;
    rule[make_pair(SYM_CLASS_MEMBER_LIST, TOK_PRIVATE)] = &Parser::class_member_list_variable;
    rule[make_pair(SYM_CLASS_MEMBER_LIST, TOK_PROTECTED)] = &Parser::class_member_list_variable;
    rule[make_pair(SYM_CLASS_MEMBER_LIST, TOK_STATIC)] = &Parser::class_member_list_variable;
    rule[make_pair(SYM_CLASS_MEMBER_LIST, TOK_TYPE)] = &Parser::class_member_list_variable;
    rule[make_pair(SYM_CLASS_MEMBER_LIST, TOK_INT)] = &Parser::class_member_list_variable;
    rule[make_pair(SYM_CLASS_MEMBER_LIST, TOK_UINT)] = &Parser::class_member_list_variable;
    rule[make_pair(SYM_CLASS_MEMBER_LIST, TOK_INIT)] = &Parser::class_member_list_constructor;
    rule[make_pair(SYM_CLASS_MEMBER_LIST, TOK_DESTROY)] = &Parser::class_member_list_destructor;
    rule[make_pair(SYM_CLASS_MEMBER_LIST, TOK_IDENT)] = &Parser::class_member_list_function;
    rule[make_pair(SYM_CLASS_MEMBER_LIST, TOK_EOF)] = &Parser::empty;

    rule[make_pair(SYM_INTERFACE_MEMBER_LIST, TOK_IMPORT)] = &Parser::interface_member_list_import;
    rule[make_pair(SYM_INTERFACE_MEMBER_LIST, TOK_DEF)] = &Parser::interface_member_list_def;
    rule[make_pair(SYM_INTERFACE_MEMBER_LIST, TOK_IDENT)] = &Parser::interface_member_list_prototype;
    rule[make_pair(SYM_INTERFACE_MEMBER_LIST, TOK_EOF)] = &Parser::empty;

    rule[make_pair(SYM_STRUCT_MEMBER_LIST, TOK_IMPORT)] = &Parser::struct_member_list_import;
    rule[make_pair(SYM_STRUCT_MEMBER_LIST, TOK_DEF)] = &Parser::struct_member_list_def;
    rule[make_pair(SYM_STRUCT_MEMBER_LIST, TOK_PUBLIC)] = &Parser::struct_member_list_variable;
    rule[make_pair(SYM_STRUCT_MEMBER_LIST, TOK_PRIVATE)] = &Parser::struct_member_list_variable;
    rule[make_pair(SYM_STRUCT_MEMBER_LIST, TOK_PROTECTED)] = &Parser::struct_member_list_variable;
    rule[make_pair(SYM_STRUCT_MEMBER_LIST, TOK_STATIC)] = &Parser::struct_member_list_variable;
    rule[make_pair(SYM_STRUCT_MEMBER_LIST, TOK_INT)] = &Parser::struct_member_list_variable;
    rule[make_pair(SYM_STRUCT_MEMBER_LIST, TOK_UINT)] = &Parser::struct_member_list_variable;
    rule[make_pair(SYM_STRUCT_MEMBER_LIST, TOK_TYPE)] = &Parser::struct_member_list_variable;
    rule[make_pair(SYM_STRUCT_MEMBER_LIST, TOK_INIT)] = &Parser::struct_member_list_constructor;
    rule[make_pair(SYM_STRUCT_MEMBER_LIST, TOK_IDENT)] = &Parser::struct_member_list_function;
    rule[make_pair(SYM_STRUCT_MEMBER_LIST, TOK_EOF)] = &Parser::empty;

    rule[make_pair(SYM_MODULE_MEMBER_LIST, TOK_IMPORT)] = &Parser::module_member_list_import;
    rule[make_pair(SYM_MODULE_MEMBER_LIST, TOK_DEF)] = &Parser::module_member_list_def;
    rule[make_pair(SYM_MODULE_MEMBER_LIST, TOK_IDENT)] = &Parser::module_member_list_function;
    rule[make_pair(SYM_MODULE_MEMBER_LIST, TOK_EOF)] = &Parser::empty;

    rule[make_pair(SYM_IMPORT, TOK_IMPORT)] = &Parser::import;

    rule[make_pair(SYM_DEF, TOK_DEF)] = &Parser::def;
    rule[make_pair(SYM_VARIABLE, TOK_PUBLIC)] = &Parser::variable;
    rule[make_pair(SYM_VARIABLE, TOK_PRIVATE)] = &Parser::variable;
    rule[make_pair(SYM_VARIABLE, TOK_PROTECTED)] = &Parser::variable;
    rule[make_pair(SYM_VARIABLE, TOK_STATIC)] = &Parser::variable;
    rule[make_pair(SYM_VARIABLE, TOK_INT)] = &Parser::variable;
    rule[make_pair(SYM_VARIABLE, TOK_UINT)] = &Parser::variable;
    rule[make_pair(SYM_VARIABLE, TOK_TYPE)] = &Parser::variable;

    rule[make_pair(SYM_CONSTRUCTOR, TOK_INIT)] = &Parser::constructor;
    rule[make_pair(SYM_DESTRUCTOR, TOK_DESTROY)] = &Parser::destructor;
    rule[make_pair(SYM_FUNCTION, TOK_IDENT)] = &Parser::function;
    rule[make_pair(SYM_PROTOTYPE, TOK_IDENT)] = &Parser::prototype;

    rule[make_pair(SYM_ACCESS, TOK_PUBLIC)] = &Parser::access_public;
    rule[make_pair(SYM_ACCESS, TOK_PRIVATE)] = &Parser::access_private;
    rule[make_pair(SYM_ACCESS, TOK_PROTECTED)] = &Parser::access_protected;
    rule[make_pair(SYM_ACCESS, TOK_STATIC)] = &Parser::empty;
    rule[make_pair(SYM_ACCESS, TOK_NATIVE)] = &Parser::empty;
    rule[make_pair(SYM_ACCESS, TOK_INT)] = &Parser::empty;
    rule[make_pair(SYM_ACCESS, TOK_UINT)] = &Parser::empty;
    rule[make_pair(SYM_ACCESS, TOK_TYPE)] = &Parser::empty;
    rule[make_pair(SYM_ACCESS, TOK_LBRACE)] = &Parser::empty;

    rule[make_pair(SYM_STORAGE, TOK_STATIC)] = &Parser::storage;
    rule[make_pair(SYM_STORAGE, TOK_NATIVE)] = &Parser::empty;
    rule[make_pair(SYM_STORAGE, TOK_INT)] = &Parser::empty;
    rule[make_pair(SYM_STORAGE, TOK_UINT)] = &Parser::empty;
    rule[make_pair(SYM_STORAGE, TOK_TYPE)] = &Parser::empty;
    rule[make_pair(SYM_STORAGE, TOK_LBRACE)] = &Parser::empty;

    rule[make_pair(SYM_NATIVE, TOK_NATIVE)] = &Parser::native;
    rule[make_pair(SYM_NATIVE, TOK_INT)] = &Parser::empty;
    rule[make_pair(SYM_NATIVE, TOK_UINT)] = &Parser::empty;
    rule[make_pair(SYM_NATIVE, TOK_TYPE)] = &Parser::empty;
    rule[make_pair(SYM_NATIVE, TOK_LBRACE)] = &Parser::empty;

    rule[make_pair(SYM_TYPE, TOK_INT)] = &Parser::type_int;
    rule[make_pair(SYM_TYPE, TOK_UINT)] = &Parser::type_uint;
    rule[make_pair(SYM_TYPE, TOK_TYPE)] = &Parser::type_qualified_name; 

    rule[make_pair(SYM_INITIALIZER, TOK_ASSIGN)] = &Parser::initializer_expression;
    rule[make_pair(SYM_INITIALIZER, TOK_SEMI)] = &Parser::initializer_zero;
    
    rule[make_pair(SYM_QUALIFIED_NAME, TOK_TYPE)] = &Parser::qualified_name;
    rule[make_pair(SYM_QUALIFIED_NAME_TAIL, TOK_SCOPE)] = &Parser::qualified_name_tail;
    rule[make_pair(SYM_QUALIFIED_NAME_TAIL, TOK_SEMI)] = &Parser::empty;
    rule[make_pair(SYM_QUALIFIED_NAME_TAIL, TOK_TYPE)] = &Parser::empty;  
    rule[make_pair(SYM_QUALIFIED_NAME_TAIL, TOK_IDENT)] = &Parser::empty;
    rule[make_pair(SYM_QUALIFIED_NAME_TAIL, TOK_LBRACE)] = &Parser::empty;
    
    rule[make_pair(SYM_COMPOUND_STATEMENT, TOK_LBRACE)] = &Parser::compound_statement;
    rule[make_pair(SYM_STATEMENT_LIST, TOK_STRING)] = &Parser::statement_list;
    rule[make_pair(SYM_STATEMENT_LIST, TOK_NUMBER)] = &Parser::statement_list;
    rule[make_pair(SYM_STATEMENT_LIST, TOK_IDENT)] = &Parser::statement_list;
    rule[make_pair(SYM_STATEMENT_LIST, TOK_TYPE)] = &Parser::statement_list;
    rule[make_pair(SYM_STATEMENT_LIST, TOK_INT)] = &Parser::statement_list;
    rule[make_pair(SYM_STATEMENT_LIST, TOK_UINT)] = &Parser::statement_list;
    rule[make_pair(SYM_STATEMENT_LIST, TOK_LPAREN)] = &Parser::statement_list; 
    rule[make_pair(SYM_STATEMENT_LIST, TOK_RBRACE)] = &Parser::empty;
    rule[make_pair(SYM_STATEMENT, TOK_STRING)] = &Parser::statement_expression;
    rule[make_pair(SYM_STATEMENT, TOK_NUMBER)] = &Parser::statement_expression;
    rule[make_pair(SYM_STATEMENT, TOK_IDENT)] = &Parser::statement_expression;
    rule[make_pair(SYM_STATEMENT, TOK_LPAREN)] = &Parser::statement_expression; 
    rule[make_pair(SYM_STATEMENT, TOK_TYPE)] = &Parser::statement_definition;
    rule[make_pair(SYM_STATEMENT, TOK_INT)] = &Parser::statement_definition;
    rule[make_pair(SYM_STATEMENT, TOK_UINT)] = &Parser::statement_definition;


    rule[make_pair(SYM_ASSIGNMENT, TOK_STRING)] = &Parser::assignment;
    rule[make_pair(SYM_ASSIGNMENT, TOK_NUMBER)] = &Parser::assignment;
    rule[make_pair(SYM_ASSIGNMENT, TOK_IDENT)] = &Parser::assignment;
    rule[make_pair(SYM_ASSIGNMENT, TOK_TYPE)] = &Parser::assignment;
    rule[make_pair(SYM_ASSIGNMENT, TOK_LPAREN)] = &Parser::assignment; 
    rule[make_pair(SYM_ASSIGNMENT_LIST, TOK_ASSIGN)] = &Parser::assignment_list_assign;
    rule[make_pair(SYM_ASSIGNMENT_LIST, TOK_MULASSIGN)] = &Parser::assignment_list_mulassign;
    rule[make_pair(SYM_ASSIGNMENT_LIST, TOK_DIVASSIGN)] = &Parser::assignment_list_divassign;
    rule[make_pair(SYM_ASSIGNMENT_LIST, TOK_MODASSIGN)] = &Parser::assignment_list_modassign;
    rule[make_pair(SYM_ASSIGNMENT_LIST, TOK_SUBASSIGN)] = &Parser::assignment_list_subassign;
    rule[make_pair(SYM_ASSIGNMENT_LIST, TOK_ADDASSIGN)] = &Parser::assignment_list_addassign;
    rule[make_pair(SYM_ASSIGNMENT_LIST, TOK_LSHIFTASSIGN)] = &Parser::assignment_list_lshiftassign;
    rule[make_pair(SYM_ASSIGNMENT_LIST, TOK_RSHIFTASSIGN)] = &Parser::assignment_list_rshiftassign;
    rule[make_pair(SYM_ASSIGNMENT_LIST, TOK_BITANDASSIGN)] = &Parser::assignment_list_bitandassign;
    rule[make_pair(SYM_ASSIGNMENT_LIST, TOK_BITXORASSIGN)] = &Parser::assignment_list_bitxorassign;
    rule[make_pair(SYM_ASSIGNMENT_LIST, TOK_BITORASSIGN)] = &Parser::assignment_list_bitorassign;
    rule[make_pair(SYM_ASSIGNMENT_LIST, TOK_RPAREN)] = &Parser::empty;
    rule[make_pair(SYM_ASSIGNMENT_LIST, TOK_RBRACKET)] = &Parser::empty;
    rule[make_pair(SYM_ASSIGNMENT_LIST, TOK_SEMI)] = &Parser::empty;
    
    rule[make_pair(SYM_LOGICAL_OR, TOK_STRING)] = &Parser::logical_or;
    rule[make_pair(SYM_LOGICAL_OR, TOK_NUMBER)] = &Parser::logical_or;
    rule[make_pair(SYM_LOGICAL_OR, TOK_IDENT)] = &Parser::logical_or;
    rule[make_pair(SYM_LOGICAL_OR, TOK_TYPE)] = &Parser::logical_or;
    rule[make_pair(SYM_LOGICAL_OR, TOK_LPAREN)] = &Parser::logical_or; 
    rule[make_pair(SYM_LOGICAL_OR_LIST, TOK_OR)] = &Parser::logical_or_list;
    rule[make_pair(SYM_LOGICAL_OR_LIST, TOK_RPAREN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_OR_LIST, TOK_RBRACKET)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_OR_LIST, TOK_SEMI)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_OR_LIST, TOK_ASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_OR_LIST, TOK_MULASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_OR_LIST, TOK_DIVASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_OR_LIST, TOK_MODASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_OR_LIST, TOK_SUBASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_OR_LIST, TOK_ADDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_OR_LIST, TOK_LSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_OR_LIST, TOK_RSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_OR_LIST, TOK_BITANDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_OR_LIST, TOK_BITXORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_OR_LIST, TOK_BITORASSIGN)] = &Parser::empty;
    
    rule[make_pair(SYM_LOGICAL_AND, TOK_STRING)] = &Parser::logical_and;
    rule[make_pair(SYM_LOGICAL_AND, TOK_NUMBER)] = &Parser::logical_and;
    rule[make_pair(SYM_LOGICAL_AND, TOK_IDENT)] = &Parser::logical_and;
    rule[make_pair(SYM_LOGICAL_AND, TOK_TYPE)] = &Parser::logical_and;
    rule[make_pair(SYM_LOGICAL_AND, TOK_LPAREN)] = &Parser::logical_and; 
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_AND)] = &Parser::logical_and_list;
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_RPAREN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_RBRACKET)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_SEMI)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_ASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_MULASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_DIVASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_MODASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_SUBASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_ADDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_LSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_RSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_BITANDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_BITXORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_BITORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_LOGICAL_AND_LIST, TOK_OR)] = &Parser::empty;
    
    rule[make_pair(SYM_BITWISE_OR, TOK_STRING)] = &Parser::bitwise_or;
    rule[make_pair(SYM_BITWISE_OR, TOK_NUMBER)] = &Parser::bitwise_or;
    rule[make_pair(SYM_BITWISE_OR, TOK_IDENT)] = &Parser::bitwise_or;
    rule[make_pair(SYM_BITWISE_OR, TOK_TYPE)] = &Parser::bitwise_or;
    rule[make_pair(SYM_BITWISE_OR, TOK_LPAREN)] = &Parser::bitwise_or; 
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_BITOR)] = &Parser::bitwise_or_list_or;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_BITXOR)] = &Parser::bitwise_or_list_xor;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_RPAREN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_RBRACKET)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_SEMI)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_ASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_MULASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_DIVASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_MODASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_SUBASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_ADDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_LSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_RSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_BITANDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_BITXORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_BITORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_OR)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_OR_LIST, TOK_AND)] = &Parser::empty;
    
    rule[make_pair(SYM_BITWISE_AND, TOK_STRING)] = &Parser::bitwise_and;
    rule[make_pair(SYM_BITWISE_AND, TOK_NUMBER)] = &Parser::bitwise_and;
    rule[make_pair(SYM_BITWISE_AND, TOK_IDENT)] = &Parser::bitwise_and;
    rule[make_pair(SYM_BITWISE_AND, TOK_TYPE)] = &Parser::bitwise_and;
    rule[make_pair(SYM_BITWISE_AND, TOK_LPAREN)] = &Parser::bitwise_and; 
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_BITAND)] = &Parser::bitwise_and_list;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_RPAREN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_RBRACKET)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_SEMI)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_ASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_MULASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_DIVASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_MODASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_SUBASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_ADDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_LSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_RSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_BITANDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_BITXORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_BITORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_OR)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_AND)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_BITOR)] = &Parser::empty;
    rule[make_pair(SYM_BITWISE_AND_LIST, TOK_BITXOR)] = &Parser::empty;

    rule[make_pair(SYM_EQUALITY, TOK_STRING)] = &Parser::equality;
    rule[make_pair(SYM_EQUALITY, TOK_NUMBER)] = &Parser::equality;
    rule[make_pair(SYM_EQUALITY, TOK_IDENT)] = &Parser::equality;
    rule[make_pair(SYM_EQUALITY, TOK_TYPE)] = &Parser::equality;
    rule[make_pair(SYM_EQUALITY, TOK_LPAREN)] = &Parser::equality; 
    rule[make_pair(SYM_EQUALITY_LIST, TOK_EQUAL)] = &Parser::equality_list_equal;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_NEQUAL)] = &Parser::equality_list_nequal;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_RPAREN)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_RBRACKET)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_SEMI)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_ASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_MULASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_DIVASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_MODASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_SUBASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_ADDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_LSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_RSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_BITANDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_BITXORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_BITORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_OR)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_AND)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_BITAND)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_BITOR)] = &Parser::empty;
    rule[make_pair(SYM_EQUALITY_LIST, TOK_BITXOR)] = &Parser::empty;

    rule[make_pair(SYM_RELATION, TOK_STRING)] = &Parser::relation;
    rule[make_pair(SYM_RELATION, TOK_NUMBER)] = &Parser::relation;
    rule[make_pair(SYM_RELATION, TOK_IDENT)] = &Parser::relation;
    rule[make_pair(SYM_RELATION, TOK_TYPE)] = &Parser::relation;
    rule[make_pair(SYM_RELATION, TOK_LPAREN)] = &Parser::relation; 
    rule[make_pair(SYM_RELATION_LIST, TOK_GT)] = &Parser::relation_list_gt;
    rule[make_pair(SYM_RELATION_LIST, TOK_LT)] = &Parser::relation_list_lt;
    rule[make_pair(SYM_RELATION_LIST, TOK_GTEQ)] = &Parser::relation_list_gteq;
    rule[make_pair(SYM_RELATION_LIST, TOK_LTEQ)] = &Parser::relation_list_lteq;
    rule[make_pair(SYM_RELATION_LIST, TOK_RPAREN)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_RBRACKET)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_SEMI)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_ASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_MULASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_DIVASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_MODASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_SUBASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_ADDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_LSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_RSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_BITANDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_BITXORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_BITORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_OR)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_AND)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_BITAND)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_BITOR)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_BITXOR)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_EQUAL)] = &Parser::empty;
    rule[make_pair(SYM_RELATION_LIST, TOK_NEQUAL)] = &Parser::empty;

    rule[make_pair(SYM_SHIFT, TOK_STRING)] = &Parser::shift;
    rule[make_pair(SYM_SHIFT, TOK_NUMBER)] = &Parser::shift;
    rule[make_pair(SYM_SHIFT, TOK_IDENT)] = &Parser::shift;
    rule[make_pair(SYM_SHIFT, TOK_TYPE)] = &Parser::shift;
    rule[make_pair(SYM_SHIFT, TOK_LPAREN)] = &Parser::shift; 
    rule[make_pair(SYM_SHIFT_LIST, TOK_LSHIFT)] = &Parser::shift_list_lshift;
    rule[make_pair(SYM_SHIFT_LIST, TOK_RSHIFT)] = &Parser::shift_list_rshift;
    rule[make_pair(SYM_SHIFT_LIST, TOK_RPAREN)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_RBRACKET)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_SEMI)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_ASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_MULASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_DIVASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_MODASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_SUBASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_ADDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_LSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_RSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_BITANDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_BITXORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_BITORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_OR)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_AND)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_BITAND)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_BITOR)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_BITXOR)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_EQUAL)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_NEQUAL)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_GT)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_LT)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_GTEQ)] = &Parser::empty;
    rule[make_pair(SYM_SHIFT_LIST, TOK_LTEQ)] = &Parser::empty;

    rule[make_pair(SYM_ADDITION, TOK_STRING)] = &Parser::addition;
    rule[make_pair(SYM_ADDITION, TOK_NUMBER)] = &Parser::addition;
    rule[make_pair(SYM_ADDITION, TOK_IDENT)] = &Parser::addition;
    rule[make_pair(SYM_ADDITION, TOK_TYPE)] = &Parser::addition;
    rule[make_pair(SYM_ADDITION, TOK_LPAREN)] = &Parser::addition; 
    rule[make_pair(SYM_ADDITION_LIST, TOK_PLUS)] = &Parser::addition_list_plus;
    rule[make_pair(SYM_ADDITION_LIST, TOK_MINUS)] = &Parser::addition_list_plus;
    rule[make_pair(SYM_ADDITION_LIST, TOK_RPAREN)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_RBRACKET)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_SEMI)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_ASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_MULASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_DIVASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_MODASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_SUBASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_ADDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_LSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_RSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_BITANDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_BITXORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_BITORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_OR)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_AND)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_BITAND)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_BITOR)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_BITXOR)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_EQUAL)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_NEQUAL)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_GT)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_LT)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_GTEQ)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_LTEQ)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_LSHIFT)] = &Parser::empty;
    rule[make_pair(SYM_ADDITION_LIST, TOK_RSHIFT)] = &Parser::empty;

    rule[make_pair(SYM_MULTIPLICATION, TOK_STRING)] = &Parser::multiplication;
    rule[make_pair(SYM_MULTIPLICATION, TOK_NUMBER)] = &Parser::multiplication;
    rule[make_pair(SYM_MULTIPLICATION, TOK_IDENT)] = &Parser::multiplication;
    rule[make_pair(SYM_MULTIPLICATION, TOK_TYPE)] = &Parser::multiplication;
    rule[make_pair(SYM_MULTIPLICATION, TOK_LPAREN)] = &Parser::multiplication; 
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_STAR)] = &Parser::multiplication_list_mul;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_SLASH)] = &Parser::multiplication_list_div;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_MOD)] = &Parser::multiplication_list_mod;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_RPAREN)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_RBRACKET)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_SEMI)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_ASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_MULASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_DIVASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_MODASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_SUBASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_ADDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_LSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_RSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_BITANDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_BITXORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_BITORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_OR)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_AND)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_BITAND)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_BITOR)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_BITXOR)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_EQUAL)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_NEQUAL)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_GT)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_LT)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_GTEQ)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_LTEQ)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_LSHIFT)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_RSHIFT)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_PLUS)] = &Parser::empty;
    rule[make_pair(SYM_MULTIPLICATION_LIST, TOK_MINUS)] = &Parser::empty;

    rule[make_pair(SYM_UNARY, TOK_STRING)] = &Parser::unary_postfix;
    rule[make_pair(SYM_UNARY, TOK_NUMBER)] = &Parser::unary_postfix;
    rule[make_pair(SYM_UNARY, TOK_IDENT)] = &Parser::unary_postfix;
    rule[make_pair(SYM_UNARY, TOK_TYPE)] = &Parser::unary_postfix;
    rule[make_pair(SYM_UNARY, TOK_LPAREN)] = &Parser::unary_postfix; 
    rule[make_pair(SYM_UNARY, TOK_INC)] = &Parser::unary_inc;
    rule[make_pair(SYM_UNARY, TOK_DEC)] = &Parser::unary_dec;
    rule[make_pair(SYM_UNARY, TOK_PLUS)] = &Parser::unary_plus;
    rule[make_pair(SYM_UNARY, TOK_MINUS)] = &Parser::unary_minus;
    rule[make_pair(SYM_UNARY, TOK_BANG)] = &Parser::unary_bang;
    rule[make_pair(SYM_UNARY, TOK_TILDE)] = &Parser::unary_tilde;
    rule[make_pair(SYM_UNARY, TOK_STAR)] = &Parser::unary_star;
    
    rule[make_pair(SYM_POSTFIX, TOK_STRING)] = &Parser::postfix;
    rule[make_pair(SYM_POSTFIX, TOK_NUMBER)] = &Parser::postfix;
    rule[make_pair(SYM_POSTFIX, TOK_IDENT)] = &Parser::postfix;
    rule[make_pair(SYM_POSTFIX, TOK_TYPE)] = &Parser::postfix;
    rule[make_pair(SYM_POSTFIX, TOK_LPAREN)] = &Parser::postfix; 
    rule[make_pair(SYM_POSTFIX_LIST, TOK_LPAREN)] = &Parser::postfix_list_call;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_LBRACKET)] = &Parser::postfix_list_index;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_DOT)] = &Parser::postfix_list_dot;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_INC)] = &Parser::postfix_list_inc;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_DEC)] = &Parser::postfix_list_dec;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_RPAREN)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_RBRACKET)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_SEMI)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_ASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_MULASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_DIVASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_MODASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_SUBASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_ADDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_LSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_RSHIFTASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_BITANDASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_BITXORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_BITORASSIGN)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_OR)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_AND)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_BITAND)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_BITOR)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_BITXOR)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_EQUAL)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_NEQUAL)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_GT)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_LT)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_GTEQ)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_LTEQ)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_LSHIFT)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_RSHIFT)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_PLUS)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_MINUS)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_STAR)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_SLASH)] = &Parser::empty;
    rule[make_pair(SYM_POSTFIX_LIST, TOK_MOD)] = &Parser::empty;

    rule[make_pair(SYM_PRIMARY, TOK_STRING)] = &Parser::primary_string;
    rule[make_pair(SYM_PRIMARY, TOK_NUMBER)] = &Parser::primary_number;
    rule[make_pair(SYM_PRIMARY, TOK_IDENT)] = &Parser::primary_ident;
    rule[make_pair(SYM_PRIMARY, TOK_TYPE)] = &Parser::primary_qualified_name;
    rule[make_pair(SYM_PRIMARY, TOK_LPAREN)] = &Parser::primary_group; 

    return rule;
}

