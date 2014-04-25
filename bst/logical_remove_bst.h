// Mugurel Andreica

#ifndef __LRBST_H
#define __LRBST_H

#include <math.h>
#include "augmented_bst.h"

template<typename T> class LogicalRemoveBSTNode: public AugmentedBSTNode<T> {
  public:
    LogicalRemoveBSTNode<T> *left_son, *right_son, *parent;

    using AugmentedBSTNode<T>::info;
    using AugmentedBSTNode<T>::compare;
    using AugmentedBSTNode<T>::size;
    using AugmentedBSTNode<T>::h;

    // Este 1 daca informatia din nodul curent a fost steara logic si 0 altfel.
    char removed;

    // Numarul de noduri sterse logic din subarborele cu radacina in nodul curent.
    int num_removed;

    LogicalRemoveBSTNode(int (*compare)(T, T), T info): AugmentedBSTNode<T>(compare, info) {
      left_son = right_son = parent = NULL;
      removed = 0;
      num_removed = 0;
    }

    LogicalRemoveBSTNode(int (*compare)(T, T), T info, double unused_constructor_arg): AugmentedBSTNode<T>(compare, info) {
      left_son = right_son = parent = NULL;
      removed = 0;
      num_removed = 0;
    }

    // Insereaza informatia in arbore.
    virtual BSTNode<T>* insert_info(T x) {
      BSTNode<T>* result;

      if (removed && compare(info, x) == 0) {
        // Refolosim un nod ce a fost sters logic anterior si contine aceeasi valoare
        // ca si valoarea noua ce se doreste a fi inserata.
        removed = 0;
        LogicalRemoveBSTNode<T>* node = this;

        // Actualizeaza valorile num_removed ale nodului si ale tuturor stramosilor sai.
        while (node != NULL) {
          node->num_removed--;
          node = node->parent;
        }
        result = this;
      } else {
        int next_son;
        if (compare(x, info) <= 0)
          next_son = 0;
        else
          next_son = 1;

        if (next_son == 0) {  // left son
          if (left_son == NULL) {
            left_son = new LogicalRemoveBSTNode<T>(compare, x);
            left_son->parent = this;
            result = left_son;
          } else
            result = left_son->insert_info(x);
        } else { // right son
          if (right_son == NULL) {
            right_son = new LogicalRemoveBSTNode<T>(compare, x);
            right_son->parent = this;
            result = right_son;
          } else
            result = right_son->insert_info(x);
        }		
      }

      update_node_values(0);
      return result;
    }

    // Intoarce un pointer catre nodul de contine informatia si NULL altfel.
    virtual LogicalRemoveBSTNode<T>* find_info(T x) {
      if (!removed && compare(x, info) == 0)
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
      LogicalRemoveBSTNode<T> *t = find_info(x);
      if (t != NULL)
        return t->remove_self();
      else
        return 0;
    }

    void update_node_values(int propagate_up) {
      size = 1;
      h = 0;
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

    // Intoarce 1 daca e stearsa (fizic) radacina arborelui si 0 altfel.
    virtual int remove_self() {
      removed = 1;
      LogicalRemoveBSTNode<T>* node = this;

      // Actualizeaza valorile num_removed ale nodului si ale tuturor stramosilor sai.
      while (node != NULL) {
        node->num_removed++;
        node = node->parent;
      }
      return 0;
    }

    virtual int is_removed() {
      return removed;
    }

    // Afisaza statistici despre nodul curent.
    virtual void print_statistics() {
      fprintf(stderr, "Height: %d; Size: %d; Number of logically removed nodes: %d\n", h, size, num_removed);
    }
};

#endif
