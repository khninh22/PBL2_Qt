#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>

/**
 * @brief File Manager - Centralized file I/O management
 * @details PRACTICAL APPLICATION:
 *          - Centralize file paths (data/, data/backup/)
 *          - Consistent error handling
 *          - Auto-create directories
 *          - File existence checks
 */
class FileManager
{
private:
    std::string dataDir;      // "data/"
    std::string backupDir;    // "data/backup/"
    
public:
    // Constructor
    FileManager();
    FileManager(const std::string &dataPath, const std::string &backupPath);
    
    // Path getters
    std::string getDataDir() const { return dataDir; }
    std::string getBackupDir() const { return backupDir; }
    
    // File path builders
    std::string getFilePath(const std::string &filename) const;
    std::string getBackupPath(const std::string &filename) const;
    
    // File operations
    bool kiemTraFileExists(const std::string &filepath) const;
    long long layKichThuocFile(const std::string &filepath) const;
    bool xoaFile(const std::string &filepath) const;
    bool saoChepFile(const std::string &nguon, const std::string &dich) const;
    
    // Directory operations
    bool taoDuongDan(const std::string &path) const;
    bool kiemTraDuongDanExists(const std::string &path) const;
    std::vector<std::string> layDanhSachFile(const std::string &duongDan, const std::string &pattern = "*") const;
    
    // Initialization
    void khoiTaoThuMuc();  // Tạo data/ và data/backup/ nếu chưa có
};

#endif // FILEMANAGER_H
