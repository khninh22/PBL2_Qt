#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <string>
#include <fstream>
using namespace std;

// Enum vai trò người dùng
enum UserRole
{
    ROLE_NONE = 0,
    ROLE_ADMIN = 1,   // Chủ - quyền đầy đủ
    ROLE_EMPLOYEE = 2 // Nhân viên - bị hạn chế
};

class AuthManager
{
private:
    static const string ADMIN_PASSWORD; // Mật khẩu Admin mặc định
    string currentUser;
    UserRole currentRole;
    string currentEmployeeId;

    // File lưu thông tin admin
    string adminPasswordFile;

public:
    AuthManager();

    // Đăng nhập
    bool loginAsAdmin(const string &password);
    bool loginAsEmployee(const string &username, const string &password, class QuanLyThueSan *qlts);

    // Thông tin đăng nhập hiện tại
    bool isLoggedIn() const;
    UserRole getCurrentRole() const;
    string getCurrentUser() const;
    string getCurrentEmployeeId() const;

    // Đăng xuất
    void logout();

    // Đổi mật khẩu admin
    bool changeAdminPassword(const string &oldPass, const string &newPass);

    // Kiểm tra quyền
    bool hasFullAccess() const;      // Admin có quyền đầy đủ
    bool canManageEmployees() const; // Chỉ admin
    bool canViewStatistics() const;  // Admin + một số nhân viên

    // Load/Save
    void loadAdminPassword();
    void saveAdminPassword(const string &password);
};

#endif // AUTHMANAGER_H
