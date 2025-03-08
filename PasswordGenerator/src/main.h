#pragma once

std::string generatePassword(bool includeLowercase, bool includeUppercase, bool includeNumbers, bool includeSymbols,const char* lowercaseLetters, const char* uppercaseLetters, const char* numbers, const char* symbols, int totalLength);

