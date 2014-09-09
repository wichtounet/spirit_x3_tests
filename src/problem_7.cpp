#include <string>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>

namespace x3 = boost::spirit::x3;

typedef std::string::iterator base_iterator_type;
typedef boost::spirit::classic::position_iterator2<base_iterator_type> pos_iterator_type;

namespace x3_ast {
    struct expression;

    typedef x3::variant<char, x3::forward_ast<expression>, int> value;

    struct expression {
        value a ;
        int b;
        value c;
    };

} //end of x3_ast namespace

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::expression, 
    (x3_ast::value, a)
    (int, b)
    (x3_ast::value, c)
)

namespace x3_grammar {
    struct expression_class {};
    struct expression_2_class {};

    x3::rule<expression_class, x3_ast::expression> const expression("expression");
    x3::rule<expression_2_class, x3_ast::expression> const expression_2("expression_2");

    auto const skipper = x3::ascii::space;

    auto const expression_def = 
        x3::int_ >> x3::int_ >> x3::int_;

    auto const expression_2_def = 
        expression >> x3::int_ >> expression;

    BOOST_SPIRIT_DEFINE(
        expression_2 = expression_2_def,
        expression = expression_def
    );

    auto parser = expression_2;

} // end of grammar namespace

int main(int argc, char** argv){
    std::string file_contents = "useless";

    pos_iterator_type it(file_contents.begin(), file_contents.end(), "fake");
    pos_iterator_type end;

    x3_ast::expression result;
    x3::phrase_parse(it, end, x3_grammar::parser, x3_grammar::skipper, result);

    return 0;
}
