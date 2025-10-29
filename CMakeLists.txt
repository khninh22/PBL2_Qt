cmake_minimum_required(VERSION 3.16)

project(QuanLyThueSanQt VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Core sources
set(CORE_SOURCES
    core/KhachHang.cpp
    core/SanBong.cpp
    core/LichDatSan.cpp
    core/DichVu.cpp
    core/ChiTietDichVu.cpp
    core/NhanVien.cpp
    core/QuanLyThueSan.cpp
    core/AuthManager.cpp
)

set(CORE_HEADERS
    core/KhachHang.h
    core/SanBong.h
    core/LichDatSan.h
    core/DichVu.h
    core/ChiTietDichVu.h
    core/NhanVien.h
    core/QuanLyThueSan.h
    core/MangDong.h
    core/AuthManager.h
)

# UI sources
set(UI_SOURCES
    ui/MainWindow.cpp
    ui/SanBongDialog.cpp
    ui/KhachHangDialog.cpp
    ui/DatSanDialog.cpp
    ui/SanBookingDialog.cpp
    ui/DichVuDialog.cpp
    ui/NhanVienDialog.cpp
    ui/LoginDialog.cpp
)

set(UI_HEADERS
    ui/MainWindow.h
    ui/SanBongDialog.h
    ui/KhachHangDialog.h
    ui/DatSanDialog.h
    ui/SanBookingDialog.h
    ui/DichVuDialog.h
    ui/NhanVienDialog.h
    ui/LoginDialog.h
)

# Main executable
add_executable(${PROJECT_NAME}
    main.cpp
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    ${UI_SOURCES}
    ${UI_HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# ✅ Tạo thư mục data trong bin/ để lưu dữ liệu
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/data"
    COMMENT "Creating data directory in bin folder"
)

# Windows specific settings - Hide console window
if(WIN32)
    # Use linker flag to hide console instead of WIN32_EXECUTABLE (Qt 6.9 compatibility)
    target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
endif()
