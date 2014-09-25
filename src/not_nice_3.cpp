#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace x3_ast {

typedef x3::variant<int, char, double> value;

} //end of x3_ast namespace

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-shift-op-parentheses"

namespace x3_grammar {

    struct value_class {};
    struct primary_value_class {};

    x3::rule<value_class, x3_ast::value> const value("value");
    x3::rule<primary_value_class, x3_ast::value> const primary_value("value");

    auto const primary_value_def = 
            x3::int_ 
        |   x3::char_ 
        |   x3::double_;

    auto const value_def = 
            primary_value
        |   '(' > primary_value > ')'
        //SOLUTION | '(' > x3::int_ > ')'
        //SOLUTION | '(' > x3::char_ > ')'
        //SOLUTION | '(' > x3::double_ > ')'
        ;

    BOOST_SPIRIT_DEFINE(
        value = value_def,
        primary_value = primary_value_def
    );

    auto parser = value;

} // end of grammar namespace

#pragma clang diagnostic pop

int main(int argc, char** argv){
    std::string file_contents = "fake";

    auto& parser = x3_grammar::parser;

    x3_ast::value result;
    boost::spirit::x3::ascii::space_type space;

    x3::phrase_parse(file_contents.begin(), file_contents.end(), parser, space, result);

    return 0;
}
