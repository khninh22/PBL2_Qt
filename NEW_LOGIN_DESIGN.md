# 📸 HƯỚNG DẪN THAY THẾ ẢNH NỀN

## ✅ ĐÃ HOÀN THÀNH:

- ✅ Thiết kế lại giao diện đăng nhập hoàn toàn
- ✅ Form login đơn giản, nền đen, đè lên giữa ảnh
- ✅ Có nút mắt (👁/🙈) để hiện/ẩn mật khẩu
- ✅ Bố cục: Đăng Nhập → Vai trò → Tài khoản → Mật khẩu → Nút Đăng Nhập
- ✅ Bỏ nút thoát
- ✅ Bỏ dấu tích (✅) trước "Đăng Nhập"
- ✅ Kích thước 1200x700 phù hợp với ảnh nền

## 📸 CẦN LÀM: THAY ẢNH NỀN

### Bước 1: Lưu Ảnh Từ Attachments

1. Mở ảnh cầu thủ đá bóng đã gửi (trong attachments)
2. Click chuột phải → **Save Image As...**
3. Đặt tên: `login_bg.jpg`

### Bước 2: Thay Thế File

```powershell
# Xóa ảnh cũ (nếu có)
del d:\PBL2_GUI\resources\login_bg.jpg

# Copy ảnh mới vào
copy "đường_dẫn_ảnh_mới\login_bg.jpg" "d:\PBL2_GUI\resources\login_bg.jpg"
```

HOẶC thủ công:

1. Mở thư mục `d:\PBL2_GUI\resources\`
2. Xóa file `login_bg.jpg` cũ
3. Copy ảnh mới vào với tên `login_bg.jpg`

### Bước 3: Chạy Lại

```powershell
.\build\Desktop_Qt_6_9_3_MinGW_64_bit-Debug\bin\QuanLyThueSanQt.exe
```

## 🎨 THIẾT KẾ MỚI

### Giao diện:

```
┌─────────────────────────────────────────┐
│                                         │
│  [Ảnh nền sân bóng toàn màn hình]     │
│                                         │
│         ┌──────────────┐                │
│         │  [Nền đen]   │                │
│         │              │                │
│         │ ĐĂNG NHẬP    │                │
│         │              │                │
│         │ Vai trò      │                │
│         │ [ADMIN ▼]    │                │
│         │              │                │
│         │ Mật khẩu     │                │
│         │ [_______] 👁 │                │
│         │              │                │
│         │ [Đăng Nhập]  │                │
│         └──────────────┘                │
│                                         │
└─────────────────────────────────────────┘
```

### Đặc điểm:

- ✅ Form đen đè lên giữa ảnh nền
- ✅ Ảnh nền hiển thị nguyên bản (không bị crop/méo)
- ✅ Nút mắt bên phải field mật khẩu
- ✅ Click → Hiện/ẩn mật khẩu
- ✅ Giao diện tối giản, hiện đại
- ✅ Không có nút thoát
- ✅ Text "Đăng Nhập" không có icon

---

**🎯 Kết quả:** Màn hình đăng nhập đẹp, chuyên nghiệp với ảnh nền sân bóng!
