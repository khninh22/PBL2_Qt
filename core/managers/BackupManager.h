#ifndef BACKUPMANAGER_H
#define BACKUPMANAGER_H

#include "FileManager.h"
#include <string>
#include <vector>

/**
 * @brief Backup Manager - Auto-backup and restore functionality
 * @details PRACTICAL APPLICATION:
 *          - Auto-backup before each save operation
 *          - Restore from backup in UI
 *          - Timestamped backups
 *          - Data recovery for user errors
 */
class BackupManager
{
private:
    FileManager *fileManager;
    std::string prefixBackup; // "backup_"
    
    // Helper: Generate timestamped filename
    std::string taoTenFileBackup(const std::string &tenGoc) const;
    
public:
    // Constructor
    BackupManager(FileManager *fm);
    
    // Backup operations
    bool taoBackup(const std::string &tenFile);
    bool taoBackupToanBo(); // Backup all data files
    
    // Restore operations
    bool khoiPhuc(const std::string &fileBackup, const std::string &fileDich);
    std::vector<std::string> layDanhSachBackup() const;
    
    // Cleanup
    void xoaBackupCu(int soNgayGiu = 30); // Xóa backup cũ hơn N ngày
};

#endif // BACKUPMANAGER_H
