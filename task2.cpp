#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

// Simple XOR Encryption/Decryption Function
std::string xorEncryptDecrypt(const std::string &text, char key = 'K') {
    std::string output = text;
    for (char &c : output) {
        c ^= key;
    }
    return output;
}

// Function to store encrypted file
void storeFile() {
    std::string country, state, district, description;
    std::cin.ignore();  // flush newline

    std::cout << "Enter Country: ";
    std::getline(std::cin, country);
    std::cout << "Enter State: ";
    std::getline(std::cin, state);
    std::cout << "Enter District: ";
    std::getline(std::cin, district);
    std::cout << "Enter Description: ";
    std::getline(std::cin, description);

    // Create directory
    fs::path dirPath = "./data/" + country + "/" + state + "/" + district;
    try {
        fs::create_directories(dirPath);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error creating directories: " << e.what() << '\n';
        return;
    }

    // Prepare file content
    std::string content = "Country: " + country + "\nState: " + state + "\nDistrict: " + district + "\nDescription: " + description;
    std::string encrypted = xorEncryptDecrypt(content);

    // Write to file
    std::ofstream outFile(dirPath / "info.dat", std::ios::binary);
    if (!outFile) {
        std::cerr << "Error opening file for writing.\n";
        return;
    }

    outFile << encrypted;
    outFile.close();
    std::cout << "File saved successfully.\n";
}

// Function to retrieve and decrypt file
void retrieveFile() {
    std::string country, state, district;
    std::cin.ignore();  // flush newline

    std::cout << "Enter Country: ";
    std::getline(std::cin, country);
    std::cout << "Enter State: ";
    std::getline(std::cin, state);
    std::cout << "Enter District: ";
    std::getline(std::cin, district);

    fs::path filePath = "./data/" + country + "/" + state + "/" + district + "/info.dat";

    if (!fs::exists(filePath)) {
        std::cerr << "File not found.\n";
        return;
    }

    // Read encrypted content
    std::ifstream inFile(filePath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening file for reading.\n";
        return;
    }

    std::string encryptedContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    std::string decrypted = xorEncryptDecrypt(encryptedContent);

    std::cout << "\n--- File Content ---\n" << decrypted << "\n";
}

// Main menu
int main() {
    while (true) {
        std::cout << "\nChoose an option:\n1. Store a new file\n2. Retrieve and view a file\n3. Exit\nEnter choice: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                storeFile();
                break;
            case 2:
                retrieveFile();
                break;
            case 3:
                std::cout << "Exiting...\n";
                return 0;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    }
}
