#include "qr_generator.hpp"

#include <bitset>
#include <cstddef>

// D:\msys\ucrt64\bin\g++.exe demo_qr_generator.cpp classes.cpp icons\app.res -o executables\demo_qr_generator.exe -lraylib -lopengl32 -lgdi32 -lwinmm



int main()
{

    //raylibsUtils DRAW;
    QRCode prototype;

    std::string text = "Hello é € 😃";

    for (std::size_t i = 0; i < text.size(); )
    {
        char32_t character = prototype.decode_utf8(text, i);

        if (character <= 0xFF)
            std::cout << "  Latin-1\n";
        else
            std::cout << "  Not Latin-1\n";
    }
    
    std::cin.get();

    return 0;
};