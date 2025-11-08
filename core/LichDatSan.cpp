#include "LichDatSan.h"
#include <iomanip>
#include <sstream>

// Hàm format thời gian (DEPRECATED - use NgayGio::toString() instead)
string formatTime(time_t t)
{
    if (t == 0)
        return "N/A";
    char buffer[20];
#ifdef _WIN32
    tm local_tm;
    localtime_s(&local_tm, &t);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", &local_tm);
#else
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", localtime(&t));
#endif
    return string(buffer);
}

// Constructor (NEW: Object pointer version - RECOMMENDED)
LichDatSan::LichDatSan(string ma, KhachHang *kh, SanBong *san,
                       const NgayGio &batDau, const NgayGio &ketThuc,
                       double tongTien_, string trangThaiDat_)
{
    maLichDat = ma;
    khachHang = kh;  // ✅ Store object pointer
    sanBong = san;   // ✅ Store object pointer
    tempMaKH = "";   // Clear temp
    tempMaSan = "";  // Clear temp
    thoiGianBatDau = batDau;
    thoiGianKetThuc = ketThuc;
    tongTien = tongTien_;
    trangThaiDat = trangThaiDat_;
    trangThaiTT = "Chưa Thanh Toán";
    thoiGianThanhToan = NgayGio(); // Default empty
}

// Constructor (BACKWARD COMPATIBLE: string ID version - for loading old data)
LichDatSan::LichDatSan(string ma, string maKH_, string maSan_,
                       const NgayGio &batDau, const NgayGio &ketThuc,
                       double tongTien_, string trangThaiDat_)
{
    maLichDat = ma;
    khachHang = nullptr; // ✅ Will be resolved by QuanLyThueSan after loading
    sanBong = nullptr;   // ✅ Will be resolved by QuanLyThueSan after loading
    tempMaKH = maKH_;    // Store temporarily for resolution
    tempMaSan = maSan_;  // Store temporarily for resolution
    thoiGianBatDau = batDau;
    thoiGianKetThuc = ketThuc;
    tongTien = tongTien_;
    trangThaiDat = trangThaiDat_;
    trangThaiTT = "Chưa Thanh Toán";
    thoiGianThanhToan = NgayGio(); // Default empty
}

// Constructor (BACKWARD COMPATIBLE: time_t version)
LichDatSan::LichDatSan(string ma, string maKH_, string maSan_,
                       time_t batDau, time_t ketThuc,
                       double tongTien_, string trangThaiDat_)
{
    maLichDat = ma;
    khachHang = nullptr; // ✅ Will be resolved by QuanLyThueSan after loading
    sanBong = nullptr;   // ✅ Will be resolved by QuanLyThueSan after loading
    tempMaKH = maKH_;    // Store temporarily for resolution
    tempMaSan = maSan_;  // Store temporarily for resolution
    thoiGianBatDau = NgayGio(batDau);   // Convert time_t → NgayGio
    thoiGianKetThuc = NgayGio(ketThuc); // Convert time_t → NgayGio
    tongTien = tongTien_;
    trangThaiDat = trangThaiDat_;
    trangThaiTT = "Chưa Thanh Toán";
    thoiGianThanhToan = NgayGio(); // Default empty
}

// Getters (Object pointer version)
string LichDatSan::getMaLichDat() const
{
    return maLichDat;
}

KhachHang* LichDatSan::getKhachHang() const
{
    return khachHang; // ✅ Return object pointer
}

SanBong* LichDatSan::getSanBong() const
{
    return sanBong; // ✅ Return object pointer
}

string LichDatSan::getMaSan() const
{
    return (sanBong != nullptr) ? sanBong->getMaSan() : tempMaSan; // ✅ Return pointer data or temp ID
}

string LichDatSan::getMaKH() const
{
    return (khachHang != nullptr) ? khachHang->getMaKH() : tempMaKH; // ✅ Return pointer data or temp ID
}

string LichDatSan::getTempMaKH() const
{
    return tempMaKH; // ✅ Get unresolved ID for pointer resolution
}

string LichDatSan::getTempMaSan() const
{
    return tempMaSan; // ✅ Get unresolved ID for pointer resolution
}

NgayGio LichDatSan::getThoiGianBatDau() const
{
    return thoiGianBatDau;
}

NgayGio LichDatSan::getThoiGianKetThuc() const
{
    return thoiGianKetThuc;
}

double LichDatSan::getTongTien() const
{
    return tongTien;
}

string LichDatSan::getTrangThaiDat() const
{
    return trangThaiDat;
}

string LichDatSan::getTrangThaiTT() const
{
    return trangThaiTT;
}

NgayGio LichDatSan::getThoiGianThanhToan() const
{
    return thoiGianThanhToan;
}

// Getters (BACKWARD COMPATIBLE: time_t version)
time_t LichDatSan::getThoiGianBatDauTimeT() const
{
    return thoiGianBatDau.toTimeT();
}

time_t LichDatSan::getThoiGianKetThucTimeT() const
{
    return thoiGianKetThuc.toTimeT();
}

time_t LichDatSan::getThoiGianThanhToanTimeT() const
{
    return thoiGianThanhToan.toTimeT();
}

// Setters
void LichDatSan::setKhachHang(KhachHang *kh)
{
    khachHang = kh; // ✅ Set object pointer
}

void LichDatSan::setSanBong(SanBong *san)
{
    sanBong = san; // ✅ Set object pointer
}

void LichDatSan::setTrangThaiDat(const string &s)
{
    trangThaiDat = s;
}

void LichDatSan::setTrangThaiTT(const string &s)
{
    trangThaiTT = s;
}

void LichDatSan::setThoiGianThanhToan(const NgayGio &t)
{
    thoiGianThanhToan = t;
}

void LichDatSan::setThoiGianThanhToan(time_t t)
{
    thoiGianThanhToan = NgayGio(t); // Convert time_t → NgayGio
}

void LichDatSan::setTongTien(double tien)
{
    tongTien = tien;
}

// Hiển thị thông tin lịch đặt sân (✅ USE NgayGio::toString() + Object pointers)
void LichDatSan::hienThi() const
{
    cout << left
         << setw(12) << maLichDat
         << setw(10) << (khachHang ? khachHang->getMaKH() : "N/A")  // ✅ Navigate pointer
         << setw(10) << (sanBong ? sanBong->getMaSan() : "N/A")     // ✅ Navigate pointer
         << setw(18) << thoiGianBatDau.toString()   // ✅ NEW: NgayGio display
         << setw(18) << thoiGianKetThuc.toString()  // ✅ NEW: NgayGio display
         << right << setw(12) << fixed << setprecision(0) << tongTien
         << left
         << setw(14) << trangThaiDat
         << setw(16) << trangThaiTT;

    // Check if payment time is set (not default empty NgayGio)
    if (thoiGianThanhToan.getNgay().getNam() != 2000) // Default NgayGio has year 2000
        cout << setw(20) << thoiGianThanhToan.toString();
    else
        cout << setw(20) << "N/A";

    cout << endl;
}

// Serialization: Ghi đối tượng vào file binary (✅ SAVE as string IDs for compatibility)
void LichDatSan::serialize(ofstream &out) const
{
    // Ghi độ dài và nội dung của maLichDat
    size_t len = maLichDat.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(maLichDat.c_str(), len);

    // Ghi maKH (extract from object pointer, or empty if null)
    string maKH = (khachHang != nullptr) ? khachHang->getMaKH() : "";
    len = maKH.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(maKH.c_str(), len);

    // Ghi maSan (extract from object pointer, or empty if null)
    string maSan = (sanBong != nullptr) ? sanBong->getMaSan() : "";
    len = maSan.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(maSan.c_str(), len);

    // ✅ NEW: Ghi NgayGio batDau
    thoiGianBatDau.ghiFile(out);

    // ✅ NEW: Ghi NgayGio ketThuc
    thoiGianKetThuc.ghiFile(out);

    // Ghi tongTien (double)
    out.write(reinterpret_cast<const char *>(&tongTien), sizeof(tongTien));

    // Ghi độ dài và nội dung của trangThaiDat
    len = trangThaiDat.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(trangThaiDat.c_str(), len);

    // Ghi độ dài và nội dung của trangThaiTT
    len = trangThaiTT.length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(trangThaiTT.c_str(), len);

    // ✅ NEW: Ghi NgayGio thanhToan
    thoiGianThanhToan.ghiFile(out);
}

// Deserialization: Đọc đối tượng từ file binary (✅ Load as string IDs - will be resolved later)
void LichDatSan::deserialize(ifstream &in)
{
    // NOTE: This reads string IDs from file (backward compatible)
    // QuanLyThueSan will call setKhachHang() and setSanBong() to resolve pointers after loading
    
    // Đọc maLichDat
    size_t len;
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    maLichDat.resize(len);
    in.read(&maLichDat[0], len);

    // Đọc maKH (store in tempMaKH, will be used to find KhachHang* later)
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    tempMaKH.resize(len);
    in.read(&tempMaKH[0], len);
    khachHang = nullptr; // Will be resolved by QuanLyThueSan

    // Đọc maSan (store in tempMaSan, will be used to find SanBong* later)
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    tempMaSan.resize(len);
    in.read(&tempMaSan[0], len);
    sanBong = nullptr; // Will be resolved by QuanLyThueSan

    // ✅ TRY NEW FORMAT FIRST (NgayGio)
    // Check if next bytes look like NgayGio (3 ints for NgayThang + 3 ints for time)
    streampos pos = in.tellg();
    int testNgay;
    in.read(reinterpret_cast<char *>(&testNgay), sizeof(testNgay));
    
    // NgayGio format: ngay (1-31), thang (1-12), nam (1900-2100), gio (0-23), phut (0-59), giay (0-59)
    if (testNgay >= 1 && testNgay <= 31) {
        // Looks like new NgayGio format
        in.seekg(pos); // Reset to start of NgayGio
        thoiGianBatDau.docFile(in);
        thoiGianKetThuc.docFile(in);
    } else {
        // Old time_t format (backward compatible)
        in.seekg(pos); // Reset
        time_t batDau, ketThuc;
        in.read(reinterpret_cast<char *>(&batDau), sizeof(batDau));
        in.read(reinterpret_cast<char *>(&ketThuc), sizeof(ketThuc));
        thoiGianBatDau = NgayGio(batDau);
        thoiGianKetThuc = NgayGio(ketThuc);
    }

    // Đọc tongTien
    in.read(reinterpret_cast<char *>(&tongTien), sizeof(tongTien));

    // Đọc trangThaiDat
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    trangThaiDat.resize(len);
    in.read(&trangThaiDat[0], len);
    
    // ✅ FIX: Chuẩn hóa encoding cho các giá trị phổ biến
    if (trangThaiDat.find("Đ") != string::npos && trangThaiDat.find("ã") == string::npos) {
        // Có dấu nhưng không đúng UTF-8, normalize
        if (trangThaiDat == "Đã Đặt" || trangThaiDat.size() < 10) {
            trangThaiDat = "Đã Đặt";
        } else if (trangThaiDat.find("Hủy") != string::npos || trangThaiDat.find("H") != string::npos) {
            trangThaiDat = "Đã Hủy";
        }
    }

    // Đọc trangThaiTT
    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    trangThaiTT.resize(len);
    in.read(&trangThaiTT[0], len);
    
    // ✅ FIX: Chuẩn hóa encoding - kiểm tra byte không hợp lệ
    bool hasInvalidUTF8 = false;
    for (unsigned char c : trangThaiTT) {
        if (c >= 0x80 && c <= 0x9F) { // Windows-1252 control characters
            hasInvalidUTF8 = true;
            break;
        }
    }
    
    if (hasInvalidUTF8 || (trangThaiTT != "Đã Thanh Toán" && trangThaiTT != "Chưa Thanh Toán")) {
        // Dựa vào độ dài để phân biệt
        // "Đã Thanh Toán" = 16 bytes UTF-8 hoặc ~14 bytes Windows-1252
        // "Chưa Thanh Toán" = 18 bytes UTF-8 hoặc ~16 bytes Windows-1252
        if (len >= 15 && len <= 17) {
            trangThaiTT = "Đã Thanh Toán";
        } else if (len >= 17) {
            trangThaiTT = "Chưa Thanh Toán";
        }
    }

    // Đọc thoiGianThanhToan (check format again)
    pos = in.tellg();
    in.read(reinterpret_cast<char *>(&testNgay), sizeof(testNgay));
    
    if (testNgay >= 0 && testNgay <= 31) { // 0 for empty, 1-31 for valid day
        in.seekg(pos);
        thoiGianThanhToan.docFile(in);
    } else {
        in.seekg(pos);
        time_t thanhToan;
        in.read(reinterpret_cast<char *>(&thanhToan), sizeof(thanhToan));
        if (thanhToan != 0)
            thoiGianThanhToan = NgayGio(thanhToan);
        else
            thoiGianThanhToan = NgayGio(); // Empty
    }
}
