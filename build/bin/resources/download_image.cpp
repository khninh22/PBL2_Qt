// Image Downloader - Tải ảnh nền chất lượng cao
// Compile: g++ -std=c++17 download_image.cpp -o download_image.exe -lcurl
// Run: ./download_image.exe

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#endif

using namespace std;

/**
 * Download file using Windows API (WinINet)
 * Không cần curl, built-in Windows
 */
bool downloadFileWindows(const string &url, const string &outputPath)
{
#ifdef _WIN32
    cout << "Connecting to: " << url << endl;

    HINTERNET hInternet = InternetOpenA(
        "ImageDownloader/1.0",
        INTERNET_OPEN_TYPE_DIRECT,
        NULL, NULL, 0);

    if (!hInternet)
    {
        cerr << "Error: Cannot initialize internet connection!" << endl;
        return false;
    }

    HINTERNET hUrl = InternetOpenUrlA(
        hInternet,
        url.c_str(),
        NULL, 0,
        INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE,
        0);

    if (!hUrl)
    {
        cerr << "Error: Cannot open URL!" << endl;
        InternetCloseHandle(hInternet);
        return false;
    }

    cout << "Downloading..." << endl;

    ofstream outFile(outputPath, ios::binary);
    if (!outFile)
    {
        cerr << "Error: Cannot create output file!" << endl;
        InternetCloseHandle(hUrl);
        InternetCloseHandle(hInternet);
        return false;
    }

    const DWORD bufferSize = 4096;
    char buffer[bufferSize];
    DWORD bytesRead = 0;
    DWORD totalBytes = 0;

    while (InternetReadFile(hUrl, buffer, bufferSize, &bytesRead) && bytesRead > 0)
    {
        outFile.write(buffer, bytesRead);
        totalBytes += bytesRead;

        // Progress indicator
        if (totalBytes % (1024 * 100) == 0)
        { // Every 100KB
            cout << "\rDownloaded: " << (totalBytes / 1024) << " KB" << flush;
        }
    }

    cout << "\rDownloaded: " << (totalBytes / 1024) << " KB" << endl;

    outFile.close();
    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    cout << "Success! Saved to: " << outputPath << endl;
    return true;
#else
    cerr << "Error: This program is designed for Windows only!" << endl;
    return false;
#endif
}

int main(int argc, char *argv[])
{
    cout << "========================================" << endl;
    cout << "  Image Downloader - High Quality" << endl;
    cout << "========================================" << endl;
    cout << endl;

    string url;
    string outputPath;

    if (argc >= 2)
    {
        url = argv[1];
        outputPath = (argc >= 3) ? argv[2] : "login_bg.jpg";
    }
    else
    {
        // Default: High quality football stadium image
        cout << "Select image source:" << endl;
        cout << "1. Unsplash - Soccer Stadium (High Quality 1920x1080)" << endl;
        cout << "2. Custom URL" << endl;
        cout << "Choice (1-2): ";

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            // Unsplash API - Random soccer stadium photo
            url = "https://source.unsplash.com/1920x1080/?soccer-stadium,football-field";
            outputPath = "login_bg.jpg";
        }
        else
        {
            cout << "Enter image URL: ";
            cin.ignore();
            getline(cin, url);

            cout << "Output filename (default: login_bg.jpg): ";
            getline(cin, outputPath);
            if (outputPath.empty())
            {
                outputPath = "login_bg.jpg";
            }
        }
    }

    cout << endl;
    cout << "Source: " << url << endl;
    cout << "Output: " << outputPath << endl;
    cout << endl;

    bool success = downloadFileWindows(url, outputPath);

    if (success)
    {
        cout << endl;
        cout << "========================================" << endl;
        cout << "  DOWNLOAD COMPLETED!" << endl;
        cout << "========================================" << endl;
        cout << "Image saved to: " << outputPath << endl;
        cout << "You can now use it in your application." << endl;
        return 0;
    }
    else
    {
        cout << endl;
        cout << "========================================" << endl;
        cout << "  DOWNLOAD FAILED!" << endl;
        cout << "========================================" << endl;
        cerr << "Please check your internet connection and try again." << endl;
        return 1;
    }
}
