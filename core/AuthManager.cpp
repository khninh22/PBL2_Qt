#include "AuthManager.h"
#include "QuanLyThueSan.h"
#include <iostream>

// Mật khẩu admin mặc định
const string AuthManager::ADMIN_PASSWORD = "123";

AuthManager::AuthManager()
    : currentUser(""), currentRole(ROLE_NONE), currentEmployeeId(""),
      adminPasswordFile("data/admin.dat")
{
}

bool AuthManager::loginAsAdmin(const string &password)
{
    // Load mật khẩu admin từ file (nếu có)
    ifstream in(adminPasswordFile, ios::binary);
    string savedPassword = ADMIN_PASSWORD;

    if (in.is_open())
    {
        size_t len;
        in.read(reinterpret_cast<char *>(&len), sizeof(len));
        savedPassword.resize(len);
        in.read(&savedPassword[0], len);
        in.close();
    }

    if (password == savedPassword)
    {
        currentUser = "ADMIN";
        currentRole = ROLE_ADMIN;
        currentEmployeeId = "";
        return true;
    }
    return false;
}

bool AuthManager::loginAsEmployee(const string &username, const string &password, QuanLyThueSan *qlts)
{
    if (!qlts)
        return false;

    const MangDong<NhanVien> &dsNV = qlts->getDsNhanVien();

    for (int i = 0; i < dsNV.getKichThuoc(); i++)
    {
        const NhanVien &nv = dsNV[i];

        // Kiểm tra nhân viên còn làm việc và tài khoản/mật khẩu khớp
        if (nv.getDangLam() &&
            nv.getTaiKhoan() == username &&
            nv.getMatKhau() == password)
        {
            currentUser = nv.getHoTen();
            currentRole = ROLE_EMPLOYEE;
            currentEmployeeId = nv.getMaNV();
            return true;
        }
    }
    return false;
}

bool AuthManager::isLoggedIn() const
{
    return currentRole != ROLE_NONE;
}

UserRole AuthManager::getCurrentRole() const
{
    return currentRole;
}

string AuthManager::getCurrentUser() const
{
    return currentUser;
}

string AuthManager::getCurrentEmployeeId() const
{
    return currentEmployeeId;
}

void AuthManager::logout()
{
    currentUser = "";
    currentRole = ROLE_NONE;
    currentEmployeeId = "";
}

bool AuthManager::changeAdminPassword(const string &oldPass, const string &newPass)
{
    if (!loginAsAdmin(oldPass))
        return false;

    saveAdminPassword(newPass);
    return true;
}

bool AuthManager::hasFullAccess() const
{
    return currentRole == ROLE_ADMIN;
}

bool AuthManager::canManageEmployees() const
{
    return currentRole == ROLE_ADMIN;
}

bool AuthManager::canViewStatistics() const
{
    // Admin và một số vị trí nhân viên có thể xem thống kê
    return currentRole == ROLE_ADMIN;
}

void AuthManager::loadAdminPassword()
{
    // Được gọi trong constructor, không cần load lại
}

void AuthManager::saveAdminPassword(const string &password)
{
    ofstream out(adminPasswordFile, ios::binary);
    if (out.is_open())
    {
        size_t len = password.length();
        out.write(reinterpret_cast<const char *>(&len), sizeof(len));
        out.write(password.c_str(), len);
        out.close();
    }
}
