# LTOList

## 概要

LTOList (Logarithmic-Time-Operation List) は二分探索木を用いた線形リスト（値を一列に並べるリスト）です。このコードはLTOListをC++で実装したものです。

線形リストの有名なものとしては可変長配列（C++における `std::vector` が例）や連結リスト（C++における `std::list` が例）が挙げられます。これらの操作に要する計算時間は以下の通りです。（n: リストのサイズ）
-   可変長配列: ランダムアクセスは最悪 O(1) 時間、要素の追加・削除は最悪 O(n) 時間
-   連結リスト: ランダムアクセスは最悪 O(n) 時間、要素の追加・削除は最悪 O(1) 時間

ここで疑問となるのは、この中間となるものが実現できるか、ということです。LTOList はそれを実現するもので、ランダムアクセスも要素の追加・削除も O(log n) 時間で行えます。

LTOListは、二分探索木の各ノードに、その子要素がいくつあるかを保持するという設計をしています。また「赤黒木」を一から実装しています。

### 情報求む

このようなデータ構造は他にも考えている人は多数いるようです。ただ、何か名前が付いているのかというとそうではないようです。

-   例1: [https://kaiseh.hatenadiary.org/entry/20071231/1199122020](https://kaiseh.hatenadiary.org/entry/20071231/1199122020)（日本語記事。二分探索木ではなく「スキップリスト」で実装）
-   例2: [https://logfiles.hatenablog.com/entry/2016/12/02/103200](https://logfiles.hatenablog.com/entry/2016/12/02/103200)（日本語記事。ランダム性を用いる二分探索木である「Treap」で実装）

もし「こういう名前が付いたデータ構造である」というのをご存知の方がいらっしゃったら、お知らせください。

### 注意

O(log n) 時間を実現するのに木構造の操作（すなわち、ポインタの操作）を用いているため、実際の挿入や削除の実行時間としては、可変長配列（理論的には O(n) 時間が必要だがメモリ効率がよい）より遅くなるかもしれません。

またLTOListの二分探索木は通常の二分探索木よりもメモリを消費します。通常の二分探索木は1要素につきポインタ変数が3つ必要なのに対し、LTOListは5つ必要なためです。

### 想定される質問への回答

-   赤黒木を一から実装しているのはなぜですか？ C++の標準ライブラリで実装されている `std::map` を使って実装できたりしなかったのでしょうか？
    -   できないと判断しました。赤黒木の高さの平準化操作（木の高さを O(log n) に抑える）について、 `std::map` のそれは、要素の「全順序」（要素同士の大小関係が定められていて、その順序で並べる）でしかできないようになっています。LTOListの高さの平準化操作はそれではできないのです。

## 実装の詳細

ランダムアクセスも要素の追加・削除も、最悪ケースで O(log n) 時間で実現するにはどうすればよいのでしょうか。

まず仮に、二分探索木の各ノードに、その要素のインデックス（全体で何番目であるか）であるかを書き込むとしましょう。しかしそれでは、挿入や削除をしたときにそれ以降のインデックスを書き換えなければならず、可変長配列の場合と同様に O(n) 時間が必要となります。

これを避けるため、二分探索木の各ノードに書き込むものを、「左の子孫の数」と「右の子孫の数」とします。そうすると、挿入や削除をした際にこれらを書き換えるのに必要な計算時間は O(log n) で済みます。更新が起きた場所とその祖先のノードだけ書き換えればよいためです。

<img src="figure/BST_and_Indices.png" style="width:60%" alt="If we write indices (red) to the nodes, when we insert an element, all indices after it must be updated!">

<img src="figure/BST_and_NumChildren.png" style="width:60%" alt="If we write numbers of left/right children (red) to the nodes, when we insert an element, only the parents of it are updated!">

## 利用方法

LTOListでは `std::vector` に似たAPIを実装しています。ほとんどのAPIが実装されていますが、メモリ確保関連のAPI (`std::vector<T>::reserve` など) はLTOListには不要なため存在しません。

LTOListのAPIにおける `std::vector` との大きな違いとして、LTOListは二分探索木であるため、要素を挿入する際、イテレータの前に挿入するか後に挿入するかを選べるというものがあります。

```
LTOList<double> ll;
LTOList<double>::iterator iter = ...;
ll.insert(iter, 3.5); // insert 3.5 before 'iter' (to be consistent with 'std::vector<T>::insert')
ll.insert(0, iter, 2.5); // insert 2.5 before 'iter'
ll.insert(1, iter, 4.5); // insert 4.5 after 'iter'
```

## 例

例として用意したプログラムは、以下のコマンドでビルドできます（例として、GCCを用いて `LTOList_example_poppush.cpp` の実行ファイルを生成する場合を示します）。

```
$ g++ LTOList_example_poppush.cpp
```

また、[CMake](https://cmake.org/)が利用可能であれば、これらの例をビルドするための `Makefile` を生成することもできます。以下のコマンドで生成できます。

```
$ cmake .

(CMakeLists.txt があるディレクトリにいる場合)
```

## ドキュメント

[Doxygen](http://www.doxygen.nl/)で生成できます（すべてのAPIがドキュメント化されてはいません）。

生成されたドキュメントは [https://hhiro.net/ltolist/doxygen/](https://hhiro.net/ltolist/doxygen/) でも閲覧できます。

## 作者

H.Hiro

- メール: [main@hhiro.net](mailto:main@hhiro.net)
- ウェブサイト: [https://hhiro.net/](https://hhiro.net/)
- GitHub: [https://github.com/maraigue](https://github.com/maraigue)

MITライセンスで提供します。詳細は LICENSE.txt をご覧ください。
