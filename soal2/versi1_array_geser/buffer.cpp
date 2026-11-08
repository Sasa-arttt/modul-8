// buffer.cpp - Versi 1: array geser (head selalu 0 saat berisi; dequeue menggeser data)
#include "buffer.h"

#include <iostream>

void createBuffer(Buffer& b) {
    for (int i = 0; i < KAPASITAS; ++i) {
        b.data[i].nomor = 0;
        b.data[i].nilai = 0;
    }
    b.head = -1;
    b.tail = -1;
    b.nomorBerikutnya = 1;
    b.geser = 0;
}

bool isEmptyBuffer(const Buffer& b) { return b.head == -1; }

int jumlahData(const Buffer& b) { return isEmptyBuffer(b) ? 0 : b.tail + 1; }

bool isFullBuffer(const Buffer& b) { return jumlahData(b) == KAPASITAS; }

int ruangKosong(const Buffer& b) { return KAPASITAS - jumlahData(b); }

bool enqueue(Buffer& b, int nilai) {
    if (isFullBuffer(b)) return false;
    if (isEmptyBuffer(b)) {
        b.head = 0;
        b.tail = 0;
    } else {
        b.tail += 1;
    }
    b.data[b.tail].nomor = b.nomorBerikutnya;
    b.data[b.tail].nilai = nilai;
    b.nomorBerikutnya += 1;
    return true;
}

bool dequeue(Buffer& b, Rekaman& keluar) {
    if (isEmptyBuffer(b)) return false;
    keluar = b.data[b.head];                 // head selalu 0
    for (int i = 0; i < b.tail; ++i) {       // geser semua data setelahnya satu tempat ke kiri
        b.data[i] = b.data[i + 1];
        b.geser += 1;                        // satu rekaman dipindah = satu hitungan
    }
    if (b.tail == 0) {
        b.head = -1;
        b.tail = -1;
    } else {
        b.tail -= 1;
    }
    return true;
}

bool elemenKe(const Buffer& b, int i, Rekaman& hasil) {
    if (i < 0 || i >= jumlahData(b)) return false;
    hasil = b.data[b.head + i];
    return true;
}

void kosongkan(Buffer& b) {
    b.head = -1;
    b.tail = -1;
}

int nomorBerikut(const Buffer& b) { return b.nomorBerikutnya; }
long long totalGeser(const Buffer& b) { return b.geser; }
const char* namaVersi() { return "Versi 1 - Array geser"; }

void cetakKeadaan(const Buffer& b) {
    std::cout << "Keadaan indeks fisik:" << std::endl;
    for (int i = 0; i < KAPASITAS; ++i) {
        std::cout << "  indeks " << i << ": ";
        if (!isEmptyBuffer(b) && i >= b.head && i <= b.tail) {
            std::cout << b.data[i].nomor << ":" << b.data[i].nilai;
        } else {
            std::cout << "(kosong)";
        }
        std::cout << std::endl;
    }
    std::cout << "head = " << b.head << ", tail = " << b.tail << std::endl;
}
