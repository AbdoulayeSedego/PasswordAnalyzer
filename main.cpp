#include <iostream>
#include <string>
#include <cctype>    // For isupper (character classification functions)
#include <fstream>   // For dict (file input streams to load common passwords)
#include <random>    // For generator (secure random number generation)
#include <algorithm> // For shuffle (std::shuffle to randomize password chars)
#include <cmath>     // For entropy calc (std::log2 for approximate entropy)

std::string getMaskedInput(); // Securely reads masked password from a user
int analyzePassword(const std::string& password); // Computes strength score 0-100
std::string generatePassword(int length = 16); // Generates random strong password

int main() {
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
std::string getMaskedInput() {
    std::string password;
    std::cout << "Enter password: (hidden for security)";
    char ch;
    password.clear(); // Sanitize start—no leftovers from prev inputs
    while ( (ch = std::cin.get()) != '\n') {  // Loop until Enter a key
        if (ch != '\r') {
            password += ch; // Build string char-by-char
            std::cout << '*';  // Mask for security (shoulder-surfing defense)
        }
    }
    std::cout << std::endl;
    return password;
}

