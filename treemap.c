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

    TreeNode * current = tree->root;
    TreeNode * parent = NULL;

    while (current != NULL) {
        if (is_equal(tree, current->pair->key, key)) 
        {
            free(newNode->pair); 
            free(newNode); 
            return; 
        }
        parent = current;

        if (tree->lower_than(newNode->pair->key, current->pair->key)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    
    if (tree->lower_than(newNode->pair->key, parent->pair->key)) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    newNode->parent = parent;

    
    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {

    if (node->left == NULL && node->right == NULL) {
        if (node == node->parent->left) {
            node->parent->left = NULL;
        } else {
            node->parent->right = NULL;
        }
        free(node->pair); 
        free(node); 
    }

    else if (node->left == NULL || node->right == NULL)
    {
        TreeNode * child = (node->left != NULL) ? node->left : node->right;
        if (node == node->parent->left) 
        
            node->parent->left = child;
        else 
            
            node->parent->right = child;

        child->parent = node->parent;
        free(node->pair); 
        free(node); 
    }
    else {
        
        TreeNode * successor = minimum(node->right); 
        node->pair->key = successor->pair->key; 
        
        node->pair->value = successor->pair->value;
        removeNode(tree, successor); 
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode * current = tree->root;

    while (current != NULL) {
        if (is_equal(tree, current->pair->key, key)) {
            tree->current = current;
            return current->pair;
        } else if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode * current = tree->root;
    TreeNode * ub_node = NULL;

    while (current != NULL) {
        if (is_equal(tree, current->pair->key, key)) {
            
            return current->pair;
        } else if (tree->lower_than(current->pair->key, key)) {
            current = current->right;
        } else {
            ub_node = current;
            current = current->left;
        }
    }
    if (ub_node != NULL) {
        return ub_node->pair;
    }

    
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    
    TreeNode * minNode = minimum(tree->root);

    if (minNode != NULL) {
        tree->current = minNode; 
        return minNode->pair;
    }

    return NULL; 
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) {
        return NULL; 
    }

    TreeNode * current = tree->current;

    
    if (current->right != NULL) {
        current = minimum(current->right);
    } 
    else 
    {
        TreeNode * parent = current->parent;
        while (parent != NULL && current == parent->right) {
            current = parent;
            parent = parent->parent;
        }
        current = parent;
    }

    
    tree->current = current;

    
    if (current != NULL) {
        return current->pair;
    } else {
        return NULL; 
    }
} 