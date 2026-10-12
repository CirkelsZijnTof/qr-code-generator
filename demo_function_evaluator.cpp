#include "function_evaluator.hpp"

// D:\msys\ucrt64\bin\g++.exe demo_function_evaluator.cpp icons\app.res -o executables\demo_function_evaluator.exe

// functions for demo purposes
// normal
int test_function_1(int a, int b) 
{

    int result = a * b;

    std::cout << "normal:" << a << "," << b << "->" << result << "\n";

    return result;
};

// lambda
auto test_function_2 = [](int a, int b) 
{

    int result = a * b;

    std::cout << "lambda:" << a << "," << b << "->" << result << "\n";

    return result;
};

// normal: division
double test_function_3(double a, double b)
{
    
    if (b == 0.0) {
        throw std::runtime_error("Division by zero");
    }

    double result = a / b;

    std::cout << "normal:" << a << "," << b << "->" << result << "\n";

    return result;  
};

// lambda: division
auto test_function_4 = [](double a, double b) 
{

    if (b == 0.0) {
        throw std::runtime_error("Division by zero");
    }

    double result = a / b;

    std::cout << "lambda:" << a << "," << b << "->" << result << "\n";

    return result;
};

int main() 
{

    Probe PROBE;

    // normal function with arguments in time_complexity function call
    std::cout << PROBE.time_complexity(5, test_function_1, 2, 3) << "s\n";

    // use of lambda requires [&](){YOUR_FUNCTION(args)}; wrapper.
    std::cout << PROBE.time_complexity(5, [&](){test_function_2(2, 3);}) << "s\n";

    PROBE.evaluate_output(test_function_1(2, 3) == 6, "2 * 3 should equal 6");

    PROBE.evaluate_exception("test_function_4(0, 0)", [&](){test_function_4(0, 0);});

    std::cin.get();

};