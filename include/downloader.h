#ifndef DOWNLOADER_H
#define DOWNLPADER_H

#include <string>
#include <vector>

class Downloader {
public:
    Downloader(const std::string& output_dir, int max_conc_down);
    void downloads_files(const std::vector<std::string>& urls, std::vector <std::string>& filenames);
    void log_message(const std::string& message); 
private:
    void download_file(const std::string& url, std::vector <std::string>& filenames);
    std::string output_dir;
    int max_conc_down;
};

#endif //DOWNLOAD_H 
