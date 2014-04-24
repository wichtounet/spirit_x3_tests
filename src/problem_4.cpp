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
        x3_ast::simple_type, 
        decltype(x3::make_context<x3::skipper_tag>(skipper))
    >;
   
    parser_type type_grammar_create(){
        return x3::grammar(
            "eddi::type",
            simple_type = x3::attr(true)
            );
    }

    auto const type_grammar = type_grammar_create();
    
    auto const parser = x3::grammar(
        "eddi", 
        template_function_declaration = type_grammar
        );

} // end of grammar namespace

int main(int argc, char** argv){
    std::string file_contents = "useless";

    x3::ascii::space_type space;
    x3_ast::template_function_declaration result;
    x3::phrase_parse(file_contents.begin(), file_contents.end(), x3_grammar::parser, x3_grammar::skipper, result);

    return 0;
}
