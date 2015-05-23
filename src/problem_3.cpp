#include <istream>
#include <sstream>
#include <iostream>
#include <string>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace x3_ast {

struct declaration {
    std::vector<std::string> template_types;
    std::string name;
};

} //end of x3_ast namespace

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::declaration,
    (std::vector<std::string>, template_types)
    (std::string, name)
)

namespace x3_grammar {
    typedef x3::identity<struct identifier> identifier_id;
    typedef x3::identity<struct declaration> declaration_id;

    x3::rule<identifier_id, std::string> const identifier("identifier");
    x3::rule<declaration_id, x3_ast::declaration> const declaration("declaration");

    auto const identifier_def = x3::lexeme[+x3::alnum];
    
    auto const declaration_def = 
            x3::lit('<')
        >>  *identifier
        >>  '>'
        >>  identifier
        ;

    BOOST_SPIRIT_DEFINE(
        declaration, 
        identifier
    );

    auto parser = declaration;

} // end of grammar namespace

int main(){
    std::string file_contents("adsf");
    x3_ast::declaration result;
    boost::spirit::x3::ascii::space_type space;

    auto it = file_contents.begin();
    auto end = file_contents.end();

    bool r = x3::phrase_parse(it, end, x3_grammar::parser, space, result);

    if(r && it == end){
        return true;
    } else {

        return false;
    }
}
