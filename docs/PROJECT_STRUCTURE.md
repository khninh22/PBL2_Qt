# 📁 Cấu Trúc Thư Mục Project

## 🎯 Nguyên Tắc Tổ Chức

**Chỉ lưu data và resources trong `build/bin/`**

- ✅ Dữ liệu và tài nguyên chỉ nằm trong thư mục executable
- ✅ Root project chỉ chứa source code và config
- ✅ Tránh nhầm lẫn khi chạy từ nhiều nơi khác nhau

## 📂 Cấu Trúc Root

```
d:\PBL2_GUI\
├── .git/                 # Git repository
├── .gitignore            # Git ignore rules
├── .vscode/              # VS Code settings
├── build/                # Build output
│   └── bin/              # ⭐ EXECUTABLE VÀ DATA
│       ├── QuanLyThueSanQt.exe
│       ├── data/         # 💾 Dữ liệu (.dat files)
│       └── resources/    # 🖼️ Hình ảnh, assets
├── core/                 # ⚙️ Core logic
│   ├── *.h/.cpp          # Domain classes
│   └── MangDong.h        # Template classes
├── ui/                   # 🖥️ UI components
│   └── *Dialog.h/.cpp    # Qt dialogs
├── docs/                 # 📚 Documentation
│   └── *.md              # All docs here
├── CMakeLists.txt        # Build configuration
├── CMakeLists.txt.user   # Qt Creator settings
├── main.cpp              # Entry point
├── README.md             # Main readme
└── CHANGELOG.md          # Version history
```

## 🎯 Thư Mục Quan Trọng

### 1️⃣ `build/bin/` - Thư Mục Thực Thi

**Đây là nơi duy nhất chứa:**

- ✅ Executable (`.exe`)
- ✅ Dữ liệu (`data/*.dat`)
- ✅ Tài nguyên (`resources/*.jpg`)

**Working directory được set tự động:**

```cpp
// Trong main.cpp
QDir::setCurrent(QCoreApplication::applicationDirPath());
```

### 2️⃣ `core/` - Logic Nghiệp Vụ

Chứa tất cả logic xử lý:

- Domain classes (SanBong, KhachHang, LichDatSan, DichVu, NhanVien)
- Manager class (QuanLyThueSan, AuthManager)
- Data structures (MangDong, HashTable)
- Utilities (Validator, Exceptions)

### 3️⃣ `ui/` - Giao Diện

Chứa tất cả Qt dialogs và windows:

- MainWindow
- LoginDialog
- SanBongDialog
- KhachHangDialog
- DatSanDialog
- SanBookingDialog
- DichVuDialog
- NhanVienDialog

### 4️⃣ `docs/` - Tài Liệu

**Tất cả file .md phải nằm ở đây:**

- ✅ Feature guides
- ✅ Fix logs
- ✅ Improvements
- ✅ Analysis docs

## ⚠️ QUY TẮC QUAN TRỌNG

### ❌ KHÔNG BAO GIỜ tạo ở root:

1. ❌ `data/` ở root → CHỈ trong `build/bin/data/`
2. ❌ `resources/` ở root → CHỈ trong `build/bin/resources/`
3. ❌ File `.md` rời rạc → CHỈ trong `docs/`
4. ❌ File `.txt` config → Tích hợp vào code hoặc vào docs

### ✅ Ngoại lệ được phép ở root:

- ✅ `README.md` - Main project readme
- ✅ `CHANGELOG.md` - Version history
- ✅ `.gitignore` - Git rules
- ✅ `CMakeLists.txt` - Build config
- ✅ `main.cpp` - Entry point

## 🔧 CMakeLists.txt Auto-Create

Build system tự động tạo thư mục cần thiết:

```cmake
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/data"
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
    COMMENT "Creating data and resources directories in bin folder"
)
```

## 📊 So Sánh Trước & Sau

### ❌ Trước (Lộn xộn):

```
d:\PBL2_GUI\
├── data/                    ❌ Duplicate
├── resources/               ❌ Duplicate
├── DATAMANAGER_UPDATE.md    ❌ Rời rạc
├── DATA_LOCATION.md         ❌ Rời rạc
├── QUICK_UPDATE.txt         ❌ Không rõ ràng
├── LOGIN_GUIDE.md           ❌ Nên ở docs
├── NEW_LOGIN_DESIGN.md      ❌ Nên ở docs
└── build/bin/
    ├── data/                ✅ OK
    └── resources/           ✅ OK
```

### ✅ Sau (Gọn gàng):

```
d:\PBL2_GUI\
├── core/                    ✅ Source code
├── ui/                      ✅ UI code
├── docs/                    ✅ All docs
├── README.md                ✅ Main readme
├── CHANGELOG.md             ✅ History
├── CMakeLists.txt           ✅ Build
└── build/bin/               ✅ Executable + Data
    ├── QuanLyThueSanQt.exe
    ├── data/                ✅ Only here
    └── resources/           ✅ Only here
```

## 🚀 Lợi Ích

1. **Rõ ràng**: Biết ngay dữ liệu ở đâu
2. **Không duplicate**: Chỉ 1 nơi duy nhất
3. **Dễ deploy**: Copy cả thư mục `bin/`
4. **Dễ backup**: Backup `bin/data/`
5. **Clean root**: Root chỉ có source code

## 📝 Khi Thêm File Mới

- **Code .cpp/.h** → `core/` hoặc `ui/`
- **Documentation .md** → `docs/`
- **Data files** → Tự động trong `build/bin/data/`
- **Images** → `build/bin/resources/`

---

**Ngày cập nhật:** 30/10/2025  
**Cấu trúc:** Clean & Organized ✨
