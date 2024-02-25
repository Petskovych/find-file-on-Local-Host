#include <iostream>
#include "file_search_lib.h"

 
int main() {
    std::string filename;
    std::cout << "Enter the filename to search for: ";
    std::cin >> filename;

    // Оновлений шлях до каталогу користувачів
    std::string start_path = "C:\\Users";

    start_search(filename.c_str(), start_path.c_str());

    return 0;
}

