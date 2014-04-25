// Mugurel Andreica

#ifndef __AUG_BST_H
#define __AUG_BST_H

#include <string.h>
#include <stdio.h>

#include "bst.h"

template<typename T> class AugmentedBSTNode: public BSTNode<T> {
  public:
    using BSTNode<T>::info;
    using BSTNode<T>::compare;

    // Pointeri catre fiul stanga, fiul dreapta si parinte.
    AugmentedBSTNode<T> *left_son, *right_son, *parent;

    // h = inaltimea subarborelui cu radacina in nodul curent.
    // size = numarul de noduri din subarborele cu radacina in nodul curent.
    int h, size;

    // Constructor.
    AugmentedBSTNode(int (*compare) (T, T), T info): BSTNode<T> (compare, info) {
      h = 1;
      size = 1;
      left_son = right_son = parent = NULL;
    }

    AugmentedBSTNode(int (*compare) (T, T), T info, double unused_constructor_arg): BSTNode<T> (compare, info) {
      h = 1;
      size = 1;
      left_son = right_son = parent = NULL;
    }

    // Insereaza informatia in arbore.
    virtual BSTNode<T>* insert_info(T x) {
      int next_son;
      BSTNode<T>* result;

      if (compare(x, info) <= 0)
        next_son = 0;
      else
        next_son = 1;

      if (next_son == 0) {  // left son
        if (left_son == NULL) {
          left_son = new AugmentedBSTNode<T>(compare, x);
          left_son->parent = this;
          result = left_son;
        } else
          result = left_son->insert_info(x);
      } else { // right son
        if (right_son == NULL) {
          right_son = new AugmentedBSTNode<T>(compare, x);
          right_son->parent = this;
          result = right_son;
        } else
          result = right_son->insert_info(x);
      }

      update_node_values(0);
      return result;
    }

    // Actualizeaza campurile h si size. Daca propagate_up este nenul, atunci se
    // apeleaza recursiv pentru toti stramosii nodului curent.
    void update_node_values(int propagate_up) {
      size = 1;
      h = 1;
      if (left_son != NULL) {
        size += left_son->size;
        h = 1 + left_son->h;
      }
      if (right_son != NULL) {
        size += right_son->size;
        if (1 + right_son->h > h)
          h = 1 + right_son->h;
      }
      if (propagate_up && parent != NULL)
        parent->update_node_values(propagate_up);
    }

    // Intoarce un pointer catre nodul ce contine informatia si NULL altfel.
    virtual AugmentedBSTNode<T>* find_info(T x) {
      if (compare(x, info) == 0)
        return this;

      if (compare(x, info) <= 0) {
        if (left_son != NULL)
          return left_son->find_info(x);
        else
          return NULL;
      } else {
        if (right_son != NULL)
          return right_son->find_info(x);
        else
          return NULL;
      }
    }

    // Sterge un element din arbore.
    // Intoarce 1 daca e stearsa radacina arborelui si 0 altfel.
    virtual int remove_info(T x) {
      AugmentedBSTNode<T> *t = find_info(x);
      if (t != NULL)
        return t->remove_self();
      else
        return 0;
    }

    // Intoarce 1 daca e stearsa radacina arborelui si 0 altfel.
    virtual int remove_self() {
      AugmentedBSTNode<T> *p;
      if (left_son == NULL && right_son == NULL) {
        if (parent == NULL) { // this == root
          delete this;
          return 1;
        } else {
          if (parent->left_son == this)
            parent->left_son = NULL;
          else
            parent->right_son = NULL;
          parent->update_node_values(1);
          delete this;
          return 0;
        }
      } else {
        if (left_son != NULL) {
          // Gaseste nodul cu cea mai mare valoare din subarborele stang.
          p = left_son;
          while (p->right_son != NULL)
            p = p->right_son;
        } else { // right_son != NULL
          // Gaseste nodul cu cea mai mica valoare din subarborele drept.
          p = right_son;
          while (p->left_son != NULL)
            p = p->left_son;
        }

        memcpy(&info, &(p->info), sizeof(T));
        return p->remove_self();
      }
    }

    // Afisaza statistici despre nodul curent.
    virtual void print_statistics() {
      fprintf(stderr, "Height: %d; Size: %d\n", h, size);
    }

    virtual int is_removed() {
      return 0;
    }

    virtual int get_h() {
      return h;
    }

    virtual int get_size() {
      return size;
    }
};

// T = tipul elementelor stocate in arbore.
// TreeNodeType = tipul nodurilor arborelui (de ex., AugmentedBSTNode<T>
// sau alte tipuri de noduri care implementeaza comportamente diferite).
template<typename T, typename TreeNodeType> class AugmentedBinarySearchTree: public BinarySearchTree<T, TreeNodeType> {
  public:
    using BinarySearchTree<T, TreeNodeType>::root;

    AugmentedBinarySearchTree(int (*compare) (T, T)): BinarySearchTree<T, TreeNodeType>(compare) {
    }

    // Intoarce inaltimea arborelui.
    virtual int get_h() {
      if (root != NULL)
        return root->get_h();
      return 0;
    }

    // Intoarce numarul de noduri din arbore.
    virtual int get_size() {
      if (root != NULL)
        return root->get_size();
      return 0;
    }

    // Afisaza statistici.
    virtual void print_statistics() {
      if (root == NULL)
        fprintf(stderr, "empty tree\n");
      else
        root->print_statistics();
    }
};

#endif
