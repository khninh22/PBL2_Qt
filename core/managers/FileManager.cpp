#include "FileManager.h"
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

// Constructors
FileManager::FileManager()
    : dataDir("data/"), backupDir("data/backup/")
{
    khoiTaoThuMuc();
}

FileManager::FileManager(const std::string &dataPath, const std::string &backupPath)
    : dataDir(dataPath), backupDir(backupPath)
{
    khoiTaoThuMuc();
}

// File path builders
std::string FileManager::getFilePath(const std::string &filename) const
{
    return dataDir + filename;
}

std::string FileManager::getBackupPath(const std::string &filename) const
{
    return backupDir + filename;
}

// File operations
bool FileManager::kiemTraFileExists(const std::string &filepath) const
{
    return fs::exists(filepath) && fs::is_regular_file(filepath);
}

long long FileManager::layKichThuocFile(const std::string &filepath) const
{
    if (!kiemTraFileExists(filepath))
        return -1;
    
    try
    {
        return static_cast<long long>(fs::file_size(filepath));
    }
    catch (const fs::filesystem_error &)
    {
        return -1;
    }
}

bool FileManager::xoaFile(const std::string &filepath) const
{
    try
    {
        return fs::remove(filepath);
    }
    catch (const fs::filesystem_error &)
    {
        return false;
    }
}

bool FileManager::saoChepFile(const std::string &nguon, const std::string &dich) const
{
    try
    {
        fs::copy_file(nguon, dich, fs::copy_options::overwrite_existing);
        return true;
    }
    catch (const fs::filesystem_error &)
    {
        return false;
    }
}

// Directory operations
bool FileManager::taoDuongDan(const std::string &path) const
{
    try
    {
        return fs::create_directories(path);
    }
    catch (const fs::filesystem_error &)
    {
        return false;
    }
}

bool FileManager::kiemTraDuongDanExists(const std::string &path) const
{
    return fs::exists(path) && fs::is_directory(path);
}

std::vector<std::string> FileManager::layDanhSachFile(const std::string &duongDan, const std::string &pattern) const
{
    std::vector<std::string> files;
    
    try
    {
        if (!kiemTraDuongDanExists(duongDan))
            return files;
        
        for (const auto &entry : fs::directory_iterator(duongDan))
        {
            if (entry.is_regular_file())
            {
                std::string filename = entry.path().filename().string();
                
                // Simple pattern matching (* wildcard)
                if (pattern == "*" || filename.find(pattern) != std::string::npos)
                {
                    files.push_back(filename);
                }
            }
        }
    }
    catch (const fs::filesystem_error &)
    {
        // Return empty vector
    }
    
    return files;
}

// Initialization
void FileManager::khoiTaoThuMuc()
{
    if (!kiemTraDuongDanExists(dataDir))
    {
        taoDuongDan(dataDir);
        std::cout << "[FileManager] Created: " << dataDir << std::endl;
    }
    
    if (!kiemTraDuongDanExists(backupDir))
    {
        taoDuongDan(backupDir);
        std::cout << "[FileManager] Created: " << backupDir << std::endl;
    }
}
