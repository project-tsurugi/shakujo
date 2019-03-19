lexer grammar WhitespaceLexerRules;

WHITE_SPACE
    : [ \t\r\n] -> skip
    ;

SINGLE_LINE_COMMENT
    : '--' ~[\r\n]* -> skip
    ;

MULTI_LINE_COMMENT
    : '/*' .*? '*/' -> skip
    ;
