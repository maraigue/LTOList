# LTOList

## 概要

LTOList (Logarithmic-Time-Operation List) は二分探索木を用いた線形リスト（値を一列に並べるリスト）です。このコードはLTOListをC++で実装したものです。

線形リストの有名なものとしては可変長配列（C++における `std::vector` が例）や連結リスト（C++における `std::list` が例）が挙げられます。これらの操作に要する計算時間は以下の通りです。（n: リストのサイズ）
-   可変長配列: ランダムアクセスは最悪 O(1) 時間、要素の追加・削除は最悪 O(n) 時間
-   連結リスト: ランダムアクセスは最悪 O(n) 時間、要素の追加・削除は最悪 O(1) 時間

ここで疑問となるのは、この中間となるものが実現できるか、ということです。LTOList はそれを実現するもので、ランダムアクセスも要素の追加・削除も O(log n) 時間で行えます。

LTOListは、二分探索木の各ノードに、その子要素がいくつあるかを保持するという設計をしています。また「赤黒木」を一から実装しています。

### Tell me if you know ...

Such a data structure seems to be known to many others, but I could not find a specific name.
-   Example 1: [https://kaiseh.hatenadiary.org/entry/20071231/1199122020](https://kaiseh.hatenadiary.org/entry/20071231/1199122020); in Japanese, implemented by a "skip list" instead of BST
-   Example 2: [https://logfiles.hatenablog.com/entry/2016/12/02/103200](https://logfiles.hatenablog.com/entry/2016/12/02/103200); in Japanese, implemented by a "Treap" (BST with randomness)

If you know a specific name, please tell me.

### Caveats

Since O(log n) time is achieved by tree operations (namely, pointer operations), in reality insertions and deletions may be slower than those for VLA that theoretically requires O(n) time but much more memory-efficient.

The implementation of LTOList is based on BST, but more memory-consuming than ordinary BST that orders elements automatically: ordinary BST requires three pointers for each list element, while LTOList requires five.

### Answers to anticipated questions

-   Why a "red-black tree" is implemented from scratch? Can it be implemented by C++-standard BST-based structures, for example, `std::map`?
    -   No. The "balancing", an essential operation of BST that assures the tree height being O(log n), of `std::map` is based on the "total order" of elements. LTOList must use a different order from the total order to balance the tree; This is why LTOList cannot be implemented by `std::map` or the like.

## Implementation details

How do we achieve O(log n) time for all of a random access, an insertion and a deletion in the worst case?

First suppose that, for each node in BST, we write the index of the element. However, when we insert or delete an element, the updates of the indices require O(n) time, as an insertion or deletion for VLA requires.

To avoid this situation, for each node in BST, we write the numbers of left and right children. Then, when we insert or delete an element, the updates of the numbers require only O(log n) time; it is sufficient to update the numbers for only the nodes from the inserted or deleted nodes to the root node.

<img src="figure/BST_and_Indices.png" style="width:60%" alt="If we write indices (red) to the nodes, when we insert an element, all indices after it must be updated!">

<img src="figure/BST_and_NumChildren.png" style="width:60%" alt="If we write numbers of left/right children (red) to the nodes, when we insert an element, only the parents of it are updated!">

## Usage

LTOTree has similar APIs to those of `std::vector`; almost all APIs are implemented except for memory allocations (e.g., `std::vector<T>::reserve`).

A large difference from `std::vector` is that, since LTOTree is implemented as a BST, given an iterator we can insert another element either before or after the iterator.

```
LTOList<double> ll;
LTOList<double>::iterator iter = ...;
ll.insert(iter, 3.5); // insert 3.5 before 'iter' (to be consistent with 'std::vector<T>::insert')
ll.insert(0, iter, 2.5); // insert 2.5 before 'iter'
ll.insert(1, iter, 4.5); // insert 4.5 after 'iter'
```

## Examples

You can build examples by the following command (example for `LTOList_example_poppush.cpp`, using GCC):

```
$ g++ LTOList_example_poppush.cpp
```

You can also create a `Makefile` that builds examples by [CMake](https://cmake.org/): if you have CMake, the following command will generate a `Makefile` for all examples:

```
$ cmake .

(if you are at the directory where CMakeLists.txt locates)
```

## Document

The document can be generated with [Doxygen](http://www.doxygen.nl/). (Not all APIs are documented yet.)

The generated document is also available at [https://hhiro.net/ltolist/doxygen/](https://hhiro.net/ltolist/doxygen/) .

## Author

H.Hiro

- e-mail: [main@hhiro.net](mailto:main@hhiro.net)
- Website: [https://hhiro.net/](https://hhiro.net/)
- GitHub: [https://github.com/maraigue](https://github.com/maraigue)

Published under the MIT license. See LICENSE.txt for details.
