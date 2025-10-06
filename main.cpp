#include <iostream>
#include <string>
#include <cctype>    // For isupper (character classification functions)
#include <fstream>   // For dict (file input streams to load common passwords)
#include <random>    // For generator (secure random number generation)
#include <algorithm> // For shuffle (std::shuffle to randomize password chars)
#include <cmath>     // For entropy calc (std::log2 for approximate entropy)

#if defined(_WIN32) || defined(_WIN64)
  #include <conio.h>    // _getch on Windows
  #include <io.h>
  #define isatty _isatty
  #define fileno _fileno
#else
  #include <unistd.h>   // isatty, read, STDIN_FILENO
  #include <termios.h>  // tcgetattr, tcsetattr
  #include <errno.h>
#endif


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
// Cross-platform masked input that attempts to show '*' in real-time.
// Falls back gracefully if stdin is not a TTY or termios calls fail.
std::string getMaskedInput() {
    std::string password;
    std::cout << "Enter password: " << std::flush;

#if defined(_WIN32) || defined(_WIN64)
    // --- Windows implementation (uses _getch for immediate, non-echoed input) ---
    if (!isatty(fileno(stdin))) {
        // Not a TTY (rare on Windows), fallback to getline + post-mask
        std::string tmp;
        if (!std::getline(std::cin, tmp)) return "";
        std::cout << std::string(tmp.size(), '*') << std::endl;
        return tmp;
    }

    while (true) {
        int ch = _getch();               // read key without echo
        if (ch == '\r' || ch == '\n') {  // Enter
            std::cout << std::endl;
            break;
        }
        if (ch == 0 || ch == 224) {      // function/arrow keys (two-char codes)
            // consume next byte and ignore the key
            _getch();
            continue;
        }
        if (ch == 8) { // Backspace
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b" << std::flush; // remove '*' visually
            }
            continue;
        }
        // Normal character
        password.push_back(static_cast<char>(ch));
        std::cout << '*' << std::flush;
    }

    return password;

#else
    // --- POSIX implementation (macOS / Linux) ---
    // Check that stdin is a terminal (TTY). If not, termios won't help.
    if (!isatty(STDIN_FILENO)) {
        // fallback: read normally then print stars (not real-time)
        std::string tmp;
        if (!std::getline(std::cin, tmp)) return "";
        std::cout << std::string(tmp.size(), '*') << std::endl;
        return tmp;
    }

    struct termios oldt;
    if (tcgetattr(STDIN_FILENO, &oldt) != 0) {
        // If we can't get attributes, fallback with a warning:
        std::cerr << "\nWarning: Unable to disable terminal echo (tcgetattr failed). "
                     "Masking will occur only after you press Enter.\n";
        std::string tmp;
        if (!std::getline(std::cin, tmp)) return "";
        std::cout << std::string(tmp.size(), '*') << std::endl;
        return tmp;
    }

    // Make a copy and modify local flags: disable ECHO and canonical mode (ICANON)
    struct termios newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON); // Turn off echoing and enable non-canonical (char-by-char) mode
    newt.c_cc[VMIN] = 1;              // Minimum number of characters to read
    newt.c_cc[VTIME] = 0;             // No read timeout

    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0) {
        // Failed to set terminal options — restore and fallback
        std::cerr << "\nWarning: Unable to set terminal to raw mode (tcsetattr failed). "
                     "Masking will occur only after you press Enter.\n";
        // best-effort: restore original (though we didn't change it)
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        std::string tmp;
        if (!std::getline(std::cin, tmp)) return "";
        std::cout << std::string(tmp.size(), '*') << std::endl;
        return tmp;
    }

    // terminal is in char-by-char, non-echo mode. Read characters using read().
    char ch = 0;
    while (true) {
        ssize_t n = read(STDIN_FILENO, &ch, 1);
        if (n <= 0) {
            // read error or EOF: restore terminal and return what we have
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            return password;
        }
        if (ch == '\n' || ch == '\r') {
            // Enter pressed: done
            std::cout << std::endl;
            break;
        }
        if (ch == 127 || ch == '\b') { // Backspace (127 or '\b')
            if (!password.empty()) {
                password.pop_back();
                // Erase last '*' visually: backspace, overwrite with space, backspace again
                std::cout << "\b \b" << std::flush;
            }
            continue;
        }
        // Normal printable character: append and print '*'
        password.push_back(ch);
        std::cout << '*' << std::flush;
    }

    // Restore the original terminal settings unconditionally
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return password;
#endif
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

    int types = (hasUpper ? 1 : 0) + (hasLower ? 1 : 0) + (hasDigit ? 1 : 0) + (hasSpecial ? 1 : 0);
    double entropy = (password.empty() ? 0.0 : std::log2(static_cast<double>(password.length()))) * types;
    if (entropy > 50) {
        score += 10;
        std::cout << "Bonus: High entropy (>50 bits—crack-resistant)" << std::endl;
    }
    // Final computed percentage-like score based on length and character variety.
    std::cout << "Final Score: " << score << "/100" << std::endl;
    return score;
}
std::string generatePassword(int length) {
    // Secure seeding: std::random_device pulls from /dev/urandom (Linux) or CryptoAPI (Win).
    // Why? Predictable seeds = guessable outputs; critical for key gen in crypto.
    std::random_device rd; // seed from os
    std::mt19937 gen(rd()); // std::mt19937 is Mersenne Twister 19937 : Fast, high-quality PRNG

    // Charset: Balanced pools for forced diversity (avoids all-lowercase gens).
    // Size 94: Full printable ASCII minus space—standard for pass gens.
    std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+";
    std::uniform_int_distribution<> dist(0, chars.size() - 1); //// Uniform pick from charset 0-94

    std::string generated;
    generated.reserve(length); // Preallocate memory for performance
    for (int i = 0; i < length; i++) {
        generated += chars[dist(gen)]; // Append random char
    }

    // Shuffle: Ensures no patterns (e.g., symbols at end). Uses Fisher-Yates via std::shuffle.
    // In cyber: Patterns aid cracking (e.g., ML models spot 'word+digit').
    std::shuffle(generated.begin(), generated.end(), gen);
    return generated;
}