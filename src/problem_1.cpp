#include <iomanip>
#include <istream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

namespace x3 = boost::spirit::x3;

namespace x3_ast {

struct function_declaration {
    std::string type;
    std::string name;
};

struct import {
    int fake_;
    std::string file;
};

typedef x3::variant<
        function_declaration,
        import
    > block;

struct source_file {
    std::vector<block> blocks;
};

} //end of x3_ast namespace

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::source_file,
    (std::vector<x3_ast::block>, blocks)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::function_declaration,
    (std::string, type)
    (std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::import,
    //(int, fake_)
    (std::string, file)
)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-shift-op-parentheses"

namespace x3_grammar {

    typedef x3::identity<struct source_file> source_file_id;
    typedef x3::identity<struct function_declaration> function_declaration_id;
    typedef x3::identity<struct import> import_id;

    x3::rule<source_file_id, x3_ast::source_file> const source_file("source_file");
    x3::rule<function_declaration_id, x3_ast::function_declaration> const function_declaration("function_declaration");
    x3::rule<import_id, x3_ast::import> const import("import");

    auto const import_def = 
     //       x3::attr(1) >>
        x3::lit("import")
        >>  '"' 
        >   *x3::alpha
        >   '"';
    
    auto const function_declaration_def = 
            *x3::alpha 
        >>  *x3::alpha;
    
    auto const source_file_def = 
         *(
               import
            |   function_declaration
         );
    
    auto const parser = x3::grammar(
        "eddi", 
        source_file = source_file_def,
        function_declaration = function_declaration_def, 
        import = import_def);

} // end of grammar namespace

#pragma clang diagnostic pop

bool parse(const std::string& file){
    std::ifstream in(file.c_str(), std::ios::binary);
    in.unsetf(std::ios::skipws);

    in.seekg(0, std::istream::end);
    std::size_t size(static_cast<size_t>(in.tellg()));
    in.seekg(0, std::istream::beg);

    std::string file_contents;
    file_contents.resize(size);
    in.read(&file_contents[0], size);

    auto& parser = x3_grammar::parser;

    x3_ast::source_file result;
    boost::spirit::x3::ascii::space_type space;

    typedef std::string::iterator base_iterator_type;
    typedef boost::spirit::classic::position_iterator2<base_iterator_type> pos_iterator_type;

    pos_iterator_type it(file_contents.begin(), file_contents.end(), file);
    pos_iterator_type end;

    try {
        bool r = x3::phrase_parse(it, end, parser, space, result);

        if(r && it == end){
            return true;
        } else {
            return false;
        }
    } catch(const boost::spirit::x3::expectation_failure<pos_iterator_type>& e){
        return false;
    }
}

int main(int argc, char** argv){
    if(argc == 1){
        std::cout << "Not enough args" << std::endl;
        return 1;
    }

    std::string file(argv[1]);

    if(parse(file)){
        std::cout << "succeeded" << std::endl;
    } else {
        std::cout << "failed" << std::endl;
    }

    return 0;
}
