# 📍 Vị Trí Dữ Liệu

## ✅ Dữ Liệu Được Lưu Ở Đâu?

**Đáp án:** `build/bin/data/`

```
d:\PBL2_GUI\
└── build\
    └── bin\
        ├── QuanLyThueSanQt.exe  ← Executable
        └── data\                 ← Thư mục dữ liệu DUY NHẤT
            ├── sanbong.dat
            ├── khachhang.dat
            ├── lichdatsan.dat
            ├── dichvu.dat
            ├── chitietdichvu.dat
            └── nhanvien.dat
```

## ⚙️ Tại Sao Không Có data/ Ở Root?

**Trước đây có vấn đề:**
- Khi chạy từ VS Code, working directory = `d:\PBL2_GUI\` (root)
- Khi chạy trực tiếp .exe, working directory = `d:\PBL2_GUI\build\bin\`
- → Dữ liệu bị tạo ở 2 nơi khác nhau → **Nhầm lẫn**

**Giải pháp:**
Thêm code trong `main.cpp` để luôn set working directory về thư mục chứa executable:

```cpp
// Trong main.cpp
QDir::setCurrent(QCoreApplication::applicationDirPath());
```

→ Bây giờ dù chạy từ đâu, dữ liệu luôn ở `build/bin/data/` ✅

## 🎯 Kết Quả

| Trường Hợp | Working Directory | Đọc/Ghi Data Từ |
|------------|-------------------|-----------------|
| Chạy từ VS Code | `build/bin/` | `build/bin/data/` ✅ |
| Chạy .exe trực tiếp | `build/bin/` | `build/bin/data/` ✅ |
| Debug trong Qt Creator | `build/bin/` | `build/bin/data/` ✅ |

## 📝 Lưu Ý

- ✅ **Chỉ có 1 thư mục data duy nhất:** `build/bin/data/`
- ❌ **Không còn** `d:\PBL2_GUI\data\` (đã xóa)
- ✅ CMakeLists.txt tự động tạo thư mục `data/` khi build

## 🔍 Kiểm Tra

Nếu muốn xem dữ liệu hiện tại:
```bash
dir build\bin\data\*.dat
```

Xem chi tiết trong: `build/bin/data/README.md`
