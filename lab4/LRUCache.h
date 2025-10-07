#include<bits/stdc++.h>
using namespace std;
class Node {
    public:
    int p_no;
    int page_no;
    Node* prev;
    Node* next;
    Node(){
        p_no = -1;
        page_no = -1;
        prev = nullptr;
        next = nullptr;
    }
    Node(int p, int page){
        p_no = p;
        page_no = page;
        prev = nullptr;
        next = nullptr;
    }
};

struct pair_hash {
    size_t operator()(const pair<int,int>& p) const noexcept {
        return hash<int>()(p.first) ^ (hash<int>()(p.second) << 1);
    }
};

class LRUCache {
    Node* head;
    Node* tail;
    int capacity;
    unordered_map<pair<int,int>, Node*, pair_hash> cache;

public:
    LRUCache(int cap) : capacity(cap) {
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }
    LRUCache(){
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }

    void setCapacity(int cap){
        capacity = cap;
    }
    bool isHit(int p_no, int page_no) {
        auto key = make_pair(p_no, page_no);
        if (cache.find(key) == cache.end()) return false;

        Node* node = cache[key];
        // Move to front
        remove(node);
        insertFront(node);
        return true;
    }

    void put(int p_no, int page_no) {
        auto key = make_pair(p_no, page_no);
        if (cache.find(key) != cache.end()) {
            Node* node = cache[key];
            remove(node);
            insertFront(node);
            return;
        }

        Node* node = new Node(p_no, page_no);
        insertFront(node);
        cache[key] = node;

        if ((int)cache.size() > capacity) {
            Node* lru = tail->prev;
            remove(lru);
            cache.erase(make_pair(lru->p_no, lru->page_no));
            delete lru;
        }
    }

    pair<int,int> getLRU() {
        Node* lru = tail->prev;
        return make_pair(lru->p_no, lru->page_no);
    }

private:
    void remove(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void insertFront(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }
};