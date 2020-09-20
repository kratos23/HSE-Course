#include "list.h"
#include <utility>

namespace task {

list::ListNode::ListNode(int val) {
    this->nxt = nullptr;
    this->prev = nullptr;
    this->val = val;
}

list::list() {
    this->head = nullptr;
    this->tail = nullptr;
    this->size_ = 0;
}

list::list(size_t count, const int& value) : list::list() {
    for (int i = 0; i < count; i++) {
        push_back(value);
    }
}

list::list(const list& other) : list::list() {
    *this = other;
}

list::~list() {
    clear();
}

list& list::operator=(const list& other) {
    if (this != &other) {
        clear();
        ListNode *cur = other.head;
        ListNode **nxtPtr = &head;
        while (cur != nullptr) {
            auto *newNode = new ListNode(*cur);
            (*nxtPtr) = newNode;
            nxtPtr = &(newNode->nxt);
            cur = cur->nxt;
            size_++;
        }
        cur = head;
        while (cur != nullptr) {
            if (cur->nxt == nullptr) {
                tail = cur;
            } else {
                cur->nxt->prev = cur;
            }
            cur = cur->nxt;
        }
    }
    return *this;
}

int& list::front() {
    return head->val;
}

const int& list::front() const {
    return head->val;
}

int& list::back() {
    return tail->val;
}

const int& list::back() const {
    return tail->val;
}

bool list::empty() const {
    return size() == 0;
}

size_t list::size() const {
    return size_;
}

void list::clear() {
    while (!empty()) {
        pop_front();
    }
}

void list::push_back(const int& value) {
    auto *newNode = new ListNode(value);
    if (tail == nullptr) {
        tail = newNode;
        head = newNode;
    } else {
        newNode->prev = tail;
        tail->nxt = newNode;
        tail = newNode;
    }
    size_++;
}

void list::pop_back() {
    ListNode *newTail = tail->prev;
    if (newTail != nullptr) {
        newTail->nxt = nullptr;
    }
    delete tail;
    tail = newTail;
    if (tail == nullptr) {
        head = nullptr;
    }
    size_--;
}

void list::push_front(const int& value) {
    auto *newNode = new ListNode(value);
    if (head == nullptr) {
        tail = newNode;
        head = newNode;
    } else {
        newNode->nxt = head;
        head->prev = newNode;
        head = newNode;
    }
    size_++;
}

void list::pop_front() {
    ListNode *newHead = head->nxt;
    if (newHead != nullptr) {
        newHead->prev = nullptr;
    }
    delete head;
    head = newHead;
    if (head == nullptr) {
        tail = nullptr;
    }
    size_--;
}

void list::resize(size_t count) {
    while (size() > count) {
        pop_back();
    }
    while (size() < count) {
        push_back(int());
    }
}

void list::swap(list& other) {
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(size_, other.size_);
}

void list::remove(const int& value) {
    int valCopy = value;
    ListNode *cur = head;
    while (cur != nullptr) {
        ListNode *nxt = cur->nxt;
        if (cur->val == valCopy) {
            if (nxt != nullptr) {
                nxt->prev = cur->prev;
            } else {
                tail = cur->prev;
            }
            ListNode *prev = cur->prev;
            if (prev != nullptr) {
                prev->nxt = cur->nxt;
            } else {
                head = nxt;
            }
            delete cur;
            size_--;
        }
        cur = nxt;
    }
}

void list::unique() {
    list newList;
    ListNode *cur = head;
    while (cur != nullptr) {
        ListNode *prev = cur->prev;
        if (prev == nullptr || prev->val != cur->val) {
            newList.push_back(cur->val);
        }
        cur = cur->nxt;
    }
    clear();
    while (!newList.empty()) {
        push_back(newList.front());
        newList.pop_front();
    }
}

void list::sort() {
    if (size() <= 1) {
        return;
    }
    int origSz = size();
    list bList;
    while (size() > origSz / 2) {
        bList.push_back(back());
        pop_back();
    }
    sort();
    bList.sort();
    list newList = merge(bList);
    clear();
    while (!newList.empty()) {
        push_back(newList.front());
        newList.pop_front();
    }
}

list list::merge(const list& b) {
    list newList;
    ListNode *aPtr = head;
    ListNode *bPtr = b.head;
    while (aPtr != nullptr || bPtr != nullptr) {
        if (aPtr == nullptr) {
            while (bPtr != nullptr) {
                newList.push_back(bPtr->val);
                bPtr = bPtr->nxt;
            }
        } else if (bPtr == nullptr) {
            while (aPtr != nullptr) {
                newList.push_back(aPtr->val);
                aPtr = aPtr->nxt;
            }
        } else {
            if (aPtr->val <= bPtr->val) {
                newList.push_back(aPtr->val);
                aPtr = aPtr->nxt;
            } else {
                newList.push_back(bPtr->val);
                bPtr = bPtr->nxt;
            }
        }
    }
    return newList;
}

}