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

