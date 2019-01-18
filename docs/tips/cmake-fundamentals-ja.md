# CMake fundamentals

## CMake とは

* メタビルドシステム
  * 実際のビルドは下位のビルドシステムが行う
    * GNU Make - https://www.gnu.org/software/make/
    * Ninja - https://ninja-build.org/
    * ...
* 基本的にC/C++向け
  * コンパイル単位 (`*.{c,cpp}`) ごとにビルドの依存関係がないものが主な対象
    * Javaはコンパイル単位間に依存関係があるため、この方法だと効率が悪い

## CMake の考え方

* ビルドするライブラリや実行可能ファイルを「ターゲット」として登録し、その単位でビルド
  * `make <target>` のようにターゲット名で個々のアーティファクトをビルド可能
  * ターゲット間の依存関係をもとにビルド順序を決定
    * 例: ターゲットAの実行可能形式は、ターゲットBの共有ライブラリをリンクする
* ターゲットに対して様々な設定を行う
  * ソースコードの一覧
  * インクルードディレクトリの位置
  * リンクするターゲット
    * リンク先のライブラリや、設定などを推移的に引き継ぐことができる
  * コンパイルオプション
* 環境にインストールされたライブラリを「ターゲット」として取り込んで利用する
  * `find_package()` という機能で、自分が登録したターゲットのように利用可能になる
    * ただし、 `find_package()` に未対応のライブラリは手動で「ターゲット」として登録する作業が必要
    * CMakeで作成したプロジェクトの多くは `find_package()` に対応しているが、そこまで多くない
    * CMake以外で作成したプロジェクトでも、いくつかの有名なライブラリは CMake本体側で `find_package()` に対応している

## 用語

* プロジェクト
  * ビルド対象のプログラム一式
* サブプロジェクト
  * プロジェクトに含まれる成果物の構成単位
* サンプルプロジェクト
  * プロジェクトに含まれ、プロジェクトの成果物の利用例を示したプログラム一式の構成単位
* ライブラリ
  * `*.a` や `*.so` などの、コンパイル済みの成果物
* 実行可能形式
  * 実行可能なコンパイル済みの成果物
* ターゲット
  * CMake におけるビルドの構成単位
    * https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#binary-targets
    * ライブラリや実行可能形式が該当する
  * または、ビルドの構成単位ではないが、他のターゲットの依存先として指定することでビルドを制御する擬似的なターゲットも存在する
    * https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#pseudo-targets
    * インストール済みのライブラリや、コンパイラの設定のみを行うインターフェースライブラリなどが存在する
* パッケージ
  * 一連のライブラリや実行可能形式、およびそれに関連するヘッダファイルなどの不可分なインストール単位
    * https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html#using-packages
* 外部パッケージ
  * 環境にインストール済みのパッケージ
* 外部ターゲット
  * 外部パッケージに含まれるターゲット
* 外部ライブラリ
  * 外部パッケージに含まれるライブラリ

## CMake プロジェクトの構成例

```
./
|-- CMakeLists.txt                # (1) project cmake script
|-- <sub-project>/                # (2) individual sub-project directories
|   |-- CMakeLists.txt            # (3) cmake script for sub-project
|   |-- include/                  # (4) include directory
|   |   +-- **/*.h
|   |-- src/                      # (5) source directory
|   |   |-- CMakeLists.txt        # (6) cmake script for sources
|   |   +-- **/*.{cpp,h}
|   |-- test(s)/                  # (7) tests directory
|   |   |-- CMakeLists.txt        # (8) cmake script for tests
|   |   +-- **/*.{cpp,h}
|   +-- examples/                 # (9) example projects directory
|       |-- CMakeLists.txt        # (10) cmake script for example project
|       |-- <example-project>/    # (11) individual example projects
|       |   |-- CMakeLists.txt    # (12) cmake script for example project
|       |   +-- ...
|       +-- ...
|-- third_party/                  # (13) third party projects directory
|   |-- CMakeLists.txt            # (14) cmake script for third party projects
|   |-- <third-party-project>/    # (15) individual third party project directories
|   +-- ...
+-- cmake/                        # (16) custom cmake modules
    +-- **.cmake
```

1. project cmake script
   * プロジェクト全体に関する設定を行うスクリプト
     * CMakeのバージョン (`cmake_minimum_requirements()`)
     * プロジェクト名 (`project()`)
     * 外部パッケージの取り込み (`find_package()`)
     * パッケージングに関する設定 (`install(EXPORT)`, etc.)
     * サブディレクトリの追加 (`add_subdirectory()`)
2. individual sub-project directories
   * 公開単位の実行可能形式やライブラリのソースコード一式を含むディレクトリ
     * 外部からプロジェクトの成果物を利用する場合、このサブプロジェクト単位で実行またはリンクされることを想定する
   * 複数の公開単位や実行可能形式を構成してもよいが、基本的には一つ
     * ただし、主要成果物とは別に、テストなどの実行可能形式はいくつでも作る
   * サブプロジェクトが単一の場合、ルートディレクトリに内容をマージしてもよさそう
3. cmake script for sub-project
   * サブプロジェクトに関する設定を行うスクリプト
     * インクルードディレクトリを提供するインターフェースライブラリの登録 (`add_library(... INTERFACE)`)
       * 他のターゲットは、このライブラリをリンクすることでインクルードディレクトリを利用可能になる
       * このライブラリには実体がなく、ライブラリファイルは作成されない
    * 同じディレクトリでインストールの設定 (`install(TARGETS)`) を記述する必要がある
    * サブディレクトリの追加 (`add_subdirectory()`)
      * ただし、 `include` には `CMakeLists.txt` がないので対象外
4. include directory
   * 公開するヘッダファイルを配置するディレクトリ
     * 非公開のものは `src/` 配下に配置する
   * パス構造は名前空間と一致させておく
     * 例えば `foo::bar::MyClass` -> `include/foo/bar/MyClass.h`
   * `CMakeLists.txt` は配置しない
     * 理由はいまいちよくわからない
5. source directory
   * ソースコードを配備するディレクトリ
   * ヘッダファイルと異なり、パス配置は自由で良い
     * 常識的な範囲で
   * 非公開ヘッダファイルもここに配置する
6. cmake script for sources
   * このサブプロジェクトの成果物を作成するスクリプト
   * `add_library()` や `add_executable()` でターゲットを作成する
     * インクルードディレクトリは、 _(3) cmake script for sub-project_ で定義したインターフェースライブラリを依存先に追加することで追加できる
   * 同じディレクトリでインストールの設定 (`install(TARGETS)`) を記述する必要がある
7. tests directory
   * このサブプロジェクトに対するテストコードを配備するディレクトリ
   * テストケースは `**/*Test.cpp` に配置し、それ以外は目的に応じて `**/*TestBase.cpp` や `**/*TestUtil.cpp` のように配置する
     * 異なるルールでもいいが、最低でもテスティングペアの命名規則と、エントリかどうかは明確にしておくこと
8. cmake script for tests
   * テストのビルドと登録を行うスクリプト
   * テストコードごとから実行可能形式を作成し、CTestに追加する (`add_test()`)
9. example projects directory
   * サンプルプロジェクト群を含むディレクトリ
     * APIの利用サンプル
     * ベンチマークプログラム
     * など
10. cmake script for example projects
    * 各サンプルプロジェクトをビルド対象に追加するスクリプト
      * サブディレクトリ内のcmake scriptの追加 (`add_subdirectory()`)
11. individual example projects
    * サンプルコードを配備するディレクトリ
      * _(5) source directory_ と同等の構造でソースコードを配置する
12. cmake script for example project
    * サンプルコードをビルドするスクリプト
      * サンプルコードは基本的に実行可能形式が良さそう `add_executable()`
      * 上記でビルドした実行可能ファイルを CTestに追加する (`add_test()`) ことで、包括的なテストを行える
13. third party projects directory
    * サードパーティライブラリのプロジェクト群を含むディレクトリ
    * 事前のインストールに不適なプロジェクトのソースツリーを配置する
      * googletestなどのインストールが非推奨のもの
      * 変更が活発で安定していないプロジェクト
14. cmake script for third party projects
    * 各サードパーティプロジェクトをビルド対象に追加するスクリプト
      * _(10) cmake script for example project_ と類似しているが、不要なターゲットの実行を抑制するため、基本的には `add_subdirectory(dir-name EXCLUDE_FROM_ALL)` を指定する
      * または、バージョン管理のためだけに配置し、 `add_subdirectory()` 自体を行わない
15. individual third party project directories
    * サードパーティプロジェクトのコピー
      * バージョン管理のため、基本的にはソースコードリポジトリから特定のスナップショットをそのまま保持する
16. custom cmake modules
    * カスタムのCMakeモジュールを配置するディレクトリ
    * 主に以下のようなモジュールを配置
      * CMake非対応のモジュールに対して `find_package()` を行うための `FindXxx.cmake`
      * 共通のコンパイルオプションを設定する関数群
      * 別プロジェクトからコピーしたモジュール

## Tips

### お作法

* `CMakeLists.txt` はそのディレクトリの下位のファイルにのみアクセスする
  * 原理主義的には直下のファイルのみを参照し、サブディレクトリのファイルは `add_subdirectory()` 経由でしか参照しないという方式もあるっぽい
* 常に「ターゲットに対して設定」を心がける
  * CMake 2.x ではインクルードディレクトリやライブラリパスなどをプロジェクト全体に対して設定していた
  * CMake 3.x からは個々のライブラリや実行可能形式ファイルに対して設定を行うようになった
  * 2.xの書き方ではターゲット間の推移的依存関係の解決などが非常に面倒なので、3.xの書き方を徹底すること
* ターゲット間の依存関係は、極力 `target_link_libraries` で解決するようにする
  * 依存先のターゲットが公開しているコンパイラの設定を引き継いでくれる
    * 推移的なリンクターゲット
    * インクルードディレクトリ
    * など
  * ただし、推移的に依存する外部パッケージの探索は行ってくれない
    * `find_package()` は別途明示し、外部パッケージの取り込みを行う必要がある
      * `find_package()` を行わないと、「未解決のターゲット」としてリンクされてしまう
      * 逆に、 `find_package()` を行えば外部ターゲットとして処理される

### ルートの cmake script

* プロジェクトルートに配置した `CMakeLists.txt` はだいたいワンパターンな構成

  ```cmake
  # 先頭で CMake の最低バージョンを書く
  # 最低でも3.1
  cmake_minimum_required(VERSION 3.5)

  # プロジェクトの定義
  project(my-project
      VERSION 0.0.1
      DESCRIPTION "project description"
      LANGUAGES CXX)

  # ビルドオプションを定義
  option(BUILD_TESTS "Build test programs" ON)
  option(BUILD_EXAMPLES "Build example programs" ON)

  # cmake/*.cmake を利用可能にする
  list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

  # 必要な外部パッケージを取り込む
  # find_package(... REQUIRED)

  # 利用するCMakeモジュールを取り込む
  include(GNUInstallDirs)
  # include(...)

  # テストを有効にする
  # enable_testing() はルートでのみ指定可能
  if (BUILD_TESTS OR BUILD_EXAMPLES)
      enable_testing()
  endif()

  # サブディレクトリの取り込み
  # add_subdirectory(...)
  add_subdirectory(third_party)

  # *-config.cmake をインストール
  install(
      EXPORT my-project
      FILE my-project-config.cmake
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/my-project
      EXPORT_LINK_INTERFACE_LIBRARIES
  )
  ```

* `project()` の定義は `cmake` コマンドの対象になる `CMakeLists.txt` のみに付与する
  * たとえば、サブプロジェクトが単独でビルド可能であれば、そちらも `project()` を **付けてもよい**
  * `find_package()` で取り込んだ外部ターゲットは、現在の `project()` でのみ有効
    * `add_library()` で定義したターゲットは `project()` をまたがっても利用可能
  * いくつかの変数は `project()` スコープに存在する？
    * 変数 `PROJECT_{SOURCE,BINARY}_DIR` は現在の `project()` を記述したディレクトリに影響される

### インクルードディレクトリの指定

* ディレクトリの構成上、 `src` ディレクトリが `../include` ディレクトリを見に行くのは作法としてよろしくない
* そのため、 _(3) cmake script for sub-project_ の中でインターフェースライブラリを経由してインクルードディレクトリを渡す

  ```cmake
  # インターフェースライブラリの登録
  add_library([sub-project-name]-interface INTERFACE)

  # インクルードディレクトリの登録
  target_include_directories([sub-project-name]-interface
      INTERFACE
          $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
          $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/[sub-project-name]>
  )
  ```

* このインターフェースライブラリに指定したヘッダファイルは、インストールの対象となる
  * そのため、 `target_include_directories` はかなり面倒な記法になる
    * `$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>`
      * ビルド時のインクルードディレクトリ
      * ソースツリー上のパスを指定するため、絶対パスを使える
    * `$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/[sub-project-name]>`
      * インストールされたライブラリのインクルードディレクトリ
      * インストールプレフィックスからの相対パスを指定する
        * `${CMAKE_INSTALL_INCLUDEDIR}` は `include(GNUInstallDirs)` すると使えるようになる
        * 末尾の `[sub-project-name]` は趣味なのでなくてもよい (アンインストールが楽そう、というだけ)

### ライブラリの作成

* `add_library()` でライブラリを追加する
  * https://cmake.org/cmake/help/latest/command/add_library.html
  * 中間成果物に近いサブプロジェクトの場合、 `add_library(... STATIC ...)` を明示的に指定
    * 「絶対に共有オブジェクトにしない」という場合のみ
  * 最終成果物の場合、 `STATIC` も `SHARED` も指定しない
    * `cmake ... -DBUILD_SHARED_LIBS=ON` を指定した場合のみ `SHARED` が生成されるようになる
    * 「絶対に共有オブジェクトにする」という前提があれば `SHARED` を指定する
  * ソースセットの指定はIDEサポートが有るなら個々のファイルを指定したほうがいい
    * 面倒なので `file(GLOB ...)` を使ってしまうことが多いが、推奨は個別指定らしい
* `target_include_directories()` で追加のインクルードディレクトリを指定
  * 公開用のインクルードディレクトリはインターフェースライブラリで定義しているはずなので、ここでは指定しなくてよい
    * 追加分は `PRIVATE` でディレクトリを指定
  * 外部ライブラリのインクルードディレクトリは、 `target_link_libraries()` 経由で推移的に引っ張ってくる
    * ここにファイルパスを直接指定する場合、 `PRIVATE` にすること
    * `PRIVATE` にできないならば、「外部ライブラリの利用」を参考に `find_package()` 経由で利用可能にする
    * `include_directories()` は使わない
  * 非公開ヘッダーファイルは `*.cpp` と同じ位置に配置するのが良さそう
* `target_link_libraries()` で依存ライブラリを追加
  * 追加するライブラリは基本的に「ターゲット」であることが望ましい
    * `add_library(<target-name>)` で作成されたものか、または `find_package()` で取り込んだターゲット
    * 何らかの理由でライブラリファイルを直接指定する場合、最低でも `PRIVATE` にしておいてリークしないように
      * Macの `CoreFoundation` のようなグローバルなものであれば別にいいかも？
  * 前述のインターフェースライブラリもここに登録する

### ターゲットの命名規則

* 基本的にはサブプロジェクト名と同じにする
  * 特別な理由がない限り、トップレベルプロジェクト名をprefixとして付与しない
    * プロジェクト内で unique な名前であれば良い
    * これは、後述の `install(EXPORT)` 時に名前空間を指定してやることで、外部から利用する際にglobally uniqueな名前 をつけることができる
  * `SHARED` ライブラリになる可能性がある (`add_library(... STATIC ...)` と明示していない) なら、 `OUTPUT_NAME` プロパティを付与しておく
* テストケースは `<sub-project>-<test-name>` など
* サンプルプロジェクトは `<sub-project>-<example-project>` など

### コンパイルオプション

* コンパイルオプションは可能な限り特定のコンパイラに依存させない
  * `target_compile_options()` を使わない
    * `CMAKE_CXX_COMPILE_OPTIONS` や `compile_options()` も同様
  * ただし、警告系だけはどうしようもなかった
* できるだけCMake側が用意するプロパティで設定する
  * https://cmake.org/cmake/help/latest/manual/cmake-properties.7.html#target-properties

  ```cmake
  set_target_properties([target-name]
      PROPERTIES
          CXX_STANDARD_REQUIRED ON
          CXX_STANDARD 11
          CXX_EXTENSIONS OFF
  )
  ```

* 警告系はコンパイラごとに `target_compile_options()`
  * `PRIVATE` で指定し、依存先にリークさせないこと

  ```cmake
  if(MSVC)
      target_compile_options([target-name]
          PRIVATE /W3 /WX)
  else()
      target_compile_options([target-name]
          PRIVATE -Wall -Wextra -Werror)
  endif()
  ```

* コンパイル定義 (`-D`) を利用する場合、 `target_compile_definitions()` を利用する
  * https://cmake.org/cmake/help/latest/command/target_compile_definitions.html
  * `PRIVATE` の場合は好きに付けて良い
  * `PUBLIC` の場合は、公開ヘッダファイルに関することなので「インターフェースライブラリ」側に指定すべき
    * その場合、 `INTERFACE` を指定する

### リンク時のキーワード

* `target_link_libraries()` では、リンクするターゲットを指定する際に `PUBLIC`, `PRIVATE`, `INTERFACE` のいずれかを選択できる
  * リンクするターゲットが持つライブラリファイル、インクルードディレクトリ、定義情報 (`-D...`)、コンパイルオプションなどをどのように扱うかを規定する
* キーワードごとの挙動
  * `target_link_libraries(A PUBLIC B)`
    * `A` は `B` のライブラリファイルをリンク **する**
    * `A` をリンクするターゲットは、 `B` のライブラリファイルをリンク **する** (推移的)
  * `target_link_libraries(A PRIVATE B)`
    * `A` は `B` のライブラリファイルをリンク **する**
    * `A` をリンクするターゲットは、 `B` のライブラリファイルをリンク **しない**
      * 静的ライブラリの場合は推移的にリンク **する**
  * `target_link_libraries(A INTERFACE B)`
    * `A` は `B` のライブラリファイルをリンク **しない**
      * `A` のヘッダーファイルのみから `B` を参照し、かつそのヘッダーファイルがコンパイル単位から参照されていない場合などに利用可能
    * `A` をリンクするターゲットは、 `B` のライブラリファイルをリンク **する** (推移的)
* 決め方
  * 基本は `PRIVATE` でリンク
  * 公開ヘッダーファイルが、リンク先のヘッダーファイルを参照している場合 `PUBLIC`
  * `INTERFACE` は条件が厳しいので、行けそうだと思ったときに試してみる程度

### インストール

* `install(TARGETS)` でライブラリや実行可能ファイルをインストールできる
  * https://cmake.org/cmake/help/latest/command/install.html#installing-targets
  ```cmake
  install(
      TARGETS
          [target-name]
      EXPORT
          [package-name]
      LIBRARY
          DESTINATION ${CMAKE_INSTALL_LIBDIR}
          COMPONENT Runtime
      ARCHIVE
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/[package-name]
          COMPONENT Development
      RUNTIME
          DESTINATION ${CMAKE_INSTALL_BINDIR}
          COMPONENT Runtime
  )
  ```
  * `${CMAKE_INSTALL_*}` は `include(GNUInstallDirs)` をすると使えるようになる
  * インストールするライブラリが他のライブラリをリンクしている場合:
    * それがプロジェクト内で定義したターゲットの場合、それらも `install(TARGETS)` でインストールする必要がある
    * それが外部ターゲットの場合、 `add_target_libraries(...)` に指定したターゲットの名前だけが記録される
      * つまり、利用側でも推移的に `find_package()` をする必要がある
      * この辺を解決するには、自分で `Find<Package>.cmake` を書く必要がありそう？
* インターフェースライブラリのヘッダーファイルをインストールするには、 `install(DIRECTORY)` をわざわざ書く
  * https://cmake.org/cmake/help/latest/command/install.html#installing-directories
  ```cmake
  # インターフェースライブラリ自体もインストール
  install(
      TARGETS [target-name]
      EXPORT [package-name]
  )
  install(
      DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/[sub-project-name]
      COMPONENT Development
      PATTERN "doxygen.h" EXCLUDE
  )
  ```
  * `DIRECTORY` には `target_include_directories()` の `$<BUILD_INTERFACE:...>` に指定したパスを書く
  * `DESTINATION` には `target_include_directories()` の `$<INSTALL_INTERFACE:...>` に指定したパスを書く
  * `DIRECTORY .../` の部分には、必ず末尾に `/` を付ける
    * 付いているとディレクトリエントリをコピー
    * 付いていないとディレクトリ自体をコピー
  * 未だにこの操作がなぜ必要なのかわかっていない
  * 各種パスは `INTERFACE_INCLUDE_DIRECTORIES` から気合で解析できるので、既存のCMakeモジュールを参照のこと
* `install(EXPORT)` を行うことで、 `find_package([package-name])` ができるようになる
  * https://cmake.org/cmake/help/latest/command/install.html#installing-exports
  ```cmake
  install(
      EXPORT [package-name]
      NAMESPACE [package-name]-
      FILE [package-name]-config.cmake
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/[package-name]
      EXPORT_LINK_INTERFACE_LIBRARIES
  )
  ```
  * ターゲット名をglobal uniqueにするために、 `NAMESPACE` でパッケージ名を接頭辞に付与する
    * すでにすべてのターゲット名がglobal uniqueなら省略しても良い
    * 外部からは、上記の例では `[package-name]-[target-name]` という名前のターゲットとして利用可能になる
  * これは `[package-name]` ごとに一度だけ実行すればよいので、 _(1) project cmake script_ に記載するのが妥当か

### custom cmake module の利用

* _(15) custom cmake modules_ は `include(<module-name>)` で取り込める
* CMakeプロジェクトごとに一度だけ取り込めばいいので、 _(1) project cmake script_ が集中して取り込むのが良さそう
  * このあたりは好み

  ```cmake
  # ./cmake/*.cmake を取り込めるようにする
  list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

  # ./cmake/MyCompileOptions.cmake を取り込む
  include(MyCompileOptions)
  ```

### 外部ライブラリの利用

* 外部ライブラリは、 `find_package(<package-name>)` で取り込める
  * `add_library()` で作成したターゲットがそのまま使える
    * ただし、推移的なパッケージのロードはしてくれないようなので、推移的に `find_package()` で必要なパッケージを取り込んでやる必要がある
  * ただし、 CMake対応のプロジェクトのみ
    * `install(EXPORT)` したプロジェクトはこの方法で取り込める
  * 非標準のパスにインストールされている場合については「非標準パスの利用」を参照
    * `find_package()` 経由で外部ライブラリをルックアップするようにしておくことで、非標準パスも透過的に利用可能
* 外部ライブラリが `find_package(<package-name>)` で取り込めない場合、 `./cmake/Find<package-name>.cmake` に取り込むスクリプトを作成する
  * 作成すると、 `find_package(<package-name>)` で探せるようになる
    * これを作成しないと、依存先のプロジェクトでも同じようなコードを書く必要が出てくる
  * 基本的には `install(EXPORT)` で作成した `*-config.cmake` と同等の動きをするのが良さそう

  ```cmake
  # ./cmake/FindMyPackage.cmake
  if(TARGET [target-name])
      return()
  endif()

  find_library(MyPackage_LIBRARY_FILE NAMES [target-name])
  find_path(MyPackage_INCLUDE_DIR NAMES path/to/header.h)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(MyPackage DEFAULT_MSG
      MyPackage_LIBRARY_FILE
      MyPackage_INCLUDE_DIR)

  if(MyPackage_LIBRARY_FILE AND MyPackage_INCLUDE_DIR)
      set(MyPackage_FOUND ON)
      add_library([target-name] SHARED IMPORTED)
      set_target_properties([target-name] PROPERTIES
          IMPORTED_LOCATION "${MyPackage_LIBRARY_FILE}"
          INTERFACE_INCLUDE_DIRECTORIES "${MyPackage_INCLUDE_DIR}")
  else()
      set(MyPackage_FOUND OFF)
  endif()

  unset(MyPackage_LIBRARY_FILE CACHE)
  unset(MyPackage_INCLUDE_DIR CACHE)
  ```

  * https://cmake.org/cmake/help/latest/command/find_package.html
  * https://cmake.org/cmake/help/latest/command/find_library.html
  * https://cmake.org/cmake/help/latest/command/find_path.html

* _(1) project cmake script_ が集中して `find_package()` を行うのが良さそう
  * 依存ライブラリの一覧が明確になるため
  * `./cmake/FindXxx.cmake` を使いやすいため
  * とはいえ、好みの問題か

### 非標準パスの利用

* 非標準パス上の外部ライブラリを利用する場合、 `cmake` コマンドのオプションに `-DCMAKE_PREFIX_PATH=...` を指定する
  * このとき指定するパスは、外部ライブラリのディレクトリ (`/path/to/lib`) ではなく、インストール時のプレフィックス (`/usr/local` など)
    * ヘッダーファイルのパスもここから探索する
  * このパスは `;` 区切りで複数指定可能 (Unix系でも `;` を使う)
  * https://cmake.org/cmake/help/latest/variable/CMAKE_PREFIX_PATH.html
* 非標準パスにパッケージをインストールする場合、 `-DCMAKE_INSTALL_PREFIX=...` を指定する
  * これも `-DCMAKE_PREFIX_PATH=...` と同様にインストールプレフィックスを指定する
    * なお、 `-DCMAKE_INSTALL_PREFIX=...` の指定だけでも当該パスから外部ライブラリを探索してくれるが、 `-DCMAKE_PREFIX_PATH=...` よりも探索の優先度が低い
  * https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html
* 非標準パス上のライブラリを利用するパッケージをインストールする場合、そのままでは `RUNPATH` が設定されないためリンクエラーになる
  * `-DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON` は「CMakeの探索パスにないライブラリをリンクした場合、 `RUNPATH` を設定する」というものであり、 `-DCMAKE_PREFIX_PATH=...`, `-DCMAKE_INSTALL_PREFIX=...` のいずれも探索パスを追加するためこれだけではうまくいかない (と思われる)
  * 以下のターゲットに対する設定で `INSTALL_RPATH` や `INSTALL_RPATH_USE_LINK_PATH` を指定することで `RUNPATH` を追加できる

    ```
    option(FORCE_INSTALL_RPATH "force add library directories to runpath of installed libraries/executables" OFF)

    # ...

    # Add INSTALL_RPATH from CMAKE_INSTALL_PREFIX and CMAKE_PREFIX_PATH
    # The default behavior of CMake omits RUNPATH if it is already in CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES.
    if (FORCE_INSTALL_RPATH)
        get_target_property(target_type [target-name] TYPE)
        if (target_type STREQUAL "SHARED_LIBRARY"
                OR target_type STREQUAL "EXECUTABLE")
            get_target_property(rpath [target-name] INSTALL_RPATH)

            # add ${CMAKE_INSTALL_PREFIX}/lib if it is not in system link directories
            get_filename_component(p "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" ABSOLUTE)
            list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${p}" is_system)
            if (is_system STREQUAL "-1")
                list(APPEND rpath "${p}")
            endif()

            # add each ${CMAKE_PREFIX_PATH}/lib
            foreach (p IN LISTS CMAKE_PREFIX_PATH)
                get_filename_component(p "${p}/${CMAKE_INSTALL_LIBDIR}" ABSOLUTE)
                list(APPEND rpath "${p}")
            endforeach()

            if (rpath)
                set_target_properties([target-name] PROPERTIES
                    INSTALL_RPATH "${rpath}")
            endif()
            set_target_properties([target-name] PROPERTIES
                INSTALL_RPATH_USE_LINK_PATH ON)
        endif()
    endif (FORCE_INSTALL_RPATH)
    ```

    * https://cmake.org/cmake/help/latest/prop_tgt/INSTALL_RPATH.html
    * https://cmake.org/cmake/help/latest/prop_tgt/INSTALL_RPATH_USE_LINK_PATH.html
    * https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_IMPLICIT_LINK_DIRECTORIES.html
  * 上記と同様の挙動をターゲットベースで行わない場合、以下のように書く
    ```
    option(FORCE_INSTALL_RPATH "force add library directories to runpath of installed libraries/executables" OFF)
    include(GNUInstallDirs)
    if (FORCE_INSTALL_RPATH)
        # add ${CMAKE_INSTALL_PREFIX}/lib if it is not in system link directories
        get_filename_component(p "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" ABSOLUTE)
        list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${p}" is_system)
        if (is_system STREQUAL "-1")
            list(APPEND CMAKE_INSTALL_RPATH "${p}")
        endif()
        # add each ${CMAKE_PREFIX_PATH}/lib
        foreach (p IN LISTS CMAKE_PREFIX_PATH)
            get_filename_component(p "${p}/${CMAKE_INSTALL_LIBDIR}" ABSOLUTE)
            list(APPEND CMAKE_INSTALL_RPATH "${p}")
        endforeach()
        # add other than */lib
        set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
    endif (FORCE_INSTALL_RPATH)
    ```

    * https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_RPATH.html
    * https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_RPATH_USE_LINK_PATH.html
  * または、 `-DCMAKE_INSTALL_RPATH=...` や `-DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON` をコマンドラインから指定することで、 `RUNPATH` を設定可能
    * `-DCMAKE_INSTALL_RPATH=...` のパスは `;` 区切りで複数指定可能 (Unix系でも `;` を使う)

### テストの作成

* CTest (または `make test`) は特定のコマンドを実行してその exit code を検査する
  * 対象のコマンドは `add_test(NAME ... COMMAND ...)` で登録する
* そのため、 `add_executable()` で実行可能形式を作成し、それを `add_test()` で登録することで CTest によるテストが可能になる

  ```cmake
  add_executable([test-name] [TEST-SOURCES ...])
  add_test(NAME [test-name] COMMAND [test-name] [test-args ...])
  ```

  * https://cmake.org/cmake/help/latest/command/add_test.html

* 細粒度の単体テストを作成する場合、後述の Google Test を利用するのがよさそう
* _(11) individual example projects_ で作成した実行可能形式も同様にテストに追加するとよい
  * 引数を利用するプログラムにしておけば、複数のテストも実行できる

### Google Testの利用

* 単体テストを書くには [Google Test](https://github.com/google/googletest.git) が便利だが、CMakeから利用するには、通常の外部ライブラリとは異なる手順が必要
  1. `third_party/` 配下に Google Test のソースツリーをチェックアウト
  2. `third_party/` から `EXCLUDE_FROM_ALL` 付きでソースツリーを指定
  3. テストコードのターゲット (`add_executable()`) に対して以下をリンクに追加 `target_link_libraries`
     * `gtest_main`
       * `main` 関数を含むため、テストコードでは `main` を定義しない
       * 自分で `main` を用意する場合は変わりに `gtest` ターゲットをリンクに追加
     * `Threads::Threads` (先に `find_package(Thread REQUIRED)` しておく)
* 上記でビルドした実行可能形式は、テスト失敗時に non-zero な exit code を返すようにいなっているので、CTestで実行可能
* `gtest_main` をリンクした実行可能形式は、環境変数 `GTEST_OUTPUT` やコマンドラインオプション `--gtest_output` でレポートの出力先を制御できる
  * https://github.com/google/googletest/blob/release-1.8.0/googletest/docs/AdvancedGuide.md#generating-an-xml-report
  * プロジェクトに対して複数のテストターゲットを登録している場合、名前が衝突すると上書きされるので `add_test` に出力先を指定しておく

    ```cmake
    add_test(
        NAME [test-name]
        COMMAND [test-name] "--gtest_output=xml:[test-name]_gtest_result.xml")
    ```
