// NOTE: Per assignment rules, only this single include is allowed here.
#include "VectorStore.h"

// =====================================
// Helper functions
// =====================================

// Helper function to print n spaces for tree visualization
void printNSpace(int n) {
    for (int i = 0; i < n; i++) {
        cout << " ";
    }
}

// =====================================
// AVLTree<K, T> implementation
// =====================================

template <class K, class T>
void AVLTree<K, T>::printTreeStructure() const
{
    int height = this->getHeight();
    if (this->root == NULL)
    {
        cout << "NULL\n";
        return;
    }
    queue<AVLNode *> q;
    q.push(root);
    AVLNode *temp;
    int count = 0;
    int maxNode = 1;
    int level = 0;
    int space = pow(2, height);
    printNSpace(space / 2);
    while (!q.empty())
    {
        temp = q.front();
        q.pop();
        if (temp == NULL)
        {
            cout << " ";
            q.push(NULL);
            q.push(NULL);
        }
        else
        {
            cout << temp->data;
            q.push(temp->pLeft);
            q.push(temp->pRight);
        }
        printNSpace(space);
        count++;
        if (count == maxNode)
        {
            cout << endl;
            count = 0;
            maxNode *= 2;
            level++;
            space /= 2;
            printNSpace(space / 2);
        }
        if (level == height)
            return;
    }
}

// TODO: Implement all AVLTree<K, T> methods here
template<class K, class T> 
typename AVLTree<K,T>::AVLNode*  AVLTree<K, T>::rotateRight(typename AVLTree<K, T>::AVLNode* &node){
    typename AVLTree<K,T>::AVLNode* tempPtr = node->pLeft;
    node->pLeft = tempPtr->pRight;
    tempPtr->pRight = node;
    return tempPtr; 
}

template<class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K,T>::rotateLeft(typename AVLTree<K,T>::AVLNode*& node){
    typename AVLTree<K,T>::AVLNode* tempPtr = node->pRight;
    node->pRight = tempPtr->pLeft;
    tempPtr->pLeft = node;
    return tempPtr;
}
/////public method:
template<class K, class T>
AVLTree<K, T>::AVLTree():root(nullptr){}

template<class K, class T>
void AVLTree<K,T>::destroy(typename AVLTree<K,T>::AVLNode* node) {
    if (!node) return;
    destroy(node->pLeft);
    destroy(node->pRight);
    delete node;
}

template<class K, class T>
AVLTree<K, T>::~AVLTree() {
    destroy(root);
}

///////
template<class K, class T>
const typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::findInside(const typename AVLTree<K,T>::AVLNode* node, const K& key) const{
    if(!node) return nullptr;
    if(node->key == key) return node;
    else if(key < node->key){
        return findInside(node -> pLeft,key);
    }
    else{
        return findInside(node->pRight, key);
    }
}
template<class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::findInside(typename AVLTree<K,T>::AVLNode* node, const K& key) {
    if(!node) return nullptr;
    if(node->key == key) return node;
    else if(key < node->key){
        return findInside(node -> pLeft,key);
    }
    else{
        return findInside(node->pRight, key);
    }
}
template<class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::LeftBalance(typename AVLTree<K,T>::AVLNode* &node, bool& taller){
    AVLTree<K,T>::AVLNode* lefttree = node->pLeft;
    if(lefttree->balance == LH){
        node->balance = EH;
        lefttree->balance = RH;
        node = rotateRight(node);
        taller = false;
    }
    else{
        AVLTree<K,T>::AVLNode* righttree = lefttree -> pRight;
        if(righttree->balance == LH){
            node->balance = RH;
            lefttree -> balance = EH;
            
        }
        else if(righttree->balance ==EH){
            lefttree->balance = EH;
            node->balance = EH;
        }
        else{
            node->balance = EH;
            lefttree->balance = LH;
      
        }
        righttree->balance = EH;
        node->pLeft = rotateLeft(lefttree);
        node = rotateRight(node);
        taller = false;
    }
    return node;
}
template<class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::RightBalance(typename AVLTree<K,T>::AVLNode* &node, bool& taller){
    AVLTree<K,T>::AVLNode* righttree = node->pRight;
    if(righttree ->balance == RH){
        node->balance = EH;
        righttree ->balance = EH;
        node = rotateLeft(node);
        taller = false;
    }
    else{
        AVLTree<K,T>::AVLNode* lefttree = righttree->pLeft;
        if(lefttree->balance == RH){
            node->balance = LH;
            righttree ->balance = EH;
        }
        else if(lefttree->balance == EH){
            righttree->balance = EH;
            node->balance = EH;
        }
        else{
            node->balance =EH;
            righttree -> balance = RH;
        }
        lefttree ->balance = EH;
        node->pRight = rotateRight(righttree);
        node = rotateLeft(node);
        taller = false;
    }
    return node;
}
template<class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::AVLInsert(typename AVLTree<K,T>::AVLNode* &node, typename AVLTree<K,T>::AVLNode* newPtr, bool& taller){
    if(!node){
        node = newPtr;
        taller = true;
        return node;
    }
    if(newPtr-> key < node->key){
        node->pLeft = AVLInsert(node->pLeft, newPtr, taller);
        if(taller){
            if(node->balance == LH){
                node = LeftBalance(node,taller);
            }
            else if(node->balance == EH){
                node->balance = LH;
            }
            else{
                node->balance = EH;
                taller = false;
            }
        }
    }
    else{
        node->pRight = AVLInsert(node->pRight, newPtr, taller);
        if(taller){
            if(node->balance == LH){
                node->balance = EH;
                taller = false;
            }
            else if(node->balance == EH){
                node->balance = RH;
            }
            else{
                node = RightBalance(node, taller);
            }
        }
    }
    return node;
}
template<class K, class T>
void AVLTree<K,T>::insert(const K& key, const T& value){
    if (contains(key)) return;
    if(!root){
        root = new AVLNode(key, value);
        return;
    }
    typename AVLTree<K,T>::AVLNode* newPtr = new AVLNode(key, value);
    bool taller = false;
    root = AVLInsert(root, newPtr, taller);
}
////remove
template<class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::deleteRightBalance(typename AVLTree<K,T>::AVLNode* &node, bool &shorter){
    if(node->balance == LH){
        node->balance = EH;
    }
    else if(node->balance == EH){
        node->balance = RH;
        shorter = false;
    }
    else{
        AVLTree<K,T>::AVLNode* rightTree = node->pRight;
        if(rightTree->balance == LH){
            AVLTree<K,T>::AVLNode* leftTree = rightTree->pLeft;
            if(leftTree -> balance == LH){
                node->balance = EH;
                return node;
               
            }
            else if(leftTree -> balance == EH ){
                node->balance = LH;
                rightTree->balance = EH;
            }
            else{
                node->balance = LH;
                rightTree->balance = EH;
                
            }
            leftTree->balance = EH;
            node->pRight = rotateRight(rightTree);
            node = rotateLeft(node);
        }
        else{
            if(rightTree -> balance != EH){
                node->balance = EH;
                rightTree -> balance = EH;
            }
            else{
                node->balance = EH;
                rightTree -> balance = LH;
                shorter = false;
            }
            node = rotateLeft(node);
        }
    }
    return node;
}
template<class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::deleteLeftBalance(typename AVLTree<K,T>::AVLNode* &node, bool &shorter){
    if(node->balance == RH){
        node->balance = EH;
    }
    else if(node->balance == EH){
        node->balance = LH;
        shorter = false;
    }
    else{
        AVLTree<K,T>::AVLNode* leftTree = node->pLeft;
        if(leftTree-> balance == RH){
            AVLTree<K,T>::AVLNode* rightTree = leftTree -> pRight;
            if(rightTree->balance == RH){
                leftTree -> balance = LH;
                node->balance = EH;
        
            }
            else if(rightTree->balance ==EH){
                node-> balance = RH;
                leftTree->balance = EH;
            }
            else{
                node->balance = RH;
                leftTree -> balance = EH;
            }
            
            rightTree -> balance = EH;
            node->pLeft = rotateLeft(leftTree);
            node = rotateRight(node);
        }
        else{
            if(leftTree->balance != EH ){
                node->balance = EH;
                leftTree->balance = EH;
            }
            else{
                node->balance = LH;
                leftTree -> balance = RH;
                shorter = false;
            }
            node = rotateRight(node);
        }
    }
    return node;
}
template<class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::AVLDelete(typename AVLTree<K,T>::AVLNode* &node, const K& deleteKey, bool &shorter, bool& completed){
    if(!node){
        shorter = false;
        completed = false;
        return nullptr;
    }
    if(deleteKey < node->key){
        node->pLeft = AVLDelete(node->pLeft, deleteKey, shorter, completed);
        if(shorter){
            node = deleteRightBalance(node,shorter);
        }

    }
    else if(deleteKey > node->key){
        node->pRight = AVLDelete(node->pRight, deleteKey, shorter, completed);
        if(shorter){
            node = deleteLeftBalance(node, shorter);
        }
    }
    else{
        AVLTree<K,T>::AVLNode* deleteNode = node;
        if(node->pRight == nullptr){
            AVLTree<K,T>::AVLNode* newRoot = node->pLeft;
            completed = true;
            shorter = true;
            delete deleteNode;
            return newRoot;
        }
        else if(node->pLeft == nullptr){
            AVLTree<K,T>::AVLNode* newRoot = node->pRight;
            completed = true;
            shorter = true;
            delete deleteNode;
            return newRoot;
        }
        else{
            AVLTree<K,T>::AVLNode* exchangePtr = node->pLeft;
            while(exchangePtr -> pRight != nullptr){
                exchangePtr = exchangePtr->pRight;
            }
            node-> data =  exchangePtr->data;
            node->pLeft = AVLDelete(node->pLeft, exchangePtr -> key, shorter, completed);
            if(shorter){
                node = deleteRightBalance(node, shorter);
            }
        }
    }
    return node;
}

template<class K, class T>
void AVLTree<K,T>::remove(const K& key){
    if(!root || findInside(root, key) == nullptr) return;
    bool shorter;
    bool completed;
    AVLDelete(root, key, shorter, completed);
}
template<class K, class T>
int AVLTree<K,T>::findMax(int a, int b) const{
    if(a>=b) return a;
    return b;
}

template<class K, class T>
bool AVLTree<K, T>::contains(const K& key) const{
    if(!findInside(root, key)) return false;
	return true;
}
///
template<class K, class T>
int AVLTree<K, T>::getHeightMax(typename AVLTree<K,T>::AVLNode* node) const{
    if(!node) return 0;
    int l = getHeightMax(node->pLeft);
    int r = getHeightMax(node->pRight);
    return 1 + findMax(l,r);
}
template<class K, class T>
int AVLTree<K, T>::getHeight() const{
    return getHeightMax(root);
}
template<class K, class T>
int AVLTree<K,T>::getSizeMax(typename AVLTree<K,T>::AVLNode* node) const{ 
    if(!node) return 0;
    int l = getSizeMax(node->pLeft);
    int r = getSizeMax(node->pRight);
    return l+r+1;
}
template<class K, class T>
int AVLTree<K,T>::getSize() const{
    return getSizeMax(root);
}
template<class K, class T>
bool AVLTree<K, T>::empty() const{
    return root == nullptr;
}
template<class K, class T>
void AVLTree<K, T>::clear(){
    destroy(root);
    root = nullptr;
}
template<class K, class T>
void AVLTree<K,T>::printLevelOrder(AVLTree<K,T>::AVLNode* node) const{
    if(!node) return;
    queue<AVLTree<K,T>::AVLNode*>q;
    q.push(node);
    while(!q.empty()){
        AVLTree<K,T>::AVLNode* curr = q.front();
        q.pop();
        cout<< curr->data;
        if(curr->pLeft){
            q.push(curr->pLeft);
        }
        if(curr->pRight){
            q.push(curr->pRight);
        }
    }
}
template<class K, class T>
void AVLTree<K,T>::inorderTraversal(AVLTree<K,T>::AVLNode* node, void (*action)(const T&)) const{
    if(!node) return;
    inorderTraversal(node->pLeft, action);
    action(node->data);
    inorderTraversal(node->pRight, action);
}
template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::getInorderDataAtIdxHelper(AVLNode* node,int tarIdx,int& curIdx) const {
    if (!node) return NULL;

    AVLNode* left = getInorderDataAtIdxHelper(node->pLeft, tarIdx, curIdx);
    if (left) return left;

    if (curIdx == tarIdx) {
        return node;
    }
    curIdx++;

    return getInorderDataAtIdxHelper(node->pRight, tarIdx, curIdx);
}

template <class K, class T>
T& AVLTree<K, T>::getInorderDataAtIdx(int tarIdx){
    int curIdx = 0;
    return getInorderDataAtIdxHelper(root, tarIdx, curIdx)->data;
}
template <class K, class T>
const T& AVLTree<K, T>::getInorderDataAtIdx(int tarIdx) const {
    int curIdx = 0;
    return getInorderDataAtIdxHelper(root, tarIdx, curIdx)->data;
}


template<class K, class T>
void AVLTree<K, T>::inorderTraversal(void (*action)(const T&)) const{
    inorderTraversal(root, action);
}
// =====================================
// RedBlackTree<K, T> implementation
// =====================================

template <class K, class T>
void RedBlackTree<K, T>::printTreeStructure() const
{
    if (this->root == nullptr)
    {
        cout << "NULL\n";
        return;
    }

    queue<RBTNode *> q;
    q.push(root);
    RBTNode *temp;
    int count = 0;
    int maxNode = 1;
    int level = 0;

    // Calculate tree height for spacing
    int height = 0;
    queue<RBTNode *> heightQ;
    heightQ.push(root);
    while (!heightQ.empty())
    {
        int size = heightQ.size();
        height++;
        for (int i = 0; i < size; i++)
        {
            RBTNode *node = heightQ.front();
            heightQ.pop();
            if (node->left)
                heightQ.push(node->left);
            if (node->right)
                heightQ.push(node->right);
        }
    }

    int space = pow(2, height);
    printNSpace(space / 2);

    while (!q.empty())
    {
        temp = q.front();
        q.pop();

        if (temp == nullptr)
        {
            cout << "NULL";
            q.push(nullptr);
            q.push(nullptr);
        }
        else
        {
            // Print node data and color as tuple: (data, color)
            cout << "(" << temp->data << ",";
            if (temp->color == RED)
            {
                cout << "R)";
            }
            else
            {
                cout << "B)";
            }

            q.push(temp->left);
            q.push(temp->right);
        }

        printNSpace(space);
        count++;

        if (count == maxNode)
        {
            cout << endl;
            count = 0;
            maxNode *= 2;
            level++;
            space /= 2;
            printNSpace(space / 2);
        }

        if (level == height)
        {
            return;
        }
    }
}

// TODO: Implement all other RedBlackTree<K, T> methods here
template<class K, class T>
RedBlackTree<K,T>::RBTNode::RBTNode(const K& key, const T& value)
:key(key),data(value),color(RED),parent(nullptr), left(nullptr),right(nullptr){}
template<class K, class T>
void RedBlackTree<K,T>::RBTNode::recolorToRed(){
    this -> color = RED;
}
template<class K, class T>
void RedBlackTree<K,T>::RBTNode::recolorToBlack(){
    this -> color = BLACK;
}

template<class K, class T>
void RedBlackTree<K,T>::rotateLeft(typename RedBlackTree<K,T>::RBTNode* node){
    if(!node) return;
    RBTNode* otherside = node->right;
    node->right = otherside ->left;
    if(otherside->left != nullptr){
        otherside->left->parent = node;
    }
    otherside-> parent = node->parent;
    if(node->parent == nullptr){
        root = otherside;
    }
    else if(node == node->parent->left){
        node->parent->left = otherside;
    }
    else{
        node->parent->right = otherside;
    }
    otherside->left = node;
    node->parent = otherside;

}
template<class K, class T>
void RedBlackTree<K,T>::rotateRight(typename RedBlackTree<K,T>::RBTNode* node){
    if(!node) return;
    RBTNode* otherside = node->left;
    node->left = otherside->right;
    if(otherside->right != nullptr){
        otherside -> right->parent = node;
    }
    otherside->parent = node->parent;
    if(node->parent == nullptr){
        root = otherside;
    }
    else if(node == node->parent->right){
        node->parent->right = otherside;
    }
    else{
        node->parent->left = otherside;

    }
    otherside->right = node;
    node->parent = otherside;
} 
//  
template<class K, class T>
typename RedBlackTree<K,T>::RBTNode* RedBlackTree<K,T>::lowerBoundNode(const K& key) const{
    RBTNode* curr = root;
    RBTNode* candidate=nullptr;
    while (curr != nullptr) {
        if (curr->key >= key) {
            candidate = curr;
            curr = curr->left;
        } 
        else {
            curr = curr->right;
        }
    }
    return const_cast<RBTNode*>(candidate);
}

template<class K, class T>
typename RedBlackTree<K,T>::RBTNode* RedBlackTree<K,T>::upperBoundNode(const K& key) const{
    RBTNode* curr = root;
    RBTNode* candidate=nullptr;
    while (curr != nullptr) {
        if (curr->key > key) {
            candidate = curr;
            curr = curr->left;
        } 
        else {
            curr = curr->right;
        }
    }
    return const_cast<RBTNode*>(candidate);
}
template<class K,class T>
RedBlackTree<K,T>::RedBlackTree(){
    this->root = nullptr;
}

template<class K,class T>
void RedBlackTree<K,T>::destroy(RBTNode* node){
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}
template<class K,class T>
RedBlackTree<K,T>::~RedBlackTree(){
    destroy(root);
}

template<class K,class T>
bool RedBlackTree<K,T>::empty() const{
    return root == nullptr;
}

template<class K,class T>
int RedBlackTree<K,T>::helpMeFindHeight(RBTNode* node) const{
    if(!node) return 0;
    return 1 + helpMeFindHeight(node->left) + helpMeFindHeight(node->right);
}
template<class K,class T>
int RedBlackTree<K,T>::size() const{
    return helpMeFindHeight(root);
}

template<class K,class T>
void RedBlackTree<K,T>::clear(){
    destroy(root);
    root = nullptr;
}
//only int
template<class K, class T>
typename RedBlackTree<K,T>::RBTNode* RedBlackTree<K,T>::findInside(RBTNode* node, const K& key) const{
    if(!node) return nullptr;
    if(node->key == key) return node;
    else if(key < node->key){
        return findInside(node -> left,key);
    }
    else{
        return findInside(node->right, key);
    }
    return node;
}
//for other 
template<class K, class T>
const typename RedBlackTree<K,T>::RBTNode* RedBlackTree<K,T>::findInside(const RBTNode* node, const K& key) const{
    if(!node) return nullptr;
    if(node->key == key) return node;
    else if(key < node->key){
        return findInside(node -> left,key);
    }
    else{
        return findInside(node->right, key);
    }
    return node;
}
template<class K,class T>
void RedBlackTree<K,T>::RBT_fix_up(RBTNode* node){
    while(node!= root && node->parent && node->parent->color == RED){
        if(node->parent->parent && node->parent == node->parent->parent->left){
            typename RedBlackTree<K,T>::RBTNode* uncle = node->parent->parent->right;
            if(uncle && uncle->color == RED){// case 1
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else{
                if(node == node->parent->right){//case 2
                    node = node->parent;
                    rotateLeft(node);
                }
                //case 3 then
                if(node->parent && node->parent->parent){
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotateRight(node->parent->parent);
                }
            }
        }
        else if(node->parent->parent && node->parent){
            typename RedBlackTree<K,T>::RBTNode* uncle = node->parent->parent->left;
            if(uncle && uncle->color == RED){
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else{
                if(node == node->parent->left){//case 2
                    node = node->parent;
                    rotateRight(node);
                }
                //case 3 then
                if(node->parent && node->parent->parent){
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotateLeft(node->parent->parent);
                }
            }
        }
        
    }
    root->color = BLACK;
}
template<class K,class T>
void RedBlackTree<K,T>::insert(const K& key, const T& value){
    if(contains(key)) return;
    if(!root){
        root = new RBTNode(key, value);
        root->color = BLACK;
        return;
    } 
    RBTNode* newNode = new RBTNode(key, value);
    RBTNode* y = nullptr;
    RBTNode* x = this->root;
    while(x!= nullptr){
        y = x;
        if(key< x->key){
            x=x->left;
        }
        else{
            x=x->right;
        }
    }
    newNode->parent = y;
    if(y==nullptr){
        x = newNode;

    }   
    else if(newNode->key < y->key){
        y->left = newNode;
    } 
    else{
        y->right = newNode;
    }
    RBT_fix_up(newNode);
}
//helper for deletion
template<class K, class T>
typename RedBlackTree<K,T>::RBTNode* RedBlackTree<K,T>::getSibling(RBTNode* node){
    if (!node || node->parent == nullptr) return nullptr;
    return (node == node->parent->left) ? node->parent->right : node->parent->left;
}
template<class K, class T>
bool RedBlackTree<K,T>::isLeaf(RBTNode* &node){
    return node && node->left == nullptr && node->right == nullptr;
}
template<class K, class T>
void RedBlackTree<K,T>::replaceNode(RBTNode* oldNode, RBTNode* newNode) {
    if (oldNode->parent == nullptr) {
        root = newNode;
    } 
    else if (oldNode == oldNode->parent->left) {
        oldNode->parent->left = newNode;
    } 
    else {
        oldNode->parent->right = newNode;
    }

    if (newNode != nullptr) {
        newNode->parent = oldNode->parent;
    }
}
template<class K, class T>
void RedBlackTree<K,T>::deleteOneChild(RBTNode* &deleteNode){
    RBTNode* child = (deleteNode->left != nullptr) ? deleteNode->left : deleteNode->right;
    if (deleteNode->color == BLACK) {
        if (child && child->color == RED) {
            child->color = BLACK;
            replaceNode(deleteNode, child);
        } 
        else if(child){
            replaceNode(deleteNode, child);
            deleteCase1(child);
        }
        else{
            deleteCase1(deleteNode);
            replaceNode(deleteNode, nullptr);
        }
    }
    else{
        replaceNode(deleteNode, child);
    }
    delete deleteNode;
    deleteNode = nullptr;
}
///6 cases
template<class K, class T>
void RedBlackTree<K,T>::deleteCase1(RBTNode* node){
    if(!node) return;
    if(node->parent == nullptr){
        node->color = BLACK;
        return;
    }
    deleteCase2(node);
}
template<class K, class T>
void RedBlackTree<K,T>::deleteCase2(RBTNode* node){
    if(!node) return; 
    RBTNode* sibling = getSibling(node);
    if(!sibling) {  
        deleteCase3(node);
        return;
    }
    if(sibling->color == RED){
        node->parent->color = RED;
        sibling->color = BLACK;
        if(node == node->parent->left) rotateLeft(node->parent);
        else rotateRight(node->parent);
        sibling = getSibling(node);
    }
    deleteCase3(node);
}
template<class K, class T>
void RedBlackTree<K,T>::deleteCase3(RBTNode* node){
    if(!node || !node->parent) return;
    RBTNode* parent = node->parent;
    RBTNode* sibling = getSibling(node);
    if(!sibling) {
        deleteCase4(node);
        return;
    }
    //check dung case 3 khong?
    bool siblingLeftBlack  = (sibling->left  == nullptr 
        || sibling->left->color  == BLACK);
    bool siblingRightBlack = (sibling->right == nullptr 
        || sibling->right->color == BLACK);

    if(parent->color == BLACK && sibling->color == BLACK && siblingLeftBlack 
    && siblingRightBlack){
        sibling->color = RED;
        deleteCase1(node->parent);
    }
    else {
        deleteCase4(node);
    }
}
template<class K, class T>
void RedBlackTree<K,T>::deleteCase4(RBTNode* node){
    if(!node || !node->parent) return;
    RBTNode* sibling = getSibling(node);
    if(!sibling) return;
    if (node->parent->color == RED && sibling->color == BLACK &&(sibling->left  == nullptr 
        || sibling->left->color  == BLACK) && (sibling->right == nullptr || sibling->right->color == BLACK)) {
        sibling->color = RED;
        node->parent->color = BLACK;
        return;                                 // TERMINAL
    } 
    else deleteCase5(node);
}
template<class K, class T>
void RedBlackTree<K,T>::deleteCase5(RBTNode* node){
    if(!node || !node->parent) return;
    RBTNode* sibling = getSibling(node);
    if(!sibling) return;
    if(sibling->color == BLACK){
        if(node == node->parent->left && (sibling->right == nullptr || sibling->right->color == BLACK) && 
        sibling->left && sibling->left->color == RED){
            sibling->color = RED;
            if(sibling->left) sibling->left->color = BLACK;
            rotateRight(sibling);
            sibling = getSibling(node);
        }
        else if(node == node->parent->right && (sibling->left == nullptr 
        || sibling->left->color == BLACK) && sibling->right &&  sibling->right->color == RED){
            sibling->color = RED;
            if(sibling->right) sibling->right->color = BLACK;
            rotateLeft(sibling);
            sibling = getSibling(node);
        }
    }
    deleteCase6(node);
}
template<class K, class T>
void RedBlackTree<K,T>::deleteCase6(RBTNode* node){
    if(!node || !node->parent) return;
    RBTNode* sibling = getSibling(node);
    if(!sibling) return;
    sibling->color = node->parent->color;
    node->parent->color = BLACK;

    if (node == node->parent->left) {
        if(sibling->right) sibling->right->color = BLACK;
        rotateLeft(node->parent);
    } 
    else {
        if(sibling->left) sibling->left->color = BLACK;
        rotateRight(node->parent);
    }
    //terminate after this case
}

template<class K, class T>
void RedBlackTree<K,T>::RBTdelete(RedBlackTree<K,T>::RBTNode* &node, const K& key){
    if(!node) return;
    if (key < node->key) {
        RBTdelete(node->left, key);
    } 
    else if (key > node->key) {
        RBTdelete(node->right, key);
    } 
    else {
        if (node->left && node->right) {               // two children
            RBTNode* successor = node->left;
            while (successor->right) successor = successor->right;
            node->key  = successor->key;
            node->data = successor->data;
            RBTdelete(node->left, successor->key);
        } 
        else { // 0 or 1 child
            deleteOneChild(node);                    
        }
    }
}
template<class K, class T>
void RedBlackTree<K,T>::remove(const K& key){
    if(!findInside(root,key)) return;
    RBTdelete(root,key);
}
template<class K, class T>
typename RedBlackTree<K,T>::RBTNode* RedBlackTree<K,T>::find(const K& key) const{
    if(!root) return nullptr;
    return findInside(root, key);
}
template<class K, class T>
bool RedBlackTree<K,T>::contains(const K& key) const{
    if(!root) return false;
    return findInside(root, key) != nullptr;
}
template<class K, class T>
typename RedBlackTree<K,T>::RBTNode* RedBlackTree<K,T>::lowerBound(const K& key, bool& found) const {
    RBTNode* current = root;
    RBTNode* result = nullptr;
    found = false;

    while (current != nullptr) {
        if (current->key == key) {
            found = true;
            return current;
        }
        if (key < current->key) {
            result = current; 
            current = current->left;
        } 
        else {
            current = current->right;
        }
    }
    return result;                      
}
template<class K, class T>
typename RedBlackTree<K,T>::RBTNode* RedBlackTree<K,T>::upperBound(const K& key, bool& found) const {
    RBTNode* current = root;
    RBTNode* result = nullptr;
    found = false;

    while (current != nullptr) {
        if (current->key > key) {
            result = current;
            current = current->left;
        } 
        else {
            if (current->key == key) found = true;
            current = current->right;
        }
    }
    return result;                      
}
template<class K, class T>
void RedBlackTree<K,T>::printLevelOrder(RedBlackTree<K,T>::RBTNode* node) const{
    if(!node) return;
    queue<RedBlackTree<K,T>::RBTNode*>q;
    q.push(node);
    while(!q.empty()){
        RedBlackTree<K,T>::RBTNode* curr = q.front();
        q.pop();
        cout<< curr->data<<" ";
        if(curr->left){
            q.push(curr->left);
        }
        if(curr->right){
            q.push(curr->right);
        }
    }
    cout<<endl;
}
// =====================================
// VectorRecord implementation
// =====================================

// Overload operator << for VectorRecord - prints only the id
std::ostream &operator<<(std::ostream &os, const VectorRecord &record)
{
    os << record.id;
    return os;
}

// =====================================
// VectorStore implementation
// =====================================

// TODO: Implement all VectorStore methods here
void VectorStore::rebuildTreeWithNewRoot(VectorRecord *newRoot)
{
    // TODO Insert the deep copies in in-order sequence from the old tree
    vector<VectorRecord> allRecords;
    allRecords.reserve(count);
    for (int i = 0; i < count; i++) {
        VectorRecord rec = vectorStore->getInorderDataAtIdx(i);
        // Deep copy the vector
        rec.vector = new vector<float>(*rec.vector);
        allRecords.push_back(rec);
    }
    
    // Clear trees
    vectorStore->clear();
    normIndex->clear();
    
    // Reinsert using pivot distances (but keep distanceFromReference as-is)
    for (VectorRecord& rec : allRecords) {
        double pivotDistance = l2Distance(*newRoot->vector, *rec.vector);
        vectorStore->insert(pivotDistance, rec);  // Key = pivot distance, Value = rec with original distanceFromReference
        normIndex->insert(rec.norm, rec);
    }
}
VectorStore::VectorStore(int dim, vector<float>* (*embFunc)(const string&), const vector<float>& ref)
: vectorStore(new AVLTree<double, VectorRecord>()),  
  normIndex(new RedBlackTree<double, VectorRecord>()),
  referenceVector(new vector<float>(ref)),
  rootVector(nullptr),
  dimension(dim),  
  count(0),
  averageDistance(0.0),
  embeddingFunction(embFunc)
{}
VectorStore::~VectorStore(){
    clear();             
    delete referenceVector;   
    referenceVector = nullptr;
    delete vectorStore;
    delete normIndex;
}
int VectorStore::size(){
    return count;
}
bool VectorStore::empty(){
    return count == 0;
}
void VectorStore::clear(){
    vectorStore->clear();
    if (normIndex) normIndex->clear();
    delete rootVector;
    rootVector = nullptr;
    count = 0;
    averageDistance = 0.0;
    
}
vector<float> *VectorStore::preprocessing(string rawText){
    vector<float>* newvector = embeddingFunction(rawText);
    if(!newvector){
        return nullptr;
    }
    while(newvector->size() > static_cast<size_t>(dimension)){
        newvector->resize(dimension);
    }            
    while(newvector->size() < static_cast<size_t>(dimension)){
        newvector->push_back(0.0f);
    }
    return newvector;
}
void VectorStore::addText(string rawText){
    vector<float>* newVector = preprocessing(rawText);
    if(!newVector || newVector->size() != static_cast<size_t>(dimension)){
        delete newVector;
        return;
    }
    double disRefToNew = l2Distance(*referenceVector, *newVector);
    double normVec = 0.0;
    for(float &val : *newVector){
        normVec += val*val;
    }
    normVec = sqrt(normVec);
    int newId = count;
    VectorRecord newRecord(newId, rawText, newVector, disRefToNew, normVec);
    if (count == 0) {
        count++;
        rootVector = new VectorRecord(newRecord);   
        averageDistance = disRefToNew;           
        vectorStore->insert(disRefToNew, newRecord);
        normIndex->insert(normVec, newRecord);
        return;  
    }
    vectorStore->insert(disRefToNew, newRecord);
    normIndex->insert(normVec, newRecord);
    count++;  

    double oldAverage = averageDistance;
    averageDistance = (oldAverage * (count-1) + disRefToNew) / count;
    if (fabs(disRefToNew - averageDistance) < fabs(rootVector->distanceFromReference - averageDistance)) {
        delete rootVector;
        rootVector = new VectorRecord(newRecord);
        rebuildTreeWithNewRoot(rootVector);
    }
}
VectorRecord* VectorStore::getVector(int index){
    if(index <0 || index >=count) throw out_of_range("Index is invalid!");
    return const_cast<VectorRecord*>(&vectorStore->getInorderDataAtIdx(index));
}
string VectorStore::getRawText(int index){
    if(index <0 || index>=count) throw out_of_range("Index is invalid!");
    return vectorStore->getInorderDataAtIdx(index).rawText;
}
int VectorStore::getId(int index){
    if(index <0 || index>=count) throw out_of_range("Index is invalid!");
    return vectorStore->getInorderDataAtIdx(index).id;
}
bool VectorStore::removeAt(int index){
    if(index < 0 || index >= count) throw out_of_range("Index is invalid!");
    
    VectorRecord recordToRemove = vectorStore->getInorderDataAtIdx(index);
    bool removingCurrentRoot = (recordToRemove.id == rootVector->id);
    
    // Calculate pivot distance for removal
    double pivotDistance = l2Distance(*rootVector->vector, *recordToRemove.vector);
    
    // Remove from trees
    vectorStore->remove(pivotDistance);
    if (normIndex) normIndex->remove(recordToRemove.norm);
    
    // Update average
    double removedDist = recordToRemove.distanceFromReference;
    
    // Delete the vector
    delete recordToRemove.vector;
    
    count--;
    
    // Recalculate average and find new root
    if (count == 0) {
        averageDistance = 0.0;
        delete rootVector;
        rootVector = nullptr;
    } else {
        averageDistance = (averageDistance * (count + 1) - removedDist) / count;
        
        // Find the best root for new average
        VectorRecord* bestRoot = nullptr;
        double bestDiff = 1e100;
        
        for (int i = 0; i < count; i++) {
            const VectorRecord& rec = vectorStore->getInorderDataAtIdx(i);
            double diff = fabs(rec.distanceFromReference - averageDistance);
            if (diff < bestDiff) {
                bestDiff = diff;
                bestRoot = const_cast<VectorRecord*>(&rec);
            }
        }
        
        // Always rebuild if we removed the root, or if we found a significantly better root
        if (removingCurrentRoot || (bestRoot && bestRoot->id != rootVector->id)) {
            delete rootVector;
            rootVector = new VectorRecord(*bestRoot);
            rebuildTreeWithNewRoot(rootVector);
        }
    }
    
    return true;
}
void VectorStore::setReferenceVector(const vector<float> &newReference){
    delete referenceVector;
    referenceVector = new vector<float>(newReference);

    if (count == 0) {
        averageDistance = 0.0;
        delete rootVector;
        rootVector = nullptr;
        return;
    }
    vector<VectorRecord> allRecords;
    allRecords.reserve(count);
    double sum = 0.0;
    
    for (int i = 0; i < count; i++) {
        VectorRecord rec = vectorStore->getInorderDataAtIdx(i);
        rec.distanceFromReference = l2Distance(*referenceVector, *rec.vector);
        sum += rec.distanceFromReference;
        rec.vector = new vector<float>(*rec.vector);  // Deep copy
        allRecords.push_back(rec);
    }
    averageDistance = sum / count;
    VectorRecord* bestRoot = nullptr;
    double bestDiff = 1e100;
    
    for (VectorRecord& rec : allRecords) {
        double diff = fabs(rec.distanceFromReference - averageDistance);
        if (diff < bestDiff || !bestRoot) {
            bestDiff = diff;
            bestRoot = &rec;
        }
    }
    
    delete rootVector;
    rootVector = new VectorRecord(*bestRoot);
    vectorStore->clear();
    normIndex->clear();
    
    for (VectorRecord& rec : allRecords) {
        double pivotDistance = l2Distance(*rootVector->vector, *rec.vector);
        vectorStore->insert(pivotDistance, rec);
        normIndex->insert(rec.norm, rec);
    }
}
vector<float>* VectorStore::getReferenceVector()const{  
    return referenceVector;
}

VectorRecord *VectorStore::getRootVector() const{ 
    return rootVector;
}
double VectorStore::getAverageDistance() const{
    return averageDistance;
}
void VectorStore::setEmbeddingFunction(vector<float> *(*newEmbeddingFunction)(const std::string &)){
    embeddingFunction = newEmbeddingFunction;
}
void VectorStore::forEach(void (*action)(vector<float> &, int, string &)){
    for (int i = 0; i < count; i++) {
        VectorRecord rec = vectorStore->getInorderDataAtIdx(i);
        action(*rec.vector, rec.rawLength, rec.rawText);
    }
}
vector<int> VectorStore::getAllIdsSortedByDistance() const{
    vector<int>ids;
    ids.reserve(count);
    for(int i=0;i<count;i++){
        int id = vectorStore->getInorderDataAtIdx(i).id;
        ids.push_back(id);
    }
    return ids;
}
vector<VectorRecord *> VectorStore::getAllVectorsSortedByDistance() const{
    vector<VectorRecord*>vectors;
    vectors.reserve(count);
    for(int i=0;i<count;i++){
        vectors.push_back(const_cast<VectorRecord*>(&vectorStore->getInorderDataAtIdx(i)));
    }
    return vectors;
}
double VectorStore::cosineSimilarity(const std::vector<float> &v1, const std::vector<float> &v2){
    if(v1.size() != v2.size()) return -67.676767676767676767;
    double dotProduct = 0.0;
    double magnitude1 = 0.0;
    double magnitude2 = 0.0;

    for (size_t i = 0; i < v1.size(); ++i) {
        dotProduct += v1[i] * v2[i];
        magnitude1 += v1[i] * v1[i];
        magnitude2 += v2[i] * v2[i];
    }
    if(magnitude1 == 0 || magnitude2 == 0) return 0.0;
    return dotProduct/(sqrt(magnitude1) * sqrt (magnitude2)) ;
}
double VectorStore::l1Distance(const std::vector<float> &v1, const std::vector<float> &v2){
    if(v1.size() != v2.size()) return -1.0;
    double sum = 0.0;
    for(size_t i=0;i < v1.size();i++){
        double x = v1[i];
        double y = v2[i];
        double minus = x-y;
        sum+= abs(minus);
    }
    return sum;
}
double VectorStore::l2Distance(const std::vector<float> &v1, const std::vector<float> &v2){
    if(v1.size() != v2.size()) return -1.0;
    double sum = 0.0;
    for(size_t i=0;i < v1.size();i++){
        double diff = v1[i] - v2[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

double VectorStore::estimateD_Linear(const std::vector<float> &query, int k, double averageDistance, const std::vector<float> &reference, double c0_bias, double c1_slope){
    if(k<=0) return 0.0;
    double dr = l2Distance(query, reference);
    double D;
    D = abs(dr - averageDistance) + c1_slope * averageDistance * k + c0_bias;
    return max(0.0,D);
}


// Explicit template instantiation for the type used by VectorStore
template class AVLTree<double, VectorRecord>;
template class AVLTree<double, double>;
template class AVLTree<int, double>;
template class AVLTree<int, int>;
template class AVLTree<double, string>;
template class AVLTree<int, string>;

template class RedBlackTree<double, VectorRecord>;
template class RedBlackTree<double, double>;
template class RedBlackTree<int, double>;
template class RedBlackTree<int, int>;
template class RedBlackTree<double, string>;
template class RedBlackTree<int, string>;



