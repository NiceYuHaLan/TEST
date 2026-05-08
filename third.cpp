#pragma once
#include "sequence/ArraySequence.h"
#include <queue>
static std::string saveString = "";
enum TraversalType {
    LNR_,    
    NLR_,    
    LRN_,    
    RNL_,    
    NRL_,    
    RLN_     
};

template<typename T>
class Heap {
private:
    ArraySequence<T>* data;
    int size = 0;
    
    void Up(int index) {
        while(index > 0) {
            int parent = (index - 1) / 2;
            if ((*data)[parent] >= (*data)[index]) {
                break;
            }
            auto temp = (*data)[index];
            data->Set(index, (*data)[parent]);
            data->Set(parent, temp);
            index = parent;
        }
    }

    void Down(int index) {
        int l, r, max_val;
        while (true) {
            l = 2 * index + 1;
            r = 2 * index + 2;
            max_val = index;

            if (l < data->GetLength() && (*data)[l] > (*data)[max_val])
                max_val = l;
            if (r < data->GetLength() && (*data)[r] > (*data)[max_val])
                max_val = r;

            if (max_val == index) break;

            auto temp = (*data)[index];
            data->Set(index, (*data)[max_val]);
            data->Set(max_val, temp);

            index = max_val;
        }
    }

    void lnr(int index) {
        if (index >= size) return;
        lnr(2 * index + 1);
        saveString += std::to_string((*data)[index]) + " ";
        lnr(2 * index + 2);
    }

    void nlr(int index) {
        if (index >= size) return;
        saveString += std::to_string((*data)[index]) + " ";
        nlr(2 * index + 1);
        nlr(2 * index + 2);
    }

    void lrn(int index) {
        if (index >= size) return;
        lrn(2 * index + 1);
        lrn(2 * index + 2);
        saveString += std::to_string((*data)[index]) + " ";
    }

    void rnl(int index) {
        if (index >= size) return;
        rnl(2 * index + 2);
        saveString += std::to_string((*data)[index]) + " ";
        rnl(2 * index + 1);
    }

    void nrl(int index) {
        if (index >= size) return;
        saveString += std::to_string((*data)[index]) + " ";
        nrl(2 * index + 2);
        nrl(2 * index + 1);
    }

    void rln(int index) {
        if (index >= size) return;
        rln(2 * index + 2);
        rln(2 * index + 1);
        saveString += std::to_string((*data)[index]) + " ";
    }

public:
    Heap() {
        data = new MutableArraySequence<T>();
    }

    void Add(const T& value) {
        data->Append(value);
        size++;
        Up(data->GetLength() - 1);
    }

    T& Top() {
        return data->Get(0);
    }

    bool Empty() const {
        return size == 0;
    }

    void Remove() {
        if(Empty()){
            throw std::logic_error("Heap is empty!");
        }
        if (data->GetLength() == 1) {
            data = new MutableArraySequence<int>();
            return;
        }
        size--;
        data->Set(0, data->Get(data->GetLength() - 1));
        data = data->GetSubsequence(0, data->GetLength() - 2);
        if (!Empty()) {
            Down(0);
        }
    }

    Heap<T>* Subtree(const T& value) {
        int index = -1;
        for(int i = 0; i < data->GetLength(); i++) {
            if((*data)[i] == value) {
                index = i;
                break;
            }
        }
        if(index == -1) {
            return new Heap<T>();
        }
        Heap<T>* subtree = new Heap<T>();
        std::queue<int> q;
        q.push(index);
        while(!q.empty()) {
            int cur = q.front();
            q.pop();
            
            subtree->Add((*data)[cur]);
            
            int l = 2 * cur + 1;
            int r = 2 * cur + 2;
            
            if (l < data->GetLength()) q.push(l);
            if (r < data->GetLength()) q.push(r);
        }
        return subtree;
    }

    bool Hassubtree(Heap<T>* subtree) {
        if (subtree->Empty()) return true;
        if (subtree->size > size) return false;
        T root = subtree->Top();
        for (int i = 0; i < size; i++) {
            if (data->Get(i) == root) {
                Heap<T>* currentSubtree = Subtree(root);
                if ((*currentSubtree) == (*subtree)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool operator==(const Heap<T>& other) {
        if (size != other.size) return false;
        for (int i = 0; i < size; i++) {
            if (data->Get(i) != other.data->Get(i)) {
                return false;
            }
        }
        return true;
    }

    std::string Travel(TraversalType type) {
        saveString = "";
        switch (type) {
            case LNR_: lnr(0); break;
            case NLR_: nlr(0); break;
            case LRN_: lrn(0); break;
            case RNL_: rnl(0); break;
            case NRL_: nrl(0); break;
            case RLN_: rln(0); break;
        }
        return saveString;
    }

    void Print() {
        for(int i = 0; i < data->GetLength(); i++) {
            std::cout << data->Get(i) << " ";
        }
    }
};
