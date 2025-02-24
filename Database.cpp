// AVL_Database.cpp
#include "AVL_Database.hpp"
#include <algorithm>
#include <cmath>

Record::Record(const std::string& k, int v) : key(k), value(v) {}

AVLNode::AVLNode(Record* r) : record(r), left(nullptr), right(nullptr), height(1) {}

AVLTree::AVLTree() : root(nullptr), nodeCount(0), compare(0) {}

int AVLTree::height(AVLNode* node) {
    return node ? node->height : 0;
}

void AVLTree::updateHeight(AVLNode* node) {
    if (node) {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }
}

int AVLTree::getBalance(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

/*
Can be used to balance AVL trees
*/
AVLNode *AVLTree::rotateRight(AVLNode *y) {
    // to do..
    AVLNode *x = y->left; // declaring x as left child of y
    AVLNode *subtree = x->right; // declaring subtree has the right child of x

    x->right = y; // making the right child of x now equal to y 
    y->left = subtree; // making the left child of y now the subtree

    // updating heights since the pointers were moved
    updateHeight(y); 
    updateHeight(x);

    return x; // return the updated node
}

/*
Can be used to balance AVL trees
*/
AVLNode *AVLTree::rotateLeft(AVLNode *x) {
    // to do..
    AVLNode *y = x->right; // declaring y as right child of x
    AVLNode *subtree = y->left; // declaring subtree has the leftt child of y

    y->left = x; // making the leftt child of y now equal to x 
    x->right = subtree; // making the right child of x now the subtree

    // updating heights since the pointers were moved
    updateHeight(x);
    updateHeight(y); 

    return y; // return the updated node
}

void AVLTree::insert(Record* record) {
    // to do..
    root = insertHelper(root, record); // calling the helper function to get the new root
}

// added function given in header file
AVLNode *AVLTree::insertHelper(AVLNode *node, Record *record) {
    // if there is no node
    if (node == nullptr) {
        nodeCount++; // update the count
        return new AVLNode(record); // and add the new node to the record
    }

    // if record is less than the current
    if (record->value < node->record->value) {
        node->left = insertHelper(node->left, record); // recusive call to go through left subtree
    // if record is greater than the current
    } else if (record->value > node->record->value) {
        node->right = insertHelper(node->right, record); // recusive call to go through right subtree
    // if record is equal to the current
    } else {
        return node; // just return, do not insert a duplicate
    }

    updateHeight(node); // update height of current node now that children were modified
    
    int bf = getBalance(node); // get the balance factor from tree to figure out which case to use

    // LL Case
    // if unbalanced to the left twice
    if (bf > 1 && record->value < node->left->record->value) {
        return rotateRight(node); // rotate the tree right
    }

    // LR Case
    // if unbalanced to the left and right
    if (bf > 1 && record->value > node->left->record->value) {
        node->left = rotateLeft(node->left); // rotate the left part of the tree
        return rotateRight(node); // then rotate the right part of the tree
    }

    // RR Case
    // if unbalanced to the right twice
    if (bf < -1 && record->value > node->right->record->value) {
        return rotateLeft(node); // rotate the tree left
    }

    // RL Case
    // if unbalanced to the right and left
    if (bf < -1 && record->value < node->right->record->value) {
        node->right = rotateRight(node->right); // rotate the right part of the tree
        return rotateLeft(node); // then rotate the left part of the tree
    }

    return node; // return updated root node
}

// added function given in header file
AVLNode *AVLTree::minValueNode(AVLNode *node) {
    AVLNode *current = node; // declaring a current pointer to node
    // while current has a left child
    while (current->left) {
        current = current->left; // move current to the left child
    }
    return current; // return current (leftmost/minimum value)
}

void AVLTree::deleteNode(const std::string &key, int value) {
    // to do ..
    root = deleteHelper(root, key, value); // calling the helper function to get the new root
}

// added function given in header file
AVLNode *AVLTree::deleteHelper(AVLNode *node, const std::string &key, int value) {
    // if tree is empty
    if (node == nullptr) {
        return nullptr; // return nullptr (nothing to delete)
    }

    // if the value is in the left subtree
    if (value < node->record->value) {
        node->left = deleteHelper(node->left, key, value); // recursively look for it in the left subtree
    // if the value is in the right subtree
    } else if (value > node->record->value) {
        node->right = deleteHelper(node->right, key, value); // recursively look for it in the right subtree 
    // if the value has been found
    } else {
        // if the node that the value is in has no left or right child
        if (!node->left || !node->right) {
            AVLNode *temp; // declare a temp pointer
            // if there is a left child
            if (node->left) {
                temp = node->left; // assign temp to it
            // if there is a right child
            } else {
                temp = node->right; // assign temp to it
            }

            // if there is neither left or right child
            if (!temp) {
                temp = node; // make temp the original node
                node = nullptr; // make node nullptr
            // if there is one child 
            } else {
                *node = *temp; // make node equal to that child
            }

            delete temp->record; // if no child, make temp the parent
            delete temp; // delete that parent
            nodeCount--; // and decrease node number of nodes
        
        // if the node that the value is in has a left or right child
        } else {
            AVLNode *temp = minValueNode(node->right); // find smallest node in right subtree
            node->record = temp->record; // make current node equal to the smallest one
            node->right = deleteHelper(node->right, temp->record->key, temp->record->value); // delete the repeated one
        }
    }

    updateHeight(node); // update the height

    int bf = getBalance(node); // get the balance factor from tree to figure out which case to use

    // LL Case
    // if unbalanced to the left twice
    if (bf > 1 && getBalance(node->left) >= 0) {
        return rotateRight(node); // rotate the tree right
    }

    // LR Case
    // if unbalanced to the left and right
    if (bf > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left); // rotate the left part of the tree
        return rotateRight(node); // rotate the tree right
    }

    // RR Case
    // if unbalanced to the right twice
    if (bf < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(node); // rotate the tree left
    }

    // RL Case
    // if unbalanced to the right and left
    if (bf < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right); // rotate the right part of the tree
        return rotateLeft(node); // rotate the tree left
    }

    return node; // return updated root node
}

Record* AVLTree::search(const std::string &key, int value) {
    // to do..
    AVLNode *result = searchHelper(root, key, value); // calling the searchHelper function to find result

    // if there was a match in the tree
    if (result) {
        return result->record; // return what was found
    // if there was no match in  the tree
    } else {
        return new Record("", 0); // returninf the given default record
    }
}

// added function given in header file
AVLNode* AVLTree::searchHelper(AVLNode *node, const std::string &key, int value) const {
    compare = 0; // sets a compare value to 0
    
    // if there is nothing in the tree
    if (node == nullptr) {
        return nullptr; // return nullptr (nothing to search)
    }

    compare++; // increase the compare value
    // if value is less than the current node
    if (value < node->record->value) {
        return searchHelper(node->left, key, value); // recursively call searchHelper to look at left subtree for value
    // if value is greater than the current node
    } else if (value > node->record->value) {
        return searchHelper(node->right, key, value); // recursively call searchHelper to look at right subtree for value
    // if value is equal to the current node
    } else {
        return node; // return that node
    }
}

// IndexedDatabase Implementation
void IndexedDatabase::insert(Record* record) {
    index.insert(record);
}

Record* IndexedDatabase::search(const std::string& key, int value) {
    return index.search(key, value);
}

void IndexedDatabase::deleteRecord(const std::string& key, int value) {
    index.deleteNode(key, value);
}

/* RangeQuery Hints
1. Base: if (!node) return
2. Key Traversal Logic:
   - If value >= start: check left subtree
   - If start <= value <= end: add to result
   - If value <= end: check right subtree
*/

std::vector<Record*> IndexedDatabase::rangeQuery(int start, int end) {
    //to do..
    std::vector<Record*> result;
    rangeQueryHelper(index.root, start, end, result); // calling the helper function to get result
    return result; // return the result from the helper function
}

// added function given in header file
void IndexedDatabase::rangeQueryHelper(AVLNode *node, int start, int end, std::vector<Record*> &result) const {
    // Given base case in the RangeQueryHints
    if (!node) return;

    // If value >= start: check left subtree
    if (node->record->value >= start) {
        rangeQueryHelper(node->left, start, end, result); // recursive call to check left subtree
    }

    // If start <= value <= end: add to result
    if (start <= node->record->value && node->record->value <= end) {
        result.push_back(node->record); // push that node to the result
    }

    // If value <= end: check right subtree
    if (node->record->value <= end) {
        rangeQueryHelper(node->right, start, end, result); // recursive call to check right subtree
    }
}

void IndexedDatabase::clearHelper(AVLNode* node) {
    if (!node) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node->record;
    delete node;
}

void IndexedDatabase::clearDatabase() {
    clearHelper(index.root);
    index.root = nullptr;
}

int IndexedDatabase::calculateHeight(AVLNode* node) const {
    if (!node) return 0;
    return 1 + std::max(calculateHeight(node->left), calculateHeight(node->right));
}

int IndexedDatabase::getTreeHeight() const {
    return calculateHeight(index.root);
}

int IndexedDatabase::getSearchComparisons(const std::string& key, int value) {
    search(key, value);
    return index.getLastSearchComparisons();
}
