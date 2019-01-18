parser grammar ShakujoParser;
import SqlParserRules;
options {
    tokenVocab=ShakujoLexer;
}

programEntry
    : dmlStatement EOF
    | ddlStatement EOF
    ;

statementEntry
    : statement EOF
    ;

expressionEntry
    : expression EOF
    ;
