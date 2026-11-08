// main.cpp - Program utama Soal 1: sistem sortir dan pengiriman paket
// Pemakaian: ./paket            (interaktif)
//            ./paket --echo < tests/uji_utama.txt   (mode uji: perintah ikut dicetak)
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

#include "paket.h"

using namespace std;

const int MAKS_TOKEN = 16;

struct Statistik {
    int paket;
    long long gram;
};

// ---------- fungsi bantu ----------

// Mengubah teks menjadi bilangan bulat. Menolak huruf, desimal, dan teks terlalu panjang.
static bool parseBulat(const string& s, long long& hasil) {
    if (s.empty()) return false;
    size_t i = 0;
    bool negatif = false;
    if (s[0] == '+' || s[0] == '-') {
        negatif = (s[0] == '-');
        i = 1;
    }
    if (i >= s.size()) return false;
    if (s.size() - i > 15) return false;  // mencegah overflow
    long long v = 0;
    for (; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') return false;
        v = v * 10 + (s[i] - '0');
    }
    hasil = negatif ? -v : v;
    return true;
}

static string kapital(string s) {
    for (size_t i = 0; i < s.size(); ++i) {
        s[i] = static_cast<char>(toupper(static_cast<unsigned char>(s[i])));
    }
    return s;
}

// Memecah baris menjadi token (array biasa). Mengembalikan jumlah token sebenarnya.
static int pecahToken(const string& baris, string token[]) {
    istringstream in(baris);
    string t;
    int total = 0;
    while (in >> t) {
        if (total < MAKS_TOKEN) token[total] = t;
        ++total;
    }
    return total;
}

static void tolak(const string& alasan) {
    cout << "DITOLAK: " << alasan << endl;
}

static void cetakAntrean(const char* label, const Queue& q) {
    cout << label << ": ";
    printInfo(q);
    cout << endl;
}

// Membaca n dan memeriksa aturan transaksi (positif, tidak melebihi isi antrean sumber).
static bool bacaN(const string& teks, const Queue& sumber, const char* nama, int& n) {
    long long v = 0;
    if (!parseBulat(teks, v)) {
        tolak("n harus berupa bilangan bulat");
        return false;
    }
    if (v <= 0) {
        tolak("n harus positif");
        return false;
    }
    int isi = nbList(sumber);
    if (v > isi) {
        tolak(string("antrean ") + nama + " hanya berisi " + to_string(isi) + " paket");
        return false;
    }
    n = static_cast<int>(v);
    return true;
}

// ---------- perintah ----------

static void cmdMasuk(Queue& A, Queue& B, const string token[], int total) {
    if (total != 3) {
        tolak("format: MASUK kode berat (kode harus satu kata tanpa spasi)");
        return;
    }
    const string& kode = token[1];
    if (kode.size() < 1 || kode.size() > static_cast<size_t>(KODE_MAKS)) {
        tolak("kode harus 1-10 karakter");
        return;
    }
    long long berat = 0;
    if (!parseBulat(token[2], berat)) {
        tolak("berat harus berupa bilangan bulat");
        return;
    }
    if (berat < BERAT_MIN || berat > BERAT_MAKS) {
        tolak("berat harus 1-5000 gram");
        return;
    }
    int pos = 0;
    if (findElm(A, kode, pos) != nullptr) {
        tolak("kode " + kode + " sudah aktif di A");
        return;
    }
    if (findElm(B, kode, pos) != nullptr) {
        tolak("kode " + kode + " sudah aktif di B");
        return;
    }
    Node* p = alokasi(kode, static_cast<int>(berat));
    if (p == nullptr) {
        tolak("alokasi memori gagal");
        return;
    }
    if (!enqueue(A, p)) {
        dealokasi(p);
        tolak("node tidak dapat ditambahkan");
        return;
    }
    cout << "MASUK berhasil: " << kode << " (" << berat << " gram) di belakang A" << endl;
}

static void cmdPindah(Queue& A, Queue& B, const string token[], int total) {
    if (total != 2) {
        tolak("format: PINDAH n");
        return;
    }
    int n = 0;
    if (!bacaN(token[1], A, "A", n)) return;

    cout << "PINDAH " << n << " berhasil:" << endl;
    for (int i = 0; i < n; ++i) {
        const Node* alamat = A.head;
        bool diA0 = fFindElm(A, alamat);
        bool diB0 = fFindElm(B, alamat);
        Node* p = dequeue(A);   // node dilepas, tidak di-delete
        enqueue(B, p);          // node yang sama disambung ke belakang B
        bool diA1 = fFindElm(A, alamat);
        bool diB1 = fFindElm(B, alamat);
        cout << "  " << p->info.kode << " (" << p->info.berat << " gram) alamat "
             << static_cast<const void*>(alamat)
             << " | fFindElm A: " << (diA0 ? "ya" : "tidak") << " -> " << (diA1 ? "ya" : "tidak")
             << ", B: " << (diB0 ? "ya" : "tidak") << " -> " << (diB1 ? "ya" : "tidak") << endl;
    }
}

static void cmdKirim(Queue& B, Statistik& stat, const string token[], int total) {
    if (total != 2) {
        tolak("format: KIRIM n");
        return;
    }
    int n = 0;
    if (!bacaN(token[1], B, "B", n)) return;

    long long batch = 0;
    cout << "KIRIM " << n << " berhasil:" << endl;
    for (int i = 0; i < n; ++i) {
        Node* p = dequeue(B);
        cout << "  Terkirim: " << p->info.kode << " (" << p->info.berat << " gram)" << endl;
        stat.paket += 1;
        stat.gram += p->info.berat;
        batch += p->info.berat;
        dealokasi(p);           // data sudah dicatat, node dibebaskan satu kali
    }
    cout << "  Berat batch ini: " << batch << " gram. Total terkirim: " << stat.paket
         << " paket, " << stat.gram << " gram" << endl;
}

static void cmdCari(const Queue& A, const Queue& B, const string token[], int total) {
    if (total != 2) {
        tolak("format: CARI kode");
        return;
    }
    int pos = 0;
    Node* p = findElm(A, token[1], pos);
    const char* lokasi = "A";
    if (p == nullptr) {
        p = findElm(B, token[1], pos);
        lokasi = "B";
    }
    if (p == nullptr) {
        cout << "TIDAK DITEMUKAN" << endl;
        return;
    }
    cout << p->info.kode << ": antrean " << lokasi << ", posisi " << pos
         << ", berat " << p->info.berat << " gram" << endl;
}

static void cmdLihat(const Queue& A, const Queue& B) {
    cetakAntrean("A (menunggu sortir)", A);
    cetakAntrean("B (siap kirim)     ", B);
}

static void cmdRekap(const Queue& A, const Queue& B, const Statistik& stat) {
    cout << "Antrean A : " << nbList(A) << " paket, " << totalBerat(A) << " gram" << endl;
    cout << "Antrean B : " << nbList(B) << " paket, " << totalBerat(B) << " gram" << endl;
    cout << "Terkirim  : " << stat.paket << " paket, " << stat.gram << " gram" << endl;
}

int main(int argc, char* argv[]) {
    bool echo = (argc > 1 && string(argv[1]) == "--echo");

    Queue A, B;
    createQueue(A);
    createQueue(B);
    Statistik stat = {0, 0};

    if (!echo) {
        cout << "Perintah: MASUK kode berat | PINDAH n | KIRIM n | CARI kode | LIHAT | REKAP"
             << " | BERSIHKAN | SELESAI" << endl;
    }

    string baris;
    string token[MAKS_TOKEN];
    while (true) {
        if (!echo) cout << "> " << flush;
        if (!getline(cin, baris)) break;                       // EOF = SELESAI
        if (!baris.empty() && baris[baris.size() - 1] == '\r') baris.erase(baris.size() - 1);

        int total = pecahToken(baris, token);
        if (total == 0) continue;
        if (token[0][0] == '#') {                              // komentar pada file uji
            if (echo) cout << baris << endl;
            continue;
        }
        if (echo) cout << "> " << baris << endl;

        string perintah = kapital(token[0]);
        if (perintah == "MASUK") {
            cmdMasuk(A, B, token, total);
        } else if (perintah == "PINDAH") {
            cmdPindah(A, B, token, total);
        } else if (perintah == "KIRIM") {
            cmdKirim(B, stat, token, total);
        } else if (perintah == "CARI") {
            cmdCari(A, B, token, total);
        } else if (perintah == "LIHAT" && total == 1) {
            cmdLihat(A, B);
        } else if (perintah == "REKAP" && total == 1) {
            cmdRekap(A, B, stat);
        } else if (perintah == "BERSIHKAN" && total == 1) {
            int dibuang = delAll(A) + delAll(B);
            cout << "BERSIHKAN: " << dibuang << " paket dihapus; statistik terkirim tidak berubah" << endl;
        } else if (perintah == "SELESAI" && total == 1) {
            break;
        } else {
            tolak("perintah tidak dikenal atau format salah");
        }
    }

    int sisa = delAll(A) + delAll(B);
    cout << "Program selesai. " << sisa << " paket sisa dibebaskan." << endl;
    return 0;
}
