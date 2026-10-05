#ifndef QR_GENERATOR_HPP
#define QR_GENERATOR_HPP

#include <iostream>
#include <array>
#include <functional>

#include "raylib.h"

// raylibs utils
class raylibsUtils
{
private:

    const char* error_msg = "Error";

    int pixels_x = 100;
    int pixels_y = 100;

    Font font;

    // helpful for formatting/design
    const char* fontname = "fonts//cour.ttf";
    const char* windowname = "QR-generator";

    const std::array<int, 3> RGB_green = {0, 180, 0};
    const std::array<int, 3> RGB_red = {180, 0, 0};
    const std::array<int, 3> RGB_orange = {220, 180, 0};

    const unsigned char alpha = 255;
    const Color white = Color{245, 245, 245, alpha};
    const Color gray = Color{100, 100, 100, alpha};
    const Color black = Color{0, 0, 0, alpha};

    const Color red = Color{180, 0, 0, alpha};
    const Color green = Color{0, 180, 0, alpha};
    const Color blue = Color{0, 0, 180, alpha};

    const Color color_background = white;

    void
    printRGBColored(std::array<int, 3> RGB, const char& text) 
    {
        std::cout << "\x1b[38;2;"
                << RGB[0] << ";"
                << RGB[1] << ";"
                << RGB[2] << "m"
                << text;

        std::cout << "\x1b[0m";
    };

    void
    printRGBColored(std::array<int, 3> RGB, const std::string& text) 
    {
        std::cout << "\x1b[38;2;"
                << RGB[0] << ";"
                << RGB[1] << ";"
                << RGB[2] << "m"
                << text;

        std::cout << "\x1b[0m";
    };

    void resourcesCheck()
    {
        if (font.texture.id == 0)
        {
            printRGBColored(RGB_red, "[ERROR] missing font");
        }
    };
    
public:

    raylibsUtils()
    {   
        
        InitWindow(pixels_x, pixels_y, windowname);

        font = LoadFont(fontname);

        resourcesCheck();

    };

    raylibsUtils(int pixels_x, int pixels_y) 
    {

        InitWindow(pixels_x, pixels_y, windowname);

        font = LoadFont(fontname);

        resourcesCheck();

    };

    void putPixel(std::array<float, 2> location, Color color) 
    {
        DrawPixel(location[0], location[1], color);
    };

    void putSquare(std::array<float, 2> location, int side_length, Color color) 
    {
        DrawRectangle(location[0], location[1], side_length, side_length, color);
    };

    void putRectangle(std::array<float, 2> location, int width, int height, Color color)
    {
        DrawRectangle(location[0], location[1], width, height, color);
    };

    void putText(std::array<float, 2> location, std::string text, Color color, int size=30, int spacing=2)
    {
        DrawTextEx(font, text.c_str(), {location[0], location[1]}, size, spacing, color);
    };

    void putError() 
    {
        putText({0, 0}, error_msg, red);
    };

    void drawScreen()
    {
        BeginDrawing();
        ClearBackground(color_background);


        EndDrawing();
    };

    // probably not going to use this
    template <typename Func, typename... Args>
    void drawScreen(Func&& func, Args&&... args)
    {

        BeginDrawing();

        ClearBackground(color_background);

        std::invoke(
            std::forward<Func>(func),
            std::forward<Args>(args)...
        );

        EndDrawing();

    };

};

// picture generating method
class QRCode
{
private:

    raylibsUtils Utils;


    

public:

    int module_width = 10; // sqrt(pixels) per module. QR-codes are painted in modules of pixels of a single color and size (square shape).
    enum class error_corr  {L=0, Low=L, 
                            M=1, Medium=M,
                            Q=2, Quartile=Q, 
                            H=3, High=H};

    char32_t decode_utf8(const std::string& text, std::size_t& i, unsigned char err_c = 0x5F)
    {
        unsigned char c = static_cast<unsigned char>(text[i]);

        // 1-byte character: 0xxxxxxx
        if (c <= 0xAF)
        {
            ++i;
            return c;
        } else {

            return err_c;
        }

    };
    

};

// math/decoding method

#endif