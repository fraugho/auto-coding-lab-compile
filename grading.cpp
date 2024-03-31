#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

void rename_files();
void compile_files(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    rename_files();
    compile_files(argc, argv);
    return 0;
}

void rename_files() {
    std::regex not_number_regex("[^0-9]+");
    
    // Use current_path() to get the directory the program is run from
    fs::path current_directory = fs::current_path();
    
    for (const auto& entry : fs::recursive_directory_iterator(current_directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".cpp") {
            // Extract the filename without extension
            std::string filename = entry.path().stem().string();
            
            // Remove all non-numeric characters from the filename
            std::string new_filename = std::regex_replace(filename, not_number_regex, "");
            
            // Construct the new path with the updated filename and original extension
            fs::path new_path = entry.path().parent_path() / (new_filename + entry.path().extension().string());
            
            // Rename the file to its new path
            fs::rename(entry.path(), new_path);
        }
    }
}

void compile_files(int argc, char *argv[]){

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <number of files to compile>\n";
        return;
    }

    int x = std::stoi(argv[1]);
    std::string command;

    for(int i = 0; i < x; ++i) {
        // Append the compile command for each file
        command += "g++ " + std::to_string(i + 1) + ".cpp -o " + std::to_string(i + 1);

        // Use ';' to separate commands so that all commands are attempted
        if (i != x - 1) {
            command += " ; ";
        }
    }

    // Print the full command to be executed, for debugging purposes
    std::cout << "Executing command: " << command << std::endl;

    // Execute the command
    int status = system(command.c_str());

    // Since 'system' only returns the status of the last command executed,
    // the status code might not reflect individual compilation failures.
    if (status == -1) {
        std::cerr << "Failed to execute command" << std::endl;
    } else {
        // If needed, further analysis of the output can be added here
        std::cout << "Command executed. Status code: " << status << std::endl;
    }
}
