#include "BackupManager.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>

// Constructor
BackupManager::BackupManager(FileManager *fm)
    : fileManager(fm), prefixBackup("backup_") {}

// Helper: Generate timestamped filename
std::string BackupManager::taoTenFileBackup(const std::string &tenGoc) const
{
    // Format: backup_YYYYMMDD_HHMMSS_filename.dat
    time_t now = time(nullptr);
    struct tm *timeinfo = localtime(&now);
    
    std::ostringstream oss;
    oss << prefixBackup
        << std::setfill('0')
        << std::setw(4) << (timeinfo->tm_year + 1900)
        << std::setw(2) << (timeinfo->tm_mon + 1)
        << std::setw(2) << timeinfo->tm_mday
        << "_"
        << std::setw(2) << timeinfo->tm_hour
        << std::setw(2) << timeinfo->tm_min
        << std::setw(2) << timeinfo->tm_sec
        << "_" << tenGoc;
    
    return oss.str();
}

// Backup operations
bool BackupManager::taoBackup(const std::string &tenFile)
{
    if (!fileManager)
        return false;
    
    std::string fileGoc = fileManager->getFilePath(tenFile);
    
    // Kiểm tra file gốc tồn tại
    if (!fileManager->kiemTraFileExists(fileGoc))
        return false;
    
    // Tạo tên file backup
    std::string tenBackup = taoTenFileBackup(tenFile);
    std::string fileBackup = fileManager->getBackupPath(tenBackup);
    
    // Sao chép file
    return fileManager->saoChepFile(fileGoc, fileBackup);
}

bool BackupManager::taoBackupToanBo()
{
    if (!fileManager)
        return false;
    
    // Danh sách các file cần backup
    std::vector<std::string> dsFile = {
        "sanbong.dat",
        "khachhang.dat",
        "lichdat.dat",
        "dichvu.dat",
        "nhanvien.dat",
        "thanhtoan.dat"
    };
    
    bool thanhCong = true;
    for (const auto &tenFile : dsFile)
    {
        if (!taoBackup(tenFile))
            thanhCong = false; // Ghi log nhưng tiếp tục
    }
    
    return thanhCong;
}

// Restore operations
bool BackupManager::khoiPhuc(const std::string &fileBackup, const std::string &fileDich)
{
    if (!fileManager)
        return false;
    
    std::string nguon = fileManager->getBackupPath(fileBackup);
    std::string dich = fileManager->getFilePath(fileDich);
    
    // Kiểm tra file backup tồn tại
    if (!fileManager->kiemTraFileExists(nguon))
        return false;
    
    // Sao chép backup về file gốc
    return fileManager->saoChepFile(nguon, dich);
}

std::vector<std::string> BackupManager::layDanhSachBackup() const
{
    if (!fileManager)
        return {};
    
    // Lấy danh sách tất cả file trong backup/
    std::vector<std::string> allFiles = fileManager->layDanhSachFile(
        fileManager->getBackupDir(), prefixBackup);
    
    // Sắp xếp theo tên (mới nhất lên đầu do timestamp)
    std::sort(allFiles.begin(), allFiles.end(), std::greater<std::string>());
    
    return allFiles;
}

// Cleanup
void BackupManager::xoaBackupCu(int soNgayGiu)
{
    if (!fileManager)
        return;
    
    time_t now = time(nullptr);
    time_t gioiHan = now - (soNgayGiu * 24 * 3600);
    
    std::vector<std::string> dsBackup = layDanhSachBackup();
    
    for (const auto &tenFile : dsBackup)
    {
        std::string fullPath = fileManager->getBackupPath(tenFile);
        
        // Lấy thời gian tạo file (simplified - compare filename timestamp)
        // TODO: Parse timestamp from filename và so sánh
        // Hiện tại chỉ xóa nếu file quá cũ dựa trên filesystem
        
        // For now, skip implementation - cần parse timestamp từ tên file
    }
}
