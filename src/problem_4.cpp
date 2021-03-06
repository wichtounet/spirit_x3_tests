#include <string>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace x3_ast {

struct simple_type {
    bool const_;
};

struct template_function_declaration {
    simple_type return_type;
};

} //end of x3_ast namespace

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::simple_type,
    (bool, const_)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::template_function_declaration,
    (x3_ast::simple_type, return_type)
)

namespace x3_grammar {

    auto const skipper = x3::ascii::space;

    typedef x3::identity<struct simple_type> simple_type_id;
    x3::rule<simple_type_id, x3_ast::simple_type> const simple_type("simple_type");

    typedef x3::identity<struct template_function_declaration> template_function_declaration_id;
    x3::rule<template_function_declaration_id, x3_ast::template_function_declaration> const template_function_declaration("template_function_declaration");

    using parser_type = x3::any_parser<
        std::string::iterator,
        x3_ast::simple_type
    >;

    auto const simple_type_def = x3::attr(true);

    BOOST_SPIRIT_DEFINE(simple_type);

    auto simple_type_grammar = simple_type;

    parser_type type_grammar_create(){
        return x3::skip(skipper)[simple_type_grammar];
    }

    auto const template_function_declaration_def = type_grammar_create();

    BOOST_SPIRIT_DEFINE(template_function_declaration);

    auto parser = template_function_declaration;

} // end of grammar namespace

int main(int argc, char** argv){
    std::string file_contents = "useless";

    x3::ascii::space_type space;
    x3_ast::template_function_declaration result;
    x3::phrase_parse(file_contents.begin(), file_contents.end(), x3_grammar::parser, x3_grammar::skipper, result);

    return 0;
}
