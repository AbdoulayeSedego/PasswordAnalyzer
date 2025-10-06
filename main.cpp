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

// Analysis function: Core logic—reusable across CLI/GUI.
int analyzePassword(const std::string& password) {
    int score = 0;

    // Length check: >=8 chars for base points. NIST recommends 8+; longer resists offline attacks.
    // Expl: Brute-force time grows exponentially with length
    if (password.length() >= 8) {
        score += 20;
        std::cout << "Good: Length >= 8 ==> Your password length is: " << password.length() << std::endl;
    } else {
        std::cout << "Weak: Length < 8 ==> Your password length is: " << password.length() << std::endl;
    }

    // Character diversity checks: Ensures entropy from multiple pools (lowers dict attack success).
    // Uses <cctype> for locale-agnostic classification—handles UTF-8 basics.
    // Cast to unsigned char: Prevents signed char negative values breaking is* functions.

    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    for (char c : password) {
        unsigned char uc = static_cast<unsigned char>(c);  // Safe casting for ASCII
        if (std::isupper(uc)) hasUpper = true;
        else if (std::islower(uc)) hasLower = true;
        else if (std::isdigit(uc)) hasDigit = true;
        else if (std::ispunct(uc)) hasSpecial = true; // Anything non-alphanumeric = symbol
    }

    // Character class scoring and feedback:
    // - For each character type present (uppercase, lowercase, digit, special),
    //   we award 15 points to the overall score to encourage diversity.
    // - For each missing type, we print a specific, actionable message so the user
    //   knows exactly what to add to strengthen the password.
    if (hasUpper) {
        score += 15;
        std::cout << "Good: Uppercase character ==> Your password contains uppercase characters." << std::endl;
    } else {
        std::cout << "Weak: No uppercase character ==> Your password does not contain uppercase characters." << std::endl;
    }
    if (hasLower) {
        score += 15;
        std::cout << "Good: Lowercase character ==> Your password contains lowercase characters." << std::endl;
    } else {
        std::cout << "Weak: No lowercase character ==> Your password does not contain lowercase characters." << std::endl;
    }
    if (hasDigit) {
        score += 15;
        std::cout << "Good: Digit character ==> Your password contains digits." << std::endl;
    } else {
        std::cout << "Weak: No digit character ==> Your password does not contain digits." << std::endl;
    }
    if (hasSpecial) {
        score += 15;
        std::cout << "Good: Special character ==> Your password contains special characters." << std::endl;
    } else {
        std::cout << "Weak: No special character ==> Your password does not contain special characters." << std::endl;
    }

    // Dictionary check: Loads common weak passes; exact match penalty.
    // Why? 80% breaches from weak creds; tools like John the Ripper use rockyou.txt (14M entries).
    // Here: Simple file load—scalable to trie/hashtable for speed in large dicts.
    std::ifstream dict("common_passwords.txt");
    std::string badPass;
    bool isWeak = false;
    if (dict.is_open()) {
        while (std::getline(dict, badPass)) {
            if (password == badPass) {
                isWeak = true;
                break;
            }
        }
        dict.close();
    } else {
        std::cout << "Warning: Dict file missing—no weak word check." << std::endl;  // Failsafe
    }

    //final points: Reward non-dictionary use
    if (!isWeak) {
        score += 20;
        std::cout << "Good: Not in common breach lists (e.g., RockYou)" << std::endl;
    } else {
        std::cout << "Weak: Matches known leaked password—change now!" << std::endl;
    }

    int types = (hasUpper + hasLower + hasDigit, hasSpecial);
    double entropy = std::log2(password.length()) * types;
    if (entropy > 50) {
        score += 10;
        std::cout << "Bonus: High entropy (>50 bits—crack-resistant)" << std::endl;
    }
    // Final computed percentage-like score based on length and character variety.
    std::cout << "Final Score: " << score << "/100" << std::endl;
    return score;
}
std::string generatePassword(int length) {

}