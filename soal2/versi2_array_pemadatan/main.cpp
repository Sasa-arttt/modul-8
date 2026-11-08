// main.cpp - Program utama Soal 2: buffer pencatatan hasil ukur (kapasitas 6)
// File ini IDENTIK untuk ketiga versi; hanya buffer.h/buffer.cpp yang berbeda.
// Pemakaian: ./buffer             (interaktif)
//            ./buffer --echo < ../tests/uji_utama.txt   (mode uji: perintah ikut dicetak)
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "buffer.h"

using namespace std;

const int MAKS_TOKEN = 16;

// ---------- fungsi bantu ----------

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

static void cetakIsi(const Buffer& b) {
    int n = jumlahData(b);
    if (n == 0) {
        cout << "(kosong)";
        return;
    }
    for (int i = 0; i < n; ++i) {
        Rekaman r;
        elemenKe(b, i, r);
        if (i > 0) cout << ", ";
        cout << r.nomor << ":" << r.nilai;
    }
}

// ---------- perintah ----------

static void cmdTulis(Buffer& b, const string token[], int total) {
    if (total < 2) {
        tolak("format: TULIS k v1 ... vk");
        return;
    }
    long long k = 0;
    if (!parseBulat(token[1], k)) {
        tolak("k harus berupa bilangan bulat");
        return;
    }
    if (k < 1 || k > KAPASITAS) {
        tolak("k harus 1-6");
        return;
    }
    if (total != k + 2) {
        tolak("jumlah nilai harus tepat k (" + to_string(k) + ")");
        return;
    }
    // Validasi SELURUH batch sebelum mengubah apa pun.
    int nilai[KAPASITAS];
    for (int i = 0; i < k; ++i) {
        long long v = 0;
        if (!parseBulat(token[2 + i], v)) {
            tolak("nilai ke-" + to_string(i + 1) + " (" + token[2 + i] + ") bukan bilangan bulat");
            return;
        }
        if (v < NILAI_MIN || v > NILAI_MAKS) {
            tolak("nilai ke-" + to_string(i + 1) + " (" + token[2 + i] + ") di luar rentang -1000..1000");
            return;
        }
        nilai[i] = static_cast<int>(v);
    }
    if (ruangKosong(b) < k) {
        tolak("ruang kosong hanya " + to_string(ruangKosong(b)) + ", butuh " + to_string(k));
        return;
    }
    // Batch valid: tambahkan satu per satu.
    for (int i = 0; i < k; ++i) {
        enqueue(b, nilai[i]);
    }
    cout << "Berhasil. Isi: ";
    cetakIsi(b);
    cout << endl;
}

static void cmdAmbil(Buffer& b, const string token[], int total) {
    if (total != 2) {
        tolak("format: AMBIL k");
        return;
    }
    long long k = 0;
    if (!parseBulat(token[1], k)) {
        tolak("k harus berupa bilangan bulat");
        return;
    }
    if (k < 1 || k > KAPASITAS) {
        tolak("k harus 1-6");
        return;
    }
    if (jumlahData(b) < k) {
        tolak("data hanya " + to_string(jumlahData(b)) + " rekaman, diminta " + to_string(k));
        return;
    }
    cout << "Keluar: ";
    for (int i = 0; i < k; ++i) {
        Rekaman r;
        dequeue(b, r);
        if (i > 0) cout << ", ";
        cout << r.nomor << ":" << r.nilai;
    }
    cout << endl << "Tersisa: ";
    cetakIsi(b);
    cout << endl;
}

static void cmdLihat(const Buffer& b) {
    cout << namaVersi() << endl;
    cout << "Urutan FIFO: ";
    cetakIsi(b);
    cout << endl;
    cetakKeadaan(b);
    cout << "Nomor berikutnya: " << nomorBerikut(b) << endl;
    cout << "Penghitung pergeseran: " << totalGeser(b) << " rekaman" << endl;
}

static void cmdRingkas(const Buffer& b) {
    int n = jumlahData(b);
    cout << "Jumlah: " << n << endl;
    if (n == 0) {
        cout << "Total: 0" << endl;
        cout << "Minimum: TIDAK ADA" << endl;
        cout << "Maksimum: TIDAK ADA" << endl;
        cout << "Rata-rata: TIDAK ADA" << endl;
        return;
    }
    long long total = 0;
    int minimum = 0, maksimum = 0;
    for (int i = 0; i < n; ++i) {
        Rekaman r;
        elemenKe(b, i, r);
        total += r.nilai;
        if (i == 0 || r.nilai < minimum) minimum = r.nilai;
        if (i == 0 || r.nilai > maksimum) maksimum = r.nilai;
    }
    cout << "Total: " << total << endl;
    cout << "Minimum: " << minimum << endl;
    cout << "Maksimum: " << maksimum << endl;
    cout << "Rata-rata: " << fixed << setprecision(2)
         << static_cast<double>(total) / n << endl;
    cout.unsetf(ios::floatfield);
}

int main(int argc, char* argv[]) {
    bool echo = (argc > 1 && string(argv[1]) == "--echo");

    Buffer buf;
    createBuffer(buf);

    if (!echo) {
        cout << namaVersi() << endl;
        cout << "Perintah: TULIS k v1..vk | AMBIL k | LIHAT | RINGKAS | KOSONGKAN | SELESAI" << endl;
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
        if (perintah == "TULIS") {
            cmdTulis(buf, token, total);
        } else if (perintah == "AMBIL") {
            cmdAmbil(buf, token, total);
        } else if (perintah == "LIHAT" && total == 1) {
            cmdLihat(buf);
        } else if (perintah == "RINGKAS" && total == 1) {
            cmdRingkas(buf);
        } else if (perintah == "KOSONGKAN" && total == 1) {
            kosongkan(buf);
            cout << "Buffer dikosongkan. Nomor berikutnya tetap " << nomorBerikut(buf)
                 << ", penghitung pergeseran tetap " << totalGeser(buf) << endl;
        } else if (perintah == "SELESAI" && total == 1) {
            break;
        } else {
            tolak("perintah tidak dikenal atau format salah");
        }
    }
    cout << "Program selesai." << endl;
    return 0;
}
