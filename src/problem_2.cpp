#include <iomanip>
#include <iostream>
#include <string>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

namespace x3 = boost::spirit::x3;

struct function_declaration {
    std::string return_type;
    std::string name;
};

BOOST_FUSION_ADAPT_STRUCT(
    ::function_declaration,
    (std::string, return_type)
    (std::string, name)
)

int main(int argc, char** argv){
    ::function_declaration result;
    boost::spirit::x3::ascii::space_type space;

    std::string file_contents("first second");
    auto it = file_contents.begin();
    auto end = file_contents.end();

    bool r = x3::phrase_parse(it, end, x3::lexeme[+x3::alpha] >> x3::lexeme[+x3::alpha], space, result);

    if(r && it == end){
        std::cout << "parse success" << std::endl;
    } else {
        std::cout << "parse fail" << std::endl;
    }

    return 0;
}
