# 🔐 Hướng Dẫn Hệ Thống Đăng Nhập

## 📋 Tổng Quan

Hệ thống quản lý thuê sân bóng yêu cầu **đăng nhập** trước khi sử dụng, với **2 vai trò** khác nhau:

### 👑 ADMIN (Chủ Sân)

- **Quyền hạn**: Toàn quyền quản lý tất cả chức năng
- **Mật khẩu mặc định**: `admin123`
- **Tài khoản**: Không cần (chỉ cần mật khẩu)

### 👔 NHÂN VIÊN (NVSTT)

- **Quyền hạn**: Bị hạn chế một số chức năng
- **Tài khoản/Mật khẩu**: Do Admin quản lý và cấp
- **Hạn chế**:
  - ❌ Không thể xóa Sân bóng
  - ❌ Không thể xóa Khách hàng
  - ❌ Không thể xóa Dịch vụ
  - ❌ Không thể xem/quản lý tab Nhân viên
  - ✅ Có thể thêm/sửa các đối tượng
  - ✅ Có thể đặt sân, thanh toán
  - ✅ Có thể xem thống kê cơ bản

## 🚀 Quy Trình Đăng Nhập

### Bước 1: Chạy Ứng Dụng

```bash
./bin/QuanLyThueSanQt.exe
```

### Bước 2: Màn Hình Đăng Nhập

Màn hình đăng nhập sẽ hiển thị với:

- 🎨 Ảnh nền sân bóng đẹp mắt
- 🎯 Form đăng nhập rõ ràng
- 📝 Hướng dẫn chi tiết

### Bước 3: Chọn Vai Trò

**Đăng nhập với ADMIN:**

1. Chọn vai trò: `🔑 ADMIN (Chủ Sân)`
2. Nhập mật khẩu: `admin123`
3. Nhấn `✅ Đăng Nhập`

**Đăng nhập với NHÂN VIÊN:**

1. Chọn vai trò: `👔 NHÂN VIÊN`
2. Nhập tài khoản: (do Admin cấp)
3. Nhập mật khẩu: (do Admin cấp)
4. Nhấn `✅ Đăng Nhập`

## 🔧 Quản Lý Tài Khoản Nhân Viên

### Tạo Tài Khoản Nhân Viên (Chỉ Admin)

1. Đăng nhập với quyền Admin
2. Vào tab **👔 Nhân Viên**
3. Nhấn **➕ Thêm Nhân Viên**
4. Điền đầy đủ thông tin:
   - 📋 Mã Nhân Viên
   - 👤 Họ Tên
   - 📞 Số Điện Thoại
   - 💼 Vị Trí
   - 💰 Lương Cơ Bản
   - **📧 Tài Khoản** ← Dùng để đăng nhập
   - **🔒 Mật Khẩu** ← Dùng để đăng nhập
5. Nhấn **OK** để lưu

### Sửa Tài Khoản/Mật Khẩu

1. Chọn nhân viên cần sửa
2. Nhấn **✏️ Sửa**
3. Cập nhật tài khoản/mật khẩu mới
4. Nhấn **OK**

### Xóa Nhân Viên

1. Chọn nhân viên cần xóa
2. Nhấn **❌ Xóa**
3. Xác nhận xóa

⚠️ **Lưu ý**: Nhân viên đã nghỉ việc sẽ không thể đăng nhập!

## 🔑 Đổi Mật Khẩu Admin

1. Đăng nhập với quyền Admin
2. Vào menu **⚙️ Hệ Thống**
3. Chọn **🔑 Đổi Mật Khẩu**
4. Nhập:
   - Mật khẩu hiện tại
   - Mật khẩu mới
   - Xác nhận mật khẩu mới
5. Nhấn **OK**

## 🚪 Đăng Xuất

1. Vào menu **⚙️ Hệ Thống**
2. Chọn **🚪 Đăng Xuất**
3. Xác nhận đăng xuất
4. Ứng dụng sẽ đóng và cần đăng nhập lại

## 📊 So Sánh Quyền Hạn

| Chức năng          | Admin | Nhân viên |
| ------------------ | ----- | --------- |
| Thêm Sân bóng      | ✅    | ✅        |
| Sửa Sân bóng       | ✅    | ✅        |
| Xóa Sân bóng       | ✅    | ❌        |
| Thêm Khách hàng    | ✅    | ✅        |
| Sửa Khách hàng     | ✅    | ✅        |
| Xóa Khách hàng     | ✅    | ❌        |
| Đặt sân            | ✅    | ✅        |
| Hủy lịch           | ✅    | ✅        |
| Thanh toán         | ✅    | ✅        |
| Thêm Dịch vụ       | ✅    | ✅        |
| Sửa Dịch vụ        | ✅    | ✅        |
| Xóa Dịch vụ        | ✅    | ❌        |
| Quản lý Nhân viên  | ✅    | ❌        |
| Xem Thống kê       | ✅    | ✅        |
| Đổi mật khẩu Admin | ✅    | ❌        |

## 🔒 Bảo Mật

### Lưu Trữ Dữ Liệu

- Mật khẩu Admin: `data/admin.dat` (Binary)
- Thông tin Nhân viên: `data/nhanvien.dat` (Binary)
- Tài khoản/mật khẩu nhân viên: Lưu trong object NhanVien

### Khuyến Nghị Bảo Mật

✅ Đổi mật khẩu Admin sau lần đầu đăng nhập  
✅ Đặt mật khẩu mạnh cho nhân viên  
✅ Định kỳ thay đổi mật khẩu  
✅ Không chia sẻ tài khoản Admin  
✅ Xóa tài khoản nhân viên đã nghỉ việc

## 🐛 Xử Lý Lỗi

### Quên Mật Khẩu Admin

**Giải pháp**: Xóa file `data/admin.dat` để reset về mật khẩu mặc định `admin123`

### Nhân Viên Không Đăng Nhập Được

**Kiểm tra**:

- ✓ Tài khoản/mật khẩu có đúng không?
- ✓ Nhân viên còn làm việc không (trường `dangLam`)?
- ✓ Admin đã tạo tài khoản chưa?

### Mất Dữ Liệu Đăng Nhập

**Giải pháp**: Đăng nhập Admin → Vào tab Nhân viên → Tạo lại tài khoản

## 📸 Ảnh Nền Đăng Nhập

Ảnh nền được lưu tại: `resources/login_bg.jpg` hoặc `resources/login_bg.png`

**Yêu cầu**:

- Định dạng: JPG, PNG
- Kích thước đề xuất: 1920x1080 hoặc tương đương
- Nội dung: Ảnh sân bóng đẹp mắt

**Nếu không có ảnh**: Hệ thống sẽ hiển thị gradient màu xanh làm nền thay thế.

## 💡 Tips

1. **Admin lần đầu**: Đăng nhập → Đổi mật khẩu ngay lập tức
2. **Tạo nhân viên mẫu**: Tạo 1-2 tài khoản nhân viên để test
3. **Ghi chép**: Lưu danh sách tài khoản nhân viên ở nơi an toàn
4. **Backup**: Sao lưu thường xuyên thư mục `data/`

---

**Chúc bạn sử dụng hệ thống hiệu quả! ⚽**
