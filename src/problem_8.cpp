#include <iomanip>
#include <istream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/support/iterators/line_pos_iterator.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace x3_ast {

enum class Operator : unsigned int { ADD, SUB, DIV, MUL, MOD};

struct variable_value {
    std::string variable_name;
};

struct expression;

typedef x3::variant<
            variable_value,
            x3::forward_ast<expression>
        > value;

typedef x3::variant<value> operation_value;

struct operation {
    Operator op;
    operation_value value;
};

struct expression {
    value first;
    std::vector<operation> operations;
};

} //end of x3_ast namespace

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::variable_value, 
    (std::string, variable_name)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::expression, 
    (x3_ast::value, first)
    (std::vector<x3_ast::operation>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::operation, 
    (x3_ast::Operator, op)
    (x3_ast::operation_value, value)
)

namespace x3_grammar {
    typedef std::string::iterator base_iterator_type;
    typedef boost::spirit::line_pos_iterator<base_iterator_type> iterator_type;

    struct value_class {};
    struct primary_value_class {};
    struct variable_value_class {};
    struct unary_expression_class {};
    struct cast_expression_class {};
    struct additive_expression_class {};
    struct multiplicative_expression_class {};

    x3::rule<variable_value_class, x3_ast::variable_value> const variable_value("variable_value");
    x3::rule<unary_expression_class, x3_ast::value> const unary_expression("unary_expression");
    x3::rule<additive_expression_class, x3_ast::expression> const additive_expression("additive_expression");
    x3::rule<multiplicative_expression_class, x3_ast::expression> const multiplicative_expression("multiplicative_expression");
    x3::rule<cast_expression_class, x3_ast::value> const cast_expression("cast_expression");
    x3::rule<value_class, x3_ast::value> const value("value");
    x3::rule<primary_value_class, x3_ast::value> const primary_value("primary_value");

    auto const identifier =
                x3::lexeme[(x3::char_('_') >> *(x3::alnum | x3::char_('_')))]
            |   x3::lexeme[(x3::alpha >> *(x3::alnum | x3::char_('_')))]
            ;

    auto const variable_value_def = identifier;

    auto const primary_value_def = variable_value;

    auto const unary_expression_def = primary_value;

    auto const cast_expression_def = unary_expression;

    auto const multiplicative_expression_def =
            cast_expression
        >>  *(x3::attr(x3_ast::Operator::DIV) > cast_expression);

    auto const additive_expression_def =
            multiplicative_expression
        >>  *(x3::attr(x3_ast::Operator::ADD) > multiplicative_expression);

    auto const value_def = additive_expression;

    BOOST_SPIRIT_DEFINE(
        value = value_def,
        primary_value = primary_value_def,
        variable_value = variable_value_def,
        unary_expression = unary_expression_def,
        additive_expression = additive_expression_def,
        multiplicative_expression = multiplicative_expression_def,
        cast_expression = cast_expression_def
    );

} // end of grammar namespace

int main(){
    std::string file ="test";

    std::ifstream in(file.c_str(), std::ios::binary);
    in.unsetf(std::ios::skipws);

    in.seekg(0, std::istream::end);
    std::size_t size(static_cast<size_t>(in.tellg()));
    in.seekg(0, std::istream::beg);

    std::string file_contents;
    file_contents.resize(size);
    in.read(&file_contents[0], size);

    x3_ast::value result;

    x3_grammar::iterator_type it(file_contents.begin());
    x3_grammar::iterator_type end(file_contents.end());

    auto const parser = x3_grammar::value;
    auto& skipper = x3::ascii::space;

    bool r = x3::phrase_parse(it, end, parser, skipper, result);
    return r && it == end;
}
