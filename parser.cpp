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
    sstack_.push(SYM_NATIVE);
    sstack_.push(SYM_ACCESS);

    // TODO: ADD ARGUMENT LIST
    sstack_.push(TOK_RPAREN);
    sstack_.push(TOK_LPAREN);

    sstack_.push(TOK_INIT);
}

void Parser::destructor() {
    sstack_.push(SYM_COMPOUND_STATEMENT);
    sstack_.push(SYM_NATIVE);

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
    sstack_.push(SYM_EXPRESSION); 
    sstack_.push(TOK_EQUAL);
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

void Parser::compound_statement() {
    sstack_.push(TOK_RBRACE);

    // TODO: STATEMENTLIST
    sstack_.push(TOK_LBRACE);
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

    rule[make_pair(SYM_INITIALIZER, TOK_EQUAL)] = &Parser::initializer_expression;
    rule[make_pair(SYM_INITIALIZER, TOK_SEMI)] = &Parser::initializer_zero;
    
    rule[make_pair(SYM_QUALIFIED_NAME, TOK_TYPE)] = &Parser::qualified_name;
    rule[make_pair(SYM_QUALIFIED_NAME_TAIL, TOK_SCOPE)] = &Parser::qualified_name_tail;
    rule[make_pair(SYM_QUALIFIED_NAME_TAIL, TOK_SEMI)] = &Parser::empty;
    rule[make_pair(SYM_QUALIFIED_NAME_TAIL, TOK_TYPE)] = &Parser::empty;  
    rule[make_pair(SYM_QUALIFIED_NAME_TAIL, TOK_IDENT)] = &Parser::empty;
    rule[make_pair(SYM_QUALIFIED_NAME_TAIL, TOK_LBRACE)] = &Parser::empty;
    
    rule[make_pair(SYM_COMPOUND_STATEMENT, TOK_LBRACE)] = &Parser::compound_statement;

    return rule;
}

