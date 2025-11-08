cmake_minimum_required(VERSION 3.16)

project(QuanLyThueSanQt VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets PrintSupport)

# Core sources
set(CORE_SOURCES
    # ✅ NEW: OOP Hierarchy Base Classes
    core/ConNguoi.cpp
    core/NguoiDung.cpp
    # ✅ NEW: Date/Time Classes
    core/NgayThang.cpp
    core/NgayGio.cpp
    # Entity Classes (refactored with inheritance)
    core/KhachHang.cpp
    core/SanBong.cpp
    core/LichDatSan.cpp
    core/DichVu.cpp
    core/ChiTietDichVu.cpp
    core/NhanVien.cpp
    core/QuanLyThueSan.cpp
    core/AuthManager.cpp
    # ✅ Manager Pattern - Single Responsibility
    core/managers/SanBongManager.cpp
    core/managers/KhachHangManager.cpp
    core/managers/LichDatManager.cpp
    core/managers/DichVuManager.cpp
    core/managers/NhanVienManager.cpp
    core/managers/ThanhToanManager.cpp
    # ✅ NEW: File & Backup Managers
    core/managers/FileManager.cpp
    core/managers/BackupManager.cpp
    # ✅ NEW: ThongKe Hierarchy - Statistics OOP
    core/ThongKe/ThongKe.cpp
    core/ThongKe/ThongKeDoanhThu.cpp
    core/ThongKe/ThongKeKhachHang.cpp
)

set(CORE_HEADERS
    # ✅ NEW: OOP Hierarchy Base Classes
    core/ConNguoi.h
    core/NguoiDung.h
    # ✅ NEW: Date/Time Classes
    core/NgayThang.h
    core/NgayGio.h
    # ✅ NEW: Algorithms (Header-only Templates)
    core/QuickSort.h
    # Data Structures & Utilities
    core/MangDong.h
    core/HashTable.h
    core/Exceptions.h
    core/Validator.h
    core/QuanLyThueSanValidated.h
    # Entity Headers (refactored with inheritance)
    core/KhachHang.h
    core/SanBong.h
    core/LichDatSan.h
    core/DichVu.h
    core/ChiTietDichVu.h
    core/NhanVien.h
    core/QuanLyThueSan.h
    core/AuthManager.h
    # ✅ Manager Pattern Headers
    core/managers/SanBongManager.h
    core/managers/KhachHangManager.h
    core/managers/LichDatManager.h
    core/managers/DichVuManager.h
    core/managers/NhanVienManager.h
    core/managers/ThanhToanManager.h
    # ✅ NEW: File & Backup Managers
    core/managers/FileManager.h
    core/managers/BackupManager.h
    # ✅ NEW: ThongKe Hierarchy - Statistics OOP
    core/ThongKe/ThongKe.h
    core/ThongKe/ThongKeDoanhThu.h
    core/ThongKe/ThongKeKhachHang.h
)

# UI sources
set(UI_SOURCES
    ui/MainWindow.cpp
    ui/SanBongDialog.cpp
    ui/KhachHangDialog.cpp
    ui/DatSanDialog.cpp
    ui/SanBookingDialog.cpp
    ui/KhachHangSelectionDialog.cpp
    ui/DichVuDialog.cpp
    ui/NhanVienDialog.cpp
    ui/LoginDialog.cpp
    ui/PaymentDialog.cpp
    ui/ThongKeDialog.cpp
    ui/RestoreDialog.cpp
    ui/ChartWidgets.cpp
)

set(UI_HEADERS
    ui/MainWindow.h
    ui/SanBongDialog.h
    ui/KhachHangDialog.h
    ui/DatSanDialog.h
    ui/SanBookingDialog.h
    ui/KhachHangSelectionDialog.h
    ui/DichVuDialog.h
    ui/NhanVienDialog.h
    ui/LoginDialog.h
    ui/PaymentDialog.h
    ui/ThongKeDialog.h
    ui/ChartWidgets.h
    ui/RestoreDialog.h
)

# Main executable
add_executable(${PROJECT_NAME}
    main.cpp
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    ${UI_SOURCES}
    ${UI_HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::PrintSupport)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# ✅ Tạo thư mục data và resources trong bin/
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/data"
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
    COMMENT "Creating data and resources directories in bin folder"
)

# Windows specific settings - Hide console window
if(WIN32)
    # Use linker flag to hide console instead of WIN32_EXECUTABLE (Qt 6.9 compatibility)
    target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
endif()
