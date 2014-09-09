//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include <iomanip>
#include <istream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

#define BOOST_SPIRIT_X3_NO_RTTI

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/support/iterators/line_pos_iterator.hpp>

#include <boost/fusion/adapted/boost_tuple.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace x3 = boost::spirit::x3;

namespace eddic {
namespace ast {

enum class BuiltinType : unsigned int {
    SIZE,
    LENGTH
};

enum class Operator : unsigned int {
    ASSIGN,

    ADD,
    SUB,
    DIV,
    MUL,
    MOD,

    AND,
    OR,
    NOT,

    DEC,
    INC,

    EQUALS,
    NOT_EQUALS,
    LESS,
    LESS_EQUALS,
    GREATER,
    GREATER_EQUALS,

    //Prefix operators

    STAR,               /**< Star operator, to dereference pointers. */ 
    ADDRESS,            /**< Address operator, get address of variable. */

    //Postfix operators

    BRACKET,            /**< Bracket operator, to access array index. */ 
    DOT,                /**< Dot operator, to access to members */ 
    CALL                /**< Call operator, to call member functions */ 
};

}}

using namespace eddic;

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

//*****************************************

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

struct variable_value : x3::position_tagged {
    std::string variable_name;
};

struct true_ { };
struct false_ { };
struct null { };

struct new_array;
struct new_;
struct builtin_operator;
struct function_call;

struct expression;
struct prefix_operation;
struct cast;
struct ternary;
struct assignment;

typedef x3::variant<
            integer_literal,
            integer_suffix_literal,
            float_literal,
            string_literal,
            char_literal,
            variable_value,
            true_,
            false_,
            null,
            x3::forward_ast<new_array>,
            x3::forward_ast<new_>,
            builtin_operator,
            function_call,
            x3::forward_ast<prefix_operation>,
            x3::forward_ast<cast>,
            x3::forward_ast<ternary>,
            x3::forward_ast<expression>,
            x3::forward_ast<assignment>
        > value;

struct function_call : x3::position_tagged {
    std::string function_name;
    std::vector<type> template_types;
    std::vector<value> values;
};

struct builtin_operator {
    ast::BuiltinType type;
    std::vector<value> values;
};

struct new_array : x3::position_tagged {
    type type;
    value size;
};

struct new_ : x3::position_tagged {
    type type;
    std::vector<value> values;
};

struct call_operation_value {
    std::string function_name;
    std::vector<type> template_types;
    std::vector<value> values;
};

struct identifier_pack {
    std::string identifier;
};

typedef x3::variant<
        value,
        identifier_pack,
        call_operation_value
    >  operation_value;

struct operation {
    ast::Operator op;
    operation_value value;
};

struct expression : x3::position_tagged {
    value first;
    std::vector<operation> operations;
};

struct prefix_operation : x3::position_tagged {
    value left_value;
    ast::Operator op;
};

struct cast : x3::position_tagged {
    type type;
    value value;
};

struct ternary : x3::position_tagged {
    value condition;
    value true_value;
    value false_value;
};

struct assignment : x3::position_tagged {
    value left_value;
    value value;
    ast::Operator op = ast::Operator::ASSIGN; //If not specified, it is not a compound operator
};

} //end of x3_ast namespace

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

//***************

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
    x3_ast::new_array, 
    (x3_ast::type, type)
    (x3_ast::value, size)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::new_, 
    (x3_ast::type, type)
    (std::vector<x3_ast::value>, values)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::builtin_operator, 
    (ast::BuiltinType, type)
    (std::vector<x3_ast::value>, values)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::function_call, 
    (std::string, function_name)
    (std::vector<x3_ast::type>, template_types)
    (std::vector<x3_ast::value>, values)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::expression, 
    (x3_ast::value, first)
    (std::vector<x3_ast::operation>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::operation, 
    (ast::Operator, op)
    (x3_ast::operation_value, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::identifier_pack, 
    (std::string, identifier)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::call_operation_value, 
    (std::string, function_name)
    (std::vector<x3_ast::type>, template_types)
    (std::vector<x3_ast::value>, values)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::prefix_operation, 
    (ast::Operator, op)
    (x3_ast::value, left_value)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::cast, 
    (x3_ast::type, type)
    (x3_ast::value, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::ternary, 
    (x3_ast::value, condition)
    (x3_ast::value, true_value)
    (x3_ast::value, false_value)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::assignment, 
    (x3_ast::value, left_value)
    (ast::Operator, op)
    (x3_ast::value, value)
)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-shift-op-parentheses"
#pragma clang diagnostic ignored "-Wunused-parameter"

/* Error handling */

namespace x3_grammar {
    template <typename Iterator>
    using error_handler = x3::error_handler<Iterator>;

    // tag used to get our error handler from the context
    struct error_handler_tag;

    struct error_handler_base {
        template <typename Iterator, typename Exception, typename Context>
        x3::error_handler_result on_error(Iterator& first, Iterator const& last, Exception const& x, Context const& context){
            std::string message = "Error! Expecting: " + x.which() + " here:";
            auto& error_handler = x3::get<error_handler_tag>(context).get();
            error_handler(x.where(), message);
            return x3::error_handler_result::fail;
        }
    };

    struct annotation_base {
        template <typename T, typename Iterator, typename Context>
        inline void on_success(Iterator const& first, Iterator const& last, T& ast, Context const& context){
            auto& error_handler = x3::get<error_handler_tag>(context).get();
            error_handler.tag(ast, first, last);
        }
    }; 

    typedef std::string::iterator base_iterator_type;
    typedef boost::spirit::line_pos_iterator<base_iterator_type> iterator_type;
    typedef x3::phrase_parse_context<x3::ascii::space_type>::type phrase_context_type;
    typedef error_handler<iterator_type> error_handler_type;

    typedef x3::with_context<
        error_handler_tag
        , std::reference_wrapper<error_handler_type> const
        , phrase_context_type>::type
        context_type;

    x3::symbols<char, ast::Operator> multiplicative_op;
    x3::symbols<char, ast::Operator> additive_op;
    x3::symbols<char, ast::Operator> relational_op;
    x3::symbols<char, ast::Operator> logical_and_op;
    x3::symbols<char, ast::Operator> logical_or_op;
    x3::symbols<char, ast::Operator> postfix_op;
    x3::symbols<char, ast::Operator> prefix_op;
    x3::symbols<char, ast::Operator> assign_op;
    x3::symbols<char, ast::Operator> unary_op;
    x3::symbols<char, ast::BuiltinType> builtin_op;

    /* Match operators into symbols */

    void add_keywords(){
        unary_op.add
            ("+", ast::Operator::ADD)
            ("-", ast::Operator::SUB)
            ("!", ast::Operator::NOT)
            ("*", ast::Operator::STAR)
            ("&", ast::Operator::ADDRESS)
            ;

        additive_op.add
            ("+", ast::Operator::ADD)
            ("-", ast::Operator::SUB)
            ;

        multiplicative_op.add
            ("/", ast::Operator::DIV)
            ("*", ast::Operator::MUL)
            ("%", ast::Operator::MOD)
            ;

        relational_op.add
            (">=", ast::Operator::GREATER_EQUALS)
            (">", ast::Operator::GREATER)
            ("<=", ast::Operator::LESS_EQUALS)
            ("<", ast::Operator::LESS)
            ("!=", ast::Operator::NOT_EQUALS)
            ("==", ast::Operator::EQUALS)
            ;

        logical_and_op.add
            ("&&", ast::Operator::AND)
            ;

        logical_or_op.add
            ("||", ast::Operator::OR)
            ;

        postfix_op.add
            ("++", ast::Operator::INC)
            ("--", ast::Operator::DEC)
            ;

        prefix_op.add
            ("++", ast::Operator::INC)
            ("--", ast::Operator::DEC)
            ;

        builtin_op.add
            ("size", ast::BuiltinType::SIZE)
            ("length", ast::BuiltinType::LENGTH)
            ;

        assign_op.add
            ("=",  ast::Operator::ASSIGN)
            ("+=", ast::Operator::ADD)
            ("-=", ast::Operator::SUB)
            ("/=", ast::Operator::DIV)
            ("*=", ast::Operator::MUL)
            ("%=", ast::Operator::MOD)
            ;
    }

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
    struct value_class {};
    struct primary_value_class {};
    struct variable_value_class;
    struct new_array_class;
    struct new_class;
    struct false_class;
    struct true_class;
    struct null_class;
    struct builtin_operator_class;
    struct function_call_class;
    struct postfix_expression_class;
    struct prefix_expression_class;
    struct unary_expression_class;
    struct unary_operation_class;
    struct call_operation_value_class;
    struct identifier_pack_class;
    struct cast_class;
    struct cast_expression_class;
    struct conditional_expression_class;
    struct additive_expression_class;
    struct multiplicative_expression_class;
    struct relational_expression_class;
    struct logical_or_expression_class;
    struct logical_and_expression_class;
    struct ternary_class;
    struct assignment_class;
    struct assignment_expression_class;

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
    x3::rule<new_array_class, x3_ast::new_array> const new_array("new_array");
    x3::rule<new_class, x3_ast::new_> const new_("new_");
    x3::rule<false_class, x3_ast::false_> const false_("false");
    x3::rule<true_class, x3_ast::true_> const true_("true");
    x3::rule<null_class, x3_ast::null> const null("null");
    x3::rule<variable_value_class, x3_ast::variable_value> const variable_value("variable_value");
    x3::rule<builtin_operator_class, x3_ast::builtin_operator> const builtin_operator("builtin_operator");
    x3::rule<function_call_class, x3_ast::function_call> const function_call("function_call");
    x3::rule<postfix_expression_class, x3_ast::expression> const postfix_expression("postfix_expression");
    x3::rule<prefix_expression_class, x3_ast::prefix_operation> const prefix_expression("prefix_expression");
    x3::rule<unary_expression_class, x3_ast::value> const unary_expression("unary_expression");
    x3::rule<unary_operation_class, x3_ast::prefix_operation> const unary_operation("unary_operation");
    x3::rule<additive_expression_class, x3_ast::expression> const additive_expression("additive_expression");
    x3::rule<multiplicative_expression_class, x3_ast::expression> const multiplicative_expression("multiplicative_expression");
    x3::rule<relational_expression_class, x3_ast::expression> const relational_expression("relational_expression");
    x3::rule<logical_or_expression_class, x3_ast::expression> const logical_or_expression("logical_or_expression");
    x3::rule<logical_and_expression_class, x3_ast::expression> const logical_and_expression("logical_and_expression");
    x3::rule<call_operation_value_class, x3_ast::call_operation_value> const call_operation_value("call_operation_value");
    x3::rule<identifier_pack_class, x3_ast::identifier_pack> const identifier_pack("identifier_pack");
    x3::rule<cast_class, x3_ast::cast> const cast("cast");
    x3::rule<assignment_class, x3_ast::assignment> const assignment("assignment");
    x3::rule<assignment_expression_class, x3_ast::value> const assignment_expression("assignment_expression");
    x3::rule<ternary_class, x3_ast::ternary> const ternary("ternary");
    x3::rule<cast_expression_class, x3_ast::value> const cast_expression("cast_expression");
    x3::rule<conditional_expression_class, x3_ast::value> const conditional_expression("conditional_expression");
    x3::rule<value_class, x3_ast::value> const value("value");
    x3::rule<primary_value_class, x3_ast::value> const primary_value("primary_value");
    
    struct source_file_class : error_handler_base {};
    struct variable_value_class : annotation_base {};

    struct new_array_class : annotation_base {};
    struct new_class : annotation_base {};
    struct false_class {};
    struct true_class {};
    struct null_class {};
    struct builtin_operator_class : annotation_base {};
    struct function_call_class : annotation_base {};
    struct postfix_expression_class : annotation_base {};
    struct prefix_expression_class : annotation_base {};
    struct unary_expression_class : annotation_base {};
    struct unary_operation_class : annotation_base {};
    struct cast_class : annotation_base {};
    struct ternary_class : annotation_base {};
    struct cast_expression_class : annotation_base {};
    struct assignment_class : annotation_base {};
    struct assignment_expression_class {};
    struct call_operation_value_class {};
    struct identifier_pack_class {};
    struct additive_expression_class : annotation_base {};
    struct multiplicative_expression_class : annotation_base {};
    struct relational_expression_class : annotation_base {};
    struct logical_or_expression_class : annotation_base {};
    struct logical_and_expression_class : annotation_base {};
    struct conditional_expression_class {};

    /* Utilities */

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

    /* Types */ 

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

    //TODO Perhaps need to skip here

    /* Values */ 

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
    
    auto const new_array_def =
            x3::lit("new")
        >>  type
        >>  '['
        >>  value
        >>  ']';
    
    auto const new_def =
            x3::lit("new")
        >>  type
        >>  '('
        >>  -(value % ',')
        >>  ')';

    auto const true_def = x3::eps >> x3::lit("true");
    auto const false_def = x3::eps >> x3::lit("false");
    auto const null_def = x3::eps >> x3::lit("null");

    auto const builtin_operator_def =
            builtin_op
        >>  '('
        >>  value % ','
        >>  ')';
    
    auto const function_call_def =
            identifier
        >>  -(
                    '<'
                >>  type % ','
                >>  '>'
            )
        >>  '('
        >>  -(value % ',')
        >>  ')';

    auto const primary_value_def =
            new_array
        |   new_
        |   true_
        |   false_
        |   null
        |   builtin_operator
        |   function_call
        |   variable_value
        |   integer_suffix_literal
        |   float_literal
        |   integer_literal
        |   string_literal
        |   char_literal
        ;
        //|   '(' >> value >> ')';
    
    auto const call_operation_value_def =
            identifier
        >>  -(
                    '<'
                >>  type % ','
                >>  '>'
            )
        >>  '('
        >>  -(value % ',')
        >>  ')';

    auto const identifier_pack_def = identifier;

    auto const postfix_expression_def =
            primary_value
        >>  +(
                         '['
                     >>  x3::attr(ast::Operator::BRACKET)
                     >>  value
                     >>  ']'
                |
                         '.'
                     >>  x3::attr(ast::Operator::CALL)
                     >>  call_operation_value
                |
                         '.'
                     >>  x3::attr(ast::Operator::DOT)
                     >>  identifier_pack
                |
                         postfix_op
                     >>  x3::attr(x3_ast::identifier_pack())
            );
    
    auto const prefix_expression_def =
            prefix_op
        >>  unary_expression;

    auto const unary_operation_def =
            unary_op
        >>  cast_expression;
    
    auto const unary_expression_def =
            postfix_expression
        |   prefix_expression
        |   unary_operation
        |   primary_value;

    auto const cast_def =
            '('
        >>  type
        >>  ')'
        >>  cast_expression;
    
    auto const cast_expression_def =
            cast
        |   unary_expression;

    auto const multiplicative_expression_def =
            cast_expression
        >>  *(multiplicative_op > cast_expression);

    auto const additive_expression_def =
            multiplicative_expression
        >>  *(additive_op > multiplicative_expression);

    auto const value_def = additive_expression;

    BOOST_SPIRIT_DEFINE(
        value = value_def,
        primary_value = primary_value_def,
        integer_literal = integer_literal_def,
        integer_suffix_literal = integer_suffix_literal_def,
        float_literal = float_literal_def,
        char_literal = char_literal_def,
        string_literal = string_literal_def,
        variable_value = variable_value_def,
        new_array = new_array_def,
        new_ = new_def,
        true_ = true_def,
        false_ = false_def,
        null = null_def,
        builtin_operator = builtin_operator_def,
        function_call = function_call_def,
        call_operation_value = call_operation_value_def,
        postfix_expression = postfix_expression_def,
        prefix_expression = prefix_expression_def,
        unary_operation = unary_operation_def,
        unary_expression = unary_expression_def,
        cast = cast_def,
        additive_expression = additive_expression_def,
        multiplicative_expression = multiplicative_expression_def,
        cast_expression = cast_expression_def,
        identifier_pack = identifier_pack_def
    );

    const auto parser = value;

} // end of grammar namespace

#pragma clang diagnostic pop

int main(){
    std::string file ="test";

    std::ifstream in(file.c_str(), std::ios::binary);
    in.unsetf(std::ios::skipws);

    //Collect the size of the file
    in.seekg(0, std::istream::end);
    std::size_t size(static_cast<size_t>(in.tellg()));
    in.seekg(0, std::istream::beg);

    x3_grammar::add_keywords();

    std::string file_contents;
    file_contents.resize(size);
    in.read(&file_contents[0], size);

    x3_ast::value result;

    x3_grammar::iterator_type it(file_contents.begin());
    x3_grammar::iterator_type end(file_contents.end());

    x3_grammar::error_handler_type error_handler(it, end, std::cerr, file);

    auto const parser = x3::with<x3_grammar::error_handler_tag>(std::ref(error_handler))[x3_grammar::parser];
    auto& skipper = x3_grammar::skipper;

    bool r = x3::phrase_parse(it, end, parser, skipper, result);

    if(r && it == end){
        return 1;
    } else {
        return 0;
    }
}
