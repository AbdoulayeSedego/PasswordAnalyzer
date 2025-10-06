#include <iostream>
#include <fstream>   // For dict loading
#include "../common/PasswordAnalyzer.h"  // All declarations (getMaskedInput, analyzePassword, generatePassword)


int main() {
    // Load dict once for efficiency (O(1) lookups)
    std::ifstream dict("../common_passwords.txt");
    if (dict.is_open()) {
        std::string badPass;
        while (std::getline(dict, badPass)) {
            weakPasswords.insert(badPass);
        }
        dict.close();
        std::cout << "Dict loaded: " << weakPasswords.size() << " entries." << std::endl;
    } else {
        std::cout << "Warning: Dict file missing—no weak word check." << std::endl;
    }

    while (true) {
        // Menu display: Simple text-based UI; in full apps, could use libraries like argparse for CLI polish.
        std::cout << "\n=== Password Strength Analyzer CLI ===" << std::endl;
        std::cout << "1. Analyze password" << std::endl;
        std::cout << "2. Generate strong password" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choose: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear newline for subsequent input ops
        if (choice == 3) {
            break;
        }
        else if (choice == 2) {
            std::cout << "Generated password: " << generatePassword() << std::endl;
            std::cout << "(Tip: Copy and analyze it with option 1.)" << std::endl;
        }
        else if (choice == 1) {
            std::string password = getMaskedInput();
            int score = analyzePassword(password);
            std::cout << "Password strength: " << score << "%" << std::endl;

            if (score < 50) {
                std::cout << "Overall: Weak Password — vulnerable to dictionary/brute-force attacks." << std::endl;
            } else if (score < 80) {
                std::cout << "Overall: Fair Password — add more complexity for better resistance to dictionary/brute-force attacks." << std::endl;
            } else {
                std::cout << "Overall: Strong Password — resists common cracking methods." << std::endl;
            }
        } else {
            std::cout << "Invalid choice—try again." << std::endl;
        }
    }

    //END Of Main
    return 0;
}