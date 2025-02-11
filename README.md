# Vaultify - Password Generator

Vaultify is a password generator tool that allows users to customize the length and character types of the generated password. The application uses a simple graphical user interface (GUI) built with **ImGui** and **DirectX 11** for rendering.

## Features
- Customizable password length
- Select how many lowercase, uppercase, numeric, and special characters to include
- Shuffle the generated password for randomness
- Built using **ImGui**, **DirectX 11**, and **C++**

## Requirements
- Windows OS
- Visual Studio (or any compatible C++ compiler)
- DirectX 11 SDK
- ImGui Library

## Setup and Usage
### 1. Setting Up ImGui and DirectX 11
Ensure that the necessary ImGui files and DirectX 11 SDK are included in your project directory. Your project should link against the ImGui and DirectX libraries for rendering the GUI and managing device context.

### 2. Code Overview

Below are important snippets of the code that demonstrate how the password generator works:

#### Main Password Generation Logic
The `generatePassword` function creates the password by selecting characters from predefined arrays for lowercase letters, uppercase letters, numbers, and symbols.

```cpp
std::string generatePassword(const int numLowercaseLetters, const int numUppercaseLetters, const int numNumbers, const int numSymbols, 
    const char* lowercaseLetters, const char* uppercaseLetters, const char* numbers, const char* symbols) {
    
    int localLower = numLowercaseLetters;
    int localUpper = numUppercaseLetters;
    int localNumbers = numNumbers;
    int localSymbols = numSymbols;


    std::string password = "";

    // Use a better random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    
    for (int i = 0; i < localLower; ++i) {
        password += lowercaseLetters[gen() % 26];
    }
    for (int i = 0; i < localUpper; ++i) {
        password += uppercaseLetters[gen() % 26];
    }
    for (int i = 0; i < localNumbers; ++i) {
        password += numbers[gen() % 10];
    }
    for (int i = 0; i < localSymbols; ++i) {
        password += symbols[gen() % 30];
    }
    random_shuffle(password.begin(), password.end());
    return password;
}
```

## ImGui GUI Setup
The ImGui interface is used to allow the user to choose how many characters of each type (lowercase, uppercase, numeric, special characters) they want to include in the generated password.
```cpp
            ImGui::Begin("Vaultify", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);    
            
            ImGui::PushFont(mainFont);

            ImGui::SetCursorPos(ImVec2(200,137.5));
            ImGui::PushItemWidth(352);
            static int i12 = 0;
            ImGui::SliderInt(" Lowercase Letters", &numLowercaseLetters, 0, 10);
            ImGui::PopItemWidth();

			// Amount of uppercase letters
            ImGui::SetCursorPos(ImVec2(200,234.5));
            ImGui::PushItemWidth(352);
            static int i14 = 0;
			ImGui::SliderInt(" Uppercase Letters", & numUppercaseLetters, 0, 10);
            ImGui::PopItemWidth();

			// Amount of numbers
            ImGui::SetCursorPos(ImVec2(200,338.5));
            ImGui::PushItemWidth(352);
            static int i16 = 0;
			ImGui::SliderInt(" Numbers", &numNumbers, 0, 10);
            ImGui::PopItemWidth();

			// Amount of special characters
            ImGui::SetCursorPos(ImVec2(200,428.5));
            ImGui::PushItemWidth(352);
            static int i19 = 0;
			ImGui::SliderInt(" Special Characters", &numSymbols, 0, 10);
            ImGui::PopItemWidth();

            //assign generated password to currentPassword variable
            ImGui::SetCursorPos(ImVec2(1000,300));
            if (ImGui::Button("Generate Password", ImVec2(400, 56))) {
				currentPassword = generatePassword(numLowercaseLetters, numUppercaseLetters, numNumbers, numSymbols, 
                lowercaseLetters, uppercaseLetters, numbers, symbols);

            }

            ImGui::SetCursorPos(ImVec2(1000,400));
            if (ImGui::Button("Copy to Clipboard", ImVec2(400, 56))) {
                ImGui::SetClipboardText(currentPassword.c_str());
            }

            ImGui::SetCursorPos(ImVec2(1000,200));
            ImGui::Text("Generated Password: %s", currentPassword.c_str()); //display generated password once button is clicked

            ImGui::SetCursorPos(ImVec2(30,950));
            ImGui::Text("Made by Yanis");
            ImGui::PopFont();
            ImGui::End();
        }
```
## Acknowledgements

- **ImGui**: A powerful and easy-to-use GUI library, used here for creating the user interface. Check out ImGui's [GitHub Repository](https://github.com/ocornut/imgui).
- **DirectX 11**: Used for rendering and managing device contexts in the Windows environment. Documentation can be found on the official [Microsoft DirectX 11 page](https://learn.microsoft.com/en-us/windows/win32/direct3d11/direct3d-11-graphics).
