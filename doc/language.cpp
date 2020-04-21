namespace logo {

  /// \page Language
  /// \title The language as implemented
  /// # The Language Itself
  /// Basic structure
  /// ```
  /// Function SayHello()
  ///   print("Hello :)");
  /// EndFunc
  /// Function Add2To(x)
  ///   return x+2;
  /// EndFunc
  /// Function Main()
  ///   Variable x,y,z;
  ///   z=readNumber();
  ///   y=readNumber();
  ///   x=z/2+y;
  ///   print(x);
  /// EndFunc
  /// Function Looping()
  ///   Variable x,y;
  ///   y = 5;
  ///   x=y;
  ///   While x>0 Do
  ///     print(x);
  ///     x = x - 1;
  ///   EndWhile
  ///   For x=0; x<=y;x=x-1 Do
  ///      print(x);
  ///   EndFor
  ///   x = y
  ///   Do While x>0;
  ///     If x>5 Then
  ///       print("What a large X");
  ///     EndIf
  ///     If x<=0 then
  ///       print("What a small X");
  ///       break;
  ///     EndIf
  ///     x = x - 1
  ///   EndDo
  /// EndFunc
  ///
  /// ```
  /// Syntax:
  /// ```
  ///   <File> = (<FunctionDef> | <Include>) <File>
  ///          | EOF;
  ///   <Whitespace> = <comment> | [\t \n\r];
  ///   <comment> = ("#"| "//") .+ "\n"
  ///             | "/*" .+ "*/";
  ///   <FunctionDef> = "Function" <Identifier> <Parameters>
  ///                   <Block> "EndFunc"
  ///   <Identifier> = [A-Za-z][0-9_a-zA-Z]+;
  ///   <Parameters> = "(" <ParamId> ")";
  ///   <ParamId> = <Identifier> ("," <Identifier>)* | ;
  ///   <Block> = <Statement> Newline | ;
  ///   <Statement> = (<Declaration> | <Return> | <Assignment> | <Call>) ";"
  ///               | <ControlFlow> ;
  ///   <ControlFlow> = <DoLoop>
  ///                 | <WhileLoop>
  ///                 | <ForLoop>
  ///                 | <IfElse>
  ///                 | <LoopBreak>;
  ///   <DoLoop>    = "Do While" <Expression> ";" <Block> "EndDo";
  ///   <WhileLoop> = "While" <Expression> "do" <Block> "EndWhile";
  ///   <ForLoop>   = "For" <ForHeader> "Do"
  ///                   <Block>
  ///                  "EndFor";
  ///   <ForHeader> = <Assignment> ";" <Expression> ";" <Assignment>
  ///   <IfElse>    = "If" <Expression> "then"
  ///                      <Block>
  ///                 ("Else" <Block>)?
  ///                 "EndIf";
  ///   <Declaration> = "Variable" <VariableDec>;
  ///   <VariableDec> = <Identifier> ("," <Identifier)*;
  ///   <Assignment> = <Identifier> ("=" <Expression>| "++" | "--");
  ///   <Expression> = <Comparison>;
  ///   <BooleanExpr> = <Comparison> (("||"|"&&") <Comparison>)*
  ///   <Comparison> = <AddOp> (([<>!=]"="?) <Addop>)?
  ///   <AddOp> = <MultOp> ([+-] <MultOp>)*;
  ///   <MultOp> = <ExponentOp> (("*"|"/") <ExponentOp>)*;
  ///   <ExponentOp> = <Atom> ("^" <Atom>)*;
  ///   <Atom> = <Identifier> | "(" <Expression> ")" | <Call> | <Number>;
  ///   <Call> = <Identifier> "(" (<Expression> ("," <Expression>) ")";
  ///   <Return> = "return" <Expression>;
  /// ```

} // namespace logo