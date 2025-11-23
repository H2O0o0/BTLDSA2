#ifndef VECTORSTORE_H
#define VECTORSTORE_H

#pragma once
// NOTE: Per assignment rules, only this single include is allowed here.
#include "main.h"



// ------------------------------
// AVL balance enum
// ------------------------------
enum BalanceValue {
    LH = -1, // Left Higher
    EH = 0,  // Equal Height
    RH = 1   // Right Higher
};

// ------------------------------
// Generic AVL Tree (template)
// ------------------------------
template <class K, class T>
class AVLTree {
    friend class VectorStore; // Allow VectorStore to access protected/private members
    
    public:
        class AVLNode {
        public:
            K key;
            T data;
            AVLNode* pLeft;
            AVLNode* pRight;
            BalanceValue balance;

            AVLNode(const K& key, const T& value)
                : key(key), data(value), pLeft(nullptr), pRight(nullptr), balance(EH) {}
            //data hoac value    
            friend class VectorStore; // Allow VectorStore to access AVLNode members
        };

    protected:
        AVLNode* root;

        AVLNode* rotateRight(AVLNode*& node);
        AVLNode* rotateLeft(AVLNode*& node);
        void clearHelper(AVLNode* node);

    public:
        AVLTree();
        void destroy(AVLNode* node);
        ~AVLTree();
        //helper for insert:
        const AVLNode* findInside(const AVLNode* node, const K& key) const;
        AVLNode* findInside(AVLNode* node, const K& key);

        AVLNode* LeftBalance(AVLNode* &node, bool& taller);
        AVLNode* RightBalance(AVLNode* &node, bool& taller);
        AVLNode* AVLInsert(AVLNode* &node, AVLNode* newPtr, bool& taller);

        void insert(const K& key, const T& value);
        
        AVLNode* deleteRightBalance(AVLNode* &node, bool &shorter);
        AVLNode* deleteLeftBalance(AVLNode* &node, bool &shorter);
        AVLNode* AVLDelete(AVLNode* &node, const K& deleteKey, bool &shorter, bool& completed);
        void remove(const K& key);

        bool contains(const K& key) const;

        int getHeightMax(AVLNode* node) const;
        int getHeight() const;
        
        int getSizeMax(AVLNode* node) const;
        int getSize() const;
        bool empty() const;
        void clear();

        int findMax(int a, int b) const;

        void printTreeStructure() const;
        void printLevelOrder(AVLNode* node) const;

        AVLNode* getInorderDataAtIdxHelper(AVLNode* node, int tarIdx,int& curIdx) const;
        T& getInorderDataAtIdx(int tarIdx);
        const T& getInorderDataAtIdx(int tarIdx) const;

        void inorderTraversal(AVLNode* node, void (*action)(const T&)) const;
        void inorderTraversal(void (*action)(const T&)) const;

        AVLNode* getRoot() const { return root; }
};

enum Color { RED, BLACK };

// RedBlackTree class
template <class K, class T>
class RedBlackTree {
    friend class VectorStore; // Allow VectorStore to access protected/private members
    
public:
// RBTNode class

class RBTNode {
    public:
        K key;
        T data;
        Color color;
        RBTNode* parent;
        RBTNode* left;
        RBTNode* right;

        // Constructor
        RBTNode(const K& key, const T& value);
        
        void recolorToRed();
        void recolorToBlack();
        
        friend class VectorStore; // Allow VectorStore to access RBTNode members
    };

private:
    RBTNode* root;
protected:
    void rotateLeft(RBTNode* node);
    void rotateRight(RBTNode* node);

    RBTNode* lowerBoundNode(const K& key) const;
    RBTNode* upperBoundNode(const K& key) const;

    RBTNode* getSibling(RBTNode* node);
    //fordelete 6 cases
    void replaceNode(RBTNode* oldNode, RBTNode* newNode);
    void deleteCase1(RBTNode* node);
    void deleteCase2(RBTNode* node);
    void deleteCase3(RBTNode* node);
    void deleteCase4(RBTNode* node);
    void deleteCase5(RBTNode* node);
    void deleteCase6(RBTNode* node);

    void deleteOneChild(RBTNode* &del);
    void RBTdelete(RBTNode* &node, const K& key);

public:
    RedBlackTree();
    void destroy(RBTNode* node);
    ~RedBlackTree();
    
    bool empty() const;

    int helpMeFindHeight(RBTNode* node) const;
    int size() const;
    void clear();

    RBTNode* findInside(RBTNode* node, const K& key) const;
    const RBTNode* findInside(const RBTNode* node, const K& key) const;

    void RBT_fix_up(RBTNode* node);
    void insert(const K& key, const T& value);

    bool isLeaf(RBTNode* &node);
    ///
   
    void remove(const K& key);
    RBTNode* find(const K& key) const;
    bool contains(const K& key) const;

    RBTNode* lowerBound(const K& key, bool& found) const;
    RBTNode* upperBound(const K& key, bool& found) const;
    void printLevelOrder(RBTNode* node) const;
    void printTreeStructure() const;
};


// ------------------------------
// VectorRecord
// ------------------------------
class VectorRecord {
public:
    int id;
    std::string rawText;
    int rawLength;
    std::vector<float> *vector;
    double distanceFromReference;
    double norm;

    VectorRecord()
        : id(-1), rawLength(0), vector(nullptr), distanceFromReference(0.0), norm(0.0) {}

    VectorRecord(int _id,
                 const std::string &_rawText,
                 std::vector<float> *_vec,
                 double _dist,
                 double _norm = 0)
        : id(_id),
          rawText(_rawText),
          rawLength(static_cast<int>(_rawText.size())),
          vector(_vec),
          distanceFromReference(_dist),
          norm(_norm) {}

    // Overload operator << to print only the id
    friend std::ostream &operator<<(std::ostream &os, const VectorRecord &record);
};

// ------------------------------
// VectorStore
// ------------------------------
class VectorStore
{
private:
    AVLTree<double, VectorRecord> *vectorStore;//pointer for using AVL
    RedBlackTree<double, VectorRecord> *normIndex;//pointer using RBT

    std::vector<float> *referenceVector;
    VectorRecord *rootVector;

    int dimension;
    int count;
    double averageDistance;

    std::vector<float> *(*embeddingFunction)(const std::string &);

    double distanceByMetric(const std::vector<float> &a,
                            const std::vector<float> &b,
                            const std::string &metric) const;

    void rebuildRootIfNeeded();
    void rebuildTreeWithNewRoot(VectorRecord *newRoot); //xay lai cay

    VectorRecord *findVectorNearestToDistance(double targetDistance) const;

public:
    VectorStore(int dimension,
                std::vector<float> *(*embeddingFunction)(const std::string &),
                const std::vector<float> &referenceVector);
    ~VectorStore();

    void test_vectorStore() { vectorStore->printTreeStructure(); }
    void test_normIndex() { normIndex->printTreeStructure(); }

    int size();
    bool empty();
    void clear();

    std::vector<float> *preprocessing(std::string rawText);
    void addText(std::string rawText);

    VectorRecord *getVector(int index);
    std::string getRawText(int index);
    int getId(int index);

    bool removeAt(int index);

    void setReferenceVector(const std::vector<float> &newReference);
    std::vector<float> *getReferenceVector() const;
    VectorRecord *getRootVector() const;
    double getAverageDistance() const;
    void setEmbeddingFunction(std::vector<float> *(*newEmbeddingFunction)(const std::string &));

    void forEach(void (*action)(std::vector<float> &, int, std::string &));
    std::vector<int> getAllIdsSortedByDistance() const;
    std::vector<VectorRecord *> getAllVectorsSortedByDistance() const;

    double cosineSimilarity(const std::vector<float> &v1, const std::vector<float> &v2);
    double l1Distance(const std::vector<float> &v1, const std::vector<float> &v2);
    double l2Distance(const std::vector<float> &v1, const std::vector<float> &v2);

    double estimateD_Linear(const std::vector<float> &query, int k, double averageDistance, const std::vector<float> &reference, double c0_bias = 1e-9, double c1_slope = 0.05);

    int findNearest(const std::vector<float> &query, std::string metric = "cosine");
    int *topKNearest(const std::vector<float> &query, int k, std::string metric = "cosine");

    int *rangeQueryFromRoot(double minDist, double maxDist) const;
    int *rangeQuery(const std::vector<float> &query, double radius, std::string metric = "cosine") const;
    int *boundingBoxQuery(const std::vector<float> &minBound, const std::vector<float> &maxBound) const;

    double getMaxDistance() const;
    double getMinDistance() const;
    VectorRecord computeCentroid(const std::vector<VectorRecord *> &records) const;
};


#endif // VECTORSTORE_H
