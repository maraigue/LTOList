# LTOListのインストール

LTOListは、ファイルがヘッダーファイルの `LTOList.hpp` 1つだけからなるライブラリです。お好きなディレクトリに置いたうえで、 `#include "path/to/LTOList.hpp"` の形で読み込んでください。

**`CMakeLists.txt` のファイルはありますが、LTOListを利用するだけなら `cmake` は不要です。**

# サンプルやテストを実行する

## 準備

サンプルやテストは `cmake` (https://cmake.org/) を使ってビルドします。LTOListのあるディレクトリにて以下のコマンドを利用すると、 `Makefile` が生成されます。

```
cmake .
```

## サンプルをビルドする

サンプルは `LTOList_example_*.cpp` というファイル名で作られています。

`cmake` を使って上記の通り `Makefile` が生成されましたら、サンプルをビルドするのは、 `make [ExampleName]` というコマンドで行えます。ただし `[ExampleName]` はサンプルのファイル名から拡張子 `.cpp` を取り除いたものです。

例えば `LTOList_example_performance.cpp` をビルドするには、以下のコマンドを使うと実行ファイルが生成されます：

```
make LTOList_example_performance
```

## テストをビルドする

テストは `LTOList_test_*.cpp` というファイル名で作られています。

まず、これらのテストは Catch2 (https://github.com/catchorg/Catch2) を用いて作られています。ビルドの前に、 https://github.com/catchorg/Catch2/tree/master/single_include にある `catch2` というディレクトリを、中身も含めて `CMakeLists.txt` と同じディレクトリに置きます (注意： `catch2` というディレクトリが `CMakeLists.txt` と同じディレクトリに置かれるのであって、 `catch2` の中身が `CMakeLists.txt` と同じディレクトリに置かれるのではありません)。

そして、 `cmake` を使って上記の通り `Makefile` が生成されましたら、テストをビルドするのは `make [TestName]` というコマンドで行えます。ただし `[TestName]` はテストのファイル名から拡張子 `.cpp` を取り除いたものです。

また、コマンド `make test_[TestName]` を使うと、テストの実行ファイルを生成して (もし存在しなければ) から実行もします。さらに、コマンド `make test` を使うと、すべてのテストについてこれを行います。
