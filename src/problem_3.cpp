#include <istream>
#include <sstream>
#include <iostream>
#include <string>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace x3_ast {

struct declaration {
    std::string name;
    char test;
    std::vector<std::string> template_types;
};

} //end of x3_ast namespace

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::declaration,
    (std::string, name)
    (char, test)
    (std::vector<std::string>, template_types)
)

namespace x3_grammar {
    typedef x3::identity<struct identifier> identifier_id;
    typedef x3::identity<struct declaration> declaration_id;

    x3::rule<identifier_id, std::string> const identifier("identifier");
    x3::rule<declaration_id, x3_ast::declaration> const declaration("declaration");

    auto const identifier_def = 
                x3::lexeme[(x3::char_('_') >> *(x3::alnum | x3::char_('_')))]
            |   x3::lexeme[(x3::alpha >> *(x3::alnum | x3::char_('_')))]
            ;

    auto const declaration_def = 
            identifier 
        >>  x3::char_('<')
        >>  identifier % ',';

    auto const parser = x3::grammar(
        "eddi", 
        identifier = identifier_def,
        declaration = declaration_def
        );

} // end of grammar namespace

int main(){
    x3_ast::declaration result;
    x3::space_type skip;

    std::string file_contents("asdf");
    auto it = file_contents.begin();
    auto end = file_contents.end();

    bool r = x3::phrase_parse(it, end, x3_grammar::parser, skip, result);

    if(r && it == end){
        return true;
    } else {
        return false;
    }
}
