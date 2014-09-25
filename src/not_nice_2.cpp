#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace x3_ast {

struct null { };

typedef x3::variant<int, char, null> instruction;

} //end of x3_ast namespace

namespace x3_grammar {

    struct instruction_class {};
    struct null_class {};

    x3::rule<instruction_class, x3_ast::instruction> const instruction("instruction");
    x3::rule<null_class, x3_ast::null> const null("null");

    auto const null_def = 
        //SOLUTION x3::eps > 
        x3::lit("null");

    auto const instruction_def = x3::int_ | x3::char_ | null;

    BOOST_SPIRIT_DEFINE(
        instruction = instruction_def,
        null = null_def
    );

    auto parser = instruction;

} // end of grammar namespace

int main(int argc, char** argv){
    std::string file_contents = "fake";

    auto& parser = x3_grammar::parser;

    x3_ast::instruction result;
    boost::spirit::x3::ascii::space_type space;

    x3::phrase_parse(file_contents.begin(), file_contents.end(), parser, space, result);

    return 0;
}
