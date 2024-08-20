# antrian-rumah-sakit

## Deskripsi
Proyek ini adalah aplikasi sistem antrian rumah sakit sederhana yang dibuat menggunakan SDL2 dan SDL_ttf. Aplikasi ini memungkinkan pengguna untuk mendaftarkan pasien, melayani pasien, dan menampilkan antrian pasien dengan antarmuka grafis sederhana.
![image](https://github.com/user-attachments/assets/6b64e76c-a8a1-4968-b14d-af3499e379e2)


## Fitur
- **Pendaftaran Pasien**: Menambahkan pasien baru ke antrian dengan nama, ID, dan poli.
- **Pelayanan Pasien**: Mengambil pasien dari antrian untuk dilayani.
- **Tampilan Antrian**: Menampilkan daftar pasien yang ada dalam antrian.
- **Antarmuka Grafis**: Menyediakan tombol untuk pendaftaran, pelayanan, dan keluar, serta tampilan tabel untuk antrian pasien.

## Prerequisites
Sebelum menjalankan proyek ini, pastikan Anda telah menginstal SDL2 dan SDL_ttf. Anda dapat mengunduh library ini dari [SDL2 website](https://www.libsdl.org/) dan [SDL_ttf website](https://www.libsdl.org/projects/SDL_ttf/).

### Instalasi SDL2 dan SDL_ttf
- **Windows**: Ikuti petunjuk instalasi di [SDL2 untuk Windows](https://www.libsdl.org/download-2.0.php).
- **Linux**: Anda bisa menginstal SDL2 dan SDL_ttf menggunakan package manager, misalnya dengan `sudo apt-get install libsdl2-dev libsdl2-ttf-dev` di Ubuntu.

## Cara Menjalankan
1. **Unduh atau Kloning Repository:**

    ```sh
    git clone https://github.com/username/repository.git
    cd repository
    ```

2. **Kompilasi Program:**

    Gunakan perintah berikut untuk mengkompilasi program:

    ```sh
    g++ main.cpp -o sistem_antrian -lSDL2 -lSDL2_ttf
    ```

3. **Jalankan Program:**

    Setelah kompilasi berhasil, jalankan program dengan perintah:

    ```sh
    ./sistem_antrian
    ```

## Struktur Kode
- **`main.cpp`**: Kode sumber utama yang berisi implementasi logika aplikasi dan antarmuka grafis.
- **`renderText`**: Fungsi untuk merender teks pada layar.
- **`renderButton`**: Fungsi untuk merender tombol dengan efek interaksi.
- **`renderTable`**: Fungsi untuk merender tabel antrian pasien.
- **`AntrianRumahSakit`**: Kelas yang mengelola antrian pasien dan pelayanan pasien.

## Cara Penggunaan
- Klik tombol "Daftar Pasien" untuk memulai pendaftaran pasien. Ikuti petunjuk untuk memasukkan nama, ID, dan poli pasien.
- Klik tombol "Layani Pasien" untuk melayani pasien pertama dalam antrian.
- Klik tombol "Keluar" untuk menutup aplikasi.

## Lisensi
Proyek ini dilisensikan di bawah MIT License. Lihat file `LICENSE` untuk rincian lebih lanjut.

## Kontak
Jika Anda memiliki pertanyaan atau masukan, silakan hubungi [email@example.com](mailto:email@example.com).

---

Terima kasih telah menggunakan proyek ini!
