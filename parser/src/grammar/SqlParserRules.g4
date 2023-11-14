parser grammar SqlParserRules;

// top level statement
dmlStatement
    : selectStatement
    | insertStatement
    | updateStatement
    | deleteStatement
    ;

ddlStatement
    : createTableStatement
    | createIndexStatement
    | dropTableStatement
    | dropIndexStatement
    ;

// -- INSERT
insertStatement
    : insertOperation K_INTO insertionTarget insertColumnsAndSources
    ;

insertOperation
    : K_INSERT
    | K_INSERT K_OR K_REPLACE
    | K_INSERT K_IF K_NOT K_EXISTS
    | K_UPDATE K_OR K_INSERT
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

// -- UPDATE
updateStatement
    : K_UPDATE targetTable K_SET setClauseList ( K_WHERE searchCondition )?
    ;

targetTable
    : name
    ;

setClauseList
    : setClause (',' setClause)*
    ;

setClause
    : updateTarget '=' updateSource
    ;

updateTarget
    : simpleName
    ;

updateSource
    : expression
    ;

// -- DELETE
deleteStatement
    : K_DELETE K_FROM targetTable ( K_WHERE searchCondition )?
    ;

// -- SELECT
selectStatement
    : querySpecification
    ;

querySpecification
    : K_SELECT setQuantifier? selectList tableExpression orderByClause?
    ;

setQuantifier
    : K_ALL
    | K_DISTINCT
    ;

selectList
    : '*'
    | selectSublist (',' selectSublist)*
    ;

selectSublist
    : derivedColumn
    ;

derivedColumn
    : expression (K_AS? columnName)?
    ;

tableExpression
    : fromClause whereClause? groupByClause? havingClause?
    ;

fromClause
    : K_FROM tableReference (',' tableReference)*
    ;

tableReference
    : tablePrimary
    | tableReference joinedTableRest
    ;

tablePrimary
    : name correlationSpec?
    | '(' querySpecification ')' correlationSpec
    | '(' joinedTable ')'
    ;

correlationSpec
    : K_AS? simpleName ( '(' columnName ( ',' columnName )* ')' )?
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
    : booleanPrimary K_IS (K_TRUE|K_FALSE|K_NULL)
    | booleanPrimary K_IS K_NOT (K_TRUE|K_FALSE|K_NULL)
    | booleanPrimary
    ;

booleanPrimary
    : parenthesizedBooleanValueExpression
    | expression
    ;

parenthesizedBooleanValueExpression
    : '(' booleanValueExpression ')'
    ;

orderByClause
    : K_ORDER K_BY sortSpecificationList
    ;

sortSpecificationList
    : sortSpecification (',' sortSpecification)*
    ;

sortSpecification
    : expression orderingSpecification?
    ;

orderingSpecification
    : K_ASC
    | K_DESC
    ;

groupByClause
    : K_GROUP K_BY groupingElementList
    ;

groupingElementList
    : groupingElement (',' groupingElement)*
    ;

groupingElement
    : name
    ;

havingClause
    : K_HAVING searchCondition
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

// create index
createIndexStatement
    : indexDefinition
    ;

indexDefinition
    : K_CREATE K_INDEX ( indexDefinitionOption )* indexName=name?
            K_ON tableName=name indexElementList
    ;

indexDefinitionOption
    : K_IF K_NOT K_EXISTS
    ;

indexElementList
    : '(' indexElement ( ',' indexElement )* ')'
    ;

indexElement
    : columnName orderingSpecification?
    ;

// drop table
dropTableStatement
    : K_DROP K_TABLE ( dropTableOption )? name
    ;

dropTableOption
    : K_IF K_EXISTS
    ;

// drop index
dropIndexStatement
    : K_DROP K_INDEX ( dropIndexOption )? name
    ;

dropIndexOption
    : K_IF K_EXISTS
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
    : andExpression op='&' comparisonExpression
    | comparisonExpression
    ;

comparisonExpression
    : shiftExpression op='=' shiftExpression
    | shiftExpression op='<>' shiftExpression
    | shiftExpression op='<' shiftExpression
    | shiftExpression op='>' shiftExpression
    | shiftExpression op='<=' shiftExpression
    | shiftExpression op='>=' shiftExpression
    | shiftExpression op=K_LIKE shiftExpression
    | shiftExpression K_NOT op=K_LIKE shiftExpression
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
    | additiveExpression op='||' multiplicativeExpression
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
    | placeholder
    | '(' expression ')'
    ;

functionCall
    : name '(' expressionList? ')'
    | name '(' '*' ')'
    | name '(' K_ALL expression ')'
    | name '(' K_DISTINCT expression ')'
    | K_CAST '(' expression K_AS dataType ')'
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

placeholder
    : NAMED_PLACEHOLDER
    | QUESTION
    ;

dataType
    : K_INT ( '(' dataSize ')' )?
    | K_BIGINT
    | K_FLOAT ( '(' dataSize ')' )?
    | K_REAL
    | K_DOUBLE ( K_PRECISION )?
    | K_DECIMAL ( '(' precision=flexibleDataSize ( ',' scale=flexibleDataSize )? ')' )?
    | K_CHAR ( '(' dataSize ')' )?
    | K_VARCHAR '(' size=flexibleDataSize ')'
    | K_STRING
    | K_BINARY ( '(' dataSize ')' )?
    | K_VARBINARY '(' size=flexibleDataSize ')'
    | K_DATE
    | K_TIME ( withTimeZone )?
    | K_TIMESTAMP ( withTimeZone )?
    ;

flexibleDataSize
    : INTEGRAL_NUMBER
    | ASTERISK
    ;

dataSize
    : INTEGRAL_NUMBER
    ;

withTimeZone
    : K_WITH K_TIME K_ZONE
    ;