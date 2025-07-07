#include <iostream> // Untuk input/output seperti cin, cout
#include <vector>   // Untuk menggunakan std::vector
#include <iomanip>  // Untuk manipulasi output (setw, fixed, setprecision)
#include <regex>    // Untuk validasi menggunakan regular expression
#include <fstream>  // Untuk operasi file (ofstream, ifstream)
#include <sstream>  // Untuk stringstream, digunakan dalam pemrosesan tanggal

using namespace std; // Menggunakan namespace std agar tidak perlu menulis std:: berulang kali

// Struktur untuk menyimpan data mobil
struct mobil {
    string nama;
    double hargaPerHari;
};

// Struktur untuk menyimpan identitas pengguna
struct Identitas {
    string nama;
    string nik;
    string nomorTelepon;
    string alamat;
    string username;
    string password;
};

// Fungsi untuk memvalidasi NIK (harus 16 digit angka)
bool validasiNIK(const string& nik) {
    return regex_match(nik, regex("\\d{16}"));
}

// Fungsi untuk memvalidasi Nomor Telepon (harus 12 digit angka)
bool validasiNomorTelepon(const string& nomorTelepon) {
    return regex_match(nomorTelepon, regex("\\d{12}"));
}

// Fungsi untuk menyimpan data identitas ke file
void simpanKeFile(const Identitas& identitas) {
    // Membuka file dalam mode append (ios::app) agar data baru ditambahkan di akhir
    ofstream file("data_identitas.txt", ios::app);
    if (file.is_open()) { // Memastikan file berhasil dibuka
        file << "Nama          : " << identitas.nama << endl;
        file << "NIK           : " << identitas.nik << endl;
        file << "Nomor Telepon: " << identitas.nomorTelepon << endl;
        file << "Alamat        : " << identitas.alamat << endl;
        file << "Username      : " << identitas.username << endl;
        file << "Password      : " << identitas.password << endl;
        file << "-----------------------------------" << endl;
        file.close(); // Menutup file setelah selesai menulis
        cout << "Data berhasil disimpan ke database!" << endl;
    } else {
        // Pesan error jika gagal membuka file
        cerr << "Gagal membuka file untuk menyimpan data. " << endl;
    }
}

// Fungsi untuk memeriksa kredensial login dari file
bool cekLogin(const string& username, const string& password) {
    ifstream file("data_identitas.txt"); // Membuka file untuk dibaca
    if (!file.is_open()) {
        cerr << "Gagal membuka file untuk login!" << endl;
        return false;
    }

    string line;
    string fileUsername, filePassword;

    // Membaca file baris per baris
    while (getline(file, line)) {
        // Mencari baris yang mengandung "Username"
        if (line.find("Username      :") != string::npos) {
            // Mengambil substring setelah "Username      :"
            fileUsername = line.substr(15);
        }
        // Mencari baris yang mengandung "Password"
        else if (line.find("Password      :") != string::npos) {
            // Mengambil substring setelah "Password      :"
            filePassword = line.substr(15);
            // Membandingkan username dan password yang dimasukkan dengan yang ada di file
            if (fileUsername == username && filePassword == password) {
                file.close();
                return true; // Login berhasil
            }
        }
    }

    file.close();
    return false; // Login gagal (username/password tidak cocok atau tidak ditemukan)
}

// Fungsi untuk memperbarui data identitas di file
void perbaruiDataKeFile(const Identitas& identitas) {
    ifstream fileIn("data_identitas.txt"); // Membuka file untuk membaca
    if (!fileIn.is_open()) {
        cerr << "Gagal membuka file untuk membaca data!" << endl;
        return;
    }

    stringstream buffer; // Menggunakan stringstream untuk menyimpan seluruh isi file sementara
    string line;
    bool userDitemukan = false;

    // Membaca file input baris per baris
    while (getline(fileIn, line)) {
        // Jika baris mengandung "Username" dan username cocok dengan yang ingin diperbarui
        if (line.find("Username      :") != string::npos && line.substr(15) == identitas.username) {
            userDitemukan = true;
            // Menulis data identitas yang baru ke buffer
            buffer << "Nama          : " << identitas.nama << endl;
            buffer << "NIK           : " << identitas.nik << endl;
            buffer << "Nomor Telepon: " << identitas.nomorTelepon << endl;
            buffer << "Alamat        : " << identitas.alamat << endl;
            buffer << "Username      : " << identitas.username << endl;
            buffer << "Password      : " << identitas.password << endl;
            buffer << "-----------------------------------" << endl;

            // Melewatkan 6 baris berikutnya yang merupakan data lama dari pengguna ini
            for (int i = 0; i < 6; i++) {
                getline(fileIn, line);
            }
        } else {
            // Jika bukan baris yang diperbarui, salin baris asli ke buffer
            buffer << line << endl;
        }
    }

    fileIn.close(); // Menutup file input

    if (!userDitemukan) {
        cout << "Username tidak ditemukan dalam database." << endl;
        return;
    }

    // Membuka kembali file dalam mode overwrite untuk menulis data yang sudah diperbarui
    ofstream fileOut("data_identitas.txt");
    if (!fileOut.is_open()) {
        cerr << "Gagal membuka file untuk menulis data!" << endl;
        return;
    }

    fileOut << buffer.str(); // Menulis seluruh isi buffer ke file
    fileOut.close(); // Menutup file output
    cout << "Data berhasil diperbarui!" << endl;
}

// Fungsi untuk menu pengaturan
void menuPengaturan() {
    string username, password;
    Identitas identitasBaru; // Membuat objek identitas baru untuk menyimpan data yang diperbarui

    cout << "Masukkan Username untuk verifikasi: ";
    cin >> username;
    cout << "Masukkan Password untuk verifikasi: ";
    cin >> password;

    // Memeriksa login sebelum mengizinkan perubahan data
    if (!cekLogin(username, password)) {
        cout << "Username atau Password salah!" << endl;
        return;
    }

    cout << "Masukkan data baru Anda.\n";
    cout << "Nama: ";
    cin.ignore(); // Membersihkan newline character dari buffer cin
    getline(cin, identitasBaru.nama); // Menggunakan getline untuk nama (bisa mengandung spasi)

    // Loop untuk validasi NIK
    do {
        cout << "Masukkan NIK (16 digit angka): ";
        cin >> identitasBaru.nik; // Menggunakan cin >> untuk NIK (tidak mengandung spasi)
        if (!validasiNIK(identitasBaru.nik)) {
            cout << "NIK tidak valid. Harus terdiri dari 16 digit angka.\n";
        }
    } while (!validasiNIK(identitasBaru.nik));

    // Loop untuk validasi Nomor Telepon
    do {
        cout << "Masukkan Nomor Telepon (minimal 12 digit angka): ";
        cin >> identitasBaru.nomorTelepon; // Menggunakan cin >> untuk nomor telepon
        if (!validasiNomorTelepon(identitasBaru.nomorTelepon)) {
            cout << "Nomor telepon tidak valid. Harus terdiri dari minimal 12 digit angka.\n";
        }
    } while (!validasiNomorTelepon(identitasBaru.nomorTelepon));

    cin.ignore(); // Membersihkan newline character setelah cin >>
    cout << "Masukkan Alamat: ";
    getline(cin, identitasBaru.alamat); // Menggunakan getline untuk alamat

    identitasBaru.username = username; // Username tidak berubah, diambil dari input verifikasi
    cout << "Masukkan Password baru: ";
    getline(cin, identitasBaru.password); // Menggunakan getline untuk password

    perbaruiDataKeFile(identitasBaru); // Memanggil fungsi untuk memperbarui data
}

int main() {
    Identitas identitas; // Mendeklarasikan objek identitas
    int pilihan1;

    // Menampilkan menu utama
    cout << "Pilihlah yang ingin dilakukan :\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Pengaturan\n";
    cout << "4. Pusat Bantuan\n";
    cout << "5. Sewa Mobil\n";
    cout << "6. Pengembalian Mobil\n";
    cout << "Tekan tombol lainnya untuk keluar.\n";
    cin >> pilihan1; // Menerima pilihan dari pengguna
    cin.ignore(); // Membersihkan newline character dari buffer cin setelah cin >> pilihan1

    // Menggunakan switch-case untuk menangani pilihan menu
    switch (pilihan1) {
        case 1: { // Register
            cout << "Masukkan Nama: ";
            getline(cin, identitas.nama); // Menggunakan getline untuk nama (bisa mengandung spasi)
            do {
                cout << "Masukkan NIK (16 digit angka): ";
                cin >> identitas.nik; // Menggunakan cin >> untuk NIK
                if (!validasiNIK(identitas.nik)) {
                    cout << "NIK tidak valid. Harus terdiri dari 16 digit angka. \n";
                }
            } while (!validasiNIK(identitas.nik));
            do {
                cout << "Masukkan Nomor Telepon (minimal 12 digit angka): ";
                cin >> identitas.nomorTelepon; // Menggunakan cin >> untuk nomor telepon
                if (!validasiNomorTelepon(identitas.nomorTelepon)) {
                    cout << "Nomor telepon tidak valid. Harus terdiri dari minimal 12 digit angka! \n";
                }
            } while (!validasiNomorTelepon(identitas.nomorTelepon));
            cin.ignore(); // Membersihkan newline character setelah cin >> identitas.nomorTelepon
            cout << "Masukkan Alamat: ";
            getline(cin, identitas.alamat); // Menggunakan getline untuk alamat
            cout << "Masukkan Username: ";
            getline(cin, identitas.username); // Menggunakan getline untuk username
            cout << "Masukkan Password: ";
            getline(cin, identitas.password); // Menggunakan getline untuk password
            simpanKeFile(identitas); // Menyimpan data identitas ke file
            break; // Keluar dari switch case
        }
        case 2: { // Login
            cout << "Username: ";
            cin >> identitas.username;
            cout << "Password: ";
            cin >> identitas.password;

            if (cekLogin(identitas.username, identitas.password)) {
                cout << "Login Berhasil!" << endl;
            } else {
                cout << "Login Gagal! Username atau Password salah." << endl;
            }
            break; // Keluar dari switch case
        }
        case 3: { // Pengaturan
            menuPengaturan(); // Memanggil fungsi menu pengaturan
            break; // Keluar dari switch case
        }
        case 4: { // Pusat Bantuan
            string bantuan;
            cout << "Anda ingin menghubungi customer service atau melihat FAQ? ";
            cin >> bantuan;
            if (bantuan == "cs") {
                cout << "Anda akan segera dihubungkan ke customer service" << endl;
            } else {
                cout << "Berikut adalah beberapa Frequently Asked Question." << endl;
            }
            break; // Keluar dari switch case
        }
        case 5: { // Sewa Mobil
            vector<mobil> daftarMobil = {
                {"Avanza", 300000},
                {"Innova", 300000},
                {"Fortuner", 550000},
                {"Alphard", 1200000}
            };

            double totalHarga = 0.0;
            string status = "Tidak Lunas";

            cout << "=========================================" << endl;
            cout << "           Daftar Mobil                " << endl;
            cout << "=========================================" << endl;

            for (size_t i = 0; i < daftarMobil.size(); i++) {
                cout << fixed << setprecision(0); // Mengatur output double tanpa desimal
                cout << i + 1 << ". " << setw(12) << left << daftarMobil[i].nama << "Rp " << daftarMobil[i].hargaPerHari << "/hari\n" << endl;
            }

            int pilihan;
            cout << "\nPilih nomor mobil yang ingin anda sewa: ";
            cin >> pilihan;

            if (pilihan < 1 || pilihan > daftarMobil.size()) {
                cout << "Pilihan tidak valid. Program selesai. \n";
                // Jika program utama tidak memiliki loop, return 0; akan menghentikan aplikasi.
                // Jika ada loop menu utama, gunakan break;
                return 0; // Menghentikan program
            }

            string tglsewa;
            string tglselesai;

            cout << "Tanggal Sewa         : ";
            cin.ignore(); // Membersihkan newline character
            getline(cin, tglsewa);
            cout << "Tanggal Selesai      : ";
            getline(cin, tglselesai);

            int durasi;
            cout << "Masukkan durasi sewa (dalam hari): ";
            cin >> durasi;

            if (durasi < 1) {
                cout << "Durasi sewa harus minimal 1 hari. Program selesai. \n";
                return 0; // Menghentikan program
            }

            mobil mobilTerpilih = daftarMobil[pilihan - 1];
            totalHarga = mobilTerpilih.hargaPerHari * durasi;

            cout << "\nMobil yang disewa    : " << mobilTerpilih.nama << endl;
            cout << "Tanggal Sewa         : " << tglsewa << endl;
            cout << "Tanggal Selesai      : " << tglselesai << endl;
            cout << "Durasi sewa          : " << durasi << endl;
            cout << fixed << setprecision(0);
            cout << "Total biaya          : " << totalHarga << endl;

            string metode;
            string pilihan1_metode = "Transfer"; // Mengubah nama variabel agar tidak bentrok dengan pilihan1 di main
            string pilihan2_metode = "QRIS";     // Mengubah nama variabel

            do {
                cout << "\nIngin melanjutkan pembayaran menggunakan " << pilihan1_metode << " atau " << pilihan2_metode << "?" << endl;
                cin >> metode;

                if (metode != pilihan1_metode && metode != pilihan2_metode) {
                    cout << "\nMetode Pembayaran tidak valid. Silahkan coba lagi." << endl;
                }
            } while (metode != pilihan1_metode && metode != pilihan2_metode);
            cout << "Anda memilih: " << metode << endl;

            double pembayaran;
            while (true) {
                cout << "\nMasukkan jumlah pembayaran: Rp ";
                cin >> pembayaran;

                if (pembayaran == totalHarga) {
                    status = "Lunas";
                    cout << "\nTransaksi berhasil\n";
                    break;
                } else if (pembayaran > totalHarga) {
                    cout << "Nominal tidak sesuai, masukkan jumlah yang tepat.\n";
                } else {
                    cout << "Uang anda kurang. Masukkan jumlah yang cukup.\n";
                }
            }

            cout << "\nStatus Transaksi: " << status << endl;
            cout << "\nTerimakasih telah menggunakan layanan sewa mobil kami.\n";
            return 0; // Menghentikan program
        }
        case 6: { // Pengembalian Mobil
            string nama2;
            string notelp2;
            string tglSelesai;   // Tanggal selesai sewa yang seharusnya
            string tglKembali;   // Tanggal mobil dikembalikan
            double hargaDendaPerHari = 50000;

            cout << "=================================================================" << endl;
            cout << "             Form Pengembalian Mobil                           " << endl;
            cout << "=================================================================" << endl;
            cout << "Nama                          : ";
            cin.ignore(); // Membersihkan newline character
            getline(cin, nama2);
            cout << "Nomor Telepon                 : ";
            getline(cin, notelp2);
            cout << "Tanggal Selesai Sewa (YYYY-MM-DD) : ";
            getline(cin, tglSelesai);
            cout << "Tanggal Kembali Mobil (YYYY-MM-DD): ";
            getline(cin, tglKembali);

            // Lambda function untuk mengkonversi tanggal string (YYYY-MM-DD) menjadi integer YYYYMMDD
            // Perhatian: Perhitungan selisih hari dengan metode ini (mengurangi integer YYYYMMDD)
            // tidak akurat untuk menghitung selisih hari kalender, terutama lintas bulan/tahun.
            // Untuk perhitungan tanggal yang akurat, Anda perlu menggunakan library tanggal/waktu
            // seperti <chrono> atau mengimplementasikan logika tanggal yang lebih kompleks.
            auto convertToDateInt = [](const string& date) {
                stringstream ss(date);
                string year, month, day;
                getline(ss, year, '-');
                getline(ss, month, '-');
                getline(ss, day);
                return stoi(year + month + day); // Menggabungkan YYYY, MM, DD menjadi satu integer
            };

            int selesaiInt = convertToDateInt(tglSelesai);
            int kembaliInt = convertToDateInt(tglKembali);

            if (kembaliInt > selesaiInt) {
                // Perhitungan daysLate ini tidak akurat karena hanya mengurangi integer YYYYMMDD.
                // Contoh: 20230105 - 20230101 = 4 (benar)
                // Tapi: 20230201 - 20230131 = 70 (salah, seharusnya 1 hari)
                // Ini perlu diperbaiki dengan logika perhitungan tanggal yang sebenarnya.
                int daysLate = (kembaliInt - selesaiInt) / 10000 * 365
                             + ((kembaliInt - selesaiInt) % 10000) / 100 * 30
                             + (kembaliInt - selesaiInt) % 100;
                double totalDenda = daysLate * hargaDendaPerHari;
                cout << "\nPengembalian terlambat " << daysLate << " hari." << endl;
                cout << "Denda yang harus dibayar: Rp " << totalDenda << endl;
            } else {
                cout << "\nPengembalian tepat waktu. Terima kasih!" << endl;
            }
            break; // Keluar dari switch case
        }
        default: // Pilihan selain 1-6
            cout << "Program telah dihentikan. Terimakasih" << endl;
            return 0; // Menghentikan program
    }
    // Jika Anda ingin program kembali ke menu utama setelah setiap case,
    // Anda perlu membungkus seluruh switch-case ini dalam sebuah loop (misalnya while(true))
    // dan hanya return 0; atau exit(0); ketika pengguna memilih opsi keluar.

    // Jika program mencapai sini, berarti salah satu case telah dieksekusi dan program akan berakhir.
    return 0; // Menghentikan program secara normal
}
