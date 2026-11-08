// paket.h - Deklarasi ADT Queue paket (singly linked list dengan head dan tail)
#ifndef PAKET_H
#define PAKET_H

#include <string>

const int KODE_MAKS  = 10;    // panjang kode paket maksimum
const int BERAT_MIN  = 1;     // gram
const int BERAT_MAKS = 5000;  // gram

struct Paket {
    std::string kode;
    int berat;
};

struct Node {
    Paket info;
    Node* next;
};

struct Queue {
    Node* head;
    Node* tail;
};

// Queue kosong: head = tail = nullptr. Queue berisi: tail->next = nullptr.
void createQueue(Queue& q);
bool isEmptyQueue(const Queue& q);

// Membuat satu node baru (next = nullptr). Mengembalikan nullptr jika alokasi gagal.
Node* alokasi(const std::string& kode, int berat);
// Membebaskan node dan mengisi pointer pemanggil dengan nullptr (anti double delete).
void dealokasi(Node*& p);

// Menambahkan node yang sudah dialokasikan ke belakang queue. false jika node ditolak.
bool enqueue(Queue& q, Node* p);
// Melepas node terdepan TANPA delete; next node yang lepas = nullptr. nullptr jika kosong.
Node* dequeue(Queue& q);

// Mencari berdasarkan kode. posisi diisi mulai dari 1 (0 jika tidak ada).
Node* findElm(const Queue& q, const std::string& kode, int& posisi);
// Memeriksa keanggotaan berdasarkan ALAMAT node (alamat tidak pernah diakses isinya).
bool fFindElm(const Queue& q, const Node* p);

int nbList(const Queue& q);           // jumlah node
long long totalBerat(const Queue& q); // jumlah berat seluruh node (gram)
void printInfo(const Queue& q);       // mencetak dari depan, tanpa newline
int delAll(Queue& q);                 // melepas dan membebaskan semua node; mengembalikan jumlahnya

#endif
