parser grammar SqlParserRules;

// top level statement
dmlStatement
    : selectStatement
    | insertStatement
    ;

ddlStatement
    : createTableStatement
    ;

// -- INSERT
insertStatement
    : K_INSERT K_INTO insertionTarget insertColumnsAndSources
    ;

insertionTarget
    : name
    ;

insertColumnsAndSources
    : ( '(' insertColumnList ')' )? insertValuesConstructor
    ;

insertColumnList
    : simpleName (',' simpleName)*
    ;

insertValuesConstructor
    : K_VALUES '(' insertValuesExpressionList ')'
    ;

insertValuesExpressionList
    : insertValuesExpression (',' insertValuesExpression)*
    ;

insertValuesExpression
    : expression
    // FIXME DEFAULT
    ;

// -- SELECT
selectStatement
    : query
    ;

query
    : K_SELECT projectionSpec fromClause whereClause?
    ;

projectionSpec
    : any='*'
    | projectionColumnList
    ;

projectionColumnList
    : projectionColumn (',' projectionColumn)*
    ;

projectionColumn
    : expression (K_AS simpleName)?
    ;

fromClause
    : K_FROM tableReference (',' tableReference)*
    ;

tableReference
    : tablePrimary
    | tableReference joinedTableRest
    ;

tablePrimary
    : tableName
    | '(' joinedTable ')'
    ;

tableName
    : name (K_AS simpleName)? // FIXME: correlation name wit derived columns
    ;

joinedTable
    : tableReference joinedTableRest
    ;

joinedTableRest
    : joinType? K_JOIN tablePrimary joinSpecification?
    | K_CROSS K_JOIN tablePrimary
    | K_NATURAL joinType? K_JOIN tablePrimary
    | K_UNION K_JOIN  tablePrimary
    ;

joinType
    : K_INNER
    | K_LEFT (K_OUTER)?
    | K_RIGHT (K_OUTER)?
    | K_FULL (K_OUTER)?
    ;

joinSpecification
    : K_ON searchCondition
    ;

whereClause
    : K_WHERE searchCondition
    ;

// search condition
searchCondition
    : booleanValueExpression
    ;

booleanValueExpression
    : booleanValueExpression op=K_OR booleanTerm
    | booleanTerm
    ;

booleanTerm
    : booleanTerm op=K_AND booleanFactor
    | booleanFactor
    ;

booleanFactor
    : op=K_NOT booleanTest
    | booleanTest
    ;

booleanTest
    : booleanPrimary K_IS (K_TRUE|K_FALSE)
    | booleanPrimary K_IS K_NOT (K_TRUE|K_FALSE)
    | booleanPrimary
    ;

booleanPrimary
    : parenthesizedBooleanValueExpression
    | expression
    ;

parenthesizedBooleanValueExpression
    : '(' booleanValueExpression ')'
    ;

// create table
createTableStatement
    : tableDefinition
    ;

tableDefinition
    : K_CREATE K_TABLE ( tableDefinitionOption )* name tableElementList
    ;

tableDefinitionOption
    : K_IF K_NOT K_EXISTS
    ;

tableElementList
    : '(' tableElement ( ',' tableElement )* ')'
    ;

tableElement
    : columnDefinition
    | tableConstraintDefinition
    ;

columnDefinition
    : columnName dataType ( defaultClause )? ( columnConstraintDefinition )*
    ;

columnName
    : simpleName
    ;

defaultClause
    : K_DEFAULT literal
    ;

columnConstraintDefinition
    : K_NOT K_NULL
    | K_NULL
    | K_PRIMARY K_KEY
    ;

tableConstraintDefinition
    : primaryKeyDefinition
    ;

primaryKeyDefinition
    : K_PRIMARY K_KEY '(' columnOrderList ')'
    ;

columnOrderList
    : columnOrder ( ',' columnOrder )*
    ;

columnOrder
    : columnName K_ASC
    | columnName K_DESC
    | columnName
    ;

// general statement
statement
    : expressionStatement
    ;

expressionStatement
    : expression ';'
    ;

// expressions

expressionList
    : expression (',' expression)* ','?
    ;

expression
    : assignExpression
    ;

assignExpression
    : name op=':=' assignExpression {false}?
    | signConditionalOrExpression
    ;

signConditionalOrExpression
    : signConditionalOrExpression op='||' signConditionalAndExpression {false}?
    | signConditionalAndExpression
    ;

signConditionalAndExpression
    : signConditionalAndExpression op='&&' inclusiveOrExpression {false}?
    | inclusiveOrExpression
    ;

inclusiveOrExpression
    : inclusiveOrExpression op='|' exclusiveOrExpression
    | exclusiveOrExpression
    ;

exclusiveOrExpression
    : exclusiveOrExpression op='^' andExpression
    | andExpression
    ;

andExpression
    : andExpression op='&' equalityExpression
    | equalityExpression
    ;

equalityExpression
    : equalityExpression op='=' relationalExpression
    | equalityExpression op='<>' relationalExpression
    | relationalExpression
    ;

relationalExpression
    : relationalExpression op='<' shiftExpression
    | relationalExpression op='>' shiftExpression
    | relationalExpression op='<=' shiftExpression
    | relationalExpression op='>=' shiftExpression
    | shiftExpression
    ;

shiftExpression
    : shiftExpression op='<<' additiveExpression
    | shiftExpression op='>>' additiveExpression
    | shiftExpression op='>>>' additiveExpression
    | additiveExpression
    ;

additiveExpression
    : additiveExpression op='+' multiplicativeExpression
    | additiveExpression op='-' multiplicativeExpression
    | multiplicativeExpression
    ;

multiplicativeExpression
    : multiplicativeExpression op='*' unaryExpression
    | multiplicativeExpression op='/' unaryExpression
    | multiplicativeExpression op='%' unaryExpression
    | unaryExpression
    ;

unaryExpression
    : op='+' unaryExpression
    | op='-' unaryExpression
    | op='~' unaryExpression
    | op='!' unaryExpression {false}?
    | postfixExpression
    ;

postfixExpression
    : primaryExpression
    ;

primaryExpression
    : functionCall
    | name
    | literal
    ;

functionCall
    : name '(' expressionList? ')'
    ;

name
    : name '.' simpleName
    | simpleName
    ;

simpleName
    : IDENTIFIER
    ;

literal
    : INTEGRAL_NUMBER
    | FLOATING_POINT_NUMBER
    | K_TRUE
    | K_FALSE
    | K_NULL
    | STRING
    ;

dataType
    : K_INT ( dataSize )?
    | K_BIGINT
    | K_FLOAT ( dataSize )?
    | K_DOUBLE ( K_PRECISION )?
    | K_CHAR dataSize
    | K_STRING
    ;

dataSize
    : '(' INTEGRAL_NUMBER ')'
    ;
