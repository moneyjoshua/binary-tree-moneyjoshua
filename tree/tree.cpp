#include "tree.hpp"

using namespace std;

auto tree::insert(int val) -> tree_node * {
    // Код для добавления элемента в дерево
    
    // узел для добавления
    unique_ptr<tree_node> new_node(new tree_node);
    new_node->value = val;

    tree_node * n = root.get();
    tree_node * p = nullptr;
    // Поиск по val
    while(n != nullptr && n->value != val){
        if (val < n->value){
            p = n;
            n = n->left.get();
        } else {
            p = n;
            n = n->right.get();
        } 
    }
    // не нашли
    if (n == nullptr) {
        // 0 итераций
        if (p == nullptr) { 
            root = move(new_node);
            return root.get();
        }
        // >= 1 итерации
        else {
            // вставляем
            new_node->up = p;
            if (val < p->value) {
                p->left = move(new_node);
                return p->left.get();
            }
            if (val > p->value) {
                p->right = move(new_node);
                return p->right.get();
            }
        }
    // нашли, не вставляем
    } else {
        return n;
    }

    return nullptr;
}


auto tree::remove(int val) -> bool {
    // Код для удаления элемента из дерева

    // поиск узла по val
    tree_node * node = root.get();
    while(node != nullptr && node->value != val) {
        if (val < node->value) {
            node = node->left.get();
        }
        else if (val > node->value) {
            node = node->right.get();
        }
    }

    // Не нашли
    if (node == nullptr){
        return false;
    }
    // Нашли
    else {
        // У удаляемого узла 2 дочерних
        if (node->left != nullptr && node->right != nullptr) {
            tree_node *bottom = node->right.get();
            while (bottom->left != nullptr) {
                bottom = bottom->left.get();
            }
            int remember = bottom->value;
            remove(bottom->value);
            node->value = remember;
            return true;
        // У удаляемого узла нет дочерних
        } else if (node->left == nullptr && node->right == nullptr) {
            if (node != root.get()) {
                if (node->up->left.get() == node) {
                    node->up->left = nullptr;
                }
                if (node->up->right.get() == node) {
                    node->up->right = nullptr;
                }
                return true; 
            } else {
                root = nullptr;
                return true;
            }
        // У удаляемого узла 1 дочерний
        } else {
            // 1 дочерний узел
            unique_ptr<tree_node> under;
            if (node->left != nullptr) {
                under = move(node->left);                
            }
            if (node->right != nullptr) {
                under = move(node->right);
            }
            if (node == root.get()) {
                root = move(under);
                return true;
            }
            under->up = node->up;
            if (node->up->left->value == node->value){
                node->up->left = move(under);
            }
            if (node->up->right->value == node->value){
                node->up->right = move(under);
            }
            
            return true;
        }
    }
}