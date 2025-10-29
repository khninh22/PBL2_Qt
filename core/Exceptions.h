#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
using namespace std;

// ============================
//  CUSTOM EXCEPTIONS
//  Xử lý lỗi theo hướng đối tượng
// ============================

// Base exception class
class QuanLyException : public exception
{
protected:
    string message;
    string code; // Mã lỗi để dễ xử lý

public:
    QuanLyException(const string &msg, const string &errorCode = "ERR_UNKNOWN")
        : message(msg), code(errorCode) {}

    const char *what() const noexcept override
    {
        return message.c_str();
    }

    string getCode() const { return code; }
    string getMessage() const { return message; }
};

// Không tìm thấy
class KhongTimThayException : public QuanLyException
{
public:
    KhongTimThayException(const string &loai, const string &ma)
        : QuanLyException(loai + " khong tim thay: " + ma, "ERR_NOT_FOUND") {}
};

// Dữ liệu không hợp lệ
class DuLieuKhongHopLeException : public QuanLyException
{
public:
    DuLieuKhongHopLeException(const string &field, const string &reason)
        : QuanLyException("Du lieu khong hop le [" + field + "]: " + reason, "ERR_INVALID_DATA") {}
};

// Trùng lặp (mã đã tồn tại)
class TrungLapException : public QuanLyException
{
public:
    TrungLapException(const string &loai, const string &ma)
        : QuanLyException(loai + " da ton tai: " + ma, "ERR_DUPLICATE") {}
};

// Nghiệp vụ không hợp lệ
class NghiepVuException : public QuanLyException
{
public:
    NghiepVuException(const string &msg)
        : QuanLyException("Loi nghiep vu: " + msg, "ERR_BUSINESS_LOGIC") {}
};

// File không tìm thấy hoặc lỗi đọc/ghi
class FileException : public QuanLyException
{
public:
    FileException(const string &filename, const string &operation)
        : QuanLyException("Loi file [" + filename + "]: " + operation, "ERR_FILE_IO") {}
};

// Không đủ quyền
class PhanQuyenException : public QuanLyException
{
public:
    PhanQuyenException(const string &action)
        : QuanLyException("Khong du quyen thuc hien: " + action, "ERR_PERMISSION") {}
};

// Xung đột dữ liệu (ví dụ: trùng lịch)
class XungDotException : public QuanLyException
{
public:
    XungDotException(const string &msg)
        : QuanLyException("Xung dot: " + msg, "ERR_CONFLICT") {}
};

#endif // EXCEPTIONS_H
