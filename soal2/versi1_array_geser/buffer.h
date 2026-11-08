// buffer.h - ADT buffer pencatatan hasil ukur (kapasitas 6) - Versi 1: array geser
#ifndef BUFFER_H
#define BUFFER_H

const int KAPASITAS = 6;
const int NILAI_MIN = -1000;
const int NILAI_MAKS = 1000;

struct Rekaman {
    int nomor;   // nomor urut otomatis, mulai dari 1
    int nilai;   // -1000 .. 1000 (0 dan negatif sah)
};

struct Buffer {
    Rekaman data[KAPASITAS];
    int head;   // -1 jika kosong; jika berisi selalu 0
    int tail;   // -1 jika kosong
    int nomorBerikutnya;   // nomor untuk rekaman berikutnya (hanya naik jika berhasil masuk)
    long long geser;       // jumlah rekaman yang dipindahkan karena pergeseran
};

void createBuffer(Buffer& b);
bool isEmptyBuffer(const Buffer& b);
bool isFullBuffer(const Buffer& b);
int jumlahData(const Buffer& b);
int ruangKosong(const Buffer& b);

// Menambah satu rekaman di belakang. false jika penuh (buffer tidak berubah).
bool enqueue(Buffer& b, int nilai);
// Mengambil satu rekaman dari depan. false jika kosong (buffer tidak berubah).
bool dequeue(Buffer& b, Rekaman& keluar);
// Mengambil rekaman urutan ke-i dalam urutan FIFO (0 = terdepan) tanpa mengubah buffer.
bool elemenKe(const Buffer& b, int i, Rekaman& hasil);
// Mengosongkan buffer; nomor berikutnya dan penghitung pergeseran tidak diubah.
void kosongkan(Buffer& b);

int nomorBerikut(const Buffer& b);
long long totalGeser(const Buffer& b);
const char* namaVersi();
// Mencetak keadaan setiap indeks fisik dan penanda (head/tail atau head/count).
void cetakKeadaan(const Buffer& b);

#endif
