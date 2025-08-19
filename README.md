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
2. Pastikan sudah menginstall SDL2 dan SDL2_ttf. Aplikasi ini sudah diuji dengan SDL2 versi 2.0.20.

    * **Untuk pengguna Windows (ribet njirrrr mending instal WSL):**
        1.  Unduh SDL2 dari [sini](https://github.com/libsdl-org/SDL/releases/download/release-2.0.20/SDL2-devel-2.0.20-mingw.tar.gz) dan SDL2_ttf dari [sini](https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.0.18/SDL2_ttf-devel-2.0.18-mingw.tar.gz).
        2.  Buat folder di lokasi yang mudah diakses, misalnya `C:\Libs\SDL2`.
        3.  Ekstrak **isi** dari kedua file `.tar.gz` yang diunduh ke dalam folder `C:\Libs\SDL2`. Pastikan file `.dll`, `.lib`, dan `.h` dari kedua pustaka berada di subfolder yang sesuai (`bin`, `lib`, `include`) di dalam `C:\Libs\SDL2`.
        4.  Sebelum menjalankan `cmake ..`, atur variabel lingkungan `SDL2_DIR` ke `C:\Libs\SDL2` atau tambahkan baris berikut di awal `CMakeLists.txt`:
            ```cmake
            set(SDL2_DIR "C:/Libs/SDL2" CACHE PATH "Path to SDL2 installation")
            ```
        5. Setelah itu, pastikan untuk menambahkan `C:\Libs\SDL2\bin` ke dalam PATH agar program dapat menemukan file `.dll` saat dijalankan.

    * **Untuk pengguna Linux (Ubuntu/Debian):**
        Install melalui *package manager*:
        ```bash
        sudo apt-get install libsdl2-dev libsdl2-ttf-dev
        ```

    * **Untuk pengguna macOS:**
        Install melalui Homebrew:
        ```bash
        brew install sdl2 sdl2_ttf
        ```

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
6. Jalankan:
    ```bash
    ./quaternion_visualizer
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