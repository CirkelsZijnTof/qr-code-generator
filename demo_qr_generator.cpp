#include "qr_generator.hpp"

// D:\msys\ucrt64\bin\g++.exe demo_qr_generator.cpp classes.cpp icons\app.res -o executables\demo_qr_generator.exe -lraylib -lopengl32 -lgdi32 -lwinmm

int main()
{

    QRCode prototype;
    BitPlacement placer(7);

    std::string test_easy_1 = "12345678901";
    std::string test_easy_2 = "123";
    std::string test_easy_3 = "Hello";
    std::string test_standard_1 = "https://www.qrcode.com/";
    std::string test_standard_2 = "https://www.youtube.com/watch?v=dQw4w9WgXcQ";
    std::string test_difficult = "Hello é€😃"; // Emoji: 4 bytes. Euro-sign: 3. é: 2. Standard char, 1.

    //prototype.generate(test_easy_2);


    std::cin.get();

    return 0;
};