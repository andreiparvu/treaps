// Mugurel Andreica

#ifndef __GRBST_H
#define __GRBST_H

#include <math.h>
#include <stdio.h>

#include "augmented_bst.h"
#include "linked_list.h"

//#define USE_LINKED_LIST

template<typename T, typename TreeNodeType> class GlobalRebuildingBinarySearchTree: public AugmentedBinarySearchTree<T, TreeNodeType> {
  public:
    using AugmentedBinarySearchTree<T, TreeNodeType>::compare;
    using AugmentedBinarySearchTree<T, TreeNodeType>::root;

    double gr_multiplier;

    GlobalRebuildingBinarySearchTree(int (*compare)(T, T), double gr_multiplier): AugmentedBinarySearchTree<T, TreeNodeType>(compare) {
      this->gr_multiplier = gr_multiplier;
    }

    virtual void insert_info(T x) {
      AugmentedBinarySearchTree<T, TreeNodeType>::insert_info(x);
      if (this->get_h() > get_maximum_allowed_h())
        rebuild_tree();
    }

    virtual void remove_info(T x) {
      AugmentedBinarySearchTree<T, TreeNodeType>::remove_info(x);
      if (this->get_h() > get_maximum_allowed_h())
        rebuild_tree();
    }

    // Intoarce inaltimea maxima acceptata pentru un arbore pana la care nu este necesara
    // refacerea totala a arborelui.
    int get_maximum_allowed_h() {
      return (int) ceil(gr_multiplier * sqrt(this->get_size()));
    }

    // Reconstruieste arborele ca fiind complet echilibrat.
    void rebuild_tree() {
      if (root != NULL)
        rebuild_sub_tree(root, -1.0);
    }

    // Reconstruieste in mod complet echilibrat subarborele cu radacina in nodul node.
    void rebuild_sub_tree(TreeNodeType *node, double extra_constructor_arg) {
#ifdef USE_LINKED_LIST
      LinkedList<T> ll(compare);
#else
      // Alocam spatiu pentru node->size elemente.
      // Numarul final de elemente s-ar putea sa fie mai mic, caci unele noduri
      // s-ar putea sa nu fie OK.
      T *elems = new T[node->size];
#endif

      TreeNodeType **new_sub_tree_root, *parent = node->parent;
      if (parent != NULL && node == parent->left_son)
        new_sub_tree_root = &(parent->left_son);
      else if (parent != NULL && node == parent->right_son)
        new_sub_tree_root = &(parent->right_son);
      else
        new_sub_tree_root = &root;

      int N = 0;

#ifdef USE_LINKED_LIST
      gather_info_in_order(node, &ll, &N);
#else
      gather_info_in_order(node, elems, &N);
#endif

      *new_sub_tree_root = NULL;

#ifdef USE_LINKED_LIST
      build_balanced_sub_tree(ll.pfirst, N, new_sub_tree_root, extra_constructor_arg);
#else
      build_balanced_sub_tree(elems, N, new_sub_tree_root, extra_constructor_arg);
      delete elems;
#endif

      if ((*new_sub_tree_root) != NULL) {
        if (parent != NULL)
          (*new_sub_tree_root)->parent = parent;
        (*new_sub_tree_root)->update_node_values(1);
      }
    }

    // Extrage toate elementele din subarborele nodului node, in ordine crescatoare,
    // si le adauga in lista ll.
#ifdef USE_LINKED_LIST
    void gather_info_in_order(TreeNodeType *node, LinkedList<T> *ll, int *N) {
#else
      void gather_info_in_order(TreeNodeType *node, T *elems, int *N) {
#endif
        if (node == NULL)
          return;

#ifdef USE_LINKED_LIST
        gather_info_in_order(node->left_son, ll, N);
#else
        gather_info_in_order(node->left_son, elems, N);			
#endif

        // Daca nodul e sters logic (vom discuta despre asta ulterior),
        // vom sari peste informatia din acest nod.
        if (!node->is_removed()) {
#ifdef USE_LINKED_LIST
          ll->addLast(node->info);
#else
          elems[*N] = node->info;
#endif
          (*N)++;
        }

#ifdef USE_LINKED_LIST
        gather_info_in_order(node->right_son, ll, N);
#else
        gather_info_in_order(node->right_son, elems, N);
#endif

        delete node;
      }

      // Construieste in mod echilibrat subarborele cu radacina in nodul *root
      // si avand ca valori primele N elemente din lista/vector, pornind de la
      // elementul pleft/inceputul vectorului.		
#ifdef USE_LINKED_LIST
      void build_balanced_sub_tree(struct list_elem<T> *pleft, int N, TreeNodeType **root, double extra_constructor_arg) {
#else
        void build_balanced_sub_tree(T *pleft, int N, TreeNodeType **root, double extra_constructor_arg) {
#endif
          if (N == 0)
            return;
          int M = N / 2;

#ifdef USE_LINKED_LIST
          struct list_elem<T> *pright = pleft;
          int i = 0;
          while (i < M) {
            pright = pright->next;
            i++;
          }
#else
          T *pright = pleft + M;
#endif

          // Daca sunt mai multe elemente egale cu elementul din mijloc,
          // din cauza conventiei stabilite, acestea trebuie sa se afle in subarborele stang.
          while (M < (N - 1) &&
#ifdef USE_LINKED_LIST
              compare(pright->info, pright->next->info) == 0) {
            pright = pright->next;
#else
            compare(*pright, *(pright + 1)) == 0) {
              pright++;
#endif
              M++;
            }

#ifdef USE_LINKED_LIST
            if (extra_constructor_arg < 0)
              *root = new TreeNodeType(compare, pright->info);
            else
              *root = new TreeNodeType(compare, pright->info, extra_constructor_arg);
            pright = pright->next;
#else
            if (extra_constructor_arg < 0)
              *root = new TreeNodeType(compare, *pright);
            else
              *root = new TreeNodeType(compare, *pright, extra_constructor_arg);
            pright++;
#endif

            build_balanced_sub_tree(pleft, M, &((*root)->left_son), extra_constructor_arg);
            build_balanced_sub_tree(pright, N - M - 1, &((*root)->right_son), extra_constructor_arg);

            if ((*root)->left_son != NULL)
              (*root)->left_son->parent = *root;
            if ((*root)->right_son != NULL)
              (*root)->right_son->parent = *root;

            (*root)->update_node_values(0);
          }
        };

#endif
