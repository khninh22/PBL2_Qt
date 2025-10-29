# Hệ Thống Quản Lý Thuê Sân Bóng - GUI Edition

Phiên bản GUI của hệ thống quản lý thuê sân bóng, được phát triển bằng Qt Framework.

## 🎯 Tính Năng

### ⚽ Quản Lý Sân Bóng

- ✅ Thêm, sửa, xóa sân bóng
- ✅ Quản lý trạng thái bảo trì
- ✅ Phân loại sân: 5, 7, 11 người
- ✅ Quản lý giá thuê theo loại sân

### 👥 Quản Lý Khách Hàng

- ✅ Thêm, sửa, xóa khách hàng
- ✅ Hệ thống điểm tích lũy
- ✅ Cấp độ thành viên (Đồng, Bạc, Vàng, Kim Cương)
- ✅ Giảm giá theo cấp độ

### 📅 Quản Lý Lịch Đặt Sân

- ✅ Đặt sân với kiểm tra trùng lịch
- ✅ Tìm sân trống theo khung giờ
- ✅ Hủy lịch đặt
- ✅ Thanh toán tự động tích điểm

### 🍔 Quản Lý Dịch Vụ

- ✅ Thêm, sửa, xóa dịch vụ (đồ ăn, đồ uống)
- ✅ Quản lý giá và tồn kho
- ✅ Thêm dịch vụ vào lịch đặt

### 👔 Quản Lý Nhân Viên

- ✅ Thêm, sửa, xóa nhân viên
- ✅ Quản lý vị trí và lương
- ✅ Trạng thái làm việc

### 📊 Thống Kê

- ✅ Doanh thu theo ngày/tháng
- ✅ Tổng số sân và khách hàng
- ✅ Báo cáo trực quan

## 🏗️ Cấu Trúc Dự Án

```
PBL2_GUI/
│
├── core/                      # Lớp Core (OOP + DSA)
│   ├── MangDong.h            # Template mảng động
│   ├── KhachHang.h/cpp       # Lớp Khách hàng
│   ├── SanBong.h/cpp         # Lớp Sân bóng
│   ├── LichDatSan.h/cpp      # Lớp Lịch đặt sân
│   ├── DichVu.h/cpp          # Lớp Dịch vụ
│   ├── ChiTietDichVu.h/cpp   # Chi tiết dịch vụ
│   ├── NhanVien.h/cpp        # Lớp Nhân viên
│   └── QuanLyThueSan.h/cpp   # Lớp quản lý chính
│
├── ui/                        # Lớp UI (Qt Widgets)
│   ├── MainWindow.h/cpp       # Cửa sổ chính
│   ├── SanBongDialog.h/cpp    # Dialog sân bóng
│   ├── KhachHangDialog.h/cpp  # Dialog khách hàng
│   ├── DatSanDialog.h/cpp     # Dialog đặt sân
│   ├── DichVuDialog.h/cpp     # Dialog dịch vụ
│   └── NhanVienDialog.h/cpp   # Dialog nhân viên
│
├── data/                      # Dữ liệu (Binary files)
│   ├── sanbong.dat
│   ├── khachhang.dat
│   ├── lichdatsan.dat
│   ├── dichvu.dat
│   ├── chitietdichvu.dat
│   └── nhanvien.dat
│
├── CMakeLists.txt             # CMake configuration
├── main.cpp                   # Entry point
└── README.md                  # This file
```

## 📋 Yêu Cầu Hệ Thống

- **Qt Framework**: 6.x trở lên
- **CMake**: 3.16 trở lên
- **C++ Compiler**: Hỗ trợ C++17
- **Qt Creator** (khuyến nghị)

## 🚀 Hướng Dẫn Build

### Sử dụng Qt Creator (Đơn giản nhất)

1. Mở Qt Creator
2. File → Open File or Project
3. Chọn file `CMakeLists.txt`
4. Cấu hình Kit (chọn compiler và Qt version)
5. Nhấn Build (Ctrl+B)
6. Nhấn Run (Ctrl+R)

### Sử dụng Command Line

```bash
# Tạo thư mục build
mkdir build
cd build

# Configure với CMake
cmake ..

# Build
cmake --build .

# Run (Windows)
bin\QuanLyThueSanQt.exe

# Run (Linux/Mac)
./bin/QuanLyThueSanQt
```

### Sử dụng Visual Studio (Windows)

1. Mở Visual Studio
2. File → Open → CMake
3. Chọn file `CMakeLists.txt`
4. Build → Build All
5. Debug → Start Debugging (F5)

## 🎨 Giao Diện

### Màu Sắc Chủ Đạo

- **Xanh lá (Primary)**: #4CAF50 - Nút chính, header
- **Đỏ (Danger)**: #f44336 - Nút xóa
- **Xanh dương**: #2196F3 - Thông tin
- **Cam**: #FF9800 - Cảnh báo
- **Tím**: #9C27B0 - Điểm nhấn

### Đặc Điểm UI

- ✅ Giao diện hiện đại, dễ sử dụng
- ✅ Biểu tượng emoji trực quan
- ✅ Màu sắc hài hòa, dễ nhìn
- ✅ Responsive layout
- ✅ Material Design inspired

## 💾 Lưu Trữ Dữ Liệu

Dự án sử dụng **Binary Serialization** để lưu trữ dữ liệu:

- Hiệu suất cao
- Bảo mật tốt
- Tiết kiệm dung lượng
- Mỗi lớp tự quản lý serialize/deserialize

## 📚 Kiến Trúc

### Tách Biệt Core và UI

**Core Layer** (OOP + DSA):

- Chứa toàn bộ logic nghiệp vụ
- Không phụ thuộc vào Qt
- Có thể tái sử dụng cho Console hoặc Web

**UI Layer** (Qt Widgets):

- Chỉ xử lý hiển thị và tương tác
- Gọi các phương thức từ Core
- Không chứa logic nghiệp vụ

### Design Patterns

- **MVC Pattern**: Tách Model (Core) và View (UI)
- **Dialog Pattern**: Sử dụng Dialog cho các form nhập liệu
- **Observer Pattern**: Qt Signal/Slot mechanism

## 🔧 Các Chức Năng Nâng Cao

### Giá Theo Khung Giờ

- **Ban ngày** (06:00-17:00): Giá thường
- **Ban tối** (17:00-22:00): Giá cao hơn

### Hệ Thống Điểm Thưởng

- Mỗi 10,000 VNĐ = 1 điểm
- 100 điểm = 50,000 VNĐ voucher
- Tự động nâng cấp theo điểm

### Giảm Giá Theo Cấp Độ

- **Đồng**: 0%
- **Bạc** (≥200 điểm): 5%
- **Vàng** (≥500 điểm): 10%
- **Kim Cương** (≥1000 điểm): 15%

## 🐛 Debug và Testing

### Kiểm tra dữ liệu

File `.dat` là binary, không đọc được bằng text editor.
Dùng chức năng "Xem" trong ứng dụng.

### Xóa dữ liệu

Xóa tất cả file `.dat` trong thư mục `data/` để reset.

### Log errors

Xem Console output trong Qt Creator khi chạy.

## 📖 Tài Liệu Tham Khảo

- [Qt Documentation](https://doc.qt.io/)
- [C++ Reference](https://en.cppreference.com/)
- [CMake Documentation](https://cmake.org/documentation/)

## 👨‍💻 Phát Triển

### Thêm Tính Năng Mới

1. Thêm logic vào lớp Core (nếu cần)
2. Tạo UI tương ứng trong lớp UI
3. Kết nối Signal/Slot
4. Test và debug

### Best Practices

- Luôn tách logic Core và UI
- Sử dụng const reference cho hiệu suất
- Kiểm tra nullptr trước khi dereference
- Lưu dữ liệu sau mỗi thao tác quan trọng

## 📝 License

Dự án học tập - OOP + DSA Project

## 🎓 Ghi Chú

Đây là dự án OOP với DSA, tập trung vào:

- **OOP**: Inheritance, Encapsulation, Polymorphism
- **DSA**: Mảng động tự cài đặt, Sorting, Searching
- **GUI**: Qt Widgets, Signal/Slot, Layout Management
- **File I/O**: Binary Serialization

---

**Chúc bạn thành công! 🎉**
