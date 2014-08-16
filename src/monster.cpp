#include <string>

#define BOOST_SPIRIT_X3_NO_RTTI

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

typedef std::string::iterator pos_iterator_type;

namespace x3_ast {

struct simple_type;
struct array_type;
struct pointer_type;
struct template_type;

typedef x3::variant<
        simple_type, 
        x3::forward_ast<array_type>, 
        x3::forward_ast<template_type>, 
        x3::forward_ast<pointer_type>
    > type;

struct simple_type {
    bool const_;
    std::string base_type;
};

struct array_type {
    type base_type;
};

struct pointer_type {
    type base_type;
};

struct template_type {
    std::string base_type;
    std::vector<type> template_types;
};

struct integer_literal {
    int value;
};

struct integer_suffix_literal {
    int value;
    std::string suffix;
};

struct float_literal {
    double value;
};

struct string_literal {
    std::string value;
};

struct char_literal {
    char value;
};

struct variable_value {
    std::string variable_name;
};

typedef x3::variant<
            integer_literal,
            integer_suffix_literal,
            float_literal,
            string_literal,
            char_literal,
            variable_value
        > value;

struct foreach;
struct while_;
struct do_while;
struct foreach_in;
struct variable_declaration;
struct struct_declaration;
struct array_declaration;
struct return_;
struct delete_;
struct if_;

typedef x3::variant<
        foreach,
        foreach_in,
        if_,
        while_,
        do_while,
        return_,
        delete_,
        variable_declaration,
        struct_declaration,
        array_declaration
    > instruction;

struct while_ {
    value condition;
    std::vector<instruction> instructions;
};

struct do_while {
    value condition;
    std::vector<instruction> instructions;
};

struct foreach_in {
    type variable_type;
    std::string variable_name;
    std::string array_name;
    std::vector<instruction> instructions;
};

struct foreach {
    type variable_type;
    std::string variable_name;
    int from;
    int to;
    std::vector<instruction> instructions;
};

struct variable_declaration {
    type variable_type;
    std::string variable_name;
    boost::optional<x3_ast::value> value;
};

struct struct_declaration {
    type variable_type;
    std::string variable_name;
    std::vector<value> values;
};

struct array_declaration {
    type array_type;
    std::string array_name;
    value size;
};

struct return_ {
    int fake_;
    value return_value;
};

struct delete_ {
    int fake_;
    value value;
};

struct else_if {
    value condition;
    std::vector<instruction> instructions;
};

struct else_ {
    int fake_;
    std::vector<instruction> instructions;
};

struct if_ {
    value condition;
    std::vector<instruction> instructions;
    std::vector<else_if> else_ifs;
    boost::optional<x3_ast::else_> else_;
};

struct function_parameter {
    type  parameter_type;
    std::string parameter_name;
};

struct template_function_declaration {
    std::vector<std::string> template_types;
    type return_type;
    std::string name;
    std::vector<function_parameter> parameters;
    std::vector<instruction> instructions;
};

struct global_variable_declaration {
    type variable_type;
    std::string variable_name;
    boost::optional<x3_ast::value> value;
};

struct global_array_declaration {
    type array_type;
    std::string array_name;
    value size;
};

struct standard_import {
    std::string file;
};

struct import {
    std::string file;
};

struct member_declaration {
    type type;
    std::string name;
};

typedef x3::variant<
        member_declaration,
        array_declaration,
        template_function_declaration
    > struct_block;

struct template_struct {
    std::vector<std::string> template_types;
    std::string name;
    boost::optional<type> parent_type;
    std::vector<struct_block> blocks;
};

typedef x3::variant<
        standard_import,
        import,
        template_struct,
        template_function_declaration,
        global_array_declaration,
        global_variable_declaration
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
    x3_ast::simple_type, 
    (bool, const_)
    (std::string, base_type)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::array_type, 
    (x3_ast::type, base_type)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::pointer_type, 
    (x3_ast::type, base_type)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::template_type, 
    (std::string, base_type)
    (std::vector<x3_ast::type>, template_types)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::integer_literal, 
    (int, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::integer_suffix_literal, 
    (int, value)
    (std::string, suffix)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::float_literal, 
    (double, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::string_literal, 
    (std::string, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::char_literal, 
    (char, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::variable_value, 
    (std::string, variable_name)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::foreach_in, 
    (x3_ast::type, variable_type)
    (std::string, variable_name)
    (std::string, array_name)
    (std::vector<x3_ast::instruction>, instructions)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::foreach, 
    (x3_ast::type, variable_type)
    (std::string, variable_name)
    (int, from)
    (int, to)
    (std::vector<x3_ast::instruction>, instructions)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::while_, 
    (x3_ast::value, condition)
    (std::vector<x3_ast::instruction>, instructions)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::do_while, 
    (std::vector<x3_ast::instruction>, instructions)
    (x3_ast::value, condition)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::variable_declaration, 
    (x3_ast::type, variable_type)
    (std::string, variable_name)
    (boost::optional<x3_ast::value>, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::struct_declaration, 
    (x3_ast::type, variable_type)
    (std::string, variable_name)
    (std::vector<x3_ast::value>, values)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::array_declaration, 
    (x3_ast::type, array_type)
    (std::string, array_name)
    (x3_ast::value, size)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::return_, 
    (int, fake_)
    (x3_ast::value, return_value)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::delete_, 
    (int, fake_)
    (x3_ast::value, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::else_if, 
    (x3_ast::value, condition)
    (std::vector<x3_ast::instruction>, instructions)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::else_, 
    (int, fake_)
    (std::vector<x3_ast::instruction>, instructions)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::if_, 
    (x3_ast::value, condition)
    (std::vector<x3_ast::instruction>, instructions)
    (std::vector<x3_ast::else_if>, else_ifs)
    (boost::optional<x3_ast::else_>, else_)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::template_function_declaration,
    (std::vector<std::string>, template_types)
    (x3_ast::type, return_type)
    (std::string, name)
    (std::vector<x3_ast::function_parameter>, parameters)
    (std::vector<x3_ast::instruction>, instructions)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::function_parameter, 
    (x3_ast::type, parameter_type)
    (std::string, parameter_name)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::global_variable_declaration, 
    (x3_ast::type, variable_type)
    (std::string, variable_name)
    (boost::optional<x3_ast::value>, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::global_array_declaration, 
    (x3_ast::type, array_type)
    (std::string, array_name)
    (x3_ast::value, size)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::import,
    (std::string, file)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::standard_import,
    (std::string, file)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::member_declaration,
    (x3_ast::type, type)
    (std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::template_struct,
    (std::vector<std::string>, template_types)
    (std::string, name)
    (boost::optional<x3_ast::type>, parent_type)
    (std::vector<x3_ast::struct_block>, blocks)
)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-shift-op-parentheses"

namespace x3_grammar {
    typedef x3::identity<struct source_file> source_file_id;

    typedef x3::identity<struct type> type_id;
    typedef x3::identity<struct simple_type> simple_type_id;
    typedef x3::identity<struct array_type> array_type_id;
    typedef x3::identity<struct pointer_type> pointer_type_id;
    typedef x3::identity<struct template_type> template_type_id;
    
    typedef x3::identity<struct integer_literal> integer_literal_id;
    typedef x3::identity<struct integer_suffix_literal> integer_suffix_literal_id;
    typedef x3::identity<struct float_literal> float_literal_id;
    typedef x3::identity<struct string_literal> string_literal_id;
    typedef x3::identity<struct char_literal> char_literal_id;
    typedef x3::identity<struct variable_value> variable_value_id;
    typedef x3::identity<struct value> value_id;
    
    typedef x3::identity<struct instruction> instruction_id;
    typedef x3::identity<struct foreach> foreach_id;
    typedef x3::identity<struct foreach_in> foreach_in_id;
    typedef x3::identity<struct while_> while_id;
    typedef x3::identity<struct do_while> do_while_id;
    typedef x3::identity<struct variable_declaration> variable_declaration_id;
    typedef x3::identity<struct struct_declaration> struct_declaration_id;
    typedef x3::identity<struct array_declaration> array_declaration_id;
    typedef x3::identity<struct return_> return_id;
    typedef x3::identity<struct delete_> delete_id;
    typedef x3::identity<struct if_> if_id;
    typedef x3::identity<struct else_if> else_if_id;
    typedef x3::identity<struct else_> else_id;

    typedef x3::identity<struct function_parameter> function_parameter_id;
    typedef x3::identity<struct template_function_declaration> template_function_declaration_id;
    typedef x3::identity<struct global_variable_declaration> global_variable_declaration_id;
    typedef x3::identity<struct global_array_declaration> global_array_declaration_id;
    typedef x3::identity<struct import> import_id;
    typedef x3::identity<struct standard_import> standard_import_id;
    typedef x3::identity<struct member_declaration> member_declaration_id;
    typedef x3::identity<struct template_struct> template_struct_id;

    x3::rule<source_file_id, x3_ast::source_file> const source_file("source_file");

    x3::rule<type_id, x3_ast::type> const type("type");
    x3::rule<simple_type_id, x3_ast::simple_type> const simple_type("simple_type");
    x3::rule<array_type_id, x3_ast::array_type> const array_type("array_type");
    x3::rule<pointer_type_id, x3_ast::pointer_type> const pointer_type("pointer_type");
    x3::rule<template_type_id, x3_ast::template_type> const template_type("template_type");
    
    x3::rule<integer_literal_id, x3_ast::integer_literal> const integer_literal("integer_literal");
    x3::rule<integer_suffix_literal_id, x3_ast::integer_suffix_literal> const integer_suffix_literal("integer_suffix_literal");
    x3::rule<float_literal_id, x3_ast::float_literal> const float_literal("float_literal");
    x3::rule<string_literal_id, x3_ast::string_literal> const string_literal("string_literal");
    x3::rule<char_literal_id, x3_ast::char_literal> const char_literal("char_literal");
    x3::rule<variable_value_id, x3_ast::variable_value> const variable_value("variable_value");
    x3::rule<value_id, x3_ast::value> const value("value");
    
    x3::rule<instruction_id, x3_ast::instruction> const instruction("instruction");
    x3::rule<foreach_id, x3_ast::foreach> const foreach("foreach");
    x3::rule<foreach_in_id, x3_ast::foreach_in> const foreach_in("foreach_in");
    x3::rule<while_id, x3_ast::while_> const while_("while");
    x3::rule<do_while_id, x3_ast::do_while> const do_while("do_while");
    x3::rule<variable_declaration_id, x3_ast::variable_declaration> const variable_declaration("variable_declaration");
    x3::rule<struct_declaration_id, x3_ast::struct_declaration> const struct_declaration("struct_declaration");
    x3::rule<array_declaration_id, x3_ast::array_declaration> const array_declaration("array_declaration");
    x3::rule<return_id, x3_ast::return_> const return_("return");
    x3::rule<delete_id, x3_ast::delete_> const delete_("delete");
    x3::rule<if_id, x3_ast::if_> const if_("if");
    x3::rule<else_if_id, x3_ast::else_if> const else_if("else_if");
    x3::rule<else_id, x3_ast::else_> const else_("else");

    x3::rule<function_parameter_id, x3_ast::function_parameter> const function_parameter("function_parameter");
    x3::rule<template_function_declaration_id, x3_ast::template_function_declaration> const template_function_declaration("template_function_declaration");
    x3::rule<global_variable_declaration_id, x3_ast::global_variable_declaration> const global_variable_declaration("global_variable_declaration");
    x3::rule<global_array_declaration_id, x3_ast::global_array_declaration> const global_array_declaration("global_array_declaration");
    x3::rule<standard_import_id, x3_ast::standard_import> const standard_import("standard_import");
    x3::rule<import_id, x3_ast::import> const import("import");
    x3::rule<member_declaration_id, x3_ast::member_declaration> const member_declaration("member_declaration");
    x3::rule<template_struct_id, x3_ast::template_struct> const template_struct("template_struct");

    auto const skipper = 
            x3::ascii::space
        |   ("/*" >> *(x3::char_ - "*/") >> "*/")
        |   ("//" >> *(x3::char_ - (x3::eol | x3::eoi)) >> (x3::eol | x3::eoi));
   
    auto const const_ = 
            (x3::lit("const") > x3::attr(true))
        |   x3::attr(false);

    x3::real_parser<double, x3::strict_real_policies<double>> strict_double;

    auto const identifier = 
                x3::lexeme[(x3::char_('_') >> *(x3::alnum | x3::char_('_')))]
            |   x3::lexeme[(x3::alpha >> *(x3::alnum | x3::char_('_')))]
            ;

    auto const type_def =
            array_type
        |   pointer_type
        |   template_type
        |   simple_type;

    auto const simple_type_def = 
            const_
        >>  identifier;

    auto const template_type_def =
            identifier 
        >>  '<'
        >>  type % ','
        >>  '>';
    
    auto const array_type_def =
            (
                    template_type
                |   simple_type
            )
        >>  '['
        >>  ']';

    auto const pointer_type_def =
           (
                    template_type
                |   simple_type
            )
        >>  '*';

    BOOST_SPIRIT_DEFINE(
        type = type_def,
        simple_type = simple_type_def,
        template_type = template_type_def,
        array_type = array_type_def,
        pointer_type = pointer_type_def
    );

    const auto type_g = type;

    using type_parser_type = x3::any_parser<pos_iterator_type, x3_ast::type>;

    const type_parser_type type_grammar = x3::skip(skipper)[type_g];

    auto const integer_literal_def =
        x3::int_;
    
    auto const integer_suffix_literal_def =
        x3::lexeme[
                x3::int_ 
            >>  +x3::alpha
        ];

    auto const float_literal_def =
        strict_double;
    
    auto const char_literal_def =
            x3::lit('\'')
        >>  x3::char_
        >>  x3::lit('\'');
    
    auto const string_literal_def =
            x3::lit('"') 
        >>  x3::no_skip[*(x3::char_ - '"')] 
        >>  x3::lit('"');

    auto const variable_value_def =
        identifier;

    auto const value_def =
            variable_value
        |   integer_suffix_literal
        |   float_literal
        |   integer_literal
        |   string_literal
        |   char_literal;

    BOOST_SPIRIT_DEFINE(
        value = value_def,
        integer_literal = integer_literal_def,
        integer_suffix_literal = integer_suffix_literal_def,
        float_literal = float_literal_def,
        char_literal = char_literal_def,
        string_literal = string_literal_def,
        variable_value = variable_value_def
    );

    auto value_g = value;

    using value_parser_type = x3::any_parser<pos_iterator_type, x3_ast::value>;

    const value_parser_type value_grammar = x3::skip(skipper)[value_g];

    auto const instruction_def =
            if_
        |   foreach
        |   foreach_in
        |   while_
        |   do_while
        |   (return_ > ';')
        |   (delete_ > ';')
        |   (struct_declaration > ';')
        |   (array_declaration > ';')
        |   (variable_declaration > ';');

    auto const foreach_def =
            x3::lit("foreach")
        >>  '('
        >>  type_grammar
        >>  identifier
        >>  "from"
        >>  x3::int_
        >>  "to"
        >>  x3::int_
        >>  ')'
        >>  '{'
        >>  *instruction
        >>  '}';
    
    auto const foreach_in_def =
            x3::lit("foreach")
        >>  '('
        >>  type_grammar
        >>  identifier
        >>  "in"
        >>  identifier
        >>  ')'
        >>  '{'
        >>  *instruction
        >>  '}';
    
    auto const while_def =
            x3::lit("while")
        >>  '('
        >>  value_grammar
        >>  ')'
        >>  '{'
        >>  *instruction
        >>  '}';
    
    auto const do_while_def =
            x3::lit("do")
        >>  '{'
        >>  *instruction
        >>  '}'
        >>  "while"
        >>  '('
        >>  value_grammar
        >>  ')'
        >>  ';';
    
    auto const variable_declaration_def =
            type_grammar
        >>  identifier
        >>  -('=' >> value_grammar);
    
    auto const struct_declaration_def =
            type_grammar
        >>  identifier
        >>  '('
        >>  -(value_grammar % ',')
        >>  ')';
    
    auto const array_declaration_def =
            type_grammar
        >>  identifier
        >>  '['
        >>  value_grammar
        >>  ']';

    auto const return_def =
            x3::lit("return")
        >>  x3::attr(1)
        >>  value_grammar;

    auto const delete_def =
            x3::lit("delete")
        >>  x3::attr(1)
        >>  value_grammar;

    auto const if_def =
            x3::lit("if")
        >>  '('
        >>  value_grammar
        >>  ')'
        >>  '{'
        >>  *instruction
        >>  '}'
        >>  *else_if
        >>  -else_;

    auto const else_if_def =
            x3::lit("else")
        >>  x3::lit("if")
        >>  '('
        >>  value_grammar
        >>  ')'
        >>  '{'
        >>  *instruction
        >>  '}';
    
    auto const else_def =
            x3::lit("else")
        >>  x3::attr(1)
        >>  '{'
        >>  *instruction
        >>  '}';

    BOOST_SPIRIT_DEFINE(
        instruction = instruction_def,
        foreach = foreach_def,
        foreach_in = foreach_in_def,
        while_ = while_def,
        do_while = do_while_def,
        variable_declaration = variable_declaration_def,
        struct_declaration = struct_declaration_def,
        array_declaration = array_declaration_def,
        return_ = return_def,
        delete_ = delete_def,
        if_ = if_def,
        else_if = else_if_def,
        else_ = else_def
    );

    auto instruction_g = instruction;

    using instruction_parser_type = x3::any_parser<pos_iterator_type, x3_ast::instruction>;

    const instruction_parser_type instruction_grammar = x3::skip(skipper)[instruction_g];

    auto const source_file_def = 
         *(
                standard_import
            |   import
            |   template_struct
            |   template_function_declaration
            |   (global_array_declaration > ';')
            |   (global_variable_declaration > ';')
         );

    auto const standard_import_def = 
            x3::lit("import")
        >>  '<' 
        >   *x3::alpha
        >   '>';
    
    auto const import_def = 
            x3::lit("import")
        >>  '"' 
        >   *x3::alpha
        >   '"';
    
    auto const template_function_declaration_def = 
            -(
                    x3::lit("template")
                >>  '<'
                >>  (x3::lit("type") >> identifier) % ','
                >>  '>'
            )
        >>  type_grammar 
        >>  identifier
        >>  '(' 
        >>  -(function_parameter % ',')
        >   ')'
        >   '{' 
        >   *instruction_grammar
        >   '}';

    auto const global_variable_declaration_def =
            type_grammar
        >>  identifier
        >>  -('=' >> value_grammar);
    
    auto const global_array_declaration_def =
            type_grammar
        >>  identifier
        >>  '['
        >>  value_grammar
        >>  ']';

    auto const function_parameter_def =
            type_grammar
        >>  identifier;

    auto const member_declaration_def =
            type_grammar
        >>  identifier
        >>  ';';

    auto template_struct_def =
            -(  
                    x3::lit("template")
                >>  '<'
                >>  (x3::lit("type") >> identifier) % ','
                >>  '>'
            )
        >>  x3::lit("struct")
        >>  identifier
        >>  -(
                    "extends" 
                >>  type_grammar
             )
        >>  '{'
        >>  *(
                    member_declaration
                |   (array_declaration >> ';')
                |   template_function_declaration
             )
        >>  '}';

    BOOST_SPIRIT_DEFINE(
        source_file = source_file_def,
        function_parameter = function_parameter_def, 
        template_function_declaration = template_function_declaration_def, 
        global_variable_declaration = global_variable_declaration_def,
        global_array_declaration = global_array_declaration_def,
        standard_import = standard_import_def,
        import = import_def,
        member_declaration = member_declaration_def,
//        array_declaration = array_declaration_def,
        template_struct = template_struct_def
    );

    auto const parser = source_file;

} // end of grammar namespace

#pragma clang diagnostic pop

int main(int argc, char* argv[]){
    std::string file_contents = "asdf";

    auto& parser = x3_grammar::parser;
    auto& skipper = x3_grammar::skipper;

    x3_ast::source_file result;
    return x3::phrase_parse(file_contents.begin(), file_contents.end(), parser, skipper, result);
}
