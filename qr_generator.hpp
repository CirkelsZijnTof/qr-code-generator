#ifndef QR_GENERATOR_HPP
#define QR_GENERATOR_HPP

#include <iostream>
#include <fstream>
#include <iomanip>
#include <array>
#include <functional>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <cmath>

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

    // clears terminal screen
    void clearTerminal() 
    {
        system("cls");
    };
    
public:

    raylibsUtils()
    {   
        
        clearTerminal();
        InitWindow(pixels_x, pixels_y, windowname);

        font = LoadFont(fontname);

        resourcesCheck();

    };

    raylibsUtils(int pixels_x, int pixels_y) 
    {

        clearTerminal();
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

class BitBuffer {
public:

    std::vector<uint8_t> bytes;
    int bit_length = 0;

    void appendBits(uint32_t value, int length)
    {
        if (length < 0 || length > 32)
            throw std::invalid_argument("Invalid bit length");

        if (length < 32 && (value >> length) != 0)
            throw std::invalid_argument("Value doesn't fit");

        for (int i = length - 1; i >= 0; --i)
        {
            // Create a new byte whenever needed.
            if (bit_length % 8 == 0)
                bytes.push_back(0);

            // Extract the next bit, from left to right.
            uint8_t bit = (value >> i) & 1;

            // Move that bit into its correct position.
            int position = 7 - (bit_length % 8);

            bytes.back() |= bit << position;

            ++bit_length;
        }
    }
};

class BitPlacement
{
private:

    //ledge
    //pawns
    //rocky
    //thumb

    //slant
    //price

    int v; // not smaller than 1, not bigger than 40

    int mask_pattern = 0;

    int size;
    int intervals;
    int distance;
    int step;
    int total_finders;

    // for big finders
    int big_bound = 8;
    bool big_finder_top_left[8][8] = {
        {1,1,1,1,1,1,1,0},
        {1,0,0,0,0,0,1,0},
        {1,0,1,1,1,0,1,0},
        {1,0,1,1,1,0,1,0},
        {1,0,1,1,1,0,1,0},
        {1,0,0,0,0,0,1,0},
        {1,1,1,1,1,1,1,0},
        {0,0,0,0,0,0,0,0}
    };

    bool big_finder_bot_left[8][8] = {
        {0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,0},
        {1,0,0,0,0,0,1,0},
        {1,0,1,1,1,0,1,0},
        {1,0,1,1,1,0,1,0},
        {1,0,1,1,1,0,1,0},
        {1,0,0,0,0,0,1,0},
        {1,1,1,1,1,1,1,0}
    };

    bool big_finder_top_right[8][8] = {
        {0,1,1,1,1,1,1,1},
        {0,1,0,0,0,0,0,1},
        {0,1,0,1,1,1,0,1},
        {0,1,0,1,1,1,0,1},
        {0,1,0,1,1,1,0,1},
        {0,1,0,0,0,0,0,1},
        {0,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0}
    };
    
    // for small finders
    int small_bound = 5;
    bool small_finder[5][5] = {
        {1,1,1,1,1},
        {1,0,0,0,1},
        {1,0,1,0,1},
        {1,0,0,0,1},
        {1,1,1,1,1}
    };
    int total_small_finders;

    std::vector<std::vector<bool>> qr_container;
    std::vector<std::vector<bool>> qr_reserved;

    uint8_t placer_pos_x = 0;
    uint8_t placer_pos_y = 0;

    int getDimensions() // in modules
    {
        return 17 + 4 * (v);
    };

    void reserveSquare(int start_pos_x, int start_pos_y, int bound)
    {

        for(int p_y = start_pos_y; p_y < bound + start_pos_y; p_y++) {
            for(int p_x = start_pos_x; p_x < bound + start_pos_x; p_x++) {
                qr_reserved[p_y][p_x] = true;
            }
        }
    }

    void setSmallFinderLocations(int N_modules_wide)
    {

        intervals = floor(v / 7) + 1;
        distance = 4 * v + 4;
        step = ceil(distance / intervals / 2) * 2;

        total_finders = intervals * intervals - 3;

        std::vector<int> tracks {6};

        // algorithm to set tracks
        for(int index = 0; index < intervals; index++)
        {

            tracks.push_back(distance + 6 - (intervals - 1 - index) * step);

        }

        // algorithm to set coordinates
        int array_size = tracks.size();
        std::vector<int> locs = {0,0};

        for(int index_y = 0; index_y < array_size; index_y++) 
        {
            for(int index_x = 0; index_x < array_size; index_x++) 
            {
                
                // skip if at corner
                if((index_x == 0 && index_y == 0)) {
                    continue;

                } else if((index_x == array_size-1 && index_y == 0) || (index_y == array_size-1 && index_x == 0)) {
                    continue;

                }

                // reserve the found location
                reserveSquare(tracks[index_x]-2, tracks[index_y]-2, small_bound);

            }

        }

    };

    template<typename T>
    void print1dVector(const std::vector<T>& printable)
    {

        for(int index = 0; index < printable.size(); index++)
        {
            std::cout << printable[index] << " ";
        }
        std::cout << "\n";

    };

    // general function. prints vectors for debugging purposes
    template<typename T>
    void print2dVector(const std::vector<std::vector<T>>& printable)
    {

        for(int y = 0; y < printable.size(); y++) {
            for(int x = 0; x < printable[y].size(); x++) {
                std::cout << printable[y][x] << " ";

            }
            std::cout << "\n"; 
        }

    };

public:

    void setReserved()
    {

        // coordinates for big finder patterns
        int top_left_x = 0;
        int top_left_y = 0;

        int bot_left_x = 0;
        int bot_left_y = size - big_bound;

        int top_right_x = size - big_bound;
        int top_right_y = 0;

        // set big finder patterns
        reserveSquare(top_left_x, top_left_y, big_bound);

        reserveSquare(bot_left_x, bot_left_y, big_bound);

        reserveSquare(top_right_x, top_right_y, big_bound);

        // set small finder patterns
        if(v > 1)
        {
            setSmallFinderLocations(getDimensions());

        };
        
        //reserveSquare();

        // debug
        print2dVector(qr_reserved);

    };

    // constructor
    BitPlacement(int version) 
    {

        v = version;
        size = getDimensions();

        qr_container.assign(size, std::vector<bool>(size, false));
        qr_reserved.assign(size, std::vector<bool>(size, false));

        setReserved();



    };

};

// user input, decoding and picture generating method
// TODO: add Kanji support
// TODO: (hopefully easy) save a qr code as png
class QRCode
{
private:

    //raylibsUtils Utils;
    BitBuffer Buffer;

    std::string folder = "qr_templates//";

    // navigate by index = version + EC level - 1
    std::string information_distribution = folder + "codewords.txt";
    std::vector<std::vector<std::string>> info_dist;

    std::string interleave_format = folder + "version_1.txt";
    std::vector<std::vector<std::string>> inter_format;

    // hard-coded qr-code stuff
    enum class error_corr  {L=0, Low=L, 
                            M=1, Medium=M,
                            Q=2, Quartile=Q, 
                            H=3, High=H};
                
    enum class qr_encoding {
        Numeric=1,
        Alphanumeric=2,
        Byte=4,
        Kanji=8}; // unused

    int data_bits[4][3] = {
        {10, 12, 14}, // numeric
        {9, 11, 13}, // alphanumeric
        {8, 16, 16}, // byte
        {8, 10, 12}, // kanji
    };

    uint8_t padding_0 = 236;
    uint8_t padding_1 = 17;

    // defines/handles data codewords
    qr_encoding mode;

    int N_data_bits;
    int bits_capacity;
    int ecc_capacity;

    error_corr ec_level = error_corr::M;

    uint8_t data_length;

    std::vector<uint8_t> data_codewords;

    // for drawing the qr-code
    int minimum_version = 1; // not smaller than 1, not bigger than 40
    int version = 1; // not smaller than 1, not bigger than 40

    int mask_pattern = 0;
    int module_width = 10; // sqrt(pixels) per module. QR-codes are painted in modules of pixels of a single color and size (square shape).

    void printRGBColored(std::array<int, 3> RGB, const char& text) 
    {
        std::cout << "\x1b[38;2;"
                << RGB[0] << ";"
                << RGB[1] << ";"
                << RGB[2] << "m"
                << text;

        std::cout << "\x1b[0m";
    };

    void printRGBColored(std::array<int, 3> RGB, const std::string& text) 
    {
        std::cout << "\x1b[38;2;"
                << RGB[0] << ";"
                << RGB[1] << ";"
                << RGB[2] << "m"
                << text;

        std::cout << "\x1b[0m";
    };

    char32_t decode_utf8(std::string_view text, std::size_t& i)
    {
        const auto c = static_cast<unsigned char>(text[i]);

        if (c <= 0x7F)
        {
            ++i;
            return c;
        }

        if ((c & 0xE0) == 0xC0)
        {
            char32_t result =
                (c & 0x1F) << 6 |
                (static_cast<unsigned char>(text[i + 1]) & 0x3F);

            i += 2;
            return result;
        }

        if ((c & 0xF0) == 0xE0)
        {
            char32_t result =
                (c & 0x0F) << 12 |
                (static_cast<unsigned char>(text[i + 1]) & 0x3F) << 6 |
                (static_cast<unsigned char>(text[i + 2]) & 0x3F);

            i += 3;
            return result;
        }

        char32_t result =
            (c & 0x07) << 18 |
            (static_cast<unsigned char>(text[i + 1]) & 0x3F) << 12 |
            (static_cast<unsigned char>(text[i + 2]) & 0x3F) << 6 |
            (static_cast<unsigned char>(text[i + 3]) & 0x3F);

        i += 4;
        return result;
    }

    std::vector<std::vector<std::string>> 
    readFile(const std::string& filename, char seperator = ',', char delimiter = '\n') {

        std::vector<std::vector<std::string>> data; // assume 2d array of strings to be stored in RAM

        std::ifstream fileStream;
        std::string line;

        fileStream.open(filename);

        if (!fileStream.is_open()) {

            std::cout << filename << "-> error: Could not open file.\n";

        } else { 
        
            // logic for reading from fileStream based on input array
            while (std::getline(fileStream, line)) {
                std::vector<std::string> row;
                std::string cell;
                std::istringstream lineStream(line);

                while (std::getline(lineStream, cell, seperator)) {
                    row.push_back(cell);
                }
                data.push_back(row);
            }

            // close the file stream
            fileStream.close();

        };
    
        return data;
    };

    bool getBit(char32_t input, int index)
    {
        return (input >> index) & 1;
    }

    bool isEven(uint32_t number)
    {
        return (number >> 0) & 0;
    }

    bool is_numeric(char32_t c)
    {
        return c >= U'0' && c <= U'9';
    }

    bool is_alphanumeric(char32_t c)
    {
        constexpr std::u32string_view characters =
            U"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

        return characters.find(c) != std::u32string_view::npos;
    }

    int alphanumericValue(char c)
    {
        const std::string charset =
            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

        auto pos = charset.find(c);

        if (pos == std::string::npos)
            throw std::invalid_argument("Invalid QR alphanumeric character");

        return static_cast<int>(pos);
    }

    // error correction
    uint8_t gfMultiply(uint8_t a, uint8_t b)
    {
        uint8_t result = 0;

        for (int i = 0; i < 8; ++i)
        {
            if (b & 1)
                result ^= a;

            const bool high_bit = (a & 0x80) != 0;

            a <<= 1;

            if (high_bit)
                a ^= 0x1D; // Reduction using primitive polynomial 0x11D

            b >>= 1;
        }

        return result;
    }

    uint8_t gfPower(uint8_t a, int exponent)
    {
        uint8_t result = 1;

        for (int i = 0; i < exponent; ++i)
            result = gfMultiply(result, a);

        return result;
    }

    std::vector<uint8_t> makeGenerator(int ecc_count)
    {
        if (ecc_count < 1 || ecc_count > 255)
            throw std::invalid_argument("Invalid ECC count");

        std::vector<uint8_t> generator = {1};

        for (int i = 0; i < ecc_count; ++i)
        {
            const uint8_t root = gfPower(2, i);

            std::vector<uint8_t> next(generator.size() + 1, 0);

            for (std::size_t j = 0; j < generator.size(); ++j)
            {
                next[j] ^= generator[j];
                next[j + 1] ^= gfMultiply(generator[j], root);
            }

            generator = std::move(next);
        }

        return generator;
    };

    std::vector<uint8_t> reedSolomonEncode(const std::vector<uint8_t>& data, int ecc_count) 
    {

        if (data.empty())
            throw std::invalid_argument("Data cannot be empty");

        if (ecc_count < 1 || ecc_count > 255 ||
            data.size() + static_cast<std::size_t>(ecc_count) > 255)
        {
            throw std::invalid_argument("Invalid block or ECC size");
        }

        const auto generator = makeGenerator(ecc_count);

        // Append zero bytes to make room for the remainder.
        std::vector<uint8_t> work = data;
        work.resize(data.size() + ecc_count, 0);

        // Polynomial long division over GF(256).
        for (std::size_t i = 0; i < data.size(); ++i)
        {
            const uint8_t factor = work[i];

            if (factor == 0)
                continue;

            for (std::size_t j = 0; j < generator.size(); ++j)
            {
                work[i + j] ^= gfMultiply(factor, generator[j]);
            }
        }

        // The final ecc_count bytes are the remainder.
        return std::vector<uint8_t>(
            work.begin() + data.size(),
            work.end()
        );
    };

    void reedSolomonEncode(int ecc_count) 
    {

        std::vector<uint8_t> data = Buffer.bytes;

        if (data.empty())
            throw std::invalid_argument("Data cannot be empty");

        if (ecc_count < 1 || ecc_count > 255 ||
            data.size() + static_cast<std::size_t>(ecc_count) > 255)
        {
            throw std::invalid_argument("Invalid block or ECC size");
        }

        const auto generator = makeGenerator(ecc_count);

        // Append zero bytes to make room for the remainder.
        std::vector<uint8_t> work = data;
        work.resize(data.size() + ecc_count, 0);

        // Polynomial long division over GF(256).
        for (std::size_t i = 0; i < data.size(); ++i)
        {
            const uint8_t factor = work[i];

            if (factor == 0)
                continue;

            for (std::size_t j = 0; j < generator.size(); ++j)
            {
                work[i + j] ^= gfMultiply(factor, generator[j]);
            }
        }

        // append result to given buffer
        std::vector<uint8_t> result = std::vector<uint8_t>(
            work.begin() + data.size(),
            work.end());

        for(uint8_t ec_byte : result)
        {
            Buffer.appendBits(ec_byte, 8);
        }

    };

    int versionSelector() {return 0;}; // TODO

    int getCapacityIndex()
    {
        // navigate by index = version + EC level - 1

        return version + int(ec_level) - 1;
    };

    uint8_t getLengthOfInput(std::string_view text)
    {
        return text.size();
    };

    void encodeNumeric(std::string_view text, BitBuffer& Buffer)
    {
        
        char group[4];
        int digit;

        std::size_t i = 0;

        // split numeric text into trios
        for(; i + 2 < data_length; i += 3) 
        {
            group[0] = text[i];
            group[1] = text[i + 1];
            group[2] = text[i + 2];
            group[3] = '\0';

            digit = atoi(group);

            Buffer.appendBits(digit, 10);
        }

        if(i < data_length)
        {
            int remainder = data_length - i;

            if(remainder == 1) {
                digit = text[i] - '0';

                Buffer.appendBits(digit, 4);

            } else {
                digit = (text[i] - '0') * 10 + (text[i + 1] - '0');

                Buffer.appendBits(digit, 7);

            }
        }

    };

    void encodeAlphanumeric(std::string_view text, BitBuffer& Buffer)
    {
        std::size_t i = 0;

        for(; i + 1 < data_length; i += 2)
        {
            int a = alphanumericValue(text[i]);
            int b = alphanumericValue(text[i + 1]);

            Buffer.appendBits(45 * a + b, 11);
        }

        if(i < data_length)
        {
            Buffer.appendBits(text[i], 6);
        }

    };

    void encodeByte(std::string_view text, BitBuffer& Buffer) 
    {
        std::size_t i = 0;

        for(; i < data_length; i++) 
        {   
            uint8_t c = static_cast<uint8_t>(static_cast<unsigned char>(text[i]));
            Buffer.appendBits(c, 8);
        }

    };

    void encodeInput(std::string_view text)
    {

        N_data_bits = getDataBits(mode);

        Buffer.appendBits(uint8_t(mode), 4);
        Buffer.appendBits(data_length, N_data_bits);

        if(mode == qr_encoding::Numeric) {
            encodeNumeric(text, Buffer);

        } else if(mode == qr_encoding::Alphanumeric) {
            encodeAlphanumeric(text, Buffer);

        } else {
            encodeByte(text, Buffer);

        }

    };

    void addTerminator()
    {

        int remaining = bits_capacity - Buffer.bit_length;
        int terminator = std::min(4, remaining);

        if(terminator > 0)
            Buffer.appendBits(0, terminator);

        int alignment = (8 - (Buffer.bit_length % 8)) % 8;
        alignment = std::min(alignment, bits_capacity - Buffer.bit_length);

        if(alignment > 0)
            Buffer.appendBits(0, alignment);

    };

    void addPadding() 
    {

        bool current_padding_type = 0;

        while(Buffer.bit_length < bits_capacity)
        {
            if(current_padding_type)
            {
                Buffer.appendBits(padding_0, 8);
            } else {
                Buffer.appendBits(padding_1, 8);
            }
            current_padding_type = !current_padding_type;
        }
        
    };

public:

    void parseInput(std::string_view text)
    {
        
        data_length = getLengthOfInput(text);
        bool numeric = true;
        bool alphanumeric = true;

        for (std::size_t i = 0; i < data_length; )
        {
            const char32_t c = decode_utf8(text, i);

            if (!is_numeric(c))
                //std::cout << int(c) << ": not numeric\n";
                numeric = false;

            if (!is_alphanumeric(c))
                //std::cout << int(c) << ": not alphanumeric\n";
                alphanumeric = false;

        }

        if (numeric) {
            mode = qr_encoding::Numeric;

        } else if(alphanumeric) {
            mode = qr_encoding::Alphanumeric;

        } else {
            mode = qr_encoding::Byte;
        }

    };

    int getDataBits(qr_encoding mode)
    {
        int mode_index = 0;
        int bits_index = 0;

        if(version >= 10 && version <= 26) {
            bits_index = 1;

        } else if(version >= 27) {
            bits_index = 2;

        }

        if(mode == qr_encoding::Alphanumeric) {
            mode_index = 1;

        } else if(mode == qr_encoding::Byte) {
            mode_index = 2;

        }

        return data_bits[mode_index][bits_index];
    };

    void printEncodedBytes()
    {
        
        for(uint8_t byte : Buffer.bytes)
        {
            std::cout << std::hex
                << std::uppercase
                << std::setw(2)
                << std::setfill('0')
                << static_cast<int>(byte)
                << ' ';  
        }

        std::cout << "\n";

    };

    void generate(std::string text)
    {

        parseInput(text); // sets mode

        encodeInput(text);

        addTerminator();

        addPadding();

        reedSolomonEncode(ecc_capacity);

        // TODO: add big/small identifier patterns

        // TODO: add mask support

        // TODO: add walk and encoded bit placement support
    
    };
    
    QRCode()
    {
        info_dist = readFile(information_distribution);
        // inter_format = readFile(interleave_format);

        // TODO: parse string here
        
        // TODO: set version

        bits_capacity = std::stoi(info_dist[getCapacityIndex()][0]) * 8; // bits, requires version and ec level to be known
        ecc_capacity = std::stoi(info_dist[getCapacityIndex()][1]); // bytes
    };

};


#endif