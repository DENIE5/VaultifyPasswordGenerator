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
std::string generatePassword() {
    std::string password = "";

    // Use a better random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate characters as before
    for (int i = 0; i < numLowercaseLetters; ++i) {
        password += lowercaseLetters[gen() % 26];
    }
    for (int i = 0; i < numUppercaseLetters; ++i) {
        password += uppercaseLetters[gen() % 26];
    }
    for (int i = 0; i < numNumbers; ++i) {
        password += numbers[gen() % 10];
    }
    for (int i = 0; i < numSymbols; ++i) {
        password += symbols[gen() % 31];
    }

    // Shuffle the entire password string
    std::shuffle(password.begin(), password.end(), gen);

    return password;
}
```

## ImGui GUI Setup
The ImGui interface is used to allow the user to choose how many characters of each type (lowercase, uppercase, numeric, special characters) they want to include in the generated password.

```cpp
ImGui::Begin("Vaultify");

ImGui::Text("How many lowercase letters would you like?\n\n");
ImGui::SliderInt("Lowercase Letters", &numLowercaseLetters, 0, 20);

ImGui::Text("How many uppercase letters would you like?\n\n");
ImGui::SliderInt("Uppercase Letters", &numUppercaseLetters, 0, 20);

ImGui::Text("How many numbers would you like?\n\n");
ImGui::SliderInt("Numbers", &numNumbers, 0, 20);

ImGui::Text("How many special characters would you like?\n\n");
ImGui::SliderInt("Special Characters", &numSymbols, 0, 20);

// Button to generate password
if (ImGui::Button("Generate Password", ImVec2(200, 0))) {
    currentPassword = generatePassword();
}

ImGui::Text("Generated Password: %s", currentPassword.c_str()); // Display generated password once button is clicked

ImGui::End();
```

## Acknowledgements

- **ImGui**: A powerful and easy-to-use GUI library, used here for creating the user interface. Check out ImGui's [GitHub Repository](https://github.com/ocornut/imgui).
- **DirectX 11**: Used for rendering and managing device contexts in the Windows environment. Documentation can be found on the official [Microsoft DirectX 11 page](https://learn.microsoft.com/en-us/windows/win32/direct3d11/direct3d-11-graphics).
