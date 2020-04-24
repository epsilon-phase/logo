namespace logo {
  /**
   * \page Lexer State machine
   * \dot
   * digraph G{
   *   start -> {id_1,op_1};
   *   subgraph accepting_states{
   *      node[penwidth=5];
   *      plus_1;minus;increment;decrement;lor_2;land_2;
   *      id_1;
   *      id_2; exponent; times;divide;
   *   }
   *   subgraph cluster_0{
   *     id_1 ->  id_2 [label="[a-zA-Z_$]"];
   *     id_2 ->  id_2 [label="[a-zA-Z_0-9$]"];
   *     label = "identifier";
   *     color=lightgrey
   *   }
   *   subgraph cluster_1{
   *     label = "operator";
   *     color = blue;
   *     op_1 -> plus_1 [label="+"];
   *     op_1 -> minus [label="-"];
   *     minus -> decrement[label="-"];
   *     plus_1 -> increment [label="+"];
   *     op_1 -> times [label="*"];
   *     op_1 -> divide [label="/"];
   *     op_1 -> land_1 [label="&"];
   *     op_1 -> exponent [label="^"];
   *     land_1 -> land_2 [label="&"];
   *     lor_1 -> lor_2 [label="|"];
   *     op_1 -> lor_1[label="|"];
   *     {times,divide,increment,decrement,plus_1,minus,exponent,land_2,lor_2}
   * -> {ws,id_1};
   *   }
   *   subgraph cluster_2{
   *     label= "string";
   *     color= green;
   *     dq_start -> dq_end [label="\""];
   *     dq_start -> dq_midstr [label="[^\"]"];
   *     dq_midstr -> dq_escape [label="\\\""];
   *     dq_midstr -> dq_midstr [label="[^\"]"];
   *     dq_midstr->dq_end [label="\""];
   *     dq_escape->dq_end[label="\""];
   *     dq_escape->dq_midstr[label="[^\"]"];
   *     dq_end->ws;
   *   }
   *   {id_1,id_2, op_1,plus_1,increment} -> ws [label="[\\s\\t]"];
   *   ws -> ws [label="[\\s\\t]"];
   * }
   * \enddot
   * */
}