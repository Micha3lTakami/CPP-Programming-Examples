//-----------------------------------------------------------------------------
// Order.cpp
// Michael Takami mtakami PA7
// Top-level client, utilizing the Dictionary ADT, prints string representation
// of keys and values, etc.
//-----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include "Dictionary.h"

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr << "Order Usage: " << argv[0] << " input_file output_file\n";
        return 1;
    }


    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cerr << "Order Error: could not open input file " << argv[1] << "\n";
        return 1;
    }

    Dictionary dict;
    std::string line;
    int line_num = 1;
    while (std::getline(input_file, line)) {
        dict.setValue(std::move(line), line_num);
        line_num++;
    }

    std::ofstream output_file(argv[2]);
    if (!output_file) {
        std::cerr << "Order Error: could not open output file " << argv[2] << "\n";
        return 1;
    }

    output_file << dict.to_string() << "\n";
    output_file << dict.pre_string() << "\n";

    return 0;
}
   
