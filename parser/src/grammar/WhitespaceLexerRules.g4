lexer grammar WhitespaceLexerRules;

channels {
    COMMENT
}

WHITE_SPACE
    : [ \t\r\n] -> skip
    ;

SINGLE_LINE_COMMENT
    : '--' ~[\r\n]* -> channel(COMMENT)
    ;

MULTI_LINE_COMMENT
    : '/*' .*? '*/' -> channel(COMMENT)
    ;
