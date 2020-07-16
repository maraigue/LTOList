#include <cassert>
#include <iostream>
#include <stack>
#include <type_traits>
#include <stdexcept>
#include <initializer_list>

/// \~english
/// Class for LTOList.
///
/// TODO: Allocators are not supported yet.
///
/// \~japanese
/// LTOList を表すためのクラスです。
///
/// 今後対応予定: 現時点ではアロケータには対応していません。
template<class T>
class LTOList {
public:
    /// \~english
    /// The type of elements that the LTOList can store.
    ///
    /// \~japanese
    /// LTOList が格納できる値の型です。
    using value_type = T;

    /// \~english
    /// The integer type that represents the number of elements in
    /// LTOList.
    ///
    /// \~japanese
    /// LTOList に格納されている要素数を表す整数型です。
    using size_type = size_t;

    /// \~english
    /// The integer type that represents the difference of two indices
    /// in LTOList.
    ///
    /// \~japanese
    /// LTOList 中のインデックス（要素の位置）の差を表す整数型です。
    using difference_type = ptrdiff_t;

    /// \~english
    /// The type of a reference for elements that the LTOList can
    /// store.
    ///
    /// \~japanese
    /// LTOList が格納できる値の型に対する参照型です。
    using reference = T &;

    /// \~english
    /// The type of a constant-reference for elements that the LTOList
    /// can store.
    ///
    /// \~japanese
    /// LTOList が格納できる値の型に対する const 参照型です。
    using const_reference = const T &;

    /// \~english
    /// The type of a pointer for elements that the LTOList can store.
    ///
    /// \~japanese
    /// LTOList が格納できる値の型に対するポインタ型です。
    using pointer = T *;

    /// \~english
    /// The type of a constant-pointer for elements that the LTOList
    /// can store.
    ///
    /// \~japanese
    /// LTOList が格納できる値の型に対する const ポインタ型です。
    using const_pointer = const T *;

    /// \cond
    // Node is an internal class and therefore not to be documented

    struct Node;
    using pNode = Node *;

    struct Node {
        pNode parent;
        pNode child[2];
        size_type n_children[2];
        bool red; // as red-black tree
        value_type value;

        // Constructors
        Node()
            : parent(nullptr), child{ nullptr, nullptr },
              n_children{ 0, 0 }, red(false), value() {}
        Node(bool is_red)
            : parent(nullptr), child{ nullptr, nullptr },
              n_children{ 0, 0 }, red(is_red), value() {}
        Node(bool is_red, const_reference v)
            : parent(nullptr), child{ nullptr, nullptr },
              n_children{ 0, 0 }, red(is_red), value(v) {}
        Node(bool is_red, T && v)
            : parent(nullptr), child{ nullptr, nullptr },
              n_children{ 0, 0 }, red(is_red), value(std::move(v)) {}
        template<class... Args>
        Node(bool is_red, Args... args)
            : parent(nullptr), child{ nullptr, nullptr },
              n_children{ 0, 0 }, red(is_red), value(args...) {}

        // For Base::origin (see below)
        inline pNode o_root() const { return child[1]; }
        inline pNode & o_root() { return child[1]; }
        inline pNode o_front() const { return parent; }
        inline pNode & o_front() { return parent; }
        inline pNode o_back() const { return child[0]; }
        inline pNode & o_back() { return child[0]; }

        void recompute_n_children(size_type side) {
            if(side) side = 1;

            if(child[side]) {
                n_children[side] = 1 + child[side]->n_children[0] +
                                   child[side]->n_children[1];
            } else {
                n_children[side] = 0;
            }
        }

        // Return the side of the childrens in which `ch` is located
        //
        // NOTE 1:
        // `ch` is assumed to be either child of `this`, that is,
        // this method does not return the result that `ch` is not a
        // child of `this`.
        //
        // NOTE 2:
        // However, this method must checks whether `ch` is equal to
        // `child[1]`, not `child[0]`, because of the construction of
        // `Base::origin`. See the note in `struct Base` below.
        size_type side_of_child(pNode ch) {
            return (child[1] == ch ? 1 : 0);
        }
    };
    /// \endcond
    // End of the definition of Node

    /// \cond
    // Base is an internal class and therefore not to be documented

    // The base of LTOList: all operations are done by directly
    // accessing nodes
    struct Base {
        // origin: pointer for ending iterator
        // origin->child[1] (= origin->o_root()): root node in the
        // tree origin->parent (= origin->o_front()):  leftmost
        // (smallest index) node in the tree origin->child[0] (=
        // origin->o_back()): rightmost (largest index) node in the
        // tree (To let `origin` work also as a sentinel, the root
        // node must be represented as a child of `origin`.)
        //
        // The setups above are determined for the purpose of the
        // following convenience:
        // 1. Either child of `origin` must be the root node, and
        //    `origin->side_of_child(root)` must correctly identify
        //    the side of `root` from `origin`. This is because of the
        //    convenience that the root node can have a (virtual)
        //    parent node.
        // 2. The previous element of `origin` computed by `neighbor`,
        // namely `neighbor(origin, 0)`,
        //    must be the last element that truly stores an element
        //    (i.e., other than `origin`).
        //
        // `origin->parent`, `origin->child[0]` and `origin->child[1]`
        // must be either "all of them are `nullptr`" or "none of them
        // are `nullptr`". The list is empty if and only if all of
        // them are `nullptr`.
        pNode origin;

        Base() : origin(new Node(true)) {}
        ~Base() { delete origin; }

        bool empty() const { return origin->o_root() == nullptr; }

        size_type size() const {
            return origin->o_root()
                     ? (origin->o_root()->n_children[0] +
                        origin->o_root()->n_children[1] + 1)
                     : 0;
        }

        size_type position(pNode n) {
            // Find the index (number) of node `n`

            // If `end` is given
            if(n == origin) return size();

            difference_type moved = 0; // How many `n` is moved
            while(n != origin->o_root()) {
                pNode p = n->parent;
                size_type side = p->side_of_child(n);
                if(side == 0) {
                    moved += n->n_children[1] + 1;
                } else {
                    moved -= n->n_children[0] + 1;
                }
                n = p;
            }
            moved -= n->n_children[0];
            moved = -moved;
            assert(moved >= 0);
            return static_cast<size_type>(moved);
        }

        pNode advance(pNode n, difference_type dir) {
            // Step 1: Move to parents
            size_type side = (dir > 0 ? 1 : 0);
            difference_type count = (dir > 0 ? dir : -dir);

            while(count > n->n_children[side]) {
                // Loop until there are enough children under `n`

                pNode p = n->parent;
                if(p == origin) {
                    // If reached at the root
                    return origin;
                }

                // Note that 'count > n->n_children[side]' is assumed,
                // that is, `count` cannot be negative here
                if(side == p->side_of_child(n)) {
                    count += (n->n_children[1 - side] + 1);
                } else {
                    count -= (n->n_children[side] + 1);
                }
                n = p;
            }

            // Step 2: Move to children
            while(count != 0) {
                assert(count > 0);

                pNode c = n->child[side];
                if(!c) {
                    // If no more child is found
                    return origin;
                }

                count -= (1 + c->n_children[1 - side]);
                if(count < 0) {
                    count = -count;
                    side = 1 - side;
                }
                n = c;
            }
            return n;
        }

        pNode neighbor(pNode n, size_type side) {
            if(side) side = 1;

            pNode tmp = n->child[side];
            if(tmp) {
                while(tmp->child[1 - side]) {
                    tmp = tmp->child[1 - side];
                }
                return tmp;
            } else {
                tmp = n;
                for(;;) {
                    if(tmp->parent == origin) return origin;
                    if(tmp->parent->side_of_child(tmp) == 1 - side)
                        return tmp->parent;
                    tmp = tmp->parent;
                }
            }
        }

        template<class STREAM>
        static void display_tree_main(
          STREAM & sr, const pNode n, size_type depth,
          size_type offset) {
            if(!n) return;

            display_tree_main(sr, n->child[0], depth + 1, offset);

            for(size_type i = 0; i < depth; ++i) {
                sr << "  ";
            }
            sr << '<' << (n->red ? 'R' : 'B') << ">["
               << (offset + n->n_children[0]) << " this=" << n
               << " parent=" << n->parent
               << " children=" << n->n_children[0] << ","
               << n->n_children[1] << "] " << n->value << std::endl;

            display_tree_main(
              sr, n->child[1], depth + 1,
              offset + n->n_children[0] + 1);
        }

        template<class STREAM>
        inline void display_tree(STREAM & sr) const {
            sr << "{LTOList Size=" << size() << " Origin=" << origin
               << " Root=" << origin->o_root()
               << " Front=" << origin->o_front()
               << " Back=" << origin->o_back() << "}" << std::endl;
            display_tree_main(sr, origin->o_root(), 0, 0);
        }

        pNode add(pNode pos, size_type side, const_reference v) {
            pNode newnode = new Node(true, v);
            return add_main(pos, side, newnode);
        }

        pNode add(pNode pos, size_type side, T && v) {
            pNode newnode = new Node(true, std::move(v));
            return add_main(pos, side, newnode);
        }

        template<class... Args>
        pNode add_emplace(pNode pos, size_type side, Args... args) {
            pNode newnode = new Node(true, args...);
            return add_main(pos, side, newnode);
        }

        pNode add_main(pNode pos, size_type side, pNode newnode) {
            if(side) side = 1;

            // Node addition cannot be conducted if either of the
            // followings is satisfied:
            // - "pos == nullptr" and the tree is not empty
            // - "pos == end()" and "side == 1"

            // ------------------------------------------------------------
            // If no element is in the tree
            // ------------------------------------------------------------
            if(empty()) {
                newnode->red = false;
                newnode->parent = origin;
                origin->o_root() = newnode;
                origin->o_front() = newnode;
                origin->o_back() = newnode;
                return newnode;
            }

            if(pos == origin && side == 0) {
                pos = origin->o_back();
                side = 1;
            }

            if(pos == nullptr) throw std::invalid_argument("Invalid iterator");
            if(pos == origin) throw std::invalid_argument("Elements cannot be inserted after the end of the list");

            // ------------------------------------------------------------
            // If at least one element is in the tree
            // ------------------------------------------------------------

            // If the new node becomes the new first/last element,
            // point it
            if(pos == origin->o_front() && side == 0) {
                origin->o_front() = newnode;
            } else if(pos == origin->o_back() && side == 1) {
                origin->o_back() = newnode;
            }

            // Since a node insertion must begin with a leaf to
            // conduct rotations, in case `pos->child[side]` is not
            // `nullptr`, 1) Find the `side` neighbor of `pos`,
            // denoted by `pos_ins`. 2) Insert the new node to the
            // opposite of `side`.
            pNode pos_ins;
            size_type side_ins;
            if(pos->child[side] == nullptr) {
                pos_ins = pos;
                side_ins = side;
            } else {
                pos_ins = neighbor(pos, side);
                // Since `pos->child[side]` is not `nullptr`,
                // `neighbor(pos, side)` cannot be `nullptr`
                side_ins = 1 - side;
            }
            assert(pos_ins->child[side_ins] == nullptr);
            pos_ins->child[side_ins] = newnode;
            newnode->parent = pos_ins;
            pos_ins->n_children[side_ins] = 1;

            // Rotations
            pNode rotpos[4];
            size_type rotsides[3];
            rotpos[0] = newnode;
            rotpos[1] = pos_ins;
            rotsides[0] = side_ins;
            while(rotpos[1]->red) {
                assert(rotpos[0]->red);
                if(rotpos[0] == origin->o_root()) {
                    // If reached the root
                    rotpos[0]->red = false;
                    break;
                }
                if(rotpos[1] == origin->o_root()) {
                    // If reached the root
                    rotpos[1]->red = false;
                    break;
                }

                rotpos[2] = rotpos[1]->parent;
                rotpos[3] = rotpos[2]->parent;
                rotsides[1] = rotpos[2]->side_of_child(rotpos[1]);
                rotsides[2] = rotpos[3]->side_of_child(rotpos[2]);

                if(rotsides[0] == rotsides[1]) {
                    // (The figure below is rotsides[0] == 0; invert
                    // if rotsides[0] == 1)
                    //       [3]              [3]
                    //        :                :
                    //      b[2]             r[1]
                    //       / \             /   \
                    //    r[1] [D]  -->  b[0]*   b[2]
                    //     / \            / \     / \
                    //  r[0] [C]        [A] [B] [C] [D]
                    //   / \               
                    // [A] [B]
                    //
                    // (*: repainted red to black)
                    // ([A], [B], [C] and [D] has the same 'black
                    // height')

                    // Connection 2-C
                    rotpos[2]->child[rotsides[0]] =
                      rotpos[1]->child[1 - rotsides[0]];
                    if(rotpos[2]->child[rotsides[0]])
                        rotpos[2]->child[rotsides[0]]->parent =
                          rotpos[2];
                    // Connection 1-2
                    rotpos[1]->child[1 - rotsides[0]] = rotpos[2];
                    if(rotpos[2]) rotpos[2]->parent = rotpos[1];
                    // Connection 1-3
                    rotpos[3]->child[rotsides[2]] = rotpos[1];
                    if(rotpos[1]) rotpos[1]->parent = rotpos[3];

                    rotpos[0]->red = false;

                    rotpos[2]->recompute_n_children(rotsides[0]);
                    rotpos[1]->recompute_n_children(0);
                    rotpos[1]->recompute_n_children(1);
                } else { // rotsides[0] != rotsides[1]
                    // (The figure below is rotsides[0] == 0; invert
                    // if rotsides[0] == 1)
                    //   [3]               [3]
                    //    :                 :
                    //  b[2]              r[0]
                    //   / \              /   \
                    // [A] [1]r  -->  b[2]    b[1]*
                    //     / \         / \     / \
                    //  r[0] [D]     [A] [B] [C] [D]
                    //   / \               
                    // [B] [C]
                    //
                    // (*: repainted red to black)
                    // ([A], [B], [C] and [D] has the same 'black
                    // height')

                    // Connection 1-C
                    rotpos[1]->child[rotsides[0]] =
                      rotpos[0]->child[rotsides[1]];
                    if(rotpos[0]->child[rotsides[1]])
                        rotpos[0]->child[rotsides[1]]->parent =
                          rotpos[1];
                    // Connection 2-B
                    rotpos[2]->child[rotsides[1]] =
                      rotpos[0]->child[rotsides[0]];
                    if(rotpos[0]->child[rotsides[0]])
                        rotpos[0]->child[rotsides[0]]->parent =
                          rotpos[2];
                    // Connection 0-1
                    rotpos[0]->child[rotsides[1]] = rotpos[1];
                    if(rotpos[1]) rotpos[1]->parent = rotpos[0];
                    // Connection 0-2
                    rotpos[0]->child[rotsides[0]] = rotpos[2];
                    if(rotpos[2]) rotpos[2]->parent = rotpos[0];
                    // Connection 3-0
                    rotpos[3]->child[rotsides[2]] = rotpos[0];
                    if(rotpos[0]) rotpos[0]->parent = rotpos[3];

                    rotpos[1]->red = false;

                    rotpos[2]->recompute_n_children(1 - rotsides[0]);
                    rotpos[1]->recompute_n_children(rotsides[0]);
                    rotpos[0]->recompute_n_children(0);
                    rotpos[0]->recompute_n_children(1);
                }

                // Next step
                rotpos[0] = rotpos[3]->child[rotsides[2]];
                rotpos[1] = rotpos[3];
                rotsides[0] = rotsides[2];
            }

            // Recomputation of `n_children`
            // At this point, `recpos` already has correct
            // `n_children`, but its ancestors not
            pNode recpos = rotpos[0];
            if(recpos != origin) {
                for(;;) {
                    pNode par = recpos->parent;
                    if(par == origin) break;
                    size_type recpos_side =
                      par->side_of_child(recpos);

                    par->recompute_n_children(recpos_side);
                    recpos = par;
                }
            }

            return newnode;
        }

        pNode remove(pNode pos) {
            // Remove the node

            if(pos == nullptr) throw std::invalid_argument("Invalid iterator");
            if(pos == origin) throw std::invalid_argument("The end of the list cannot be deleted");

            // In case the removed node is the only node in the tree
            if(pos == origin->o_front() && pos == origin->o_back()) {
                origin->o_root() = nullptr;
                origin->o_front() = nullptr;
                origin->o_back() = nullptr;
                delete pos;
                return origin;
            }

            pNode returned_pos = nullptr;
            pNode neighbor_pos = nullptr;

            // - returned_pos: the next element of the removed element
            // - neighbor_pos: the neighboring element if the removed
            // element has two children, or null otherwise
            //
            // Noticing that
            // - returned_pos must be always identified, and
            // - neighbor_pos must be identified only if the removed
            // element (`pos`) has two children, we process as
            // follows.
            if(pos == origin->o_back()) {
                // In case we cannot take `returned_pos` directly,
                // that is, `pos` is the last element, `pos` CANNOT
                // have two children.
                returned_pos = origin; // end()
            } else {
                returned_pos = neighbor(pos, 1);
                if(pos->child[0] && pos->child[1]) {
                    neighbor_pos = returned_pos;
                }
            }

            if(neighbor_pos) {
                // Notice that `neighbor_pos` must be descendant of
                // `pos`, since this exists only if `pos->child[1]`
                // exists.
                // TODO: Test this
                pNode pos_parent = pos->parent;
                size_t pos_parent_side =
                  pos_parent->side_of_child(pos);
                pNode pos_lc = pos->child[0];
                pNode pos_rc = pos->child[1];

                pNode neighbor_pos_parent = neighbor_pos->parent;
                size_t neighbor_pos_parent_side =
                  neighbor_pos_parent->side_of_child(neighbor_pos);
                pNode neighbor_pos_lc = neighbor_pos->child[0];
                pNode neighbor_pos_rc = neighbor_pos->child[1];

                if(neighbor_pos_parent == pos) {
                    // If two swapped nodes are neighboring
                    pos_parent->child[pos_parent_side] = neighbor_pos;
                    if(neighbor_pos_parent_side == 1) {
                        if(pos_lc) pos_lc->parent = neighbor_pos;
                    } else {
                        if(pos_rc) pos_rc->parent = neighbor_pos;
                    }
                    if(neighbor_pos_lc) neighbor_pos_lc->parent = pos;
                    if(neighbor_pos_rc) neighbor_pos_rc->parent = pos;

                    pos->parent = neighbor_pos;
                    pos->child[0] = neighbor_pos_lc;
                    pos->child[1] = neighbor_pos_rc;
                    neighbor_pos->parent = pos_parent;
                    if(neighbor_pos_parent_side == 1) {
                        neighbor_pos->child[0] = pos_lc;
                        neighbor_pos->child[1] = pos;
                    } else {
                        neighbor_pos->child[0] = pos;
                        neighbor_pos->child[1] = pos_rc;
                    }
                } else {
                    pos_parent->child[pos_parent_side] = neighbor_pos;
                    if(pos_lc) pos_lc->parent = neighbor_pos;
                    if(pos_rc) pos_rc->parent = neighbor_pos;
                    neighbor_pos_parent
                      ->child[neighbor_pos_parent_side] = pos;
                    if(neighbor_pos_lc) neighbor_pos_lc->parent = pos;
                    if(neighbor_pos_rc) neighbor_pos_rc->parent = pos;

                    pos->parent = neighbor_pos_parent;
                    pos->child[0] = neighbor_pos_lc;
                    pos->child[1] = neighbor_pos_rc;
                    neighbor_pos->parent = pos_parent;
                    neighbor_pos->child[0] = pos_lc;
                    neighbor_pos->child[1] = pos_rc;
                }
                std::swap(
                  pos->n_children[0], neighbor_pos->n_children[0]);
                std::swap(
                  pos->n_children[1], neighbor_pos->n_children[1]);
                std::swap(pos->red, neighbor_pos->red);

                if(pos == origin->o_front()) {
                    // In case the removed node is the first element
                    origin->o_front() = neighbor_pos;
                }
            } else {
                // In case the removed node is ...
                if(pos == origin->o_front()) {
                    // In case the removed node is the first element
                    origin->o_front() = neighbor(pos, 1);
                } else if(pos == origin->o_back()) {
                    // In case the removed node is the last element
                    origin->o_back() = neighbor(pos, 0);
                }
            }
            assert(
              pos->child[0] == nullptr || pos->child[1] == nullptr);

            pNode pos_adjust = pos->parent;
            bool adjust_needed = !(pos->red);
            size_type side_adjust = pos_adjust->side_of_child(pos);
            pos_adjust->child[side_adjust] =
              (pos->child[1] ? pos->child[1] : pos->child[0]);
            if(pos_adjust->child[side_adjust]) {
                pos_adjust->child[side_adjust]->parent = pos_adjust;
            }

            delete pos;
            pos_adjust->recompute_n_children(side_adjust);

            // Rotate the tree to adjust the 'black'-height
            while(adjust_needed && pos_adjust != origin) {
                adjust_needed = false;

                // Here, pos_adjust->child[side_adjust] has fewer
                // 'black'-height by one than others
                pNode pos_reduced = pos_adjust->child[side_adjust];
                pNode pos_sibling =
                  pos_adjust->child[1 - side_adjust];

                assert(pos_sibling);

                if(pos_sibling->red) {
                    // (The figure below is side_adjust == 0; invert
                    // if side_adjust == 1) [a]: pos_adjust [r]:
                    // pos_reduced (possibly null) [s]: pos_sibling
                    // "%": fewer 'black'-height
                    //
                    //       b[a]
                    //      _/   \_
                    //  %[r]      r[s]
                    //   / \       / \
                    // [A] [B]  b[C] [D]b
                    //
                    // is rotated as
                    //
                    //      b[s]
                    //       / \
                    //    r[a] [D]b
                    //     / \
                    //  %[r] [C]b
                    //   / \
                    // [A] [B]
                    //
                    // Here, since the subtree [r] still has fewer
                    // 'black'-height by one, we rotate it for
                    // [r]-[a]-[C] (processed next).
                    pNode pos_adjust_parent = pos_adjust->parent;
                    size_type side_adjust_parent =
                      pos_adjust_parent->side_of_child(pos_adjust);

                    pos_adjust->child[1 - side_adjust] =
                      pos_sibling->child[side_adjust];
                    if(pos_sibling->child[side_adjust])
                        pos_sibling->child[side_adjust]->parent =
                          pos_adjust;
                    pos_sibling->child[side_adjust] = pos_adjust;
                    pos_adjust->parent = pos_sibling;
                    pos_sibling->parent = pos_adjust_parent;
                    pos_adjust_parent->child[side_adjust_parent] =
                      pos_sibling;

                    pos_adjust->recompute_n_children(0);
                    pos_adjust->recompute_n_children(1);
                    pos_sibling->recompute_n_children(side_adjust);
                    pos_adjust_parent->recompute_n_children(
                      side_adjust_parent);

                    pos_adjust->red = true;
                    pos_sibling->red = false;

                    pos_sibling = pos_adjust->child[1 - side_adjust];
                }

                assert(!(pos_sibling->red));

                // (The figure below is side_adjust == 0; invert if
                // side_adjust == 1) [a]: pos_adjust [r]: pos_reduced
                // (possibly null) [s]: pos_sibling
                // "%": fewer 'black'-height
                //
                //        [a]
                //      _/   \_
                //  %[r]      b[s]
                //   / \       / \
                // [A] [B]   [C] [D]

                if(
                  pos_sibling->child[side_adjust] &&
                  pos_sibling->child[side_adjust]->red) {
                    //        [a]
                    //      _/   \_
                    //  %[r]      b[s]
                    //   / \       / \
                    // [A] [B]  r[n] [D]
                    //           / \
                    //        [C1] [C2]
                    //
                    // is rotated as
                    //          [n]@ ... set the color that [a]
                    //          originally has
                    //        _/   \_
                    //    b[a]*      [s]b
                    //     / \       / \
                    //  %[r] [C1] [C2] [D]
                    //   / \
                    // [A] [B]
                    pNode pos_adjust_parent = pos_adjust->parent;
                    size_type side_adjust_parent =
                      pos_adjust_parent->side_of_child(pos_adjust);

                    pNode pos_nephew =
                      pos_sibling->child[side_adjust];
                    pos_adjust->child[side_adjust] = pos_reduced;
                    if(pos_reduced) pos_reduced->parent = pos_adjust;
                    pos_adjust->child[1 - side_adjust] =
                      pos_nephew->child[side_adjust];
                    if(pos_nephew->child[side_adjust])
                        pos_nephew->child[side_adjust]->parent =
                          pos_adjust;
                    pos_sibling->child[side_adjust] =
                      pos_nephew->child[1 - side_adjust];
                    if(pos_nephew->child[1 - side_adjust])
                        pos_nephew->child[1 - side_adjust]->parent =
                          pos_sibling;
                    pos_nephew->child[side_adjust] = pos_adjust;
                    pos_adjust->parent = pos_nephew;
                    pos_nephew->child[1 - side_adjust] = pos_sibling;
                    pos_sibling->parent = pos_nephew;

                    pos_nephew->parent = pos_adjust_parent;
                    pos_adjust_parent->child[side_adjust_parent] =
                      pos_nephew;

                    pos_nephew->red = pos_adjust->red;
                    pos_adjust->red = false;

                    pos_adjust->recompute_n_children(0);
                    pos_adjust->recompute_n_children(1);
                    pos_sibling->recompute_n_children(side_adjust);
                    pos_nephew->recompute_n_children(0);
                    pos_nephew->recompute_n_children(1);

                    pos_adjust = pos_nephew;

                    side_adjust = side_adjust_parent;
                    pos_adjust = pos_adjust_parent;
                } else if(
                  pos_sibling->child[1 - side_adjust] &&
                  pos_sibling->child[1 - side_adjust]->red) {
                    //        [a]
                    //      _/   \_
                    //  %[r]      b[s]
                    //   / \       / \
                    // [A] [B]   [C] [n]r
                    //               / \
                    //            [D1] [D2]
                    //
                    // is rotated as
                    //          [s]@ ... set the color that [a]
                    //          originally has
                    //        _/   \_
                    //    b[a]*     b[n]*
                    //     / \       / \
                    //  %[r] [C]  [D1] [D2]
                    //   / \
                    // [A] [B]
                    pNode pos_adjust_parent = pos_adjust->parent;
                    size_type side_adjust_parent =
                      pos_adjust_parent->side_of_child(pos_adjust);

                    pNode pos_nephew =
                      pos_sibling->child[1 - side_adjust];
                    pos_adjust->child[1 - side_adjust] =
                      pos_sibling->child[side_adjust];
                    if(pos_sibling->child[side_adjust])
                        pos_sibling->child[side_adjust]->parent =
                          pos_adjust;
                    pos_sibling->child[side_adjust] = pos_adjust;
                    pos_adjust->parent = pos_sibling;
                    pos_sibling->child[1 - side_adjust] = pos_nephew;
                    if(pos_nephew) pos_nephew->parent = pos_sibling;

                    pos_sibling->red = pos_adjust->red;
                    pos_adjust->red = false;
                    pos_nephew->red = false;

                    pos_sibling->parent = pos_adjust_parent;
                    pos_adjust_parent->child[side_adjust_parent] =
                      pos_sibling;

                    pos_adjust->recompute_n_children(0);
                    pos_adjust->recompute_n_children(1);
                    pos_sibling->recompute_n_children(0);
                    pos_sibling->recompute_n_children(1);

                    side_adjust = side_adjust_parent;
                    pos_adjust = pos_adjust_parent;
                } else {
                    //        [a]
                    //      _/   \_
                    //  %[r]      b[s]
                    //   / \       / \
                    // [A] [B]  b[C] [D]b
                    //
                    // is modified as
                    //
                    //       b[a]*
                    //      _/   \_
                    //  %[r]      r[s]*
                    //   / \       / \
                    // [A] [B]  b[C] [D]b
                    //
                    // Here, in case [a] is black before the
                    // modification, this modification makes the
                    // 'black'-height of [a] be fewer by one than
                    // others, that is, we check the rotation once
                    // more.

                    pos_sibling->red = true;
                    if(!(pos_adjust->red)) {
                        adjust_needed = true;
                    } else {
                        pos_adjust->red = false;
                    }
                    pos_adjust->recompute_n_children(side_adjust);
                    side_adjust =
                      pos_adjust->parent->side_of_child(pos_adjust);
                    pos_adjust = pos_adjust->parent;
                }
            }

            while(pos_adjust != origin) {
                pos_adjust->recompute_n_children(side_adjust);
                side_adjust =
                  pos_adjust->parent->side_of_child(pos_adjust);
                pos_adjust = pos_adjust->parent;
            }

            origin->o_root()->red = false;

            return returned_pos;
        }

        void clear() {
            if(this->empty()) return;

            // Remove all elements
            using deletion_status_t = std::pair<pNode, bool>;
            std::stack<deletion_status_t> st;
            st.push(std::make_pair(origin->o_root(), false));

            while(!(st.empty())) {
                deletion_status_t ds = st.top();
                st.pop();

                if(!ds.first) continue;

                if(ds.second) {
                    delete ds.first;
                } else {
                    st.push(std::make_pair(ds.first, true));
                    st.push(
                      std::make_pair(ds.first->child[1], false));
                    st.push(
                      std::make_pair(ds.first->child[0], false));
                }
            }

            origin->o_root() = nullptr;
            origin->o_front() = nullptr;
            origin->o_back() = nullptr;
        }

        void copy_from(const Base & other) {
            using copy_status_t = std::pair<pNode, pNode>;

            this->clear();
            if(other.empty()) return;

            // Add the root node of `this` from `other`
            pNode newOrignode = new Node();
            *newOrignode = *(other.origin->o_root());
            newOrignode->parent = origin;
            origin->o_root() = newOrignode;
            if(other.origin->o_root() == other.origin->o_front()) {
                origin->o_front() = newOrignode;
            }
            if(other.origin->o_root() == other.origin->o_back()) {
                origin->o_back() = newOrignode;
            }

            // Copy
            std::stack<copy_status_t> st;
            st.push(
              std::make_pair(newOrignode, other.origin->o_root()));

            while(!(st.empty())) {
                copy_status_t cs = st.top();
                st.pop();

                for(size_t p = 0; p <= 1; ++p) {
                    if(cs.second->child[p]) {
                        pNode newnode = new Node();
                        *newnode = *(cs.second->child[p]);
                        newnode->parent = cs.first;
                        cs.first->child[p] = newnode;
                        if(
                          cs.second->child[p] ==
                          other.origin->o_front()) {
                            origin->o_front() = newnode;
                        }
                        if(
                          cs.second->child[p] ==
                          other.origin->o_back()) {
                            origin->o_back() = newnode;
                        }

                        st.push(std::make_pair(
                          newnode, cs.second->child[p]));
                    } else {
                        cs.first->child[p] = nullptr;
                    }
                }
            }
        }
    };
    /// \endcond
    // End of the definition of Base

    // ------------------------------------------------------------
    // APIs begin here
    // ------------------------------------------------------------
    class const_iterator;

    /// \~english
    /// The iterator type to point an element in LTOList.
    ///
    /// \~japanese
    /// LTOList の要素を列挙するためのイテレータ型です。
    class iterator {
    private:
        Base * pbase_;
        pNode n_;
        iterator(Base & base, pNode n) : pbase_(&base), n_(n) {}

    public:
        iterator() : pbase_(nullptr), n_(nullptr) {}
        iterator(const iterator & other)
            : pbase_(other.pbase_), n_(other.n_) {}
        iterator operator=(const iterator & other) {
            pbase_ = other.pbase_;
            n_ = other.n_;
            return *this;
        }

        pointer operator->() const { return &(n_->value); }
        reference operator*() const { return n_->value; }
        iterator operator++() {
            n_ = pbase_->neighbor(n_, 1);
            return *this;
        }
        iterator operator--() {
            n_ = pbase_->neighbor(n_, 0);
            return *this;
        }
        iterator operator++(int) {
            iterator tmp(*this);
            n_ = pbase_->neighbor(n_, 1);
            return tmp;
        }
        iterator operator--(int) {
            iterator tmp(*this);
            n_ = pbase_->neighbor(n_, 0);
            return tmp;
        }
        iterator operator+=(difference_type dir) {
            n_ = pbase_->advance(n_, dir);
            return *this;
        }
        iterator operator-=(difference_type dir) {
            n_ = pbase_->advance(n_, -dir);
            return *this;
        }
        reference operator[](difference_type dir) {
            return pbase_->advance(n_, dir)->value;
        }

        /// \cond
        friend class ::LTOList<value_type>;
        friend class const_iterator;
        /// \endcond
        template<class TT>
        friend std::ostream & operator<<(
          std::ostream & os,
          const typename ::LTOList<TT>::iterator & it);

    private:
        template<class IT1, class IT2>
        void from_same_list(const IT1 & it1, const IT2 & it2){
            if(it1.pbase_ != it2.pbase_){
                throw std::invalid_argument("The difference of iterators cannot be taken for different lists");
            }
        }

        // Global functions that accesses private members of
        // `iterator`
        friend iterator operator+(iterator it, difference_type dir) {
            iterator after = it;
            after += dir;
            return after;
        }
        friend iterator operator+(difference_type dir, iterator it) {
            iterator after = it;
            after += dir;
            return after;
        }
        friend iterator operator-(iterator it, difference_type dir) {
            iterator after = it;
            after -= dir;
            return after;
        }
        friend difference_type
        operator-(iterator it, iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return static_cast<difference_type>(
                     it.pbase_->position(it.n_)) -
                   static_cast<difference_type>(
                     other.pbase_->position(other.n_));
        }
        friend bool operator<(iterator it, iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) <
                   other.pbase_->position(other.n_);
        }
        friend bool operator<=(iterator it, iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) <=
                   other.pbase_->position(other.n_);
        }
        friend bool operator>(iterator it, iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) >
                   other.pbase_->position(other.n_);
        }
        friend bool operator>=(iterator it, iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) >=
                   other.pbase_->position(other.n_);
        }
        friend bool operator==(iterator it, iterator other) {
            return it.n_ == other.n_;
        }
        friend bool operator!=(iterator it, iterator other) {
            return it.n_ != other.n_;
        }
        friend std::ostream &
        operator<<(std::ostream & out, iterator it) {
            out << "<LTOList::iterator " << it.n_ << ">";
            return out;
        }
    };

    /// \~english
    /// The read-only iterator type to point an element in LTOList.
    ///
    /// \~japanese
    /// LTOList の要素を列挙するための読み取り専用イテレータ型です。
    class const_iterator {
    private:
        const Base * pbase_;
        pNode n_;
        const_iterator(const Base & base, pNode n)
            : pbase_(&base), n_(n) {}

    public:
        const_iterator() : pbase_(nullptr), n_(nullptr) {}
        const_iterator(const iterator & other)
            : pbase_(other.pbase_), n_(other.n_) {}
        const_iterator(const const_iterator & other)
            : pbase_(other.pbase_), n_(other.n_) {}
        const_iterator operator=(const iterator & other) {
            pbase_ = other.pbase_;
            n_ = other.n_;
            return *this;
        }
        const_iterator operator=(const const_iterator & other) {
            pbase_ = other.pbase_;
            n_ = other.n_;
            return *this;
        }

        const_pointer operator->() const { return &(n_->value); }
        const_reference operator*() const { return n_->value; }
        const_iterator operator++() {
            n_ = pbase_->neighbor(n_, 1);
            return *this;
        }
        const_iterator operator--() {
            n_ = pbase_->neighbor(n_, 0);
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp(*this);
            n_ = pbase_->neighbor(n_, 1);
            return tmp;
        }
        const_iterator operator--(int) {
            const_iterator tmp(*this);
            n_ = pbase_->neighbor(n_, 0);
            return tmp;
        }
        const_iterator operator+=(difference_type dir) {
            n_ = pbase_->advance(n_, dir);
            return *this;
        }
        const_iterator operator-=(difference_type dir) {
            n_ = pbase_->advance(n_, -dir);
            return *this;
        }
        const_reference operator[](difference_type dir) {
            return pbase_->advance(n_, dir)->value;
        }

        /// \cond
        friend class ::LTOList<value_type>;
        /// \endcond
        template<class TT>
        friend std::ostream & operator<<(
          std::ostream & os,
          const typename ::LTOList<TT>::const_iterator & it);

    private:
        // Global functions that accesses private members of
        // `iterator`
        friend const_iterator
        operator+(const_iterator it, difference_type dir) {
            const_iterator after = it;
            after += dir;
            return after;
        }
        friend const_iterator
        operator+(difference_type dir, const_iterator it) {
            const_iterator after = it;
            after += dir;
            return after;
        }
        friend const_iterator
        operator-(const_iterator it, difference_type dir) {
            const_iterator after = it;
            after -= dir;
            return after;
        }
        friend difference_type
        operator-(const_iterator it, const_iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return static_cast<difference_type>(
                     it.pbase_->position(it.n_)) -
                   static_cast<difference_type>(
                     other.pbase_->position(other.n_));
        }
        friend bool
        operator<(const_iterator it, const_iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) <
                   other.pbase_->position(other.n_);
        }
        friend bool
        operator<=(const_iterator it, const_iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) <=
                   other.pbase_->position(other.n_);
        }
        friend bool
        operator>(const_iterator it, const_iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) >
                   other.pbase_->position(other.n_);
        }
        friend bool
        operator>=(const_iterator it, const_iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) >=
                   other.pbase_->position(other.n_);
        }
        friend bool
        operator==(const_iterator it, const_iterator other) {
            return it.n_ == other.n_;
        }
        friend bool
        operator!=(const_iterator it, const_iterator other) {
            return it.n_ != other.n_;
        }
        friend std::ostream &
        operator<<(std::ostream & out, const_iterator it) {
            out << "<LTOList::const_iterator " << it.n_ << ">";
            return out;
        }

        friend difference_type
        operator-(const_iterator it, iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return static_cast<difference_type>(
                     it.pbase_->position(it.n_)) -
                   static_cast<difference_type>(
                     other.pbase_->position(other.n_));
        }
        friend difference_type
        operator-(iterator it, const_iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return static_cast<difference_type>(
                     it.pbase_->position(it.n_)) -
                   static_cast<difference_type>(
                     other.pbase_->position(other.n_));
        }
        friend bool operator<(const_iterator it, iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) <
                   other.pbase_->position(other.n_);
        }
        friend bool operator<(iterator it, const_iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) <
                   other.pbase_->position(other.n_);
        }
        friend bool operator<=(const_iterator it, iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) <=
                   other.pbase_->position(other.n_);
        }
        friend bool operator<=(iterator it, const_iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) <=
                   other.pbase_->position(other.n_);
        }
        friend bool operator>(const_iterator it, iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) >
                   other.pbase_->position(other.n_);
        }
        friend bool operator>(iterator it, const_iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) >
                   other.pbase_->position(other.n_);
        }
        friend bool operator>=(const_iterator it, iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) >=
                   other.pbase_->position(other.n_);
        }
        friend bool operator>=(iterator it, const_iterator other) {
            from_same_list(it.pbase_, other.pbase_);
            return it.pbase_->position(it.n_) >=
                   other.pbase_->position(other.n_);
        }
        friend bool operator==(const_iterator it, iterator other) {
            return it.n_ == other.n_;
        }
        friend bool operator==(iterator it, const_iterator other) {
            return it.n_ == other.n_;
        }
        friend bool operator!=(const_iterator it, iterator other) {
            return it.n_ != other.n_;
        }
        friend bool operator!=(iterator it, const_iterator other) {
            return it.n_ != other.n_;
        }
    };

private:
    Base base_;

public:
    LTOList() : base_() {}
    LTOList(LTOList && other) : base_() {
        other.swap(*this);
        other.clear();
    }
    LTOList(const LTOList & other) : base_() {
        base_.copy_from(other.base_);
    }
    LTOList(std::initializer_list<T> init) : base_() {
        this->insert(this->begin(), init.begin(), init.end());
    }

    iterator begin() {
        return iterator(base_, base_.origin->o_front());
    }
    iterator end() {
        return iterator(
          base_, base_.origin->o_front() ? base_.origin : nullptr);
    }
    iterator rbegin() {
        return iterator(base_, base_.origin->o_front());
    }
    iterator rend() {
        return iterator(
          base_, base_.origin->o_front() ? base_.origin : nullptr);
    }
    const_iterator begin() const {
        return const_iterator(base_, base_.origin->o_front());
    }
    const_iterator end() const {
        return const_iterator(
          base_, base_.origin->o_front() ? base_.origin : nullptr);
    }
    const_iterator rbegin() const {
        return const_iterator(base_, base_.origin->o_front());
    }
    const_iterator rend() const {
        return const_iterator(
          base_, base_.origin->o_front() ? base_.origin : nullptr);
    }
    const_iterator cbegin() const {
        return const_iterator(base_, base_.origin->o_front());
    }
    const_iterator cend() const {
        return const_iterator(
          base_, base_.origin->o_front() ? base_.origin : nullptr);
    }
    const_iterator crbegin() const {
        return const_iterator(base_, base_.origin->o_front());
    }
    const_iterator crend() const {
        return const_iterator(
          base_, base_.origin->o_front() ? base_.origin : nullptr);
    }

    size_type size() const { return base_.size(); }
    bool empty() const { return base_.empty(); }

    reference at(size_type i) {
        if(i >= size())
            throw std::out_of_range(
              "reference LTOList::operator[]: Index out of range");

        difference_type adpos =
          static_cast<difference_type>(i) -
          static_cast<difference_type>(
            base_.origin->o_root()->n_children[0]);
        return base_.advance(base_.origin->o_root(), adpos)->value;
    }

    const_reference at(size_type i) const {
        if(empty())
            throw std::out_of_range(
              "const_reference LTOList::at: Index out of range");

        difference_type adpos =
          static_cast<difference_type>(i) -
          static_cast<difference_type>(
            base_.origin->o_root()->n_children[0]);
        return base_.advance(base_.origin->o_root(), adpos)->value;
    }

    inline reference operator[](size_type i) { return this->at(i); }
    inline const_reference operator[](size_type i) const {
        return this->at(i);
    }

    reference front() { return base_.origin->o_front()->value; }
    const_reference front() const {
        return base_.origin->o_front()->value;
    }
    reference back() { return base_.origin->o_back()->value; }
    const_reference back() const {
        return base_.origin->o_back()->value;
    }

    /// \~english
    /// Inserts an element to this LTOList, before or after the
    /// specified element.
    ///
    /// @param side If it is 0, the new element is stored before
    /// `position`. Otherwise it is stored after `position`.
    /// If `position` is end(), `side` must be 0; otherwise
    /// an exception `std::invalid_argument` is raised.
    /// @param position The iterator at which the new element is
    /// stored.
    /// @param val The new element. It is copied to be stored into
    /// LTOList.
    ///
    /// \~japanese
    /// LTOList に新しい要素を、指定の要素の前か後ろに追加します。
    ///
    /// @param side 0であった場合、新しい要素は `position`
    /// の前に追加されます。そうでなければ `position`
    /// の後ろに追加されます。
    /// もし `position` が end() である場合、
    /// `side` は0でなければなりません。そうでなければ例外
    /// `std::invalid_argument` が発生します。
    /// @param position 新しい要素を追加する位置を示すイテレータです。
    /// @param val 新しい要素です。 LTOList
    /// に追加するにあたりコピーされます。
    iterator
    insert(size_type side, iterator position, const_reference val) {
        return iterator(base_, base_.add(position.n_, side, val));
    }

    /// \~english
    /// Inserts an element to this LTOList, before or after the
    /// specified element.
    ///
    /// @param side If it is 0, the new element is stored before
    /// `position`. Otherwise it is stored after `position`.
    /// If `position` is end(), `side` must be 0; otherwise
    /// an exception `std::invalid_argument` is raised.
    /// @param position The iterator at which the new element is
    /// stored.
    /// @param val The new element. It is "moved" to be stored into
    /// LTOList (i.e., the value in `val` may be deleted).
    ///
    /// \~japanese
    /// LTOList に新しい要素を、指定の要素の前か後ろに追加します。
    ///
    /// @param side 0であった場合、新しい要素は `position`
    /// の前に追加されます。そうでなければ `position`
    /// の後ろに追加されます。
    /// もし `position` が end() である場合、
    /// `side` は0でなければなりません。そうでなければ例外
    /// `std::invalid_argument` が発生します。
    /// @param position 新しい要素を追加する位置を示すイテレータです。
    /// @param val 新しい要素です。 LTOList
    /// に追加するにあたり「ムーブ」されます（すなわち、 `val`
    /// の値は削除される場合があります）。
    iterator insert(size_type side, iterator position, T && val) {
        return iterator(
          base_, base_.add(position.n_, side, std::move(val)));
    }

    /// \~english
    /// Inserts an element to this LTOList, before or after the
    /// specified element.
    ///
    /// @param side If it is 0, the new element is stored before
    /// `position`. Otherwise it is stored after `position`.
    /// @param position The iterator at which the new element is
    /// stored.
    /// @param args The new element. It considers args as the
    /// arguments of the constructor of LTOList::value_type, and the
    /// constructed element is inserted into LTOList.
    ///
    /// \~japanese
    /// LTOList に新しい要素を、指定の要素の前か後ろに追加します。
    ///
    /// @param side 0であった場合、新しい要素は `position`
    /// の前に追加されます。そうでなければ `position`
    /// の後ろに追加されます。
    /// @param position 新しい要素を追加する位置を示すイテレータです。
    /// @param args 新しい要素です。 `args` を LTOList::value_type
    /// のコンストラクタの引数とし、その要素が LTOList
    /// に追加されます。
    template<class... Args>
    iterator
    emplace(size_type side, iterator position, Args... args) {
        return iterator(
          base_, base_.add_emplace(position.n_, side, args...));
    }

    /// \~english
    /// Same as
    /// insert(size_type side, iterator position, const_reference val)
    /// with `side = 0` \.
    ///
    /// \~japanese
    /// insert(size_type side, iterator position, const_reference val)
    /// で `side = 0` とした場合に同じです。
    iterator insert(iterator position, const_reference val) {
        return insert(0, position, val);
    }

    /// \~english
    /// Same as
    /// insert(size_type side, iterator position, T && val)
    /// with `side = 0` \.
    ///
    /// \~japanese
    /// insert(size_type side, iterator position, T && val)
    /// で `side = 0` とした場合に同じです。
    iterator insert(iterator position, T && val) {
        return insert(0, position, std::move(val));
    }

    /// \~english
    /// Same as
    /// emplace(size_type side, iterator position, Args... args)
    /// with `side = 0` \.
    ///
    /// \~japanese
    /// emplace(size_type side, iterator position, Args... args)
    /// で `side = 0` とした場合に同じです。
    template<class... Args>
    iterator emplace(iterator position, Args... args) {
        return emplace(0, position, args...);
    }

    /// \~english
    /// Inserts specified number of elements to this LTOList, before
    /// or after the specified element.
    ///
    /// @param side If it is 0, the new element is stored before
    /// `position`. Otherwise it is stored after `position`.
    /// If `position` is end(), `side` must be 0; otherwise
    /// an exception `std::invalid_argument` is raised.
    /// @param position The iterator at which the new element is
    /// stored.
    /// @param n The number of insertions.
    /// @param val The new element. It is copied to be stored into
    /// LTOList.
    ///
    /// \~japanese
    /// LTOList
    /// に新しい要素を、指定の要素の前か後ろに指定の回数追加します。
    ///
    /// @param side 0であった場合、新しい要素は `position`
    /// の前に追加されます。そうでなければ `position`
    /// の後ろに追加されます。
    /// もし `position` が end() である場合、
    /// `side` は0でなければなりません。そうでなければ例外
    /// `std::invalid_argument` が発生します。
    /// @param position 新しい要素を追加する位置を示すイテレータです。
    /// @param n 追加回数です。
    /// @param val 新しい要素です。 LTOList
    /// に追加するにあたりコピーされます。
    void insert(
      size_type side, iterator position, size_type n,
      const_reference val) {
        for(size_type i = 0; i < n; ++i) insert(side, position, val);
    }

    /// \~english
    /// Inserts specified number of elements to this LTOList, before
    /// or after the specified element.
    ///
    /// @param side If it is 0, the new element is stored before
    /// `position`. Otherwise it is stored after `position`.
    /// If `position` is end(), `side` must be 0; otherwise
    /// an exception `std::invalid_argument` is raised.
    /// @param position The iterator at which the new element is
    /// stored.
    /// @param n The number of insertions.
    /// @param args The new element. It considers args as the
    /// arguments of the constructor of LTOList::value_type, and the
    /// constructed element is inserted into LTOList.
    ///
    /// \~japanese
    /// LTOList
    /// に新しい要素を、指定の要素の前か後ろに指定の回数追加します。
    ///
    /// @param side 0であった場合、新しい要素は `position`
    /// の前に追加されます。そうでなければ `position`
    /// の後ろに追加されます。
    /// もし `position` が end() である場合、
    /// `side` は0でなければなりません。そうでなければ例外
    /// `std::invalid_argument` が発生します。
    /// @param position 新しい要素を追加する位置を示すイテレータです。
    /// @param n 追加回数です。
    /// @param args 新しい要素です。 `args` を LTOList::value_type
    /// のコンストラクタの引数とし、その要素が LTOList
    /// に追加されます。
    template<class... Args>
    void emplace(
      size_type side, iterator position, size_type n, Args... args) {
        for(size_type i = 0; i < n; ++i)
            emplace(side, position, args...);
    }

    /// \~english
    /// Same as
    /// insert(size_type side, iterator position, size_type n,
    /// const_reference val) with `side = 0` \.
    ///
    /// \~japanese
    /// insert(size_type side, iterator position, size_type n,
    /// const_reference val) で `side = 0` とした場合に同じです。
    void insert(iterator position, size_type n, const_reference val) {
        for(size_type i = 0; i < n; ++i) insert(0, position, val);
    }

    /// \~english
    /// Same as
    /// emplace(size_type side, iterator position, size_type n,
    /// Args... args) with `side = 0` \.
    ///
    /// \~japanese
    /// emplace(size_type side, iterator position, size_type n,
    /// Args... args) で `side = 0` とした場合に同じです。
    template<class... Args>
    void emplace(iterator position, size_type n, Args... args) {
        for(size_type i = 0; i < n; ++i)
            emplace(0, position, args...);
    }

    /// \~english
    /// Inserts a bunch of elements to this LTOList, before or after
    /// the specified element.
    ///
    /// @param side If it is 0, the new element is stored before
    /// `position`. Otherwise it is stored after `position`.
    /// If `position` is end(), `side` must be 0; otherwise
    /// an exception `std::invalid_argument` is raised.
    /// @param position The iterator at which the new element is
    /// stored.
    /// @param elem_begin The iterator (any iterator that points
    /// LTOList::value_type is accepted; not limited to
    /// LTOList::iterator) for the first element to be inserted.
    /// @param elem_end The iterator (any iterator that points
    /// LTOList::value_type is accepted; not limited to
    /// LTOList::iterator) before the last element to be inserted.
    ///
    /// \~japanese
    /// LTOList に要素の集まりを、指定の要素の前か後ろに追加します。
    ///
    /// @param side 0であった場合、新しい要素は `position`
    /// の前に追加されます。そうでなければ `position`
    /// の後ろに追加されます。
    /// もし `position` が end() である場合、
    /// `side` は0でなければなりません。そうでなければ例外
    /// `std::invalid_argument` が発生します。
    /// @param position 新しい要素を追加する位置を示すイテレータです。
    /// @param elem_begin 追加する最初の要素を表すイテレータ（
    /// LTOList::iterator に限らず、 LTOList::value_type
    /// 型を指すイテレータなら何でもよい）です。
    /// @param elem_end 追加する最後の要素の手前（ LTOList::iterator
    /// に限らず、 LTOList::value_type
    /// 型を指すイテレータなら何でもよい）を表すイテレータです。
    template<class InputIterator>
    void insert(
      size_type side, iterator position, InputIterator elem_begin,
      InputIterator elem_end) {
        iterator tmp = position;
        for(InputIterator it = elem_begin; it != elem_end; ++it) {
            tmp = insert(side, tmp, *it);
            side = 1; // insertion side should be considered only for
                      // the first insertion
        }
    }

    /// \~english
    /// Same as
    /// insert(size_type side, iterator position, InputIterator
    /// elem_begin, InputIterator elem_end) with `side = 0` \.
    ///
    /// \~japanese
    /// insert(size_type side, iterator position, InputIterator
    /// elem_begin, InputIterator elem_end) で `side = 0`
    /// とした場合に同じです。
    template<class InputIterator>
    void insert(
      iterator position, InputIterator elem_begin,
      InputIterator elem_end) {
        insert(0, position, elem_begin, elem_end);
    }

    iterator erase(iterator position) {
        return iterator(base_, base_.remove(position.n_));
    }
    iterator erase(const_iterator position) {
        return iterator(base_, base_.remove(position.n_));
    }

    iterator erase(iterator elem_begin, iterator elem_end) {
        // Should we check elem_begin < elem_end beforehand?
        while(elem_begin != elem_end) {
            elem_begin = erase(elem_begin);
        }
        return elem_begin;
    }
    iterator
    erase(const_iterator elem_begin, const_iterator elem_end) {
        while(elem_begin != elem_end) elem_begin = erase(elem_begin);
        return elem_begin;
    }

    template<class InputIterator>
    void assign(InputIterator elem_begin, InputIterator elem_end) {
        clear();
        insert(begin(), elem_begin, elem_end);
    }

    void assign(size_type n, const T & val) {
        clear();
        insert(begin(), n, val);
    }

    void push_front(const T & val) { insert(begin(), val); }
    void push_front(T && val) { insert(begin(), std::move(val)); }
    template<class... Args>
    void emplace_front(Args... args) {
        emplace(begin(), args...);
    }

    void push_back(const T & val) { insert(end(), val); }
    void push_back(T && val) { insert(end(), std::move(val)); }
    template<class... Args>
    void emplace_back(Args... args) {
        emplace(end(), args...);
    }

    void pop_front() { erase(begin()); }
    void pop_back() { erase(--(end())); }

    void clear() { base_.clear(); }

    void swap(LTOList<T> & other) {
        std::swap(base_.origin, other.base_.origin);
    }
};

template<class T>
std::ostream & operator<<(
  std::ostream & os, const typename LTOList<T>::iterator & it) {
    os << "LTOList::iterator<" << it.n_ << ">";
    return os;
}

template<class T>
std::ostream & operator<<(
  std::ostream & os, const typename LTOList<T>::const_iterator & it) {
    os << "LTOList::const_iterator<" << it.n_ << ">";
    return os;
}
