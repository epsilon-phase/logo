#include "./lexer.hpp"
using namespace logo;
using namespace language;
%%{
  
  machine lexer;
  action start {
    start=p;
  }
  action number{
    tu.tokens.emplace_back(tokens::Token{tokens::Number,
                                         std::string_view(start,p-start+1)});
  }
  ws = [ \t]+;
  comment = ('#'|'//')[^\n]+ '\n';
  equals = '=';
  number = (
    [\-+]?[0-9]+ ( '.' [0-9]+)?
    ([eE]([+\-])?[0-9]+)?
  ) %number;
  string = '"' [^\"]+ '"';
  keyword = [fF]'unction'
           | [Ee]'nd'([Ff]('or'|'unc'|'while')|[iI]'f')
           | [vV]'ariable'
           | [Dd]'o'
           | [wW]'hile'
           | [fF]'or'
           | [iI]'f'
           | [Ee]'lse';
  lparam = '(';
  rparam = ')';
  semi = ';';
  comma = ',';
  operator = ('++'|'--'|'+'|'-'|'/'|'*'|'^'|'||'|'&&');
  identifier = [a-zA-Z_\$][a-zA-Z_0-9\$]+;
  main := ((identifier | number | keyword
        | operator | lparam | rparam| semi | comma) >start
        ws+)*;
}%%
%% write data;
void logo::language::lex(TranslationUnit& tu){
  int cs,res=0;
  const char *start=nullptr;
  const char *p=tu.contents.c_str();
  const char *pe=tu.contents.c_str()+tu.contents.size()+1;
  %%write init;
  %%write exec;
}