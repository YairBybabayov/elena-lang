[[
   #grammar cf

   #define start          ::= <= root ( public_namespace ( => member+ $eof <= )) =>;
   #define start          ::= $eof;

   #define member         ::= import;
   #define member         ::= function;

   #define import         ::= 
<=
    import (
=>
                             "import" identifier ";"
<=
    )
=>;

   #define function       ::= 
<=
   singleton (
=>
                              public_prefix? name f_decl
<=
   )
=>;

   #define f_decl         ::=
<=
     function (
=>
                              f_args body 
<=
     )
=>;

   #define f_args         ::= "(" ")";

   #define body           ::= 
<=
     code (
=>
                              "{" statement next_statement
<=
     )
=>;

   #define next_statement ::= ";" statement next_statement;
   #define next_statement ::= ";" "}";
   #define next_statement ::= "}";

   #define statement      ::= expression;
   #define statement      ::= "var" decl_variable;
   #define statement      ::= a_expression ;

   #define decl_variable  ::= 
<=
        expression 
        (
=>
                              a_new_variable ":=" expression
<=
        )
=>;

   #define a_new_variable ::= new_variable
<=
           assign = 0
=>;

   #define a_expression     ::= 
<=
        expression (
=>
                              object ":=" assigning
<=
        )
=>;

   #define assigning         ::=
<=
           assign = 0
=>
                              expression;

   #define s_expression     ::= 
<=
        expression (
=>
                              s_object
<=
        )
=>;

   #define expression     ::= l4_expression;

   #define l4_expression  ::= $ l3_expression l4_operations;

   #define l3_expression  ::= $ l2_expression l3_operations;

   #define l2_expression  ::= $ l1_expression l2_operations;

   #define l1_expression  ::= <= expression ( => object l1_operations <= ) =>;

   #define l4_operations  ::= ^ <= expression ( => l4_operation next_l4_op <= ) =>;
   #define l4_operations  ::= $eps;

   #define l4_operation   ::= <= operator = "+" => "+" l3_expression;
   #define l4_operation   ::= <= operator = "-" => "-" l3_expression;

   #define next_l4_op     ::= <= ; => l4_operation next_l4_op;
   #define next_l4_op     ::= $eps;

   #define l3_operations  ::= ^ <= expression ( => l3_operation next_l3_op <= ) =>;
   #define l3_operations  ::= $eps;

   #define l3_operation   ::= <= operator = "*" => "*" l2_expression;
   #define l3_operation   ::= <= operator = "/" => "/" l2_expression;

   #define next_l3_op     ::= <= ; => l3_operation next_l3_op;
   #define next_l3_op     ::= $eps;

   #define l2_operations  ::= ^ <= expression ( => l2_operation next_l2_op <= ) =>;
   #define l2_operations  ::= $eps;

   #define l2_operation   ::= "." message m_args;
   #define l2_operation   ::= "." property;

   #define next_l2_op     ::= <= ; => l2_operation next_l2_op;
   #define next_l2_op     ::= $eps;

   #define l1_operations  ::= l1_operation next_l1_op;
   #define l1_operations  ::= $eps;

   #define l1_operation   ::= <= message = 0 => function_args;

   #define next_l1_op     ::= <= ; => l1_operation next_l1_op;
   #define next_l1_op     ::= $eps;

   #define object         ::= s_object;
   #define object         ::= "new" new_identifier;

   #define s_object       ::= integer;
   #define s_object       ::= literal;
   #define s_object       ::= reference;
   #define s_object       ::= identifier;
   #define s_object       ::= character;
   #define s_object       ::= "(" expression ")";

   #define function_args  ::= "(" ")";
   #define function_args  ::= "(" expression next_f_arg;
   #define next_f_arg     ::= "," expression next_f_arg;
   #define next_f_arg     ::= ")";

   #define m_args         ::= "(" ")";
   #define m_args         ::= "(" expression next_m_arg;
   #define m_args         ::= ":" s_expression;

   #define next_m_arg     ::= "," expression next_m_arg;
   #define next_m_arg     ::= ")";

   #define name           ::= <= nameattr ( identifier = $identifier ) =>; 

   #define new_variable   ::= <= variable_identifier = $identifier =>; 
   #define new_identifier ::= <= new_identifier = $identifier =>;

   #define property       ::= <= message = $identifier property_parameter = 0 =>;
   #define message        ::= <= message = $identifier =>;
   #define identifier     ::= <= identifier = $identifier =>;
   #define integer        ::= <= integer = $numeric =>;
   #define literal        ::= <= literal = "$literal" =>;
   #define reference      ::= <= reference = $reference =>;
   #define character      ::= <= character = $character =>;

   #define public_prefix  ::= "public";
]]