#define CURL_STATICLIB
#include <downloader.h>
#include <curl/curl.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <cctype>
#include <string>
#include <ctime>
#include <iomanip>

std::mutex queue_mutex; // Мьютекс для защиты очереди
std::queue<std::string> download_queue; // Очередь для хранения URL-адресов

// Функция для логирования с текущим временем
void Downloader::log_message(const std::string& message) {
    time_t timer;
    char buffer[26];
    std::tm* tm_info;

    time(&timer);
    tm_info = std::localtime(&timer);

    std::strftime(buffer, 26, "%H:%M:%S", tm_info);
    std::cout << "[" << buffer << "] " << message << std::endl; 
}

// Функция обратного вызова для записи данных, полученных с помощью libcurl
size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    return std::fwrite(ptr, size, nmemb, stream);
}

// Функция для генерации уникального имени файла в случае, если файл с таким именем уже существует
std::string numerator(const std::string& filename, std::vector <std::string>& filenames){
        int count = 0;

    if (filenames.empty()) {
        filenames.push_back(filename);
    }

    for(auto& fn : filenames){
        if (fn == filename){
            count++;
        }
    }

    if (count > 0) {
        return filename + "_" + std::to_string(count) + ".png";
    }
    return filename + ".png";
}

// Функция для извлечения имени файла из URL
std::string Filename(const std::string& url, std::vector <std::string>& filenames) {
    std::string filename;
    size_t last_slash = url.find_last_of('/');

    filename = url.substr(last_slash + 1);
    if(filename.length() > 100){
        filename = "Image";
    }
    else {
        for (size_t i = 0; i < filename.length(); i++) {
            if (!std::isalnum(filename[i]) && filename[i] != '_') {
                filename.erase(i, 1);
                filename.insert(i, 1, '_'); 
            }
        }
    }

    return numerator(filename, filenames);
} 

void Downloader::download_file(const std::string& url, std::vector <std::string>& filenames) {
    CURL* curl;
    FILE* fp;
    CURLcode res;

    std::string filename = Filename(url, filenames);
    filename = output_dir + "/" + filename;

    Downloader::log_message("Starting download: " + url);

    // Инициализация libcurl
    curl = curl_easy_init();
    if (!curl) {
        std::cout << "Error" << std::endl;
        Downloader::log_message("Curl initialization error: " + std::string(curl_easy_strerror(CURLE_FAILED_INIT)));
        return; 
    }
    else {
        // Открытие файла для записи
        fp = fopen(filename.c_str(), "wb");
        if (!fp){
            Downloader::log_message("File opening error: " + filename);
            curl_easy_cleanup(curl);
            return;
        }
        

        // Установка параметров libcurl
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

        // Выполнение и логирование успешной загрузки файла
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            Downloader::log_message("Error downloading " + url + ": " + curl_easy_strerror(res));
        } 
        else {
            Downloader::log_message("Finished downloading: " + url);
        }

        // Закрытие файла и очистка ресурсов libcurl
        fclose(fp);
        curl_easy_cleanup(curl);
    }
}

// Конструктор класса Downloader
Downloader::Downloader(const std::string& output_dir, int max_conc_down) : output_dir(output_dir), max_conc_down(max_conc_down) {}

// Функция для запуска загрузки файлов
void Downloader::downloads_files(const std::vector<std::string>& urls, std::vector <std::string>& filenames){
    std::vector<std::thread> threads;

    // Добавление URL-адресов в очередь
    for (const auto& url : urls){
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            download_queue.push(url);
        }

        // Запуск нового потока, если количество активных потоков меньше максимального
        if (threads.size() < max_conc_down) {
            threads.emplace_back([this, &filenames]() {
                // Цикл загрузки файлов из очереди
                while (true) {
                    std::string url;
                    {
                        std::lock_guard<std::mutex> lock(queue_mutex);
                        // Если очередь пуста, то завершаем работу потока
                        if (download_queue.empty()) break;
                        url = download_queue.front();
                        download_queue.pop();
                    }
                    // Загрузка файла
                    download_file(url, filenames);
                }
            });
        }
    }

    // Ожидание завершения всех потоков
    for (auto& thread : threads){
        thread.join();
    }
}