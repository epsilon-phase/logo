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
    std::string_view sv(start,p-start);
    tu.tokens.emplace_back(tokens::Token{__detail::identify_keyword(sv),sv});
  }
  action operator{
    std::string_view sv(start,p-start);
    tu.tokens.emplace_back(tokens::Token{__detail::identify_operator(sv),sv});
  }
  action identifier{
    std::string_view sv(start,p-start);
    auto tt = __detail::identify_keyword(sv);
    if(tt == tokens::Unknown)
      tu.tokens.emplace_back(tokens::Token{tokens::Identifier,sv});
  }
  action parenthesis{
    std::string_view sv(start,1);
    auto tt = tokens::ParenLeft;
    if(*start==')')
      tt = tokens::ParenRight;
    tu.tokens.emplace_back(tokens::Token{tt,sv});
  }
  action string{
    std::string_view sv(start+1,p-start-2);
    tu.tokens.emplace_back(tokens::Token{tokens::String,sv});
  }
  ws = [ \t]+;
  comment = ('#'|'//')[^\n]+ '\n';
  equals = '=';
  number = (
    [\-+]?[0-9]+ ( '.' [0-9]+)?
    ([eE]([+\-])?[0-9]+)?
  ) %number;
  #Notably, this has no ability to signal that it needs escaping here. Given that the representation
  #makes it easier to debug if it isn't converted inline, this behavior will remain...
  #Hopefully to be fixed at the compilation step
  string = '"' ([^\"]|'\\"')+ '"' %string;
#Keywords are meant to be case insensitive
  keyword = ([fF]'unction'
           | [Ee]'nd'([Ff]('or'|'unc'|'do')|[iI]'f'|[Ww]'hile')
           | [vV]'ariable'
           | [Dd]'o'
           | [wW]'hile'
           | [fF]'or'
           | [iI]'f'
           | [tT]'hen'
           | [Ee]'lse' 
           | [rR]'eturn') %keyword;
  lparen = '(' %parenthesis;
  rparen = ')' %parenthesis;
  semi = ';';
  comma = ',';
  operator = ('++'|'--'|'+'|'-'|'/'|'*'|'^'|'||'
           |'&&'|'>='|'=='|'<='|'!='|'>'|'<'|'=') %operator;
  identifier = ([a-zA-Z_\$][a-zA-Z_0-9\$]*) %identifier;
  main := (ws?(((number | keyword | identifier
         |operator | string) >start ws+) |
        (lparen | rparen| semi | comma) >start ws*))*;
}%%
%% write data;
void logo::language::lex(TranslationUnit& tu){
  int cs,res=0;
  const char *start=nullptr;
  const char *p=tu.contents.c_str();
  const char *pe=tu.contents.c_str()+tu.contents.size()+1;
  const char *eof = pe;
  %%write init;
  %%write exec;
}
