#include <string>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace x3_ast {

struct string_literal {
    std::string value;
};

struct variable_value {
    std::string variable_name;
};

typedef x3::variant<string_literal, variable_value> value;

struct variable_declaration {
    std::string variable_name;
};

struct return_ {
    value return_value;
};

typedef x3::variant<return_, variable_declaration> instruction;

} //end of x3_ast namespace

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::string_literal, 
    (std::string, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::variable_value, 
    (std::string, variable_name)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::variable_declaration, 
    (std::string, variable_name)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::return_, 
    (x3_ast::value, return_value)
)

namespace x3_grammar {
    typedef x3::identity<struct string_literal> string_literal_id;
    typedef x3::identity<struct variable_value> variable_value_id;
    typedef x3::identity<struct value> value_id;
    
    typedef x3::identity<struct instruction> instruction_id;
    typedef x3::identity<struct variable_declaration> variable_declaration_id;
    typedef x3::identity<struct return_> return_id;

    x3::rule<string_literal_id, x3_ast::string_literal> const string_literal("string_literal");
    x3::rule<variable_value_id, x3_ast::variable_value> const variable_value("variable_value");
    x3::rule<value_id, x3_ast::value> const value("value");
    
    x3::rule<instruction_id, x3_ast::instruction> const instruction("instruction");
    x3::rule<variable_declaration_id, x3_ast::variable_declaration> const variable_declaration("variable_declaration");
    x3::rule<return_id, x3_ast::return_> const return_("return");

    auto const skipper = x3::ascii::space;

    auto const value_def = variable_value | string_literal;

    using value_parser_type = x3::any_parser<std::string::iterator, x3_ast::value>;

    value_parser_type value_grammar_create(){
        return x3::skip(skipper)[x3::grammar(
            "eddi::value",
            value = value_def,
            string_literal = +x3::char_,
            variable_value = +x3::char_
            )];
    }

    auto const value_grammar = value_grammar_create();

    auto const instruction_def =
            (return_ >> ';')
        |   (variable_declaration >> ';');

	auto const parser = x3::grammar("eddi", 
			instruction = instruction_def,
            variable_declaration = +x3::char_,
            return_ = x3::lit("return") >>  value_grammar);

} // end of grammar namespace

bool parse(){
    std::string file_contents = "asdf";

    auto& parser = x3_grammar::parser;
    auto& skipper = x3_grammar::skipper;

    x3_ast::instruction result;
	return x3::phrase_parse(file_contents.begin(), file_contents.end(), parser, skipper, result);
}
