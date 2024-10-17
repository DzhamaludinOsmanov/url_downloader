#include <downloader.h>
#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <ctime>
#include <iomanip>

int main(int argc, char* argv[]) {
    // Проверка количества аргументов командной строки
    if (argc != 4){
        std::cerr << "Usage: " << argv[0] << "<urls_file> <output_dir> <max_conc_down>" << std::endl;
    }
    // Присвоение аргументов командной строки
    std::string urls_file = argv[1];
    std::string output_dir = argv[2];
    std::string max_conc_down = argv[3];

    std::vector <std::string> urls;
    std::vector <std::string> filenames;
    std::ifstream infile(urls_file);
    std::string url;

    while (std::getline(infile, url)) {
        if (!url.empty()) {
            urls.push_back(url);
        }
    }

    int max_concurrent_downloads = std::stoi(max_conc_down);
    // Создание объекта Downloader с указанием выходной директории и максимального количества одновременных загрузок
    Downloader downloader(output_dir, max_concurrent_downloads);

    // Логирование начала загрузки
    downloader.log_message("Starting downloads...");
    downloader.log_message("Parameters: " + std::string(argv[1]) + ", " + std::string(argv[2]) + ", " + std::string(argv[3]));

    // Запуск загрузки файло
    downloader.downloads_files(urls, filenames);

    // Логирование завершения загрузки
    downloader.log_message("Downloads finished.");

    return 0;
}