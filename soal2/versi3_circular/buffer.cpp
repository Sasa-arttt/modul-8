// buffer.cpp - Versi 3: circular queue (head dan count; tail tidak disimpan)
// Posisi tulis = (head + count) % KAPASITAS; posisi data terakhir = (head + count - 1) % KAPASITAS.
// Tidak ada elemen yang digeser.
#include "buffer.h"

#include <iostream>

void createBuffer(Buffer& b) {
    for (int i = 0; i < KAPASITAS; ++i) {
        b.data[i].nomor = 0;
        b.data[i].nilai = 0;
    }
    b.head = 0;
    b.count = 0;
    b.nomorBerikutnya = 1;
    b.geser = 0;
}

bool isEmptyBuffer(const Buffer& b) { return b.count == 0; }

bool isFullBuffer(const Buffer& b) { return b.count == KAPASITAS; }

int jumlahData(const Buffer& b) { return b.count; }

int ruangKosong(const Buffer& b) { return KAPASITAS - b.count; }

bool enqueue(Buffer& b, int nilai) {
    if (isFullBuffer(b)) return false;
    int posisiTulis = (b.head + b.count) % KAPASITAS;
    b.data[posisiTulis].nomor = b.nomorBerikutnya;
    b.data[posisiTulis].nilai = nilai;
    b.nomorBerikutnya += 1;
    b.count += 1;
    return true;
}

bool dequeue(Buffer& b, Rekaman& keluar) {
    if (isEmptyBuffer(b)) return false;
    keluar = b.data[b.head];
    b.count -= 1;
    if (b.count == 0) {
        b.head = 0;                          // keadaan kosong baku: head = count = 0
    } else {
        b.head = (b.head + 1) % KAPASITAS;
    }
    return true;
}

bool elemenKe(const Buffer& b, int i, Rekaman& hasil) {
    if (i < 0 || i >= b.count) return false;
    hasil = b.data[(b.head + i) % KAPASITAS];
    return true;
}

void kosongkan(Buffer& b) {
    b.head = 0;
    b.count = 0;
}

int nomorBerikut(const Buffer& b) { return b.nomorBerikutnya; }
long long totalGeser(const Buffer& b) { return b.geser; }   // selalu 0: tidak ada pergeseran
const char* namaVersi() { return "Versi 3 - Circular queue"; }

void cetakKeadaan(const Buffer& b) {
    std::cout << "Keadaan indeks fisik:" << std::endl;
    for (int i = 0; i < KAPASITAS; ++i) {
        std::cout << "  indeks " << i << ": ";
        int jarak = (i - b.head + KAPASITAS) % KAPASITAS;   // jarak dari head searah putaran
        if (jarak < b.count) {
            std::cout << b.data[i].nomor << ":" << b.data[i].nilai;
        } else {
            std::cout << "(kosong)";
        }
        std::cout << std::endl;
    }
    std::cout << "head = " << b.head << ", count = " << b.count;
    if (b.count > 0) {
        std::cout << " (indeks data terakhir, dihitung: " << (b.head + b.count - 1) % KAPASITAS << ")";
    }
    std::cout << std::endl;
}
