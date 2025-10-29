# 🎉 CẬP NHẬT: HỆ THỐNG ĐĂNG NHẬP ĐÃ HOÀN THÀNH!

## ✨ Tính Năng Mới

### 🔐 Hệ Thống Đăng Nhập

- ✅ Màn hình đăng nhập đẹp mắt với ảnh nền sân bóng
- ✅ 2 vai trò: **ADMIN** (Chủ) và **NHÂN VIÊN**
- ✅ Kiểm soát quyền truy cập chi tiết
- ✅ Quản lý tài khoản nhân viên
- ✅ Đổi mật khẩu Admin
- ✅ Đăng xuất an toàn

## 📋 Các File Đã Tạo/Cập Nhật

### Core (Nghiệp vụ)

1. **`core/AuthManager.h/cpp`** - Quản lý xác thực

   - Xử lý đăng nhập Admin/Nhân viên
   - Kiểm tra quyền truy cập
   - Lưu/load mật khẩu Admin

2. **`core/NhanVien.h/cpp`** - Cập nhật

   - Thêm trường: `taiKhoan`, `matKhau`
   - Hỗ trợ serialization tài khoản

3. **`core/QuanLyThueSan.h/cpp`** - Cập nhật
   - Thêm tham số tài khoản/mật khẩu vào `themNhanVien()`, `suaNhanVien()`

### UI (Giao diện)

4. **`ui/LoginDialog.h/cpp`** - Màn hình đăng nhập

   - Form đăng nhập với ảnh nền
   - Chọn vai trò (Admin/Nhân viên)
   - Xác thực và hiển thị lỗi

5. **`ui/MainWindow.h/cpp`** - Cập nhật

   - Nhận `AuthManager` từ LoginDialog
   - Áp dụng kiểm soát quyền (`applyAccessControl()`)
   - Thêm menu Đăng xuất, Đổi mật khẩu
   - Hiển thị thông tin người dùng trên StatusBar

6. **`ui/NhanVienDialog.h/cpp`** - Cập nhật
   - Thêm field Tài khoản và Mật khẩu
   - Hiển thị mật khẩu dạng password (\*)

### Khác

7. **`main.cpp`** - Cập nhật

   - Hiển thị LoginDialog trước MainWindow
   - Kiểm tra đăng nhập thành công

8. **`CMakeLists.txt`** - Cập nhật

   - Thêm `AuthManager.cpp`, `LoginDialog.cpp`

9. **`LOGIN_GUIDE.md`** - Hướng dẫn chi tiết
10. **`resources/README.md`** - Hướng dẫn ảnh nền
11. **`CHANGELOG.md`** - File này

## 🚀 Cách Sử Dụng

### 1️⃣ Lần Đầu Chạy (Admin)

```bash
# Build và chạy
cmake --build build/Desktop_Qt_6_9_3_MinGW_64_bit-Debug
./build/Desktop_Qt_6_9_3_MinGW_64_bit-Debug/bin/QuanLyThueSanQt.exe
```

**Đăng nhập:**

- Vai trò: 🔑 ADMIN (Chủ Sân)
- Mật khẩu: `admin123`

### 2️⃣ Tạo Tài Khoản Nhân Viên

1. Vào tab **👔 Nhân Viên**
2. Nhấn **➕ Thêm Nhân Viên**
3. Điền thông tin:
   - Mã NV: NV001
   - Họ tên: Nguyễn Văn A
   - SĐT: 0123456789
   - Vị trí: Thu Ngan
   - Lương: 5000000
   - **Tài khoản: nva** ← Quan trọng!
   - **Mật khẩu: 123456** ← Quan trọng!
4. Nhấn OK

### 3️⃣ Test Đăng Nhập Nhân Viên

1. Đăng xuất (Menu → Đăng Xuất)
2. Đăng nhập lại:
   - Vai trò: 👔 NHÂN VIÊN
   - Tài khoản: nva
   - Mật khẩu: 123456

### 4️⃣ Kiểm Tra Phân Quyền

**Với nhân viên, bạn sẽ thấy:**

- ❌ Tab "Nhân viên" bị ẩn hoàn toàn
- ❌ Các nút "Xóa" bị vô hiệu hóa (xám)
- ✅ Có thể Thêm/Sửa các đối tượng
- ✅ Có thể Đặt sân, Thanh toán
- ⚠️ StatusBar hiển thị: "Bạn đang đăng nhập với quyền hạn chế (NHÂN VIÊN)"

## 🎨 Tùy Chỉnh Ảnh Nền

### Cách 1: Sử dụng Ảnh Đã Cung Cấp

1. Lưu ảnh sân bóng (từ prompt) vào: `resources/login_bg.jpg`
2. Chạy lại ứng dụng

### Cách 2: Tải Ảnh Từ Internet

```bash
# Vào thư mục resources
cd resources

# Tải ảnh (hoặc copy thủ công)
# Đổi tên thành login_bg.jpg hoặc login_bg.png
```

### Cách 3: Không Dùng Ảnh

- Nếu không có ảnh, hệ thống tự động hiển thị gradient xanh đẹp mắt

## 🔒 Bảo Mật

### Dữ Liệu Lưu Trữ

```
data/
├── admin.dat           ← Mật khẩu Admin (Binary)
├── nhanvien.dat        ← Thông tin NV + TK/MK (Binary)
├── sanbong.dat
├── khachhang.dat
└── ...
```

### Khuyến Nghị

✅ Đổi mật khẩu Admin sau lần đầu  
✅ Đặt mật khẩu mạnh cho nhân viên  
✅ Backup thư mục `data/` thường xuyên  
✅ Không chia sẻ tài khoản Admin

## 🐛 Xử Lý Lỗi

### Lỗi: "Quên mật khẩu Admin"

```bash
# Xóa file để reset về mật khẩu mặc định
rm data/admin.dat
```

### Lỗi: "Nhân viên không đăng nhập được"

**Kiểm tra:**

1. Tài khoản/mật khẩu đã đúng?
2. Nhân viên còn làm việc? (trường `dangLam` = true)
3. Admin đã tạo tài khoản?

### Lỗi: "Không thấy ảnh nền"

**Giải pháp:**

1. Kiểm tra file có tồn tại: `resources/login_bg.jpg`
2. Kiểm tra định dạng: JPG hoặc PNG
3. Nếu không có, hệ thống dùng gradient thay thế

## 📊 Thay Đổi Chi Tiết

### Database Schema

```cpp
// NhanVien - Thêm 2 trường mới
class NhanVien {
    string maNV;
    string hoTen;
    string sdt;
    string viTri;
    double luongCoBan;
    bool dangLam;
    string taiKhoan;   // ← MỚI
    string matKhau;    // ← MỚI
};
```

### Phân Quyền

```cpp
enum UserRole {
    ROLE_NONE = 0,
    ROLE_ADMIN = 1,     // Toàn quyền
    ROLE_EMPLOYEE = 2   // Hạn chế
};
```

## 📖 Tài Liệu

Xem thêm chi tiết tại: **`LOGIN_GUIDE.md`**

## ✅ Checklist Hoàn Thành

- [x] Tạo lớp AuthManager
- [x] Cập nhật NhanVien với tài khoản/mật khẩu
- [x] Tạo LoginDialog với ảnh nền
- [x] Cập nhật MainWindow với phân quyền
- [x] Cập nhật NhanVienDialog
- [x] Cập nhật main.cpp
- [x] Cập nhật CMakeLists.txt
- [x] Build thành công
- [x] Viết tài liệu

## 🎯 Test Case

### TC1: Đăng Nhập Admin

1. Chạy ứng dụng
2. Chọn vai trò: ADMIN
3. Nhập mật khẩu: admin123
4. Kết quả: ✅ Đăng nhập thành công, có toàn quyền

### TC2: Đăng Nhập Nhân Viên

1. Admin tạo tài khoản NV (nva/123456)
2. Đăng xuất
3. Chọn vai trò: NHÂN VIÊN
4. Nhập: nva / 123456
5. Kết quả: ✅ Đăng nhập thành công, bị hạn chế quyền

### TC3: Đổi Mật Khẩu Admin

1. Đăng nhập Admin
2. Menu → Đổi Mật Khẩu
3. Nhập: admin123 → newpass123 → newpass123
4. Kết quả: ✅ Đổi thành công
5. Đăng xuất và đăng nhập lại với newpass123

### TC4: Kiểm Tra Phân Quyền

1. Đăng nhập Nhân viên
2. Kiểm tra:
   - ❌ Tab Nhân viên bị ẩn
   - ❌ Nút Xóa bị disable
   - ✅ Có thể Thêm/Sửa
   - ✅ Có thể Đặt sân

## 🚀 Next Steps (Tương Lai)

- [ ] Thêm log lịch sử đăng nhập
- [ ] Thêm chức năng "Quên mật khẩu"
- [ ] Thêm xác thực 2 lớp (2FA)
- [ ] Mã hóa mật khẩu (MD5/SHA256)
- [ ] Thêm session timeout
- [ ] Thêm quyền chi tiết hơn (RBAC)

## 📞 Hỗ Trợ

Nếu gặp vấn đề, hãy:

1. Đọc `LOGIN_GUIDE.md`
2. Kiểm tra console output
3. Kiểm tra file `data/*.dat`
4. Reset mật khẩu Admin nếu cần

---

**🎉 Chúc mừng! Hệ thống đăng nhập đã sẵn sàng sử dụng!**

_Phiên bản: 2.0 (GUI Edition with Authentication)_  
_Ngày cập nhật: 27/10/2025_
