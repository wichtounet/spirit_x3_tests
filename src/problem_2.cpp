#include <iomanip>
#include <iostream>
#include <string>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

namespace x3 = boost::spirit::x3;

namespace x3_ast {

struct function_declaration {
    std::string return_type;
    std::string name;
};

} //end of x3_ast namespace

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::function_declaration,
    (std::string, return_type)
    (std::string, name)
)

namespace x3_grammar {
    typedef x3::identity<struct function_declaration> function_declaration_id;
    x3::rule<function_declaration_id, x3_ast::function_declaration> const function_declaration("function_declaration");

    auto const function_declaration_def = 
            +x3::alpha 
        >>  +x3::alpha;

    auto const parser = x3::grammar(
        "eddi", 
        function_declaration = function_declaration_def
        );

} // end of grammar namespace

int main(int argc, char** argv){
    std::string file_contents("function b");

    auto& parser = x3_grammar::parser;

    x3_ast::function_declaration result;
    boost::spirit::x3::space_type space;

    typedef std::string::iterator base_iterator_type;
    typedef boost::spirit::classic::position_iterator2<base_iterator_type> pos_iterator_type;

    pos_iterator_type it(file_contents.begin(), file_contents.end(), "test");
    pos_iterator_type end;

    bool r = x3::phrase_parse(it, end, parser, space, result);

    if(r && it == end){
        return true;
    } else {
        auto& pos = it.get_position();
        std::cout <<
            "parse error at file " << pos.file <<
            " line " << pos.line << " column " << pos.column << std::endl <<
            "'" << it.get_currentline() << "'" << std::endl <<
            std::setw(pos.column) << " " << "^- here" << std::endl;

        return false;
    }

    return 0;
}
