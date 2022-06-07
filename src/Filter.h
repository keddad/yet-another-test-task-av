#include "utility"
#include "string"
#include "vector"
#include "filesystem"
#include "algorithm"
#include "fstream"
#include "map"
#include "optional"

#ifndef YET_ANOTHER_TEST_TASK_AV_FILTER_H
#define YET_ANOTHER_TEST_TASK_AV_FILTER_H


class Filter {
public:
    Filter(std::vector<std::string> queries, std::vector<std::string> filenames, std::string name) : queries(
            std::move(queries)), filenames(std::move(filenames)), name(std::move(name)) {};

    std::optional<bool> Match(const std::filesystem::directory_entry &candidate) const {
        if (std::find(filenames.begin(), filenames.end(), candidate.path().extension().string()) == filenames.end()) {
            return false;
        }

        std::ifstream file;
        std::string line;
        file.open(candidate.path());

        if (file.is_open()) {
            while (!file.eof()) {
                std::getline(file, line);

                for (auto const &query: queries) {
                    if (line.find(query) !=
                        std::string::npos) { // В задании не указанно, триггериться ли на вхождение всех строк, или хватит одной - сделал так
                        return true;
                    }
                }
            }
        } else {
            return std::nullopt;
        }

        return false;
    };

    const std::string name;
private:
    const std::vector<std::string> queries;
    const std::vector<std::string> filenames;
};


#endif //YET_ANOTHER_TEST_TASK_AV_FILTER_H
