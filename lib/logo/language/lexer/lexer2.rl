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
  action keyword{
    std::string_view sv(start,p-start+1);
    tu.tokens.emplace_back(tokens::Token{__detail::identify_keyword(sv),sv});
  }
  action operator{
    std::string_view sv(start,p-start+1);
    tu.tokens.emplace_back(tokens::Token{__detail::identify_operator(sv),sv});
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
           | [Ee]'lse' 
           | [rR]'eturn' %keyword;
  lparam = '(';
  rparam = ')';
  semi = ';';
  comma = ',';
  operator = ('++'|'--'|'+'|'-'|'/'|'*'|'^'|'||'|'&&') %operator;
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