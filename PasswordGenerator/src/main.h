#pragma once

std::string generatePassword(const int numLowercaseLetters, const int numUppercaseLetters, const int numNumbers, const int numSymbols,
    const char* lowercaseLetters, const char* uppercaseLetters, const char* numbers, const char* symbols);

void savePassword(std::string currentPassword);