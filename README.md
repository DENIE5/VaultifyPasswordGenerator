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
  
## Menu 

![image](https://github.com/user-attachments/assets/a4be16ed-c530-4b2a-8560-9f8edc001c0e)

## Code Overview

Below are important snippets of the code that demonstrate how the program works:

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
## File Saving

You can save your password to a file now. The passwords will keep being written into the same file so you don't have 20 different files for all your passwords.
```cpp
	void savePassword(std::string currentPassword) {

    std::ofstream file("SavedPasswords.txt", std::fstream::app);
    file << currentPassword << std::endl;
    file.close();
}
```

## ImGui GUI Setup
The ImGui interface is used to allow the user to choose how many characters of each type (lowercase, uppercase, numeric, special characters) they want to include in the generated password.
```cpp
                ImGui::Begin("Vaultify", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);     //ImGuiWindowFlags_NoTitleBar  - removes native imgui titlebar
    
    
    ImGui::PushFont(mainFont);

    ImGui::PushStyleColor(ImGuiCol_FrameBg, track);        
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, track_hover);  
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, track_active); 
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, grab);         
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, grab_active);

   
    ImGui::SetCursorPos(ImVec2(50,50));
    ImGui::PushItemWidth(200);
    static int i12 = 0;
    ImGui::SliderInt(" Lowercase", &numLowercaseLetters, 0, 5);
    ImGui::PopItemWidth();

	// Amount of uppercase letters
    ImGui::SetCursorPos(ImVec2(50,100));
    ImGui::PushItemWidth(200);
    static int i14 = 0;
	ImGui::SliderInt(" Uppercase", & numUppercaseLetters, 0, 5);
    ImGui::PopItemWidth();

	// Amount of numbers
    ImGui::SetCursorPos(ImVec2(50,150));
    ImGui::PushItemWidth(200);
    static int i16 = 0;
	ImGui::SliderInt(" Numbers", &numNumbers, 0, 5);
    ImGui::PopItemWidth();

	// Amount of special characters
    ImGui::SetCursorPos(ImVec2(50,200));
    ImGui::PushItemWidth(200);
    
    static int i19 = 0;
	ImGui::SliderInt(" Special", &numSymbols, 0, 5);
    ImGui::PopItemWidth();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1));

    ImGui::SetCursorPos(ImVec2(870, 5));
    if (ImGui::Button("X", ImVec2(30, 30))) {
        PostQuitMessage(0); 
    }

   

    ImGui::PopStyleColor(3);

    ImGui::PushStyleColor(ImGuiCol_Button, btn);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, btn_hover);

    //assign generated password to currentPassword variable
    ImGui::SetCursorPos(ImVec2(500,100));
    if (ImGui::Button("Generate", ImVec2(200, 30))) {
		currentPassword = generatePassword(numLowercaseLetters, numUppercaseLetters, numNumbers, numSymbols, 
        lowercaseLetters, uppercaseLetters, numbers, symbols);

    }

    ImGui::SetCursorPos(ImVec2(500,150));
    if (ImGui::Button("Copy to Clipboard", ImVec2(200, 30))) {
        ImGui::SetClipboardText(currentPassword.c_str());
    }

    ImGui::SetCursorPos(ImVec2(500, 50));
    ImGui::Text("Generated Password: %s", currentPassword.c_str()); //display generated password once button is clicked

    ImGui::SetCursorPos(ImVec2(500, 200));
    if(ImGui::Button("Save Password", ImVec2(200, 30))) {
        savePassword(currentPassword);
    }

    ImGui::SetCursorPos(ImVec2(50,425));
    ImGui::Text("Made by @Plowh");

    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::End();
}
        }
```
## Acknowledgements

- **ImGui**: A powerful and easy-to-use GUI library, used here for creating the user interface. Check out ImGui's [GitHub Repository](https://github.com/ocornut/imgui).
- **DirectX 11**: Used for rendering and managing device contexts in the Windows environment. Documentation can be found on the official [Microsoft DirectX 11 page](https://learn.microsoft.com/en-us/windows/win32/direct3d11/direct3d-11-graphics).
