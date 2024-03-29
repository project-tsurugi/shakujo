lexer grammar SqlLexerRules;
import CaseInsensitiveLexerRules;

K_SELECT: S E L E C T;
K_FROM: F R O M;

K_WHERE: W H E R E;
K_HAVING: H A V I N G;

K_GROUP: G R O U P;
K_ORDER: O R D E R;
K_BY: B Y;

K_INSERT: I N S E R T;
K_VALUES: V A L U E S;

K_DELETE: D E L E T E;

K_UPDATE: U P D A T E;
K_SET: S E T;

K_CREATE: C R E A T E;
K_DROP: D R O P;
K_TABLE: T A B L E;
K_INDEX: I N D E X;
K_UNIQUE: U N I Q U E;

K_DEFAULT: D E F A U L T;

K_PRIMARY: P R I M A R Y;
K_KEY: K E Y;

K_AND: A N D;
K_OR: O R;
K_NOT: N O T;
K_LIKE: L I K E;
K_IS: I S;
K_AS: A S;
K_INTO: I N T O;

K_INNER: I N N E R;
K_OUTER: O U T E R;
K_LEFT: L E F T;
K_RIGHT: R I G H T;
K_FULL: F U L L;
K_CROSS: C R O S S;
K_NATURAL: N A T U R A L;
K_UNION: U N I O N;
K_JOIN: J O I N;
K_ON: O N;

K_NULL: N U L L;
K_TRUE: T R U E;
K_FALSE: F A L S E;

K_ALL: A L L;
K_DISTINCT: D I S T I N C T;

K_ASC: A S C;
K_DESC: D E S C;

K_CHAR: C H A R;
K_VARCHAR: V A R C H A R;
K_STRING: S T R I N G;
K_BIGINT: B I G I N T;
K_INT: I N T;
K_FLOAT: F L O A T;
K_REAL: R E A L;
K_DOUBLE: D O U B L E;
K_PRECISION: P R E C I S I O N;
K_DECIMAL: D E C I M A L;
K_BINARY: B I N A R Y;
K_VARBINARY: V A R B I N A R Y;
K_DATE: D A T E;
K_TIME: T I M E;
K_TIMESTAMP: T I M E S T A M P;
K_WITH: W I T H;
K_ZONE: Z O N E;

K_IF: I F;
K_EXISTS: E X I S T S;

K_REPLACE: R E P L A C E;

K_CAST: C A S T;

PLUS: '+';
MINUS: '-';
ASTERISK: '*';
SLASH: '/';
PERCENT: '%';
DOT: '.';
COMMA: ',';
COLON: ':';
SEMI_COLON: ';';

VERTICAL_BAR: '|';
AMPERSAND: '&';
CARET: '^';
EXCLAMATION_MARK: '!';
TILDE: '~';

EQUAL: '=';

QUESTION: '?';

LEFT_PAREN: '(';
RIGHT_PAREN: ')';

LEFT_ANGLE: '<';
RIGHT_ANGLE: '>';


// FIXME: connect in parser rules
VERTICAL_BAR_2: '||';
AMPERSAND_2: '&&';
LEFT_ANGLE_2: '<<';
RIGHT_ANGLE_2: '>>';
RIGHT_ANGLE_3: '>>>';

EXCLAMATION_MARK_EQUAL: '!=';
COLON_EQUAL: ':=';
LEFT_ANGLE_RIGHT_ANGLE: '<>';
LEFT_ANGLE_EQUAL: '<=';
RIGHT_ANGLE_EQUAL: '>=';

// FIXME: basic lexer rules
IDENTIFIER
    : [a-zA-Z_][a-zA-Z0-9_]*
    ;

NAMED_PLACEHOLDER
    : ':' [a-zA-Z_][a-zA-Z0-9_]*
    ;

INTEGRAL_NUMBER
    : '0'
    | '1'..'9' DIGIT*
    ;

FLOATING_POINT_NUMBER
    : '.' DIGIT+
    | '0' '.' DIGIT*
    | '1'..'9' DIGIT* '.' DIGIT*
    ;

STRING
    : '\'' STRING_CHARACTER* '\''
    ;

fragment ID_LETTER
    : [a-zA-Z_]
    ;

fragment DIGIT
    : [0-9]
    ;

fragment STRING_CHARACTER
    : ~[']
    | '\\' .
    ;
