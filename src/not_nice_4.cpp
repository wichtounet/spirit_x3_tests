#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace x3_ast {

typedef x3::variant<int, char> instruction;

struct for_ {
    boost::optional<instruction> instruction_1;
    instruction instruction_2;
    instruction instruction_3;
};

} //end of x3_ast namespace

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::for_,
    (boost::optional<x3_ast::instruction>, instruction_1)
    (x3_ast::instruction, instruction_2)
    (x3_ast::instruction, instruction_3)
)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-shift-op-parentheses"

namespace x3_grammar {

    struct instruction_class {};
    struct for_class {};

    x3::rule<instruction_class, x3_ast::instruction> const instruction("instruction");
    x3::rule<for_class, x3_ast::for_> const for_("if");

    auto const instruction_def = x3::int_ | x3::char_;

    //SOLUTION: Create a rule with instruction as attribute
    //auto const sub_def = x3::int_;
    //and use it instead of x3::int_ in the next definition

    auto const for__def = 
            x3::lit("for") 
        >>  -x3::int_ 
        >>  ';' 
        >>  instruction 
        >>  ';' 
        >>  instruction;

    BOOST_SPIRIT_DEFINE(
        for_,
        instruction
    );

    auto parser = for_;

} // end of grammar namespace

#pragma clang diagnostic pop

int main(int argc, char** argv){
    std::string file_contents = "fake";

    auto& parser = x3_grammar::parser;

    x3_ast::for_ result;
    boost::spirit::x3::ascii::space_type space;

    x3::phrase_parse(file_contents.begin(), file_contents.end(), parser, space, result);

    return 0;
}
