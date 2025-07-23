# Quaternion_Visualizer
Repositori ini berisi program visualisasi quaternion yang ditulis dalam bahasa C++ menggunakan OpenGL sebagai salah satu tugas kualifikasi asisten lab Ilmu dan Rekayasa Komputasi.

# Deskripsi Program & Fitur Program

# Teknologi dan Framework

# Penjelasan Quaternion dan Kegunaannya

# Screenshot Hasil Percobaan

# Cara Menjalankan Program
## Prasyarat
0. Pastikan sudah menginstall compiler C++ yang mendukung C++17 atau lebih baru.
1. Pastikan sudah menginstall CMake, bisa diunduh dari [sini](https://cmake.org/download/). Aplikasi ini sudah diuji dengan CMake versi 4.1.0-rc2.
2. Pastikan sudah menginstall SDL2. Aplikasi ini sudah diuji dengan SDL2 versi 2.0.20.
   - Untuk pengguna Windows, bisa mengunduh dari [sini](https://github.com/libsdl-org/SDL/releases/tag/release-2.0.20).
   - Untuk pengguna Linux, bisa menginstall melalui package manager, misalnya `sudo apt-get install libsdl2-dev libsdl2-ttf-dev` untuk Ubuntu.
   - Untuk pengguna MacOS, bisa menginstall melalui Homebrew dengan perintah `brew install sdl2 sdl2_ttf`.

## Kompilasi dan Menjalankan Program
1. Buka terminal atau command prompt.
2. Arahkan ke direktori repositori ini.
3. Buat direktori build dan masuk ke dalamnya:
    ```bash
    mkdir build
    cd build
    ```
4. Jalankan perintah CMake untuk mengkonfigurasi proyek:
    ```bash
    cmake ..
    ```
5. Setelah konfigurasi selesai, kompilasi proyek dengan perintah:
    ```bash
    cmake --build .
    ```
6. Jalankan dengan perintah:
    ```bash
    ./QuaternionVisualizer
    ```

# Referensi

# Konvensi Penamaan
Hanya reminder untuk diri sendiri. :D
1. Nama file pake PascalCase (pengecualian buat main.cpp): `namaFile.cpp`, `namaFile.hpp`.
2. Nama kelas pake PascalCase: `NamaKelas`.
3. Nama metode kelas pake camelCase: `namaMetode()`.
4. Nama fungsi global pake snake_case: `nama_fungsi()`.
5. Nama atribut kelas pake _camelCase: `_namaAtribut`.
6. Nama variabel lokal dan parameter fungsi pake camelCase: `namaVariabel`.
7. Nama konstanta pake UPPER_SNAKE_CASE: `NAMA_KONSTANTA`.
8. Jangan disingkat (ntar bingung sendiri T_T).