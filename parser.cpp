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
    sstack_.push(SYM_FILE);
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

void Parser::rule_empty() {
}

void Parser::rule_file_class() {
    push(Node::Ptr(new Node("rule_file_class")));
    sstack_.push(SYM_DEFS);
    sstack_.push(SYM_IMPORTS);
    sstack_.push(TOK_SEMI);
    sstack_.push(SYM_TYPENAME);
    sstack_.push(TOK_CLASS);
}

void Parser::rule_file_struct() {
    sstack_.push(SYM_DEFS);
    sstack_.push(SYM_IMPORTS);
    sstack_.push(TOK_SEMI);
    sstack_.push(SYM_TYPENAME);
    sstack_.push(TOK_STRUCT);
}

void Parser::rule_imports() {
    sstack_.push(SYM_IMPORTS);
    sstack_.push(SYM_IMPORT);
}

void Parser::rule_import() {
    push(Node::Ptr(new Node("rule_import")));
    sstack_.push(TOK_SEMI);
    sstack_.push(SYM_TYPENAME);
    sstack_.push(TOK_IMPORT);
}

void Parser::rule_defs_public() {
    //push(Node::Ptr(new Node("rule_defs_public")));
    sstack_.push(SYM_DEFS);
    sstack_.push(SYM_DEF);
    sstack_.push(TOK_PUBLIC);
}

void Parser::rule_defs_private() {
    //push(Node::Ptr(new Node("rule_defs_private")));
    sstack_.push(SYM_DEFS);
    sstack_.push(SYM_DEF);
    sstack_.push(TOK_PRIVATE);
}

void Parser::rule_def_member() {
    push(Node::Ptr(new Node("rule_def_member")));
    sstack_.push(SYM_INIT);
    sstack_.push(TOK_IDENT);
    sstack_.push(SYM_TYPE);
}

void Parser::rule_def_ctor() {
    push(Node::Ptr(new Node("rule_def_ctor")));
    sstack_.push(SYM_FUNCTION);
    sstack_.push(TOK_INIT);
}

void Parser::rule_def_dtor() {
    push(Node::Ptr(new Node("rule_def_dtor")));
    sstack_.push(SYM_FUNCTION);
    sstack_.push(TOK_DESTROY);
}

void Parser::rule_typename() {
    push(Node::Ptr(new Node("rule_typename")));
    sstack_.push(SYM_TYPETOKEN);
    sstack_.push(TOK_TYPE);
}

void Parser::rule_typetoken() {
    //push(Node::Ptr(new Node("rule_typetoken")));
    sstack_.push(SYM_TYPETOKEN);
    sstack_.push(TOK_TYPE);
    sstack_.push(TOK_SCOPE);
}

void Parser::rule_type_uint() {
    sstack_.push(TOK_UINT);
}

void Parser::rule_type_int() {
    push(Node::Ptr(new Node("rule_type_int")));
    sstack_.push(TOK_INT);
}

void Parser::rule_init_assign() {
    push(Node::Ptr(new Node("rule_init_assign")));
    sstack_.push(TOK_SEMI);
    sstack_.push(SYM_EXPR);
    sstack_.push(TOK_EQUAL);
}

void Parser::rule_init_zero() {
    push(Node::Ptr(new Node("rule_init_zero")));
    sstack_.push(TOK_SEMI);
}

void Parser::rule_init_function() {
    push(Node::Ptr(new Node("rule_init_function")));
    sstack_.push(SYM_FUNCTION);
}

void Parser::rule_function() {
    push(Node::Ptr(new Node("rule_function")));
    sstack_.push(TOK_RBRACE);
    // sstack_.push(SYM_FUNC);
    sstack_.push(TOK_LBRACE);
    sstack_.push(TOK_RPAREN);
    // sstack_.push(SYM_ARGS);
    sstack_.push(TOK_LPAREN);
}

void Parser::rule_expr() {
    push(Node::Ptr(new Node("rule_expr")));
    sstack_.push(TOK_NUMBER);
}


std::map<TokenPair, Parser::Rule> table_rule() {
    std::map<TokenPair, Parser::Rule> rule;

    rule[make_pair(SYM_FILE, TOK_CLASS)] = &Parser::rule_file_class;
    rule[make_pair(SYM_FILE, TOK_STRUCT)] = &Parser::rule_file_struct;
    rule[make_pair(SYM_IMPORTS, TOK_IMPORT)] = &Parser::rule_imports; 
    rule[make_pair(SYM_IMPORTS, TOK_PUBLIC)] = &Parser::rule_empty;
    rule[make_pair(SYM_IMPORTS, TOK_PRIVATE)] = &Parser::rule_empty;
    rule[make_pair(SYM_IMPORT, TOK_IMPORT)] = &Parser::rule_import;
    rule[make_pair(SYM_DEFS, TOK_PUBLIC)] = &Parser::rule_defs_public;
    rule[make_pair(SYM_DEFS, TOK_PRIVATE)] = &Parser::rule_defs_private;
    rule[make_pair(SYM_DEFS, TOK_EOF)] = &Parser::rule_empty;
    rule[make_pair(SYM_DEF, TOK_TYPE)] = &Parser::rule_def_member;
    rule[make_pair(SYM_DEF, TOK_UINT)] = &Parser::rule_def_member;
    rule[make_pair(SYM_DEF, TOK_INT)] = &Parser::rule_def_member;
    rule[make_pair(SYM_DEF, TOK_INIT)] = &Parser::rule_def_ctor;
    rule[make_pair(SYM_DEF, TOK_DESTROY)] = &Parser::rule_def_dtor;
    rule[make_pair(SYM_INIT, TOK_EQUAL)] = &Parser::rule_init_assign;
    rule[make_pair(SYM_INIT, TOK_SEMI)] = &Parser::rule_init_zero;
    rule[make_pair(SYM_INIT, TOK_LPAREN)] = &Parser::rule_init_function;
    rule[make_pair(SYM_TYPE, TOK_UINT)] = &Parser::rule_type_uint;
    rule[make_pair(SYM_TYPE, TOK_INT)] = &Parser::rule_type_int;
    rule[make_pair(SYM_TYPE, TOK_TYPE)] = &Parser::rule_typename;
    rule[make_pair(SYM_TYPENAME, TOK_TYPE)] = &Parser::rule_typename;
    rule[make_pair(SYM_TYPETOKEN, TOK_SCOPE)] = &Parser::rule_typetoken;
    rule[make_pair(SYM_TYPETOKEN, TOK_SEMI)] = &Parser::rule_empty;
    rule[make_pair(SYM_TYPETOKEN, TOK_IDENT)] = &Parser::rule_empty;
    rule[make_pair(SYM_FUNCTION, TOK_LPAREN)] = &Parser::rule_function;
    rule[make_pair(SYM_EXPR, TOK_NUMBER)] = &Parser::rule_expr;
    
    return rule;
}

