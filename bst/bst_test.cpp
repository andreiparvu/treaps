// Mugurel Andreica

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "linked_list.h"
#include "bst.h"
#include "augmented_bst.h"
#include "global_rebuilding_bst.h"
#include "logical_remove_bst.h"
#include "scapegoat_tree.h"
#include "treap.h"

#define NUM_ELEMS 10000

template<typename TreeNodeType> void test_simple_bst(BinarySearchTree<int, TreeNodeType > *bst, const char *msg, int worst_case) {
  fprintf(stderr, "\n\n");
  srand(1000);
  LinkedList<int> ll(compare_int);

  int i, tstart = clock();
  for (i = 0; i < NUM_ELEMS; i++) {
    int k;
    if (worst_case)
      k = 2 * i + 1;
    else
      k = rand() % NUM_ELEMS;
    bst->insert_info(k);
    ll.addLast(k);
  }
  fprintf(stderr, "%s: After insertions\n", msg);

  struct list_elem<int> *p = ll.plast;
  for (i = NUM_ELEMS - 1; i >= 0; i--) {
    if (bst->find_info(p->info) == NULL) {
      fprintf(stderr, "ERROR! Element %d was not found in the tree\n", 2 * i + 1);
    }
    p = p->prev;
  }

  p = ll.pfirst;
  for (i = 0; i < NUM_ELEMS; i++) {
    bst->remove_info(p->info);
    p = p->next;
  }

  fprintf(stderr, "%s: After removals\n", msg);
  fprintf(stderr, "%s: Duration=%.3lf sec\n", msg, (double) (clock() - tstart) / CLOCKS_PER_SEC);
}

template<typename TreeNodeType> void test_augmented_bst(AugmentedBinarySearchTree<int, TreeNodeType> *bst, const char *msg, int worst_case) {
  fprintf(stderr, "\n\n");
  srand(1000);
  LinkedList<int> ll(compare_int);

  int i, tstart = clock();
  for (i = 0; i < NUM_ELEMS; i++) {
    int k;
    if (worst_case)
      k = 2 * i + 1;
    else
      k = rand() % NUM_ELEMS;
    bst->insert_info(k);
    ll.addLast(k);
  }
  fprintf(stderr, "%s: After insertions: ", msg);
  bst->print_statistics();

  struct list_elem<int> *p = ll.plast;
  for (i = NUM_ELEMS - 1; i >= 0; i--) {
    if (bst->find_info(p->info) == NULL) {
      fprintf(stderr, "ERROR! Element %d was not found in the tree\n", 2 * i + 1);
    }
    p = p->prev;
  }

  p = ll.pfirst;
  for (i = 0; i < NUM_ELEMS; i++) {
    bst->remove_info(p->info);
    p = p->next;
  }

  fprintf(stderr, "%s: After removals: ", msg);
  bst->print_statistics();
  fprintf(stderr, "%s: Duration=%.3lf sec\n", msg, (double) (clock() - tstart) / CLOCKS_PER_SEC);
}

int main() {
  BinarySearchTree<int, BSTNode<int> > bst(compare_int);
  test_simple_bst<BSTNode<int> >(&bst, "Standard BST (random)", 0);
  BinarySearchTree<int, BSTNode<int> > bst2(compare_int);
  test_simple_bst<BSTNode<int> >(&bst2, "Standard BST (worst case)", 1);

  AugmentedBinarySearchTree<int, AugmentedBSTNode<int> > bst3(compare_int);
  test_augmented_bst<AugmentedBSTNode<int> >(&bst3, "Augmented BST (random)", 0);
  AugmentedBinarySearchTree<int, AugmentedBSTNode<int> > bst4(compare_int);
  test_augmented_bst<AugmentedBSTNode<int> >(&bst4, "Augmented BST (worst case)", 1);

  GlobalRebuildingBinarySearchTree<int, AugmentedBSTNode<int> > bst5(compare_int, 1.25);
  test_augmented_bst<AugmentedBSTNode<int> >(&bst5, "Global Rebuilding BST 1.0 (random)", 0);
  GlobalRebuildingBinarySearchTree<int, AugmentedBSTNode<int> > bst6(compare_int, 1.25);
  test_augmented_bst<AugmentedBSTNode<int> >(&bst6, "Global Rebuilding BST 1.0 (worst case)", 1);

  GlobalRebuildingBinarySearchTree<int, AugmentedBSTNode<int> > bst7(compare_int, 2.0);
  test_augmented_bst<AugmentedBSTNode<int> >(&bst7, "Global Rebuilding BST 2.0 (random)", 0);
  GlobalRebuildingBinarySearchTree<int, AugmentedBSTNode<int> > bst8(compare_int, 2.0);
  test_augmented_bst<AugmentedBSTNode<int> >(&bst8, "Global Rebuilding BST 2.0 (worst case)", 1);

  GlobalRebuildingBinarySearchTree<int, AugmentedBSTNode<int> > bst9(compare_int, 3.0);
  test_augmented_bst<AugmentedBSTNode<int> >(&bst9, "Global Rebuilding BST 3.0 (random)", 0);
  GlobalRebuildingBinarySearchTree<int, AugmentedBSTNode<int> > bst10(compare_int, 3.0);
  test_augmented_bst<AugmentedBSTNode<int> >(&bst10, "Global Rebuilding BST 3.0 (worst case)", 1);

  GlobalRebuildingBinarySearchTree<int, AugmentedBSTNode<int> > bst11(compare_int, 4.0);
  test_augmented_bst<AugmentedBSTNode<int> >(&bst11, "Global Rebuilding BST 4.0 (random)", 0);
  GlobalRebuildingBinarySearchTree<int, AugmentedBSTNode<int> > bst12(compare_int, 4.0);
  test_augmented_bst<AugmentedBSTNode<int> >(&bst12, "Global Rebuilding BST 4.0 (worst case)", 1);

  GlobalRebuildingBinarySearchTree<int, AugmentedBSTNode<int> > bst13(compare_int, 8.0);
  test_augmented_bst<AugmentedBSTNode<int> >(&bst13, "Global Rebuilding BST 8.0 (random)", 0);
  GlobalRebuildingBinarySearchTree<int, AugmentedBSTNode<int> > bst14(compare_int, 8.0);
  test_augmented_bst<AugmentedBSTNode<int> >(&bst14, "Global Rebuilding BST 8.0 (worst case)", 1);

  GlobalRebuildingBinarySearchTree<int, AugmentedBSTNode<int> > bst15(compare_int, 16.0);
  test_augmented_bst<AugmentedBSTNode<int> >(&bst15, "Global Rebuilding BST 16.0 (random)", 0);
  GlobalRebuildingBinarySearchTree<int, AugmentedBSTNode<int> > bst16(compare_int, 16.0);
  test_augmented_bst<AugmentedBSTNode<int> >(&bst16, "Global Rebuilding BST 16.0 (worst case)", 1);

  AugmentedBinarySearchTree<int, LogicalRemoveBSTNode<int> > bst17(compare_int);
  test_augmented_bst<LogicalRemoveBSTNode<int> >(&bst17, "Logical Remove BST (random)", 0);
  AugmentedBinarySearchTree<int, LogicalRemoveBSTNode<int> > bst18(compare_int);
  test_augmented_bst<LogicalRemoveBSTNode<int> >(&bst18, "Logical Remove BST (worst case)", 1);

  GlobalRebuildingBinarySearchTree<int, LogicalRemoveBSTNode<int> > bst19(compare_int, 4.0);
  test_augmented_bst<LogicalRemoveBSTNode<int> >(&bst19, "Global Rebuilding BST 4.0 with Logical Remove (random)", 0);
  GlobalRebuildingBinarySearchTree<int, LogicalRemoveBSTNode<int> > bst20(compare_int, 4.0);
  test_augmented_bst<LogicalRemoveBSTNode<int> >(&bst20, "Global Rebuilding BST 4.0 with Logical Remove (worst case)", 1);

  ScapegoatTree<int, ScapegoatTreeNode<int> > bst21(compare_int, 1.0);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst21, "Scapegoat Tree 1.0 (random)", 0);
  ScapegoatTree<int, ScapegoatTreeNode<int> > bst22(compare_int, 1.0);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst22, "Scapegoat Tree 1.0 (worst case)", 1);

  ScapegoatTree<int, ScapegoatTreeNode<int> > bst23(compare_int, 1.25);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst23, "Scapegoat Tree 1.25 (random)", 0);
  ScapegoatTree<int, ScapegoatTreeNode<int> > bst24(compare_int, 1.25);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst24, "Scapegoat Tree 1.25 (worst case)", 1);

  ScapegoatTree<int, ScapegoatTreeNode<int> > bst25(compare_int, 1.5);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst25, "Scapegoat Tree 1.5 (random)", 0);
  ScapegoatTree<int, ScapegoatTreeNode<int> > bst26(compare_int, 1.5);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst26, "Scapegoat Tree 1.5 (worst case)", 1);

  ScapegoatTree<int, ScapegoatTreeNode<int> > bst27(compare_int, 2.0);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst27, "Scapegoat Tree 2.0 (random)", 0);
  ScapegoatTree<int, ScapegoatTreeNode<int> > bst28(compare_int, 2.0);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst28, "Scapegoat Tree 2.0 (worst case)", 1);

  ScapegoatTree<int, ScapegoatTreeNode<int> > bst29(compare_int, 2.5);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst29, "Scapegoat Tree 2.5 (random)", 0);
  ScapegoatTree<int, ScapegoatTreeNode<int> > bst30(compare_int, 2.5);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst30, "Scapegoat Tree 2.5 (worst case)", 1);

  ScapegoatTree<int, ScapegoatTreeNode<int> > bst31(compare_int, 3.0);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst31, "Scapegoat Tree 3.0 (random)", 0);
  ScapegoatTree<int, ScapegoatTreeNode<int> > bst32(compare_int, 3.0);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst32, "Scapegoat Tree 3.0 (worst case)", 1);

  ScapegoatTree<int, ScapegoatTreeNode<int> > bst33(compare_int, 4.0);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst33, "Scapegoat Tree 4.0 (random)", 0);
  ScapegoatTree<int, ScapegoatTreeNode<int> > bst34(compare_int, 4.0);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst34, "Scapegoat Tree 4.0 (worst case)", 1);

  ScapegoatTree<int, ScapegoatTreeNode<int> > bst35(compare_int, 8.0);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst35, "Scapegoat Tree 8.0 (random)", 0);
  ScapegoatTree<int, ScapegoatTreeNode<int> > bst36(compare_int, 8.0);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst36, "Scapegoat Tree 8.0 (worst case)", 1);

  ScapegoatTree<int, ScapegoatTreeNode<int> > bst37(compare_int, 16.0);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst37, "Scapegoat Tree 16.0 (random)", 0);
  ScapegoatTree<int, ScapegoatTreeNode<int> > bst38(compare_int, 16.0);
  test_augmented_bst<ScapegoatTreeNode<int> >(&bst38, "Scapegoat Tree 16.0 (worst case)", 1);

  Treap<int, TreapNode<int> > bst39(compare_int);
  test_augmented_bst<TreapNode<int> >(&bst39, "Treap (random)", 0);
  Treap<int, TreapNode<int> > bst40(compare_int);
  test_augmented_bst<TreapNode<int> >(&bst40, "Treap (worst case)", 1);

  return 0;
}
