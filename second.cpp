#pragma once
#include <iostream>
#include <functional>
#include <string>
#include <queue>
static std::string saveString = "";
enum typeOfTravel{
    NLR,
    NRL,
    LNR,
    LRN,
    RNL,
    RLN,
    LAYOT
};


template<typename T>
class Tree{
private:
    int size = 0;
    struct Node
    {   
        Node *left = nullptr;
        Node *right = nullptr;
        int height = 1;
        T value;
        Node* previous = nullptr;  
        Node* next = nullptr;
    };
    Node* root = nullptr;
    Node* last = nullptr;
    typeOfTravel threadType = LNR;

    int height(Node* node) const{
        return node ? node->height: 0;
    }

    int balanceFactor(Node* node) const{
        return height(node->right) - height(node->left);
    }

    void updateHeight(Node* node){
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }

    void rotateRight(Node*& node) {
        Node* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        updateHeight(node);
        updateHeight(newRoot);
        node = newRoot;
    }

    void rotateLeft(Node*& node) {
        Node* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        updateHeight(node);
        updateHeight(newRoot);
        node = newRoot;
    }

    void balance(Node*& node) {
        updateHeight(node);
        
        if (balanceFactor(node) == 2) {
            if (balanceFactor(node->right) < 0){
                rotateRight(node->right);
            }
            rotateLeft(node);
        }
        else if (balanceFactor(node) == -2) {
            if (balanceFactor(node->left) > 0){
                rotateLeft(node->left);
            }
            rotateRight(node);
        }
    }

    void Insert(Node* &now, const T& val){
        if (now == nullptr) {
            now = new Node{nullptr, nullptr, 1, val};
            size++;  
            return;
        }
        if (val < now->value) {
            Insert(now->left, val);
        } else if (val > now->value) {
            Insert(now->right, val);
        }
        else{
            return;
        }
        balance(now);
    }

    void Remove(Node*& now, const T& val) {
        if (!now) {
            throw std::invalid_argument("Value not found in tree");
        }
        
        if (val < now->value) {
            Remove(now->left, val);
        }
        else if (val > now->value) {
            Remove(now->right, val);
        }
        else {
            if (now->left == nullptr) {
                Node* tmp = now->right;
                delete now;
                now = tmp;
            }
            else if (now->right == nullptr) {
                Node* tmp = now->left;
                delete now;
                now = tmp;
            }
            else {
                Node* minRight = now->right;
                while (minRight->left) {
                    minRight = minRight->left;
                }
                now->value = minRight->value;
                Remove(now->right, minRight->value);
            }
            size--;
        }
        if(now){
            balance(now);
        }
    }

    Node* Find(Node* now, const T& val){
        if(!now){
            return nullptr;
        }
        if(now->value < val){
            return Find(now->right, val);
        }
        else if(now->value > val){
            return Find(now->left, val);
        }
        else{
            return now;
        }
    }

    void Print(Node* now){
        if (!now) return;
        Print(now->left);
        std::cout << now->value << " ";
        Print(now->right);
    }

    void Clear(Node* node) {
        if (!node) return;
        Clear(node->left);
        Clear(node->right);
        delete node;
        size--;
    }
    
    void Map(Node* now, Tree* tree, std::function<T(const T&)> func){
        if(!now){return;}
        Map(now->left, tree, func);
        tree->insert(func(now->value));
        Map(now->right, tree, func);
    }

    void Where(Node* now, Tree* tree, std::function<bool(const T&)> func){
        if(!now){return;}
        Where(now->left, tree, func);
        if(func(now->value)){
             tree->insert(now->value);
        }
        Where(now->right, tree, func);
    }

    T Reduce(Node* now, std::function<T(T, T)> func, T now_val ){
        
        if (!now) return now_val;

        now_val = Reduce(now->left, func, now_val);
        now_val = func(now_val, now->value);
        now_val = Reduce(now->right, func, now_val);
        
        return now_val;
    }

    void Nlr(Node* now){
        if(!now){return;}
        saveString += "[" + std::to_string(now->value) + "]";
        saveString += "{";
        Nlr(now->left);
        saveString += "}";
        saveString += "(";
        Nlr(now->right);
        saveString += ")";
    }

    void Nrl(Node* now){
        if(!now){return;}
        saveString += "[" + std::to_string(now->value) + "]";
        saveString += "(";
        Nrl(now->right);
        saveString += ")";
        saveString += "{";
        Nrl(now->left);
        saveString += "}";
    }

    void Lrn(Node* now){
        if(!now){return;}
        saveString += "{";
        Lrn(now->left);
        saveString += "}";
        saveString += "(";
        Lrn(now->right);
        saveString += "[" + std::to_string(now->value) + "]";
    }

    void Rln(Node* now){
        if(!now){return;}
        saveString += "(";
        Rln(now->right);
        saveString += ")";
        saveString += "{";
        Rln(now->left);
        saveString += "}";
        saveString += "[" + std::to_string(now->value) + "]";
    }

    void Lnr(Node* now){
        if(!now){return;}
        saveString += "{";
        Lnr(now->left);
        saveString += "}";
        saveString += "[" + std::to_string(now->value) + "]";
        saveString += "(";
        Lnr(now->right);
        saveString += ")";
    }

    void Rnl(Node* now){
        if(!now){return;}
        saveString += "(";
        Rnl(now->right);
        saveString += ")";
        saveString += "[" + std::to_string(now->value) + "]";
        saveString += "{";
        Rnl(now->left);
        saveString += "";
    }

    void Layot() {
        saveString = "";
        if (!root) return;

        std::queue<Node*> q;
        q.push(root);
        int level = root->height;
        int height = 0;
        while (!q.empty()) {
            int levelSize = q.size();
            std::string levelStr;
            
            levelStr += std::string(level + (((1 << height)  - levelSize)), ' ');
            
            for (int i = 0; i < levelSize; i++) {
                Node* current = q.front();
                q.pop();
                
                levelStr += std::to_string(current->value) + " ";
                
                if (current->left) q.push(current->left);
                if (current->right) q.push(current->right);
            }
            
            saveString += levelStr + "\n";
            level--;
            height++;
        }
        
        if (!saveString.empty()) {
            saveString.pop_back();
        }
    }

    void Subtree(Node* now, Tree<T>* tree){
        if(!now){return;}

        tree->insert(now->value);
        Subtree(now->left, tree);
        Subtree(now->right, tree);
    }

    bool Hassubtree(Node* first, Node* second){
        if(!first){
            return false;
        }

        if(first->value == second->value && Sametrees(first, second)){
            return true;
        }

        return Hassubtree(first->left, second) || Hassubtree(first->right, second);
    }

    bool Sametrees(Node* first, Node* second){
        if(!first && !second){return true;}
        if(!first || !second){return false;}
        
        if(first->value == second->value){
            return Sametrees(first->left, second->left) && Sametrees(first->right, second->right);
        }
        else{
            return false;
        }
    }

    void Merge(Node* now){
        if(!now){return;}
        Merge(now->left);
        this->insert(now->value);
        Merge(now->right);
    }

    void threadNLR(Node* now) {
        if (!now) return;
        if(last){
            last->next = now;
            now->previous = last;
        }
        last = now;
        threadNLR(now->left);
        threadNLR(now->right);
    }
    
    void threadLNR(Node* now) {
        if (!now) return;
        threadLNR(now->left);
        if(last){
            last->next = now;
            now->previous = last;
        }
        last = now;
        threadLNR(now->right);
    }

    void threadLRN(Node* now) {
        if (!now) return;
        threadLRN(now->left);
        threadLRN(now->right);
        if(last){
            last->next = now;
            now->previous = last;
        }
        last = now;
    }

    void threadNRL(Node* now) {
        if(!now){return;}
        if(last){
            last->next = now;
            now->previous = last;
        }
        last = now;
        threadLRN(now->right);
        threadLRN(now->left);
    }

    void threadRNL(Node* now) {
        if(!now){return;}
        threadLRN(now->right);
        if(last){
            last->next = now;
            now->previous = last;
        }
        last = now;
        threadLRN(now->left);
    }

    void threadRLN(Node* now) {
        if(!now){return;}
        threadLRN(now->right);
        threadLRN(now->left);
        if(last){
            last->next = now;
            now->previous = last;
        }
    }
    
    void createTree(){
         new (this) Tree();
    }

public:
    Tree() = default;

    bool empty() const {
        return size == 0;
    }

    void insert(const T& val){
        Insert(root, val);
        thread(threadType);
    }

    void print(){
        if (empty()) {
            throw std::logic_error("Empty tree");
        }
        Print(root);
    }

    void remove( const T& val){
        if (empty()) {
            throw std::logic_error("Empty tree");
        }
        Remove(root, val);
        thread(threadType);
    }

    void clear(){
        Clear(root);
        createTree();
    }
    
    bool contains(const T& val){
        return Find(root, val) != nullptr;
    }

    Tree* map(std::function<T( const T&)> func){
       Tree<T>* new_tree = new Tree<T>();
       Map(root, new_tree, func);
       return new_tree;
    }

    Tree* where(std::function<bool(const T&)> func){
       Tree<T>* new_tree = new Tree<T>();
       Where(root, new_tree, func);
       return new_tree;
    }
    
    T reduce(std::function<T(T, T)> func, T val){
        return Reduce(root, func, val);
    }

    Tree* subtree(T val){
        Node* find = Find(root,val);
        if(!Find(find, val)){return nullptr;}

        Tree<T>* new_tree = new Tree<T>();
        Subtree(find, new_tree);
        return new_tree;
    }

    bool hassubtree(Tree<T>* tree){
        if(!tree->root){
            return true;
        }
        return Hassubtree(root, tree->root);
    }

    void merge(Tree<T>* tree){
        Merge(tree->root);
    }

    std::string travel(typeOfTravel type = NLR){
        saveString = "";
        switch (type)
        {
        case NLR:
            Nlr(root);
            break;
        case NRL:
            Nrl(root);
            break;
        case LRN:
            Lrn(root);
            break;
        case LNR:
            Lnr(root);
            break;
        case RNL:
            Rnl(root);
            break;
        case RLN:
            Rln(root);
            break;
        case LAYOT:
            Layot();
            break;
        default:
            saveString = "";
            break;
        }
        return saveString;
    }

    void thread(typeOfTravel type) {
        last = nullptr;
        threadType = type;
        switch (type) {
            case NLR: threadNLR(root); break;
            case LNR: threadLNR(root); break;
            case LRN: threadLRN(root); break;
            case NRL: threadNRL(root); break;
            case RNL: threadRNL(root); break;
            case RLN: threadRLN(root); break;
            }
    }

    void printFromThread(){
        Node* current = getFirst(root);
        while (current) {
            std::cout << current->value << " ";
            current = current->next;
        }
    }

    Node* getFirst(Node* node) {
        if (!node) return nullptr;
        while (node->previous) {
            node = node->previous;
        }
        return node;
    }

    int getSize() const {
        return size;
    }

    ~Tree(){
        Clear(root);
    }
};
