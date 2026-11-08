// buffer.cpp - Versi 2: array dengan pemadatan
// dequeue hanya memajukan head; data digeser ke awal HANYA jika penambahan butuh ruang
// dan tail sudah berada di indeks terakhir (5).
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

int jumlahData(const Buffer& b) { return isEmptyBuffer(b) ? 0 : b.tail - b.head + 1; }

bool isFullBuffer(const Buffer& b) { return jumlahData(b) == KAPASITAS; }

int ruangKosong(const Buffer& b) { return KAPASITAS - jumlahData(b); }

// Memadatkan data aktif ke awal array; setiap rekaman yang dipindah menambah penghitung geser.
static void padatkan(Buffer& b) {
    int n = jumlahData(b);
    for (int i = 0; i < n; ++i) {
        b.data[i] = b.data[b.head + i];
        b.geser += 1;
    }
    b.head = 0;
    b.tail = n - 1;
}

bool enqueue(Buffer& b, int nilai) {
    if (isFullBuffer(b)) return false;
    if (isEmptyBuffer(b)) {
        b.head = 0;
        b.tail = 0;
    } else {
        if (b.tail == KAPASITAS - 1) {
            padatkan(b);     // tidak penuh, jadi head > 0 dan pemadatan pasti membuka ruang
        }
        b.tail += 1;
    }
    b.data[b.tail].nomor = b.nomorBerikutnya;
    b.data[b.tail].nilai = nilai;
    b.nomorBerikutnya += 1;
    return true;
}

bool dequeue(Buffer& b, Rekaman& keluar) {
    if (isEmptyBuffer(b)) return false;
    keluar = b.data[b.head];
    if (b.head == b.tail) {
        b.head = -1;
        b.tail = -1;
    } else {
        b.head += 1;
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
const char* namaVersi() { return "Versi 2 - Array dengan pemadatan"; }

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
