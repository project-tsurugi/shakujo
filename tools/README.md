# OLTP 手元ツール系

## codegen

IR/ASTのコードを生成するやつ

### requirements

* JDK 10

### usage

```bash
cd tools/codegen
./gradlew generate
```

* 生成したファイルが下記に上書きされる
  * `<repository-root>/model/include`
  * `<repository-root>/model/src`
* 生成したファイルはライセンスヘッダー直下に以下のコメントがあるので注意
  * `// DON'T EDIT THIS GENERATED FILE`
  * これらのファイルは上書きされる

### code reading

* entry
  * `com.ashigeru.lang.codegen.spec.sql.Main`
* modules
  * `modules/common`
    * 共通ライブラリ
  * `modules/model`
    * メタモデル
  * `modules/dsl`
    * メタモデルを記述するDSL
  * `modules/cpp`
    * C++のコード生成系
  * `modules/sql`
    * IRモデル定義
    * プログラムエントリ
