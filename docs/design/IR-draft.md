# OLTP IR design draft

## Concepts

* generic imperative programming language + relational algebra operations
  * mutable variables
  * control statements - sequences, selections, iterations
  * basic relational algebra operations
* almost structural typing
  * less type names
  * static structural duck typing for functions
    * TBD: remove this feature?
* static typing of variables and expressions
  * variable and expression types must be identified on compile time
  * minimal sub-typing for variables nor expressions
    * only nullable/non-null types
  * expression tree of relational algebra operations must be deterministic on compile time
    * temporary constraint (?)
* minimal first class values
  * types are NOT first class
  * symbols are NOT first class
  * functions are NOT first class
  * relational algebra operations are LIMITED first class
* explicit nullity
  * TBD: remove this feature?
    * This may be only required for code generation, but optimizations will become harder
  * explicit nullable types
  * explicit null check
  * operations with null may almost returns just null
  * expression null tolerance:
    * as value - operator treats nulls like normal values
    * acceptable - operator accepts null values in its operands, and returns null as a result
    * restricted - operator does not allow nullable expressions in its operands
* TBD:
  * `goto`

## Terminology

`T = U`
  ~ type/kind `T` is identical to `U`

`T != U`
  ~ type/kind `T` is different from `U`

`T <= K`
  ~ type `T` is a kind of `K`

`T </ K`
  ~ type `T` is NOT a kind of `K`

## Symbols

### The simple name

The *simple name* is a minimal *symbol* that consists of a single token.

#### Form

```yaml
kind: "SimpleName"
token: &s
```

* The token `s` must be a string of the host language (YAML/C++ API)

### The qualified name

The *qualified name* consists of a *simple name* with another *symbol* as qualifier.

#### Form

```yaml
kind: "QualifiedName"
qualifier: &n0
name: &n1
```

* The qualifier `n0` must be a *symbol*
* The name `n1` must be a *simple name*
* We will denote qualified names as `n0.n1`

## Types

TBD: WIP

### Type kinds

* `Any = Value | Void`
* `Value = Atom | Container | Null`
* `Atom = Number | Text`
* `Number = Integer | Real`
* `Container = Struct[U, ...] | Sequence[T]`
* ...

### The expression and variable types

`int.N`
~ signed integral number.
  `N` is guaranteed **bit width** of the value, one of `32`, and `64`.
~ `int.N <= Integer`

`float.N`
~ floating point number.
  `N` is guaranteed **bit width** of the value, one of `32`, and `64`.
~ `float.N <= Real`

`char.N`
~ fixed length character sequence.
  `N` is maximum **byte length** of the sequence (characters may have different byte length).
  `char.*` represents *character sequence with any length*.
~ `char.N <= Text`

`varchar.N`
~ variant length character sequence.
  `N` is maximum **byte length** of the sequence (characters may have different byte length).
  `varchar.*` represents *character sequence with any length*.
~ `varchar.N <= Text`

`string`
~ flexible length character sequence.
~ `string <= Text`

`bool`
~ boolean value.

`null`
~ just `null`.

`tuple[U, ...]`
~ named or unnamed tuple of values.
~ kind of `Struct[U, ...]`

`array.N[T]`
~ fixed length list of `T`'s values.
  `N` must be a compile time constant of integer.
  `array.*[...]` represents *array of T with any length*.
~ `T <= value`
~ `array[T] <= Sequence[T]`

`vector[T]`
~ variable length list of `T`'s values.
~ `vector[T] <= Sequence[T]`

`relation[T]`
~ bag of `T`, where `T = tuple[U, ...]`.
~ always immutable
~ `relation[T] <= Value`
  * Not a kind of sequence

`cursor[T]`
~ cursor of `relation[T]`, where `T = tuple[U, ...]`.
~ stateful but immutable for users.
~ `cursor[T] <= Sequence[T]`

`T?`
~ the `T`'s value may be `null`.
  We call that `T` is non-nullable part of `T` and `T?`.
~ `null?` is equivalent to `null`, and `T??` is equivalent to `T?`
~ If `T <= K`, then also `T? <= K`

* TBD
  * nullity is only for Atom?
  * raw byte buffers or `{array.N,vector}[int.8]`
  * `const` type for tuple elements

## Type conversions

### The tuple type composition

The *tuple type composition* creates a compound type of two tuples.

Where the first tuple type is `S = tuple[a1:U1, a2:U2, ...]` and the second tuple type is `T = tuple[b1:V1, b2:V2, ...]`, note that `a*` and `b*` are each name of corresponded element:

* The *tuple type composition* of `S` and `T` with qualifier `p` and `q` is `tuple[p.a1:U1, p.a2:U2, ..., q.b1:V1, q.b2:V2, ...]`
  * We will denote it as `composition(p:S, q:T)`
  * Each element name of the compound type will be qualified by the qualifier if it was specified
  * If the any qualifier is not specified, the original element name will be used in the compound type
  * If there are duplicated element names in the compound type, these element becomes the unnamed except the first one

Note:
* `composition(p:A, q:B, r:C)`
  * `= composition(composition(p:A, q:B), r:C)`
  * `= composition(p:A, composition(q:B, r:C))`
* `composition(p:A, q:B) != composition(q:B, p:A)`
  * positional indices are different

### The nullable element conversion

The *nullable element conversion* makes each tuple element type nullable.

Where a tuple type is `T = tuple[U, V, ...]`, and `U`, `V`, ... are nullable or non-nullable:

* The element nullable type of `T` is `tuple[U?, V?, ...]`
  * We will denote it as `nullable_element(T)`
  * Note that this conversion is idempotent, that is `nullable_element(nullable_element(T)) = nullable_element(T)`

### The binary promotion

The *binary promotion* converts two types into a common type.

Where the first type is `U`, and the second type is `V`, and `A` is non-nullable part of `U` and `B` is non-nullable part of `V`:

1. We first derives the non-nullable part of the promotion:
  * If `A` and `B` is the same type:
    * then `S` is just `A`
  * Otherwise, if `A` and `B` is both `Number`
    * then `S` is *binary numeric promotion* result of `A` and `B`
  * Otherwise, if `A` and `B` is both `Text`
    * then `S` is *binary character sequence promotion* result of `A` and `B`
  * Otherwise, the promotion was failed
2. Then the restore nullity of the promoted type:
  * If either `U` or `V`
    * `T` is just `S?`
  * Otherwise
    * `T` is `S`
3. Finally, `T` is *binary promotion* result of `U` and `V`, and we will denote it as `promote(U, V)`

#### The binary nullable promotion

The *nullable conversion* converts two types with different nullity into a common type.

Where `T` is non-nullable and `T?` is nullable type of `T`:

|      |  `T` | `T?` |`null`|
|-----:|:----:|:----:|:----:|
|  `T` |  `T` | `T?` | `T?` |
| `T?` | `T?` | `T?` | `T?` |
|`null`| `T?` | `T?` |`null`|

#### The binary numeric promotion

The *binary numeric promotion* converts two numeric values into a common type.

|   | `int.8` | `int.32` | `int.64` | `float.32` | `float.64` |
|--:|:-:|:-:|:-:|:-:|:-:|
| `int.8` | `int.8` | `int.32` | `int.64` | `float.32` | `float.64` |
| `int.32` | `int.32` | `int.32` | `int.64` | `float.64` | `float.64` |
| `int.64` | `int.64` | `int.64` | `int.64` | `float.64` | `float.64` |
| `float.32` | `float.32` | `float.64` | `float.64` | `float.32` | `float.64` |
| `float.64` | `float.64` | `float.64` | `float.64` | `float.64` | `float.64` |

#### The binary character sequence promotion

The *binary character sequence promotion* converts two textual values into a common type.

Where `S` and `T` are both textual type:

* If `S = T`,
  * the promoted type is `S`
* otherwise,
  * the promoted type is `string`

### The boolean conversion

TBD: C-style booleans?

### The assignment conversion

To assign expression result into a variable, the expression result will be converted to fit to the variable type.

Where there are an expression `e` with type `S` and a variable `v` with type `T`:

* If `T = promote(S, T)`, then we implicitly insert *cast operator* with type `T` into the `e` before assignment
* Otherwise, compile error

## Programs

### The compilation unit

The *compilation unit* includes global variables and functions and a body statement.

#### Form

```yaml
kind: "Program"
variables:
  - &v0 #: GlobalVariableDeclaration
  - &v1 #: GlobalVariableDeclaration
  - ...
functions:
  - &f0 #: GlobalFunctionDeclaration
  - &f1 #: GlobalFunctionDeclaration
  - ...
main: &s #: Statement
```

* Executes main statement `s`
* The variable declarations `v*` must be a list of zero or more *global variable declarations*
* The function declarations `f*` must be a list of zero or more *global function declarations*
* The body statement `s` must be a *statement*
* The compilation unit organizes a *variable scope*, the declared variables in `v*` are available in this compilation unit
  * The variable scope of the compilation unit may inherits the parent *system variable scope* which includes built-in variables and system variables
  * Compile error if initial value of variable declaration transitively requires the variable itself
* The compilation unit organizes a *function scope*, the declared functions in `f*` is available in this compilation unit globally
  * The function scope of the compilation unit may inherits the parent *system function scope* which includes built-in functions and system functions

### The global variable declaration

The *global variable declaration* declares a variable into the *variable scope* of the enclosing *compilation unit*.

#### Form

```yaml
kind: "GlobalVariableDeclaration"
name: &n #: Symbol
type: &t #: Type, optional
value: &e #: Expression
attributes:
  - &a0 #: GlobalVariableDeclaration::Attribute
  - &a1 #: GlobalVariableDeclaration::Attribute
  - ...
```

* The global variable declaration is a kind of *variable declaration*
* The variable name `n` must be a *symbol*
  * Compile error if the enclosing *compilation unit* has another global variable declaration with the same name
  * If the symbol is already declared in the parent *variable scope* (e.g. built-in constants), this declaration will **hide** the previously defined variables
* Either the variable type `t` or initial value `e` must be defined
  * If `t` is defined, it must be a *type*
    * It represents the *variable type*
    * Without it, the *variable type* becomes as same as `e`'s type
  * If `e` is defined, it must be an *expression*
    * It represents the initial value of the variable
    * The variable is *initialized* if `e` is defined, otherwise not *initialized*
  * If both `t` and `e` are defined, `e`'s type must be *assignment compatible* for the `v`'s type
    * compile error if they are not *assignment compatible*
  * Compile error if neither `t` nor `e` is defined
* The variable attributes `a*` must be a list of *GlobalVariableDeclaration::Attribute*
* null tolerance
  * `e` - as value

#### GlobalVariableDeclaration::Attribute catalog

`const`
~ The declared variable is immutable.
  The declaration requires `value` property for initialization, otherwise compile error

`parameter`
~ The declared variable is for program argument, and it will be initialized by the runtime.
  Compile error if both this attribute and value `e` are defined.

### The global function declaration

The *global function declaration* declares a function into the *function scope* of the enclosing *compilation unit*.

#### Form

```yaml
kind: "GlobalFunctionDeclaration"
name: &n #: Symbol
type: &t #: Type, optional
parameters:
  - &p0 #: GlobalFunctionDeclaration::Parameter
  - &p1 #: GlobalFunctionDeclaration::Parameter
  - ...
body: &s #: Statement
attributes:
  - &a0 #: GlobalFunctionDeclaration::Attribute
  - &a1 #: GlobalFunctionDeclaration::Attribute
  - ...
```

* The *global function declaration* is a kind of *function declaration*
* The function name `n` must be a *symbol*
  * Compile error if the enclosing *compilation unit* has another global function declaration with the same name
  * If the symbol is already declared in the parent *function scope* (e.g. built-in functions), this declaration will **hide** the previously defined variables
* The function type `t` must be a `type` if it is defined
  * If it is not defined, the function type will be inferred from its parameter types
* The function parameters `p*` must be a list of zero or more *GlobalFunctionDeclaration::Parameter*
* The function body `s` must be a *statement*
* The function attributes `a*` must be a list of zero or more *GlobalFunctionDeclaration::Attribute*

#### GlobalFunctionDeclaration::Parameter structure

```yaml
name: &n #: Symbol
type: &t #: Type, optional
attributes:
  - &a0 #: GlobalFunctionDeclaration::Parameter::Attribute
  - &a1 #: GlobalFunctionDeclaration::Parameter::Attribute
  - ...
```

* `n` must be a `symbol`
  * Compile error if the enclosing *function declaration* has another parameter with the same name
  * If the symbol is already declared in the parent *function scope* (e.g. global variables), this declaration will **hide** the previously defined variables
* `t` must be a `type` if it is defined
  * If it is not defined, parameter type will infer from each *function call* (static duck typing)
* `a*` must be a list of zero or more *GlobalFunctionDeclaration::Parameter::Attribute*

#### GlobalFunctionDeclaration::Attribute catalog

`inline`
~ The function body must be inlined into the caller

#### GlobalFunctionDeclaration::Parameter::Attribute catalog

`const`
~ The declared parameter is immutable from the function body.

## Imperative statements

### The empty statement

The *empty statement* does nothing. It is only for padding.

#### Form

```yaml
kind: "EmptyStatement"
```

### The block statement

The *block statement* executes statements sequentially.

#### Form

```yaml
kind: "BlockStatement"
elements:
  - &s0 #: Statement
  - &s1 #: Statement
  - ...
```

* The block statement organizes a *variable scope*, the declared variables in this block are available only in this block
* The element statements `s*` must be a list of one or more *statement*
* Variables declared in `s_i` can be refer from `s_j` (`j > i`)

### The local variable declaration

The *local variable declaration* declares a variable into the current *variable scope*.

#### Form

```yaml
kind: "LocalVariableDeclaration"
name: &n #: Symbol
type: &t #: Type, optional
value: &e #: Expression
attributes:
  - &a0 #: LocalVariableDeclaration::Attribute
  - &a1 #: LocalVariableDeclaration::Attribute
  - ...
```

* The local variable declaration is a kind of *variable declaration*
* The variable name `n` must be a *symbol*
  * The declared variable is available on the rest of the current block, includes their child blocks
  * If the symbol is already declared in the previous statements (includes outer blocks), this declaration will **hide** the previously defined variables
* Either the variable type `t` or initial value `e` must be defined
  * If `t` is defined, it must be a *type*
    * It represents the *variable type*
    * Without it, the *variable type* becomes as same as `e`'s type
  * If `e` is defined, it must be an *expression*
    * It represents the initial value of the variable
    * The variable is *initialized* if `e` is defined, otherwise not *initialized*
  * If both `t` and `e` are defined, `e`'s type must be *assignment compatible* for the `v`'s type
    * compile error if they are not *assignment compatible*
  * Compile error if neither `t` nor `e` is defined
* The variable attributes `a*` must be a list of *LocalVariableDeclaration::Attribute*
* null tolerance
  * `e` - as value
* TBD
  * more robust type inference

#### LocalVariableDeclaration::Attribute catalog

`const`
~ The declared variable is immutable.
  The declaration requires `value` property for initialization, otherwise compile error

### The branch statement

The *branch statement* first evaluates the condition expression, and then executes either statement by the condition.

#### Form

```yaml
kind: "BranchStatement"
condition: &e #: Expression
then: &s0 #: Statement
else: &s1 #: Statement
```

* `e` must be an *expression*
* `s0` and `s1` must be *statements*
* type restriction:
  * `e:bool`
* null tolerance
  * `e` - restricted

### The while statement

The *while statement* repeatedly executes the condition expression and the body statement until the condition becomes false.

#### Form

```yaml
kind: "WhileStatement"
condition: &e #: Expression
body: &s #: Statement
```

* The *while statement* is a kind of *loop statement*
* `e` must be an *expression*
* `s` must be a *statement*
* type restriction:
  * `e:bool`
* null tolerance
  * `e` - restricted

### The for statement

The *for statement* first executes the initialize statement only once, and then repeatedly executes condition expression, body statement and the update statement, until the condition becomes false.

#### Form

```yaml
kind: "ForStatement"
initialize:
  - &si0 #: Statement
  - &si1 #: Statement
  - ...
condition: &e #: Expression
update:
  - &su0 #: Statement
  - &su1 #: Statement
  - ...
body: &s #: Statement
```

* The for statement organizes a *variable scope*: variables declared in `si*` are available in this statement
* The *for statement* is a kind of *loop statement*
* The initialization statements `si*` must be a list of zero or more *statement*
* The loop condition `e` must be an *expression*
* The update statements `su*` must be a list of zero or more *statement*
* The body statement `s` must be a *statement*
* type restriction:
  * `e:bool`
* null tolerance
  * `e` - restricted

### The for-each statement

The *for-each* statement first evaluates the expression that represents a sequence of value, and then executes the body statement repeatedly for each element of sequence.

#### Form

```yaml
kind: "ForEachStatement"
name: &n #: Symbol
type: &t #: Type
enumeration: &e #: Expression
body: &s #: Statement
```

* The for-each statement organizes a *variable scope*, variable `n` is available in statement `s`
* The *for-each statement* is a kind of *loop statement*
* The *for-each statement* contains *variable declaration*
* `n` must be a *symbol*
  * The declared variable is available in the statement `s`
  * If the symbol is already declared in the previous statements (includes outer blocks), this declaration will **hide** the previously defined variables
* The variable type `t` must be a *type*
  * TBD: omissible
* `e` must be an *expression*
* `s` must be a *statement*
* type restriction:
  * `e:Sequence[T]` -> `n:T`
* null tolerance
  * `e` - restricted

### The return statement

The *return statement* moves control the out of the current program or function with a return value `e`.

#### Form

```yaml
kind: "ReturnStatement"
value: &e #: Expression
```

* `e` must be an *expression*
* type restriction:
  * `e` type must be *assignment compatible* with the enclosing function return type
* null tolerance
  * `e` - as value

### The anchor declaration

The *anchor declaration* gives an anchor of control to the body statement. The anchor will become a jump target of *{break, continue} statements*.

#### Form

```yaml
kind: "AnchorDeclaration"
name: &n #: Symbol
body: &s #: Statement
```

* `n` must be a *symbol*
  * Its symbol is only available for the enclosed statement `s`
  * If the symbol is already declared in the ancestor statements, this declaration will **hide** the symbol for the child statements
* `s` must be a *statement*

### The break statement

The *break statement* moves control out of the anchored statement, even if a *loop statement* was anchored.

#### Form

```yaml
kind: "BreakStatement"
anchor: &n #: Symbol
```

* `n` must be a *symbol*
  * The anchor must be declared in ancestor statements

### The continue statement

The *continue statement* moves control out of the current iteration of the anchored *loop statement*.

#### Form

```yaml
kind: "ContinueStatement"
anchor: &n #: Symbol
```

* Exits the current iteration of the loop statement which has anchor `n`
* `n` must be a *symbol*
  * The anchor must be declared in ancestor statements, and the anchored statement must be a *loop statement*.

### The expression statement

The *expression statement* just evaluates the body expression, and discards the evaluation result.

#### Form

```yaml
kind: "ExpressionStatement"
body: &e #: Expression
```

* `e` must be an *expression*
* type restriction:
  * `e:*` (don't care)
* null tolerance
  * `e` - as value (don't care)

### The raise statement

The *raise statement* halts the execution of the current program and passes error messages to the user.

#### Form

```yaml
kind: "RaiseStatement"
value: &e #: Expression
```

* The error data `e` must be an *expression*
* type restriction:
  * `e:tuple[...]`
* TBD
  * more type restriction


### The log statement

The *log statement* puts a log record into the system.

#### Form

```yaml
kind: "LogStatement"
level: &l #: LogLevel
value: &e #: Expression
```

* The log level `l` must be a *LogLevel*
* The log message `e` must be an *expression*
* type restriction:
  * `e:string`
* null tolerance
  * `e` - as value

#### LogStatement::Level catalog

`trace`
`debug`
`info`
`message`
`warning`
`error`
`critical`


## Basic expressions

### The literals

The *literal* represents a immediate value.

#### Form

```yaml
kind: "Literal"
type: &t #: Type
value: &v #: actual value
```

* The literal type `t` must be a *type*
* The literal value `v` must be represented in the host language (YAML/C++ API)
* type restriction:
  * `t=int.N` -> `t`
  * `t=float.N` -> `t`
  * `t=char.N` -> `t`
  * `t=string` -> `t`
  * `t=bool` -> `t`
  * `t=null` -> `t`

### The variable reference

The variable reference resolves a variable as an assigned value.

#### Form

```yaml
kind: "VariableReference"
name: &v #: Symbol
```

* `v` must be a variable name
  * The variable must be declared in this or ancestor *variable scope*
  * compile error if the variable is **possible** *uninitialized*
* Type of the expression is as same as the variable's type

### The block expression

The *block expression* first executes head statements, and the evaluates the body expression.

#### Form

```yaml
kind: "BlockExpression"
head:
  - &s0 #: Statement
  - &s1 #: Statement
  - ...
body:
  - &e #: Expression
```

* `s*` must be a list of one or more *statement*
* `e` must be an *expression*
* Variables declared in `s_i` can be refer from `s_j` (`j > i`) and `e`.
* `e` will be resolved as the outer block expression result.
* Type of the expression is as same as `e`'s type.

### The unary operators

#### Form

```yaml
kind: "UnaryOperator"
operator_kind: &o #: UnaryOperator::Kind
operand: &e #: Expression
```

* The operator `o` must be *UnaryOperator::Kind*
* The operand `e` must be *expression*

#### UnaryOperator::Kind catalog

`plus`
~ do nothing for numeric values.
~ type restriction:
  * `e:int.T` -> `int.T`
  * `e:float.T` -> `float.T`
~ null tolerance
  * acceptable

`sign_inversion`
~ inverts sign of numeric values.
  may overflow.
~ type restriction:
  * `e:int.T` -> `int.T`
  * `e:float.T` -> `float.T`
~ null tolerance
  * acceptable

`bitwise_complement`
~ returns bitwise complement of integer values.
~ type restriction:
  * `e:int.T` -> `int.T`
~ null tolerance
  * acceptable

`{logical,conditional}_not`
~ returns negate of boolean values.
~ type restriction:
  * `e:bool` -> `bool`
~ null tolerance
  * acceptable

`is_{true,false}`, `is_not_{true,false}`
~ returns whether or not the operand is `true` / `false`.
~ type restriction:
  * `e:bool` -> `bool`
~ null tolerance
  * as value

`is_null`, `is_not_null`
~ returns whether or not the operand is `null`.
~ type restriction:
  * `e:Number` -> `bool`
  * `e:Text` -> `bool`
  * `e:bool` -> `bool`
~ null tolerance
  * as value

`check_null`
~ aborts calculation only if the operand was `null`, or return non-nullable value.
~ type restriction:
  * `e0:Number`, `e1:Number` -> `bool`
  * `e0:Text`, `e1:Text` -> `bool`
  * `e0:bool`, `e1:bool` -> `bool`
~ null tolerance
  * as value

`relation_all`, `relation_any`
~ tests whether or not all/any values are `true`.
  These operators only can appear in condition term of the *selection expressions*.
~ type restriction:
  * `e:relation[tuple[bool]]` -> `bool`
~ null tolerance
  * `e` - restricted

`relation_exists`, `relation_not_exists`
~ tests whether or not the operand relation is empty.
  These operators only can appear in condition term of the *selection expressions*.
~ type restriction:
  * `e:relation[T]` -> `bool`
~ null tolerance
  * `e` - restricted

### The binary operators

#### Form

```yaml
kind: "BinaryOperator"
operator_kind: &o #: BinaryOperator::Kind
left: &e0 #: Expression
right: &e0 #: Expression
```

* The operator `o` must be *BinaryOperator::Kind*
* The operands `e0` and `e1` must be *expression*

#### BinaryOperator::Kind catalog

`add`, `subtract`, `multiply`, `divide`, `remainder`
~ arithmetic operation.
~ type restriction:
  * `e0:U`, `e1:V` -> `promote(U, V) <= Number`
~ null tolerance
  * acceptable

`bitwise_{and,or,xor}`
~ bitwise operation.
~ type restriction:
  * `e0:U`, `e1:V` -> `promote(U, V) <= Integer`
~ null tolerance
  * acceptable

`shift_{left,right_{arithmetic,logical}}`
~ shift operations.
~ type restriction:
  * `e0:int.T`, `e1:int.*` -> `int.T`
~ null tolerance
  * acceptable

`equal`, `not_equal`
~ compares equivalent of two values.
~ type restriction:
  * `e0:Number`, `e1:Number` -> `bool`
  * `e0:Text`, `e1:Text` -> `bool`
  * `e0:bool`, `e1:bool` -> `bool`
~ null tolerance
  * as value
~ equivalence
  * `Number` - numerical equality
  * `Text` - equality of two sequences
  * otherwise - identity
    * TBD: null == null ?

`in`
~ tests whether or not second element contains an equivalent value of the first element.
~ type restriction:
  * `e0:Number`, `e1:Sequence[ee:Number]` -> `bool`
  * `e0:Text`, `e1:Sequence[ee:Text]` -> `bool`
  * `e0:bool`, `e1:Sequence[ee:bool]` -> `bool`
~ null tolerance
  * `e0` - as value
  * `e1` - acceptable
  * `ee` - as value
~ equivalence:
  * same as `equal`

`{less,greater}_than`, `{less,greater}_than_or_equal`
~ compare magnitude of two values.
~ type restriction:
  * `e0:Number`, `e1:Number` -> `bool`
  * `e0:Text`, `e1:Text` -> `bool`
  * `e0:bool`, `e1:bool` -> `bool`
~ null tolerance
  * acceptable
~ ordering
  * `Number` - numeric order
  * `Text` - lexicographic order
  * `bool` - `false < true`

`logical_{and,or,xor}`
~ logical operation.
~ type restriction:
  * `e0:bool`, `e1:bool` -> `bool`
~ null tolerance
  * acceptable

`conditional_{and,or}`
~ short circuit conditional operation.
~ type restriction:
  * `e0:bool`, `e1:bool` -> `bool`
~ null tolerance
  * acceptable

`relation_in`
~ tests whether or not second one element relation contains an equivalent value of the first element.
  This operator only can appear in condition term of the *selection expressions*.
~ type restriction:
  * `e0:Number`, `e1:relation[tuple[ee:Number]]` -> `bool`
  * `e0:Text`, `e1:relation[tuple[ee:Text]]` -> `bool`
  * `e0:bool`, `e1:relation[tuple[ee:bool]]` -> `bool`
~ null tolerance
  * `e0` - as value
  * `e1` - acceptable
  * `ee` - as value
~ equivalence:
  * same as `equal`

`relation_{union,intersection,difference}`, `*_all`
~ set/bag operation of relational algebra.
~ type restriction:
  * `e0:relation[T]`, `e1:relation[T]` -> `relation[T]`
~ null tolerance
  * restricted

* TODO:
  * outer union
  * union corr

### The type operators

#### Form

```yaml
kind: "TypeOperator"
operator_kind: &o #: TypeOperator::Kind
type: &t #: Type
operand: &e #: Expression
```

* The operator `o` must be *TypeOperator::Kind*
* The type parameter `t` must be *type*
* The operand `e` must be *expression*

#### TypeOperator::Kind catalog

`cast`
~ converts values into the specified type.
~ type restriction:
  * `t=*`, `e:t` -> `t`
    * identical operation
  * `t=int.T`, `e:int.*` -> `t`
    * may overflow
  * `t=int.T`, `e:float.*` -> `t`
    * round to zero
  * `t=float.T`, `e:int.*` -> `t`
    * environment dependent rounding
  * `t=float.T`, `e:float.*` -> `t`
    * environment dependent rounding
  * `t=char.T`, `e:string` -> `t`
    * may discard exceeded trailing characters.
  * `t=char.T`, `e:char.*` -> `t`
    * may discard exceeded trailing characters.
  * `t=string`, `e:char.*` -> `t`
~ null tolerance
  * `e` - acceptable if `t` is nullable, otherwise restricted

#### note

* `parse` and `format` are not type operators, but built-in functions

### The string operators

#### Form

```yaml
kind: "StringOperator"
operator_kind: &o #: StringOperator::Kind
string: &s #: String
operand: &e #: Expression
```

* The operator `o` must be *StringOperator::Kind*
* The string parameter `s` must be a string
* The operand `e` must be an *expression*

#### StringOperator::Kind catalog

`like`, `like_ignorecase`
~ TODO

`regex`
~ TODO

#### StringOperator::Kind catalog

`like`, `like_ignorecase`
~ TODO

`regex`
~ TODO

### The implicit type casting

The *implicit type casting* is an expression which will be generated by the compiler. It converts expression type into another type.

#### Form

```yaml
kind: "ImplicitCast"
type: &t #: Type
operand: &e #: Expression
```

* The type `t` must be a *type*
* The operand `e` must be an *expression*

### The case expression

The *case expression* is an expression form of *branch statement* with multiple cases. This expression resolved as a body expression of the first case clause which condition is *true*, or the default expression if there are no *true* conditions.

#### Form

```yaml
kind: "CaseExpression"
cases:
  - &c0 #: CaseExpression::Clause
  - &c1 #: CaseExpression::Clause
  - ...
default: &e #: Expression
```

* The case clauses `c*` must be a list of one or more *CaseExpression::Clause*.
* The default expression `e` must be an *expression*
* type restriction
  * `c0.body:U0`, `c1.body:U1`, ..., `e:V` -> `promote(U0, U1, ..., V)`
* null tolerance
  * `e` - as value

#### CaseExpression::Clause structure

```yaml
condition: &e0
body: &e1
```

* The condition expression `e0` must be an *expression*
* The body expression `e1` must be an *expression*
* type restriction
  * `e0:bool`, `e1:*`
* null tolerance
  * `e0` - acceptable (TBD)
  * `e1` - as value

### The function call

The *function call* executes the body statements of the target function with given argument expressions.

#### Form

```yaml
kind: "FunctionCall"
name: &n #: Symbol
arguments:
  - &e0 #: Expression
  - &e1 #: Expression
  - ...
```

* `kind` must be *FunctionCall*
* `n` must be a *symbol*
  * The function must exist in the current *function scope*
* `e*` must be a list of zero or more *expressions*
* Type of the expression is as same as the function's return type
* null tolerance
  * depends on individual function declarations
  * compile error occurs if `e_i` is nullable and the corresponded function parameter does not accepts nullable values
    * please guard by using `check_null`
* TBD
  * evaluation strategy of arguments

### The assign expression

The *assign expression* sets a value into a previously declared variable.

#### Form

```yaml
kind: "AssignExpression"
name: &v #: Symbol
value: &e #: Expression
```

* `v` must be a variable name
  * The variable must be declared in previous *variable declaration*
* `e` must be an *expression*
  * `e`'s type must be *assignment compatible* with the `v`'s type, otherwise compile error
* The variable will become *initialized* after this operation
* Compile error if the target variable is *immutable*
* type restriction:
  * `v:S`, `e:T` -> `S`
    * where `S = promote(S, T)`
* null tolerance
  * `e` - as value
* TBD
  * move to expression?

#### Assignment compatibility

The expression type is *assignment compatible* with the variable type, only if the binary promotion of expression type and variable type equals to the variable type.

## Data structure operations

* TODO
  * errors
    * index out of bounds
    * storing immutable elements

### The array creation

The *array creation operation* creates a new array.

#### Form

```yaml
kind: "ArrayCreationExpression"
type: &t #: Type
values:
  - &e0 #: Expression
  - &e1 #: Expression
  - ...
```

* The array type `t` must be a *type* which represents an array
* The initial elements `e*` must be a list of zero or more *expressions*
  * If it is empty, the created array is not initialized
  * Otherwise, they are the initial elements of the array
    * The number of expressions must be as same as the array length
    * Each element's type must be *assignment compatible* with the element type of the array
* type restriction:
  * `t=array.N[S]`, `e*:T` -> `t`
    * where `S = promote(S, T)`
* null tolerance:
  * `e*` - as value

### The array element load

The *array element load operation* obtains an element by index from an array.

#### Form

```yaml
kind: "ArrayElementLoadExpression"
data: &e0 #: Expression
index: &e1 #: Expression
```

* The data `e0` must be an *expression* which represents an array
* The index `e1` must be an *expression* which represents an integer
* type restriction:
  * `e0:array.N[T]`, `e1:int.*` -> `T`
* null tolerance:
  * `e0` - restricted
  * `e1` - restricted

### The array element store

The *array element store operation* assigns an element by index into an array.

#### Form

```yaml
kind: "ArrayElementStoreExpression"
data: &e0 #: Expression
index: &e1 #: Expression
value: &e2 #: Expression
```

* The data `e0` must be an *expression* which represents an array
* The index `e1` must be an *expression* which represents an integer
* The value `e2` must be an *expression*
* type restriction:
  * `e0:array.N[S]`, `e1:int.*`, `e2:T` -> `S`
    * where `S = promote(S, T)`
* null tolerance:
  * `e0` - restricted
  * `e1` - restricted
  * `e2` - as value

### The vector creation

The *vector creation* creates a new vector.

#### Form

```yaml
kind: "VectorCreationExpression"
type: &t #: Type
values:
  - &e0 #: Expression
  - &e1 #: Expression
  - ...
```

* The array type `t` must be a *type* which represents a vector
* The initial elements `e*` must be a list of zero or more *expressions*
  * If it is empty, the created vector is empty
  * Otherwise, they are the initial elements of the vector
    * Each element's type must be *assignment compatible* with the element type of the vector
* type restriction:
  * `t=vector[S]`, `e*:T` -> `t`
    * where `S = promote(S, T)`
* null tolerance:
  * `e*` - as value

### The vector element load

The *vector element load operation* obtains an element by index from a vector.

#### Form

```yaml
kind: "VectorElementLoadExpression"
data: &e0 #: Expression
index: &e1 #: Expression
```

* The data `e0` must be an *expression* which represents a vector
* The index `e1` must be an *expression* which represents an integer
* type restriction:
  * `e0:vector[T]`, `e1:int.*` -> `T`
* null tolerance:
  * `e0` - restricted
  * `e1` - restricted

### The vector element store

The *vector element store operation* assigns an element by index into a vector.

#### Form

```yaml
kind: "VectorElementStoreExpression"
data: &e0 #: Expression
index: &e1 #: Expression
value: &e2 #: Expression
```

* The data `e0` must be an *expression* which represents a vector
* The index `e1` must be an *expression* which represents an integer
* The value `e2` must be an *expression*
* type restriction:
  * `e0:vector[S]`, `e1:int.*`, `e2:T` -> `S`
    * where `S = promote(S, T)`
* null tolerance:
  * `e0` - restricted
  * `e1` - restricted
  * `e2` - as value

### The vector element insert

The *vector element insert operation* inserts values into a vector.

#### Form

```yaml
kind: "VectorElementInsertStatement"
data: &e0 #: Expression
index: &e1 #: Expression
values:
  - &v0 #: Expression
  - &v1 #: Expression
  - ...
```

* The data `e0` must be an *expression* which represents a vector
* The index `e1` must be an *expression* which represents an integer
  * This operation inserts values before the target the target element
* The values `v*` must be a list of one or more *expressions*
* type restriction:
  * `e0:vector[S]`, `e1:int.*`, `v*:T`
    * where `S = promote(S, T)`
* null tolerance:
  * `e0` - restricted
  * `e1` - restricted
  * `v*` - as value

### The vector element delete

The *vector element delete operation* removes elements on a vector.

#### Form

```yaml
kind: "VectorElementDeleteStatement"
data: &e0 #: Expression
begin: &e1 #: Expression
end: &e2 #: Expression
```

* The data `e0` must be an *expression* which represents a vector
* The index `e1`, `e2` must be an *expression* which represents an integer
  * This operation removes elements `e0[i]` `e1 <= i < e2`
* type restriction:
  * `e0:vector[T]`, `e1:int.*`, `e2:int.*`
* null tolerance:
  * `e0` - restricted
  * `e1` - restricted
  * `e2` - restricted

### The vector length

The *vector element length operation* obtains length of a vector.

#### Form

```yaml
kind: "VectorLengthExpression"
data: &e #: Expression
```

* The data `e` must be an *expression* which represents a vector
* type restriction:
  * `e:vector[T]` -> `int.64`
* null tolerance:
  * `e` - restricted

### The tuple creation

The *tuple creation* creates a new tuple.

#### Form

```yaml
kind: "TupleCreationExpression"
type: &t #: Type
values:
  - &e0 #: Expression
  - &e1 #: Expression
  - ...
```

* The array type `t` must be a *type* which represents a tuple
* The initial elements `e*` must be a list of zero or more *expressions*
  * If it is empty, the created tuple is not initialized
  * Otherwise, they are the initial elements of the array
    * The number of expressions must be as same as the tuple length
    * Each element's type must be *assignment compatible* with type of the corresponded tuple element
* type restriction:
  * `t=tuple[S0, S1, ...]`, `e0:T0`, `e1:T1`, ... -> `t`
    * where `S0 = promote(S0, T0)`, `S1 = promote(S1, T1)`, ...
* null tolerance:
  * `e*` - as value

### The tuple element load

The *tuple element load operation* obtains an element from a tuple.

#### Form

```yaml
kind: "TupleElementLoadExpression"
data: &e #: Expression
index: &n #: Symbol or Index
```

* The data `e` must be an *expression* which represents a vector
* The index `n` must be a *symbol* or *index*
* type restriction:
  * `e:tuple[..., n:T, ...]` -> `T`
* null tolerance:
  * `e` - restricted

### The tuple element store

The *tuple element load operation* obtains an element from a tuple.

#### Form

```yaml
kind: "TupleElementStoreExpression"
data: &e #: Expression
index: &n #: Symbol or Index
value: &v #: Expression
```

* The data `e` must be an *expression* which represents a vector
* The index `n` must be a *symbol* or *index*
* The value `v` must be an *expression*
* type restriction:
  * `e:tuple[..., n:S, ...]` -> `S`
    * where `S = promote(S, T)`
* null tolerance:
  * `e` - restricted
  * `v` - as value

### The cursor creation

The *cursor creation operation* converts a relation into a cursor to iterate each rows of the relation.

#### Form

```yaml
kind: "CursorCreationExpression"
data: &e #: Expression
```

* The data `e` must be an *expression* which represents a relation
* The created cursor does not point to valid elements, so that programs should apply *cursor advance operation* before obtain its elements
* type restriction:
  * `e:relation[T]` -> `cursor[T]`
* null tolerance:
  * `e` - restricted

### The cursor advance

The *cursor advance operation* moves the state of a cursor to point the next element.

#### Form

```yaml
kind: "CursorAdvanceExpression"
data: &e #: Expression
```

* The data `e` must be an *expression* which represents a cursor
* This operation returns whether or not the next element exists
* type restriction:
  * `e:cursor[T]` -> `boolean`
* null tolerance:
  * `e` - restricted

### The cursor element load

The *cursor element load operation* obtains the current pointed element of a cursor.

#### Form

```yaml
kind: "CursorElementLoadExpression"
data: &e #: Expression
```

* The data `e` must be an *expression* which represents a cursor
  * The cursor must point a valid element by using *cursor advance operation*
* type restriction:
  * `e:cursor[T]` -> `T`
* null tolerance:
  * `e` - restricted

## Relational algebra expressions

### The scan operator

The *scan operator* obtains a table data as relation from the connected database.

#### Form

```yaml
kind: "ScanExpression"
table: &n0 #: Symbol
alias: &n1 #: Symbol
```

* The *scan operator* is a kind of *relational algebra expression*
* The table name `n0` must be a *symbol*
* The alias name `n1` must be a *symbol* if it is defined

### The projection operator

The *projection operator* is a unary operation of relational algebra, that adds/removes columns of relations (a.k.a. *extended projection*).

#### Form

```yaml
kind: "ProjectionExpression"
operand: &e #: Expression
alias: &n #: Symbol
initialize:
  - &s0 #: Statement
  - &s1 #: Statement
  - ...
columns:
  - &c0 #: ProjectionExpression::Column
  - &c1 #: ProjectionExpression::Column
  - ...
```

* The *projection operator* is a kind of *relational algebra expression*
* The *projection operator* organizes a *variable scope*
  * `s*` and `c*` can refer each row of `e`
* The operand `e` must be an *expression* which represents a relation
* The alias name `n1` must be a *symbol* if it is defined
* The initialize statements `s*` must be a list of zero or more *Statement*
  * The initialize statements will be executed for every rows to prepare the common subexpressions of the column specifications, and also may re-executed for every columns for optimization - avoid side effect operations
  * The variable declaration in this list can be refer from the column specifications
* The column specifications `c*` must be a list of one or more *ProjectionExpression::Column*
  * This must represent the resulting tuple type of relations
* type restriction:
  * `e:relation[T]`
* null tolerance:
  * `e` - restricted
* remarks
  * The projection operator does not support any aggregations, use *aggregation operator* instead

#### ProjectionExpression::Column structure

The *ProjectionExpression::Column* defines each column value of projection result.

```yaml
value: &e #: Expression
alias: &n #: Symbol
```

* The column value `e` must be an *expression*
  * If the column type `t` is defined, `e`'s type must be *assignment compatible* with `t`
* The column name `n` must be a *symbol* if it defined
* TBD:
  * explicit attributes?
  * type restriction

### The selection operator

The *selection* is a unary operation of relation algebra, that removes rows of relations.

#### Form

```yaml
kind: "SelectionExpression"
operand: &e0 #: Expression
condition: &e1 #: Expression
```

* The *selection operator* is a kind of *relational algebra expression*
* The *selection operator* organizes a *variable scope*
  * `e1` can refer each row of `e0`
* The operand `e0` must be an *expression* which represents a relation
* The condition expression `e1` must be an *expression* which represents a boolean
* type restriction:
  * `e:relation[T]` -> `relation[T]`


### The join operator

The *join* is a binary operation of relational algebra, that joins two or more relations.

#### Form

```yaml
kind: "JoinExpression"
operaotr_kind: &d #: JoinExpression::Kind
left: &e0 #: Expression
right: &e1 #: Expression
condition: &e2 #: Expression
```

* The *join operator* is a kind of *relation algebra expression*
* The *join operator* organizes a *variable scope*
* The join operator `d` must be a *JoinExpression::Kind*
* The join operands `e0` and `e1` must be *expression*
* The join condition `e2` must be an *expression* if it is defined
* type restriction:
  * `e2:bool`
* null tolerance:
  * `e2` - restricted (TBD: treats `null` as `false`?)

#### JoinExpression::Kind catalog

`cross`
~ Computes cartesian product of `e0` and `e1`.
~ The condition expression must not be defined.
~ type restriction:
  * `e0:Relation[U]`, `e1:Relation[V]` -> `relation[U+V]`

`inner`
~ Computes inner join of `e0` and `e1`.
~ The condition expression may or may not be defined.
~ type restriction:
  * `e0:Relation[U]`, `e1:Relation[V]` -> `relation[U+V]`

`{left,right,full}_outer`
~ Computes left/right/full outer join of `e0` and `e1`.
~ The condition expression may or may not be defined.
~ type restriction:
  * `left_outer`
    * `e0:Relation[U]`, `e1:Relation[V]` -> `relation[U+nullable_element(V)]`
  * `right_outer`
    * `e0:Relation[U]`, `e1:Relation[V]` -> `relation[nullable_element(U)+V]`
  * `full_outer`
    * `e0:Relation[U]`, `e1:Relation[V]` -> `relation[nullable_element(U)+nullable_element(V)]`

`inner`
~ Computes natural inner join of `e0` and `e1`.
~ The condition expression must not be defined.
~ type restriction:
  * `e0:Relation[U]`, `e1:Relation[V]` -> `relation[U+(V-U)]`

`natural_{left,right,full}_outer`
~ Computes natural left/right/full outer join of `e0` and `e1`.
~ The condition expression must not be defined.
~ type restriction:
  * `left_outer`
    * `e0:Relation[U]`, `e1:Relation[V]` -> `relation[U+nullable_element(V-U)]`
  * `right_outer`
    * `e0:Relation[U]`, `e1:Relation[V]` -> `relation[nullable_element(U)+(V-U)]`
  * `full_outer`
    * `e0:Relation[U]`, `e1:Relation[V]` -> `relation[nullable_element(U)+nullable_element(V-U)]`

`union_outer`
~ Computes union join of `e0` and `e1`.
~ The condition expression must not be defined.
~ type restriction:
  * `e0:Relation[U]`, `e1:Relation[V]` -> `relation[nullable_element(U)+nullable_element(V)]`

`{left,right}_semi`
~ Computes left/right semi join of `s0` and `s1`.
~ The condition expression may or may not be defined.
~ type restriction:
  * `s0=(n=p, e:relation[U])`, `s1=(n=q, e:relation[V])` -> ...
  * `left_semi`
    * `relation[U]`
  * `right_semi`
    * `relation[V]`

### The distinct operator

The *distinct operator* removes duplicated tuples in relation.

#### Form

```yaml
kind: "DistinctExpression"
operand: &e #: Expression
subsets:
  - &s0 #: Symbol or Index
  - &s1 #: Symbol or Index
  - ...
```

* The *distinct operator* is a kind of *relational algebra expression*
* The operand `e` must be an *expression* which represents a relation
* The subset symbols `s*` must be a list of zero or more *symbols* or integers
  * If it is empty, this removes duplicated tuples
  * Otherwise, this finds for duplications by the given subset of columns
* type restriction:
  * `e:relation[T]` -> `relation[T]`
* null tolerance:
  * `e` - restricted

### The aggregation operator

The *aggregation operator* aggregates tuples in relations.

#### Form

```yaml
kind: "AggregationExpression"
operand: &e #: Expression
keys:
  - &k0 #: Symbol or Index
  - &k1 #: Symbol or Index
  - ...
initialize:
  - &s0 #: Statement
  - &s1 #: Statement
  - ...
columns:
  - &c0 #: AggregationExpression::Column
  - &c1 #: AggregationExpression::Column
  - ...
```

* The *aggregation operator* is a kind of *relational algebra expression*
* The *aggregation operator* organizes a *variable scope*
  * `e1` can refer each row of `e0`
* The operand `e` must be an *expression* which represents a relation
* The grouping keys `k*` must be a list of zero or more *Symbols* or *Indices*
  * If there are no grouping keys, this aggregates the whole relation
  * Otherwise, this divides relations by the grouping keys and then aggregates each group
* The initialize statements `s*` must be a list of zero or more *Statement*
  * The initialize statements will be executed for every rows to prepare the common subexpressions of the column specifications, and also may re-executed for every columns for optimization - avoid side effect operations
  * The variable declaration in this list can be refer from the column specifications
* The column specifications `c*` must be a list of one or more *AggregationExpression::Column*
  * This must represent the resulting tuple type of relations
* type restriction:
  * `e:relation[T]`
* null tolerance:
  * `e` - restricted

#### AggregationExpression::Column structure

The *AggregationExpression::Column* defines each column value of aggregation result.

```yaml
name: &n #: Symbol
type: &t #: Type
function: &n #: Symbol
arguments:
  - &e0 #: Expression
  - &e1 #: Expression
  - ...
```

* The column name `n` must be a *symbol* if it defined
* The column type `t` must be a *type* if it defined
  * If it is defined, the aggregation result type must be *assignment compatible* with `t`
  * Otherwise, the column result type will become as same as the function result type
* The aggregation function name `n` must be a *symbol*
  * This must refer an aggregation function (TBD: UDAF)
* The aggregation function arguments `e*` must be a list of zero or more *expression*

#### The built-in aggregate functions catalog

`builtin.aggregation.any`
~ Just a value.
  The argument `e0` must not depend on the source operand except grouping keys.
~ type restriction:
  * `e0:T` -> `T`
~ null tolerance:
  * `e0` - as value

`builtin.aggregation.count_rows`
~ Counts up rows (a.k.a. `COUNT(*)`).
~ type restriction:
  * -> `int.64`

`builtin.aggregation.count_all`
~ Counts up non-null values.
~ type restriction:
  * `e0:*` -> `int.64`
~ null tolerance:
  * `e0` - as value

`builtin.aggregation.count_distinct`
~ Counts up unique non-null values.
~ type restriction:
  * `e0:*` -> `int.64`
~ null tolerance:
  * `e0` - as value

`builtin.aggregation.sum`
~ Computes sum of the non-null values.
~ type restriction:
  * `e0:int.N` -> `int.64?`
  * `e0:float.N` -> `float.64?`
~ null tolerance:
  * `e0` - as value
~ remarks:
  * The result may become null if there are no non-null values

`builtin.aggregation.{max,min}`
~ Computes maximum/minimum value of the non-null values.
~ type restriction:
  * `e0:T` -> `T?`
    * where `T` must be ordered
~ null tolerance:
  * `e0` - as value
~ remarks:
  * The result may become null if there are no non-null values

`builtin.aggregation.mean`
~ Computes mean of the non-null values.
~ Computes maximum/minimum value of the non-null values.
~ type restriction:
  * `e0:Numeric` -> `float.64?` (TBD)
~ null tolerance:
  * `e0` - as value
~ remarks:
  * The result may become null if there are no non-null values

### The order operator

```yaml
kind: "OrderExpression"
operand: &e #: Expression
elements:
  - &s0 #: OrderExpression::Element
  - &s1 #: OrderExpression::Element
  - ...
```

* The *order operator* is a kind of *relational algebra expression*
* The operand `e` must be an *expression* which represents a relation
* The elements `s*` must be a list of one or more *OrderExpression::Element*
* type restriction:
  * `e:relation[T]` -> `relation[T]`
* null tolerance:
  * `e` - restricted

#### OrderExpression::Element structure

The *OrderExpression::Element* defines each ordering key and its direction.

```yaml
column: &n #: Symbol or Index
direction: &d #: OrderExpression::Direction
```

* The column name `n` must be a *symbol* or *index*
* The order direction `d` must be a *OrderExpression::Direction*

#### OrderExpression::Direction catalog

`ascendant`
~ Ascendant order.

`descendant`
~ Descendant order.


### The limit operator

The *limit operator* drops rows in relations which the number of rows is exceeded.

#### Form

```yaml
kind: "LimitExpression"
operand: &e #: Expression
count: &v #: Integer
```

* The *limit operator* is a kind of *relational algebra expression*
* The operand `e` must be an *expression* which represents a relation
* The count `v` must be an integer
* type restriction:
  * `e:relation[T]` -> `relation[T]`
* null tolerance:
  * `e` - restricted


## DML operations

### The emit statement

The *emit statement* emits sequence of tuples as program output.

#### Form

```yaml
kind: "EmitStatement"
source: &e #: Expression
```

* The source relation `e` must be an *expression* which represents a relation
* type restriction:
  * `e:relation[T]`
* null tolerance:
  * `e` - restricted

### The update statement

The *update statement* updates tuples on a database table.

#### Form

```yaml
kind: "UpdateStatement"
table: &t #: Symbol
columns:
  - &c0 #: UpdateStatement::Column
  - &c1 #: UpdateStatement::Column
  - ...
condition: &e #: Expression
```

* The *update statement* organizes a *variable scope*
  * `c*` and `e` can refer each row of table `t`
* The table name `t` must be a *symbol*
* The column specifications `c*` must be a list of one or more *UpdateStatement::Column*
* The condition `e` must be an *expression* if it is defined
  * If it is not defined, this will update all entries in `t`
* type restriction:
  * `e:bool`
* null tolerance:
  * `e` - restricted

#### UpdateStatement::Column structure

The *UpdateStatement::Column* defines each column value of update.

```yaml
name: &n #: Symbol
value: &e #: Expression
```

* The column name `n` must be a *symbol*
* The column value `e` must be an *expression*
  * `e`'s type must be *assignment compatible* with the original column `n`'s type

### The insert relation statement

The *insert relation statement* inserts a relation into a database table.

#### Form

```yaml
kind: "InsertRelationStatement"
table: &t #: Symbol
columns:
  - &c0 #: Symbol
  - &c1 #: Symbol
  - ...
source: &e #: Expression
```

* The table name `t` must be a *symbol*
* The column name `c*` must be a list of one or more *symbol*
* The source relation `e` must be an *expression* which represents a relation
  * The number of columns in the relation must be as same as the number of specified columns `c*`
* type restriction:
  * `e:relation[T]`
    * where `T = tuple[U0, U1, ...]`
      * each `U_i` must be *assignment compatible* with the corresponded column `c_i`'s type
* null tolerance:
  * `e` - restricted

### The insert values statement

The *insert values statement* inserts a tuple into a database table.

#### Form

```yaml
kind: "InsertValuesStatement"
table: &t #: Symbol
columns:
  - &c0 #: InsertValuesStatement::Column
  - &c1 #: InsertValuesStatement::Column
  - ...
```

* The table name `t` must be a *symbol*
* The column specifications `c*` must be a list of one or more *InsertValuesStatement::Column*

#### InsertValuesStatement::Column structure

The *InsertValuesStatement::Column* defines each column value to insert.

```yaml
name: &n #: Symbol
value: &e #: Expression
```

* The column name `n` must be a *symbol*
* The column value `e` must be an *expression*
  * `e`'s type must be *assignment compatible* with the original column `n`'s type


### The delete statement

The *delete statement* deletes tuples on a database table.

#### Form

```yaml
kind: "DeleteStatement"
table: &t #: Symbol
condition: &e #: Expression
```

* The *update statement* organizes a *variable scope*
  * `e` can refer each row of `e0`
* The table name `t` must be a *symbol*
* The condition `e` must be an *expression* if it is defined
  * If it is not defined, this will delete all entries in `t`
* type restriction:
  * `e:bool`
* null tolerance:
  * `e` - restricted

## DDL operations

### The create table statement

The *create table statement* creates a new table into the database.

#### Form

```yaml
kind: "CreateTableStatement"
table: &n #: Symbol
columns:
  - &c0 #: CreateTableStatement::Column
  - &c1 #: CreateTableStatement::Column
  - ...
attributes:
  - &a0 #: CreateTableStatement::Attribute
  - &a1 #: CreateTableStatement::Attribute
  - ...
primary_keys:
  - &k0 #: CreateTableStatement::PrimaryKey
  - &k1 #: CreateTableStatement::PrimaryKey
  - ...
```

* The table name `n` must be a *symbol*
* The column specifications `c*` must be a list of one or more *CreateTableStatement::Column*.
* The attributes `a*` must be a list of zero or more *CreateTableStatement::Attribute*.
* The primary key attributes `k*` must be a list of zero or more *CreateTableStatement::PrimaryKey*
  * If it is empty, the target table does not have any primary keys
  * Otherwise, the table will have the primary key as sequence of the columns
* TBD:
  * constraints
  * triggers

#### CreateTableStatement::Column structure

```yaml
name: &n #: Symbol
type: &t #: Type
value: &v #: Expression
```

* The column name `n` must be a *symbol*
* The column type `t` must be a *type*
* The column default value `v` must be an *expression* if it is defined
  * If it is defined, the `v`'s type must be *assignment compatible* with `t`
* TBD:
  * auto increment in `v`?

#### CreateTableStatement::Attribute catalog

`if_not_exists`
~ Creates table only if it does not exist.

#### CreateTableStatement::PrimaryKey structure

```yaml
name: &n #: Symbol
direction: &d #: CreateTableStatement::Direction
```

* The column name `n` must be a *symbol*
* The sort direction `d` must be a *CreateTableStatement::Direction*

#### CreateTableStatement::Direction catalog

`dont_care`
~ Don't care.

`ascendant`
~ Ascendant order.

`descendant`
~ Descendant order.

#### NOTE

* CTAS statement = *create table statement* + *insert relation statement*

### The drop table statement

The *drop table statement* removes a table from the database.

#### Form

```yaml
kind: "DropTableStatement"
table: &n #: Symbol
attributes:
  - &a0 #: DropTableStatement::Attribute
  - &a1 #: DropTableStatement::Attribute
  - ...
```

* The table name `n` must be a *symbol*
* The attributes `a*` must be a list of zero or more *DropTableStatement::Attribute*.

#### DropTableStatement::Attribute catalog

`if_exists`
~ Drops table only if it exists.


## Transaction control operations

### The transaction block statement

The *transaction block statement* executes statements sequentially in a transaction.

#### Form

```yaml
kind: "TransactionBlockStatement"
body: &s #: Statement
```

* The body statement `s` must be a *statement*

## Built-in functions

* TBD
  * atoi
  * itoa
  * concat
  * subst
  * char
  * ...


## TODO

### The alter table statement
### The truncate table statement

### The create index statement
### The alternate index statement
### The drop index statement

### The create function statement
### The alternate function statement
### The drop function statement

### The begin transaction statement
### The commit transaction statement
### The rollback transaction statement

### The try statement
