// Mugurel Andreica

#include "augmented_bst.h"

template<typename T> class TreapNode: public AugmentedBSTNode<T> {
  public:
    using AugmentedBSTNode<T>::info;
    using AugmentedBSTNode<T>::compare;
    using AugmentedBSTNode<T>::size;
    using AugmentedBSTNode<T>::h;

    TreapNode<T> *left_son, *right_son, *parent;
    int prio;

    TreapNode(int (*compare)(T, T), T info): AugmentedBSTNode<T>(compare, info) {
      left_son = right_son = parent = NULL;
      // Choose a random priority.
      prio = rand();
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
          left_son = new TreapNode<T>(compare, x);
          left_son->parent = this;
          left_son->push_up();
          result = left_son;
        } else
          result = left_son->insert_info(x);
      } else { // right son
        if (right_son == NULL) {
          right_son = new TreapNode<T>(compare, x);
          right_son->parent = this;
          right_son->push_up();
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

    void push_up() {
      while (parent != NULL) {
        if (prio > parent->prio) {
          TreapNode<T>* gparent = parent->parent;
          if (parent->left_son == this) {
            // rotation from left to right.
            TreapNode<T>* rson = right_son;
            parent->left_son = rson;
            if (rson != NULL)
              rson->parent = parent;
            right_son = parent;
          } else {
            // rotation from right to left.
            TreapNode<T> *lson = left_son;
            parent->right_son = lson;
            if (lson != NULL)
              lson->parent = parent;
            left_son = parent;
          }
          parent->parent = this;

          if (gparent != NULL) {
            if (gparent->left_son == parent)
              gparent->left_son = this;
            else
              gparent->right_son = this;
          }

          parent->update_node_values(0);
          this->update_node_values(0);

          parent = gparent;
        } else
          break;
      }
    }

    // Intoarce un pointer catre nodul ce contine informatia si NULL altfel.
    virtual TreapNode<T>* find_info(T x) {
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
      TreapNode<T> *t = find_info(x);
      if (t != NULL)
        return t->remove_self();
      else
        return 0;
    }

    // Intoarce 1 daca e stearsa radacina arborelui si 0 altfel.
    virtual int remove_self() {
      TreapNode<T> *p;
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
};

template<typename T, typename TreeNodeType> class Treap: public AugmentedBinarySearchTree<T, TreeNodeType> {
  public:
    using AugmentedBinarySearchTree<T, TreeNodeType>::compare;
    using AugmentedBinarySearchTree<T, TreeNodeType>::root;

    Treap(int (*compare) (T, T)): AugmentedBinarySearchTree<T, TreeNodeType>(compare) {
    }

    virtual void insert_info(T x) {
      if (root == NULL)
        root = new TreeNodeType(compare, x);
      else {
        root->insert_info(x);
        while (root->parent != NULL)
          root = root->parent;
      }
    }
};
