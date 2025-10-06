#ifndef PASSWORD_ANALYZER_H
#define PASSWORD_ANALYZER_H

#include <string>
#include <unordered_set>  // For dict caching

// Global cache (loaded in callers)
extern std::unordered_set<std::string> weakPasswords;

/**
 * Core functions: Reusable for CLI (console output) and GUI (QString output).
 */
std::string getMaskedInput();  // CLI-only; returns std::string
int analyzePassword(const std::string& password);  // Returns score
std::string generatePassword(int length = 16);     // Returns std::string (fix here!)

#endif // PASSWORD_ANALYZER_H