#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}


TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * map = (TreeMap *)malloc(sizeof(TreeMap));
    if (!map) {
        return NULL;
    }

    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;

    return map;
}
    
void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode * newNode = createTreeNode(key, value);

    if (tree->root == NULL) {
        tree->root = newNode;
        return;
    }

    TreeNode * current = tree->root;
    TreeNode * parent = NULL;

    while (current != NULL) {
        if (is_equal(tree, current->pair->key, key)) {
            current->pair->value = value;
            free(newNode->pair); 
            free(newNode); 
            return;
        }
        parent = current;

        if (tree->lower_than(newNode->pair->key, current->pair->key)) {
            if (current->left == NULL) {
                current->left = newNode;
                return;
            }
            current = current->left;
        } else {
            if (current->left == NULL) {
                current->left = newNode;
                return;
            }
            current = current->right;
        }
    }
}

void removeNode(TreeMap * tree, TreeNode* node) {
// Caso sin Hijos
    if (node->left == NULL && node->right == NULL) {
        if (node == node->parent->left) {
            // El nodo es un hijo izquierdo
            node->parent->left = NULL;
        } else {
            // El nodo es un hijo derecho
            node->parent->right = NULL;
        }
        free(node->pair); // Liberar memoria del par
        free(node); // Liberar memoria del nodo
    }
// Caso 1 Hijo
    else if (node->left == NULL || node->right == NULL)
    {
        TreeNode * child = (node->left != NULL) ? node->left : node->right;
        if (node == node->parent->left) 
            // El nodo es un hijo izquierdo
            node->parent->left = child;
        else 
            // El nodo es un hijo derecho
            node->parent->right = child;

        child->parent = node->parent;
        free(node->pair); // Liberar memoria del par
        free(node); // Liberar memoria del nodo
    }
// Caso 2 Hijos
    else {
        // Encontrar el sucesor (el mínimo del subárbol derecho)
        TreeNode * successor = minimum(node->right); 
        node->pair->key = successor->pair->key; // Copiar la clave del sucesor al nodo actual
        // Copiar el valor del sucesor al nodo actual
        node->pair->value = successor->pair->value;
        removeNode(tree, successor); // Eliminar el sucesor recursivamente
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) {
        return NULL; // No se puede avanzar si el árbol está vacío o current es NULL
    }

    TreeNode * current = tree->current;

    // Caso 1: Si el nodo tiene un hijo derecho, el siguiente nodo es el mínimo del subárbol derecho
    if (current->right != NULL) {
        current = minimum(current->right);
    } 
    else 
    {
    // Caso 2: Si el nodo no tiene hijo derecho, subir por el árbol hasta encontrar un padre cuyo hijo izquierdo sea el nodo actual
        TreeNode * parent = current->parent;
        while (parent != NULL && current == parent->right) {
            current = parent;
            parent = parent->parent;
        }
        current = parent;
    }

    // Actualizar el puntero current del árbol
    tree->current = current;

    // Retornar el par asociado al nodo encontrado
    if (current != NULL) {
        return current->pair;
    } else {
        return NULL; // Llegamos al final del árbol
    }
}
