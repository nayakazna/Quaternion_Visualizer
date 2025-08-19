# Quaternion_Visualizer
Repositori ini berisi program visualisasi quaternion yang ditulis dalam bahasa C++ menggunakan OpenGL sebagai salah satu tugas kualifikasi asisten lab Ilmu dan Rekayasa Komputasi.

# Deskripsi Program & Fitur Program
Program ini merupakan aplikasi GUI desktop untuk memvisualisasikan rotasi quaternion terhadap suatu objek 3D dari file berekstensi .OBJ. Sayangnya baru di-testing di Ubuntu 22.04 LTS saja.

# Teknologi dan Framework
1. Bahasa: C++17 (of course)
2. Library eksternal: cuma SDL2, untuk mencetak dari ruang 3D ke kanvas 2D (layar)

# Penjelasan Quaternion dan Kegunaannya
(maaf sedikit copas dari makalah saya, nggak saya translate sendiri)

Kuaternion, biasanya dilambangkan sebagai $q$, adalah elemen dari ruang vektor berdimensi-4, $\mathbb{H}$, di atas $\mathbb{R}$ [^1][^2].  
Sebuah kuaternion dapat diekspresikan dalam bentuk

$$
q = \langle w, x, y, z \rangle = w + xi + yj + zk,
$$

di mana $w, x, y,$ dan $z$ adalah bilangan real, sedangkan komponen imajiner $i, j$, dan $k$ didefinisikan dengan hubungan

$$
i^2 = j^2 = k^2 = ijk = -1.
$$

Komponen imajiner juga saling berhubungan sebagai berikut:

$$
ij = -ji = k, \\
jk = -kj = i, \\
ki = -ik = j.
$$

Kita juga dapat memandang $w$ sebagai skalar dan $xi + yj + zk$ sebagai vektor di $\mathbb{R}^3$.  
Jika $w = 0$, maka kuaternion tersebut menjadi vektor berdimensi-3 biasa. Kuaternion dengan bentuk khusus ini disebut **kuaternion murni** (*pure quaternion*).


## Norma Kuaternion

Salah satu sifat utama kuaternion adalah **norma**, padanan berdimensi-4 dari panjang vektor berdimensi-3, yang merupakan bilangan real dan didefinisikan sebagai

$$
|q| = \sqrt{w^2 + x^2 + y^2 + z^2}.
$$


## Penjumlahan dan Pengurangan

Misalkan $q_1 = w_1 + x_1i + y_1j + z_1k$ dan $q_2 = w_2 + x_2i + y_2j + z_2k$.  
Penjumlahan dan pengurangan dua kuaternion $q_1$ dan $q_2$ didefinisikan sebagai

$$
q_1 \pm q_2 = (w_1 \pm w_2) + (x_1 \pm x_2)i + (y_1 \pm y_2)j + (z_1 \pm z_2)k.
$$

Penjumlahan dan pengurangan kuaternion bersifat komutatif dan asosiatif.


## Perkalian Kuaternion

Perkalian kuaternion didefinisikan sebagai

$$
\begin{aligned}
q_1 q_2 &= (w_1 + x_1i + y_1j + z_1k)(w_2 + x_2i + y_2j + z_2k) \\
&= (w_1w_2 - x_1x_2 - y_1y_2 - z_1z_2) \\
&\quad + (w_1x_2 + x_1w_2 + y_1z_2 - z_1y_2)i \\
&\quad + (w_1y_2 - x_1z_2 + y_1w_2 + z_1x_2)j \\
&\quad + (w_1z_2 + x_1y_2 - y_1x_2 + z_1w_2)k.
\end{aligned}
$$

Hal ini dapat dipandang seperti mendistribusikan setiap komponen dari $q_1$ ke $q_2$, serupa dengan penerapan hukum distributif dalam aritmetika, sambil memperhatikan identitas yang sudah didefinisikan sebelumnya.


## Konjugat Kuaternion

Konjugat dari sebuah kuaternion $q = w + xi + yj + zk$, yang dilambangkan sebagai $q^*$ atau $\bar{q}$, didefinisikan sebagai

$$
q^* = \overline{q} = w - xi - yj - zk.
$$

Salah satu hasil menarik dari ini adalah

$$
\begin{aligned}
q \cdot q^* &= (w + xi + yj + zk)(w - xi - yj - zk) \\
&= w^2 + x^2 + y^2 + z^2 \\
&= |q|^2.
\end{aligned}
$$


## Invers Kuaternion

Hasil tersebut dapat digunakan untuk menurunkan rumus **invers perkalian** dari sebuah kuaternion tak nol, $q^{-1}$.  
Karena $q \cdot q^{-1} = 1$ secara definisi, maka kita peroleh

$$
q^{-1} = \frac{q^*}{|q|^2}.
$$


## Kuaternion Satuan dan Rotasi

Kuaternion dengan norma $1$ disebut **kuaternion satuan**, yang dapat direpresentasikan sebagai

$$
q = \cos \theta + \hat{\mathbf{u}} \sin \theta,
$$

di mana $\hat{\mathbf{u}}$ adalah vektor normal berdimensi-3.  
Dengan cara serupa,

$$
q^{-1} = \cos \theta - \hat{\mathbf{u}} \sin \theta.
$$

Menggunakan bentuk ini, kita dapat merotasikan sembarang vektor $\mathbf{v}$ dengan sudut tertentu.  
Misalkan $\mathbf{u}$ adalah vektor sepanjang sumbu rotasi pilihan kita sehingga vektor normal dalam arah tersebut adalah $\hat{\mathbf{u}}$, dan misalkan $2\theta$ adalah sudut rotasi berlawanan arah jarum jam.  

Kita dapat mengekspresikan vektor $\mathbf{v}$ sebagai kuaternion murni dengan menuliskannya sebagai $p = 0 + \mathbf{v}$.  
Vektor hasil rotasi $\mathbf{v'}$, dalam bentuk kuaternion murni $p' = 0 + \mathbf{v'}$, dapat diperoleh melalui persamaan

$$
p' = qpq^{-1}.
$$

## Kegunaan Quaternion
Kuaternion banyak digunakan untuk grafika komputer dan game 3-dimensi, robotika, serta masih banyak lagi. Hal paling bermanfaat dari kuaternion adalah rotasinya. Rotasi Euler bisa mengalami gimbal lock (ketika dua sumbu rotasi berimpitan), sementara kuaternion tidak.

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
0. Taruh model OBJ yang mau diuji di folder `models`.
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
1. E. Lengyel, Mathematics for 3D Game Programming and Computer
 Graphics, 3rd ed. Boston, MA, USA: Cengage Learning, 2011, pp. 317-329. ISBN: 978-1-4354-5886-4.
2.  R. Goldman, “Understanding quaternions,” Graphical Models, vol. 73, no. 2. Elsevier BV, pp. 21–49, Mar. 2011. doi:10.1016/j.gmod.2010.10.004.
3. [https://www.youtube.com/watch?v=ih20l3pJoeU&ab_channel=javidx9](https://www.youtube.com/watch?v=ih20l3pJoeU&ab_channel=javidx9)

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