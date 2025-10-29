# 🚨 HƯỚNG DẪN: THÊM ẢNH NỀN ĐĂNG NHẬP

## ⚡ THỰC HIỆN NGAY BÂY GIỜ!

### Bước 1: Lưu Ảnh Sân Bóng

Ảnh cầu thủ đá bóng (trong sân vận động) đã gửi cần được lưu vào:

```
d:\PBL2_GUI\resources\login_bg.jpg
```

### Bước 2: Cách Lưu Ảnh

#### Cách 1: Từ Chat/Prompt

1. Tìm ảnh sân bóng đã gửi (cầu thủ mặc áo xanh đá bóng)
2. Click chuột phải vào ảnh
3. Chọn "Save Image As..." / "Lưu ảnh dưới dạng..."
4. Đặt tên: `login_bg.jpg`
5. Lưu vào: `d:\PBL2_GUI\resources\`

#### Cách 2: Copy File

```powershell
# Nếu đã tải ảnh ở đâu đó
copy "đường_dẫn_ảnh_gốc.jpg" "d:\PBL2_GUI\resources\login_bg.jpg"
```

### Bước 3: Kiểm Tra

```powershell
cd d:\PBL2_GUI\resources
dir login_bg.jpg
```

**Kết quả mong đợi:**

```
login_bg.jpg    [kích thước file]
```

✅ Nếu thấy file → Thành công!  
❌ Nếu không thấy → Làm lại Bước 1

### Bước 4: Chạy Lại Ứng Dụng

```powershell
cd d:\PBL2_GUI

# Build lại (nếu cần)
cmake --build build/Desktop_Qt_6_9_3_MinGW_64_bit-Debug

# Chạy
.\build\Desktop_Qt_6_9_3_MinGW_64_bit-Debug\bin\QuanLyThueSanQt.exe
```

## 🎯 Thông Số Ảnh

- **Tên file**: `login_bg.jpg` (CHÍNH XÁC)
- **Vị trí**: `d:\PBL2_GUI\resources\` (CHÍNH XÁC)
- **Định dạng**: JPG hoặc PNG
- **Kích thước**: Tùy ý (hệ thống tự scale)

## 💡 Mô Tả Ảnh Cần Dùng

Ảnh đã gửi trong prompt có:

- ⚽ Cầu thủ mặc áo xanh đang đá bóng
- 🏟️ Sân vận động lớn ở phía sau
- 🌥️ Bầu trời kịch tính với mây
- 💡 Ánh sáng đẹp từ đèn sân
- 🟢 Sân cỏ xanh tươi

→ **ĐÂY LÀ ẢNH HOÀN HẢO CHO MÀN HÌNH ĐĂNG NHẬP!**

## 🔧 Nếu Không Tìm Thấy Ảnh Gốc

### Tùy chọn 1: Tải Ảnh Tương Tự

- **Unsplash**: https://unsplash.com/s/photos/soccer-player-kicking
- **Pexels**: https://www.pexels.com/search/football-action/
- **Pixabay**: https://pixabay.com/images/search/football-player-stadium/

### Tùy chọn 2: Dùng Gradient (Không Đẹp Bằng)

Nếu không có ảnh, hệ thống tự động dùng gradient xanh.
Nhưng **có ảnh sẽ ĐẸP HƠN NHIỀU!**

## ✅ Checklist

- [ ] Đã lưu ảnh vào `resources/login_bg.jpg`
- [ ] Đã kiểm tra file tồn tại với `dir`
- [ ] Đã chạy lại ứng dụng
- [ ] Màn hình đăng nhập hiển thị ảnh nền đẹp

## 🆘 Xử Lý Lỗi

### Lỗi: "Không thấy ảnh nền"

```powershell
# Kiểm tra đường dẫn
cd d:\PBL2_GUI\resources
dir

# Phải thấy: login_bg.jpg
```

### Lỗi: "Ảnh bị mờ/vỡ"

- Tải ảnh với độ phân giải cao hơn
- Đề xuất: 1920x1080 trở lên

### Lỗi: "File không load"

- Đổi tên chính xác: `login_bg.jpg` (không phải Login_bg.jpg hoặc loginbg.jpg)
- Hoặc dùng `login_bg.png` nếu là PNG

---

## 📞 Tóm Tắt

**BẮT BUỘC:**

1. Lưu ảnh sân bóng → `d:\PBL2_GUI\resources\login_bg.jpg`
2. Chạy lại ứng dụng
3. Kiểm tra màn hình đăng nhập

**KẾT QUẢ:**

- Màn hình đăng nhập đẹp với ảnh nền sân bóng
- Form đăng nhập hiện rõ trên nền mờ
- Chuyên nghiệp và hấp dẫn! ⚽🎯
