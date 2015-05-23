#include <string>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

namespace x3 = boost::spirit::x3;

typedef std::string::iterator base_iterator_type;
typedef boost::spirit::classic::position_iterator2<base_iterator_type> pos_iterator_type;

namespace x3_ast {
    struct simple_type;
    struct pointer_type;
    typedef x3::variant<simple_type, x3::forward_ast<pointer_type>> type;

    struct simple_type {
        bool const_;
    };

    struct pointer_type {
        type base_type;
    };

    struct declaration {
        type return_type;
        std::string name;
    };
} //end of x3_ast namespace

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::simple_type, 
    (bool, const_)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::pointer_type, 
    (x3_ast::type, base_type)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::declaration,
    (x3_ast::type, return_type)
    (std::string, name)
)

namespace x3_grammar {
    typedef x3::identity<struct type> type_id;
    typedef x3::identity<struct simple_type> simple_type_id;
    typedef x3::identity<struct pointer_type> pointer_type_id;
    typedef x3::identity<struct declaration> declaration_id;

    x3::rule<type_id, x3_ast::type> const type("type");
    x3::rule<simple_type_id, x3_ast::simple_type> const simple_type("simple_type");
    x3::rule<pointer_type_id, x3_ast::pointer_type> const pointer_type("pointer_type");
    x3::rule<declaration_id, x3_ast::declaration> const declaration("declaration");

    auto const skipper = x3::ascii::space;

    auto const type_def = pointer_type | simple_type;
    auto const simple_type_def = x3::attr(true);
    auto const pointer_type_def = simple_type >> '*';

    BOOST_SPIRIT_DEFINE(
        type,
        simple_type,
        pointer_type
    );

    auto const type_grammar = x3::skip(skipper)[type];

    auto const declaration_def = type_grammar >> x3::lexeme[*x3::alpha];

    BOOST_SPIRIT_DEFINE(declaration);

    auto parser = declaration;

} // end of grammar namespace

int main(int argc, char** argv){
    std::string file_contents = "useless";

    pos_iterator_type it(file_contents.begin(), file_contents.end(), "fake");
    pos_iterator_type end;

    x3_ast::declaration result;
    x3::phrase_parse(it, end, x3_grammar::parser, x3_grammar::skipper, result);

    return 0;
}
