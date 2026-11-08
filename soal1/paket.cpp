// paket.cpp - Implementasi ADT Queue paket
#include "paket.h"

#include <iostream>
#include <new>

void createQueue(Queue& q) {
    q.head = nullptr;
    q.tail = nullptr;
}

bool isEmptyQueue(const Queue& q) {
    return q.head == nullptr;
}

Node* alokasi(const std::string& kode, int berat) {
    Node* p = new (std::nothrow) Node();
    if (p == nullptr) {
        return nullptr;
    }
    try {
        p->info.kode = kode;      // penyalinan string juga bisa gagal
    } catch (...) {
        delete p;
        return nullptr;
    }
    p->info.berat = berat;
    p->next = nullptr;
    return p;
}

void dealokasi(Node*& p) {
    if (p != nullptr) {
        delete p;
        p = nullptr;
    }
}

bool enqueue(Queue& q, Node* p) {
    // Node harus baru/terlepas (next = nullptr) dan bukan tail queue ini (mencegah siklus).
    if (p == nullptr || p->next != nullptr || p == q.tail) {
        return false;
    }
    if (isEmptyQueue(q)) {
        q.head = p;
        q.tail = p;
    } else {
        q.tail->next = p;
        q.tail = p;
    }
    return true;
}

Node* dequeue(Queue& q) {
    if (isEmptyQueue(q)) {
        return nullptr;
    }
    Node* p = q.head;
    q.head = p->next;
    if (q.head == nullptr) {
        q.tail = nullptr;         // queue menjadi kosong: head = tail = nullptr
    }
    p->next = nullptr;            // node lepas tidak membawa sisa rantai
    return p;
}

Node* findElm(const Queue& q, const std::string& kode, int& posisi) {
    posisi = 0;
    int urutan = 1;
    for (Node* cur = q.head; cur != nullptr; cur = cur->next, ++urutan) {
        if (cur->info.kode == kode) {
            posisi = urutan;
            return cur;
        }
    }
    return nullptr;
}

bool fFindElm(const Queue& q, const Node* p) {
    for (const Node* cur = q.head; cur != nullptr; cur = cur->next) {
        if (cur == p) {
            return true;
        }
    }
    return false;
}

int nbList(const Queue& q) {
    int n = 0;
    for (const Node* cur = q.head; cur != nullptr; cur = cur->next) {
        ++n;
    }
    return n;
}

long long totalBerat(const Queue& q) {
    long long total = 0;
    for (const Node* cur = q.head; cur != nullptr; cur = cur->next) {
        total += cur->info.berat;
    }
    return total;
}

void printInfo(const Queue& q) {
    if (isEmptyQueue(q)) {
        std::cout << "(kosong)";
        return;
    }
    for (const Node* cur = q.head; cur != nullptr; cur = cur->next) {
        std::cout << cur->info.kode << "(" << cur->info.berat << " g)";
        if (cur->next != nullptr) {
            std::cout << " -> ";
        }
    }
}

int delAll(Queue& q) {
    int jumlah = 0;
    Node* p = dequeue(q);
    while (p != nullptr) {
        dealokasi(p);
        ++jumlah;
        p = dequeue(q);
    }
    return jumlah;
}
