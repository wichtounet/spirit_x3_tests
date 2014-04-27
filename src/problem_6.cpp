#include <string>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace x3_ast {

typedef x3::variant<char, float> value;

struct return_ {
    value return_value;
};

typedef x3::variant<return_, char> instruction;

} //end of x3_ast namespace

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::return_, 
    (x3_ast::value, return_value)
)

namespace x3_grammar {
    typedef x3::identity<struct instruction> instruction_id;
    typedef x3::identity<struct return_> return_id;
    typedef x3::identity<struct value> value_id;

    x3::rule<instruction_id, x3_ast::instruction> const instruction("instruction");
    x3::rule<return_id, x3_ast::return_> const return_("return");
    x3::rule<value_id, x3_ast::value> const value("value");

    auto const skipper = x3::ascii::space;

    auto const value_def = x3::char_ | x3::float_;

    using value_parser_type = x3::any_parser<std::string::iterator, x3_ast::value>;

    value_parser_type value_grammar_create(){
        return x3::skip(skipper)[x3::grammar(
            "eddi::value",
            value = value_def
            )];
    }

    auto const value_grammar = value_grammar_create();

	auto const parser = x3::grammar("eddi", 
			instruction = (return_ >> ';') | (x3::alnum >> ';'),
            return_ = x3::lit("return") >>  value_grammar);

} // end of grammar namespace

bool parse(){
    std::string file_contents = "asdf";

    x3_ast::instruction result;
	return x3::phrase_parse(file_contents.begin(), file_contents.end(), x3_grammar::parser, x3_grammar::skipper, result);
}
