#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace x3_ast {

typedef x3::variant<int, char> instruction;

struct else_ {
    std::vector<instruction> instructions;
    //SOLUTION x3::unused_type fake_;
};

struct if_ {
    boost::optional<else_> else_;
};

} //end of x3_ast namespace

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::else_,
    (std::vector<x3_ast::instruction>, instructions)
    //SOLUTION (x3::unused_type, fake_)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::if_,
    (boost::optional<x3_ast::else_>, else_)
)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-shift-op-parentheses"

namespace x3_grammar {

    struct instruction_class {};
    struct else_class {};
    struct if_class {};

    x3::rule<instruction_class, x3_ast::instruction> const instruction("instruction");
    x3::rule<else_class, x3_ast::else_> const else_("else");
    x3::rule<if_class, x3_ast::if_> const if_("if");

    auto const instruction_def = x3::int_ | x3::char_;

    auto const else_def = *instruction 
        //SOLUTION > x3::attr(21)
        ;

    auto const if_def = -else_;

    BOOST_SPIRIT_DEFINE(
        if_ = if_def,
        else_ = else_def,
        instruction = instruction_def
    );

    auto parser = if_;

} // end of grammar namespace

#pragma clang diagnostic pop

int main(int argc, char** argv){
    std::string file_contents = "fake";

    auto& parser = x3_grammar::parser;

    x3_ast::if_ result;
    boost::spirit::x3::ascii::space_type space;

    x3::phrase_parse(file_contents.begin(), file_contents.end(), parser, space, result);

    return 0;
}
