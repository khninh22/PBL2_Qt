#include "MainWindow.h"
#include "SanBongDialog.h"
#include "KhachHangDialog.h"
#include "SanBookingDialog.h"
#include "DichVuDialog.h"
#include "NhanVienDialog.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QIcon>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QGroupBox>
#include <QGridLayout>
#include <QApplication>

MainWindow::MainWindow(AuthManager *auth, QWidget *parent)
    : QMainWindow(parent), authManager(auth), sidebarVisible(true) // Start with sidebar visible
{
    quanLy = new QuanLyThueSan();
    setupUI();
    applyAccessControl();

    // Navigate to first page
    navigateToPage(0);

    // Update all tables
    updateSanBongTable();
    updateKhachHangTable();
    updateLichDatTable();
    updateDichVuTable();
    updateNhanVienTable();
    updateThongKeDisplay();
}

MainWindow::~MainWindow()
{
    delete quanLy;
}

void MainWindow::setupUI()
{
    setWindowTitle("⚽ Hệ Thống Quản Lý Thuê Sân Bóng");
    setMinimumSize(1400, 800);

    // Central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Setup sidebar and content
    setupSidebar();
    setupContent();

    // Add to main layout
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stackedWidget, 1);

    // Global stylesheet
    setStyleSheet(
        "QMainWindow {"
        "    background-color: #f5f5f5;"
        "}"
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    padding: 10px 16px;"
        "    border-radius: 6px;"
        "    font-weight: bold;"
        "    font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3d8b40;"
        "}"
        "QPushButton[class='delete'] {"
        "    background-color: #f44336;"
        "}"
        "QPushButton[class='delete']:hover {"
        "    background-color: #da190b;"
        "}"
        "QTableWidget {"
        "    border: 1px solid #ddd;"
        "    gridline-color: #e0e0e0;"
        "    selection-background-color: #4CAF50;"
        "    background-color: white;"
        "    border-radius: 8px;"
        "}"
        "QTableWidget::item {"
        "    padding: 8px;"
        "}"
        "QHeaderView::section {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    padding: 10px;"
        "    border: none;"
        "    font-weight: bold;"
        "}");
}

void MainWindow::setupSidebar()
{
    sidebar = new QFrame();
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(250); // Start visible with full width
    sidebar->setStyleSheet(
        "#sidebar {"
        "    background-color: #2c3e50;"
        "    border-right: 1px solid #1a252f;"
        "}");

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    sidebarLayout->setSpacing(0);

    // Header with hamburger button and logo
    QWidget *headerWidget = new QWidget();
    headerWidget->setStyleSheet("background-color: #1a252f;");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(10, 15, 10, 15);
    headerLayout->setSpacing(10);

    // Hamburger button
    btnToggleSidebar = new QPushButton("☰");
    btnToggleSidebar->setFixedSize(50, 50);
    btnToggleSidebar->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    font-size: 24px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.1);"
        "}");
    connect(btnToggleSidebar, &QPushButton::clicked, this, &MainWindow::toggleSidebar);

    lblLogo = new QLabel("QUẢN LÝ SÂN");
    lblLogo->setStyleSheet(
        "color: white;"
        "font-size: 20px;"
        "font-weight: bold;"
        "background-color: transparent;");

    headerLayout->addWidget(btnToggleSidebar);
    headerLayout->addWidget(lblLogo);
    headerLayout->addStretch();

    sidebarLayout->addWidget(headerWidget);

    // Menu items
    QWidget *menuWidget = new QWidget();
    QVBoxLayout *menuLayout = new QVBoxLayout(menuWidget);
    menuLayout->setContentsMargins(10, 20, 10, 10);
    menuLayout->setSpacing(5);

    btnMenuSanBong = createMenuButton("⚽", "Sân Bóng", 0);
    btnMenuKhachHang = createMenuButton("👥", "Khách Hàng", 1);
    btnMenuLichDat = createMenuButton("📅", "Đặt Sân", 2);
    btnMenuDichVu = createMenuButton("🍔", "Dịch Vụ", 3);
    btnMenuNhanVien = createMenuButton("👔", "Nhân Viên", 4);
    btnMenuThongKe = createMenuButton("📊", "Thống Kê", 5);

    menuLayout->addWidget(btnMenuSanBong);
    menuLayout->addWidget(btnMenuKhachHang);
    menuLayout->addWidget(btnMenuLichDat);
    menuLayout->addWidget(btnMenuDichVu);
    menuLayout->addWidget(btnMenuNhanVien);
    menuLayout->addWidget(btnMenuThongKe);
    menuLayout->addStretch();

    // Logout button at bottom
    btnMenuLogout = new QPushButton("🚪 Đăng Xuất");
    btnMenuLogout->setStyleSheet(
        "QPushButton {"
        "    background-color: #e74c3c;"
        "    color: white;"
        "    border: none;"
        "    padding: 12px;"
        "    border-radius: 6px;"
        "    font-weight: bold;"
        "    font-size: 13px;"
        "    text-align: left;"
        "}"
        "QPushButton:hover {"
        "    background-color: #c0392b;"
        "}");
    connect(btnMenuLogout, &QPushButton::clicked, this, &MainWindow::onLogout);
    menuLayout->addWidget(btnMenuLogout);

    sidebarLayout->addWidget(menuWidget, 1);
}

void MainWindow::setupContent()
{
    // Create stacked widget for pages directly (no wrapper)
    stackedWidget = new QStackedWidget();
    stackedWidget->setStyleSheet("background-color: #ecf0f1;");

    stackedWidget->addWidget(createSanBongPage());
    stackedWidget->addWidget(createKhachHangPage());
    stackedWidget->addWidget(createLichDatPage());
    stackedWidget->addWidget(createDichVuPage());
    stackedWidget->addWidget(createNhanVienPage());
    stackedWidget->addWidget(createThongKePage());
}

QPushButton *MainWindow::createMenuButton(const QString &icon, const QString &text, int pageIndex)
{
    QPushButton *btn = new QPushButton(icon + "  " + text);
    btn->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: #ecf0f1;"
        "    border: none;"
        "    padding: 15px 20px;"
        "    text-align: left;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #34495e;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #4CAF50;"
        "}");

    connect(btn, &QPushButton::clicked, [this, pageIndex]()
            { navigateToPage(pageIndex); });

    menuButtons.append(btn);
    return btn;
}

void MainWindow::toggleSidebar()
{
    int targetWidth = sidebarVisible ? 70 : 250; // 60px when collapsed (just icons)

    QPropertyAnimation *animation = new QPropertyAnimation(sidebar, "minimumWidth");
    animation->setDuration(300);
    animation->setStartValue(sidebar->width());
    animation->setEndValue(targetWidth);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    QPropertyAnimation *animation2 = new QPropertyAnimation(sidebar, "maximumWidth");
    animation2->setDuration(300);
    animation2->setStartValue(sidebar->width());
    animation2->setEndValue(targetWidth);
    animation2->setEasingCurve(QEasingCurve::InOutQuad);
    animation2->start(QAbstractAnimation::DeleteWhenStopped);

    sidebarVisible = !sidebarVisible;

    // Update hamburger button and logo visibility
    if (sidebarVisible)
    {
        // Expanded - show logo and full text in buttons
        btnToggleSidebar->setText("☰");
        lblLogo->setVisible(true);

        // Enable all menu buttons
        for (auto btn : menuButtons)
        {
            btn->setEnabled(true);
        }
        if (btnMenuLogout)
            btnMenuLogout->setEnabled(true);

        // Update all menu buttons to show icon + text
        if (btnMenuSanBong)
            btnMenuSanBong->setText("⚽  Sân Bóng");
        if (btnMenuKhachHang)
            btnMenuKhachHang->setText("👥  Khách Hàng");
        if (btnMenuLichDat)
            btnMenuLichDat->setText("📅  Đặt Sân");
        if (btnMenuDichVu)
            btnMenuDichVu->setText("🍔  Dịch Vụ");
        if (btnMenuNhanVien)
            btnMenuNhanVien->setText("👔  Nhân Viên");
        if (btnMenuThongKe)
            btnMenuThongKe->setText("📊  Thống Kê");
        if (btnMenuLogout)
            btnMenuLogout->setText("🚪 Đăng Xuất");

        // Reset menu button styles to show text align left
        for (auto btn : menuButtons)
        {
            btn->setStyleSheet(
                "QPushButton {"
                "    background-color: transparent;"
                "    color: #ecf0f1;"
                "    border: none;"
                "    padding: 15px 20px;"
                "    text-align: left;"
                "    font-size: 14px;"
                "    font-weight: bold;"
                "    border-radius: 8px;"
                "}"
                "QPushButton:hover {"
                "    background-color: #34495e;"
                "}"
                "QPushButton:pressed {"
                "    background-color: #4CAF50;"
                "}");
        }
        // Re-highlight current page
        navigateToPage(stackedWidget->currentIndex());
    }
    else
    {
        // Collapsed - hide logo, show only hamburger icon
        btnToggleSidebar->setText("☰");
        lblLogo->setVisible(false);

        // Disable all menu buttons (cannot click when collapsed)
        for (auto btn : menuButtons)
        {
            btn->setEnabled(false);
        }
        if (btnMenuLogout)
            btnMenuLogout->setEnabled(false);

        // Update all menu buttons to show only icons (centered)
        if (btnMenuSanBong)
            btnMenuSanBong->setText("⚽");
        if (btnMenuKhachHang)
            btnMenuKhachHang->setText("👥");
        if (btnMenuLichDat)
            btnMenuLichDat->setText("📅");
        if (btnMenuDichVu)
            btnMenuDichVu->setText("🍔");
        if (btnMenuNhanVien)
            btnMenuNhanVien->setText("👔");
        if (btnMenuThongKe)
            btnMenuThongKe->setText("📊");
        if (btnMenuLogout)
            btnMenuLogout->setText("🚪");

        // Center icons within collapsed sidebar to avoid being cut off
        for (auto btn : menuButtons)
        {
            btn->setStyleSheet(
                "QPushButton {"
                "    background-color: transparent;"
                "    color: #7f8c8d;" // Dimmed color when disabled
                "    border: none;"
                "    padding: 15px;"      // Equal padding for centering
                "    text-align: center;" // Center to fit within 70px width
                "    font-size: 14px;"    // Same icon size as when expanded
                "    font-weight: bold;"
                "    border-radius: 8px;"
                "}"
                "QPushButton:disabled {"
                "    color: #7f8c8d;"
                "}");
        }
    }

    // Update logout button style
    if (btnMenuLogout)
    {
        if (sidebarVisible)
        {
            btnMenuLogout->setStyleSheet(
                "QPushButton {"
                "    background-color: #e74c3c;"
                "    color: white;"
                "    border: none;"
                "    padding: 12px;"
                "    border-radius: 6px;"
                "    font-weight: bold;"
                "    font-size: 13px;"
                "    text-align: left;"
                "}"
                "QPushButton:hover {"
                "    background-color: #c0392b;"
                "}");
        }
        else
        {
            btnMenuLogout->setStyleSheet(
                "QPushButton {"
                "    background-color: #e74c3c;" // Keep red color when disabled
                "    color: white;"
                "    border: none;"
                "    padding: 12px;"
                "    border-radius: 6px;"
                "    font-weight: bold;"
                "    font-size: 14px;"    // Same icon size as when expanded
                "    text-align: center;" // Center to fit within 70px width
                "}"
                "QPushButton:disabled {"
                "    background-color: #e74c3c;" // Keep red even when disabled
                "    color: #ecf0f1;"
                "}");
        }
    }
}

void MainWindow::navigateToPage(int index)
{
    stackedWidget->setCurrentIndex(index);

    // Only highlight active button if sidebar is expanded
    if (!sidebarVisible)
    {
        // Sidebar collapsed - no highlight, just update icons
        return;
    }

    // Highlight active button (only when sidebar is expanded)
    for (int i = 0; i < menuButtons.size(); i++)
    {
        if (i == index)
        {
            menuButtons[i]->setStyleSheet(
                "QPushButton {"
                "    background-color: #4CAF50;"
                "    color: white;"
                "    border: none;"
                "    padding: 15px 20px;"
                "    text-align: left;"
                "    font-size: 14px;"
                "    font-weight: bold;"
                "    border-radius: 8px;"
                "}");
        }
        else
        {
            menuButtons[i]->setStyleSheet(
                "QPushButton {"
                "    background-color: transparent;"
                "    color: #ecf0f1;"
                "    border: none;"
                "    padding: 15px 20px;"
                "    text-align: left;"
                "    font-size: 14px;"
                "    font-weight: bold;"
                "    border-radius: 8px;"
                "}"
                "QPushButton:hover {"
                "    background-color: #34495e;"
                "}");
        }
    }
}

QWidget *MainWindow::createSanBongPage()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnThemSan = new QPushButton("➕ Thêm Sân");
    btnSuaSan = new QPushButton("✏️ Sửa Sân");
    btnXoaSan = new QPushButton("❌ Xóa Sân");
    btnXoaSan->setProperty("class", "delete");
    btnBaoTri = new QPushButton("🔧 Bảo Trì");

    btnLayout->addWidget(btnThemSan);
    btnLayout->addWidget(btnSuaSan);
    btnLayout->addWidget(btnXoaSan);
    btnLayout->addWidget(btnBaoTri);
    btnLayout->addStretch();

    // Table
    tableSanBong = new QTableWidget();
    tableSanBong->setColumnCount(5);
    tableSanBong->setHorizontalHeaderLabels({"Mã Sân", "Tên Sân", "Loại Sân", "Giá Thuê", "Trạng Thái"});
    tableSanBong->horizontalHeader()->setStretchLastSection(true);
    tableSanBong->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableSanBong->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addLayout(btnLayout);
    layout->addWidget(tableSanBong);

    // Connections
    connect(btnThemSan, &QPushButton::clicked, this, &MainWindow::onThemSanBong);
    connect(btnSuaSan, &QPushButton::clicked, this, &MainWindow::onSuaSanBong);
    connect(btnXoaSan, &QPushButton::clicked, this, &MainWindow::onXoaSanBong);
    connect(btnBaoTri, &QPushButton::clicked, this, &MainWindow::onBaoTriSan);

    return widget;
}

QWidget *MainWindow::createKhachHangPage()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnThemKH = new QPushButton("➕ Thêm Khách Hàng");
    btnSuaKH = new QPushButton("✏️ Sửa Khách Hàng");
    btnXoaKH = new QPushButton("❌ Xóa Khách Hàng");
    btnXoaKH->setProperty("class", "delete");

    btnLayout->addWidget(btnThemKH);
    btnLayout->addWidget(btnSuaKH);
    btnLayout->addWidget(btnXoaKH);
    btnLayout->addStretch();

    // Table
    tableKhachHang = new QTableWidget();
    tableKhachHang->setColumnCount(5);
    tableKhachHang->setHorizontalHeaderLabels({"Mã KH", "Họ Tên", "SĐT", "Điểm Tích Lũy", "Cấp Độ"});
    tableKhachHang->horizontalHeader()->setStretchLastSection(true);
    tableKhachHang->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableKhachHang->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addLayout(btnLayout);
    layout->addWidget(tableKhachHang);

    // Connections
    connect(btnThemKH, &QPushButton::clicked, this, &MainWindow::onThemKhachHang);
    connect(btnSuaKH, &QPushButton::clicked, this, &MainWindow::onSuaKhachHang);
    connect(btnXoaKH, &QPushButton::clicked, this, &MainWindow::onXoaKhachHang);

    return widget;
}

QWidget *MainWindow::createLichDatPage()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnDatSan = new QPushButton("📅 Đặt Sân");
    btnDatSan->setStyleSheet(
        "background-color: #2196F3; "
        "font-size: 14px; "
        "padding: 10px 20px; "
        "border-radius: 5px;");
    btnHuyLich = new QPushButton("🚫 Hủy Lịch");
    btnThanhToan = new QPushButton("💰 Thanh Toán");

    btnLayout->addWidget(btnDatSan);
    btnLayout->addWidget(btnHuyLich);
    btnLayout->addWidget(btnThanhToan);
    btnLayout->addStretch();

    // Table
    tableLichDat = new QTableWidget();
    tableLichDat->setColumnCount(8);
    tableLichDat->setHorizontalHeaderLabels({"Mã Lịch", "Mã KH", "Mã Sân", "Bắt Đầu", "Kết Thúc",
                                             "Tổng Tiền", "TT Đặt", "TT Thanh Toán"});
    tableLichDat->horizontalHeader()->setStretchLastSection(true);
    tableLichDat->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableLichDat->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addLayout(btnLayout);
    layout->addWidget(tableLichDat);

    // Connections
    connect(btnDatSan, &QPushButton::clicked, this, &MainWindow::onDatSanTrucQuan);
    connect(btnHuyLich, &QPushButton::clicked, this, &MainWindow::onHuyLich);
    connect(btnThanhToan, &QPushButton::clicked, this, &MainWindow::onThanhToan);

    return widget;
}

QWidget *MainWindow::createDichVuPage()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnThemDV = new QPushButton("➕ Thêm Dịch Vụ");
    btnSuaDV = new QPushButton("✏️   Sửa Dịch Vụ");
    btnXoaDV = new QPushButton("❌ Xóa Dịch Vụ");
    btnXoaDV->setProperty("class", "delete");

    btnLayout->addWidget(btnThemDV);
    btnLayout->addWidget(btnSuaDV);
    btnLayout->addWidget(btnXoaDV);
    btnLayout->addStretch();

    // Table
    tableDichVu = new QTableWidget();
    tableDichVu->setColumnCount(5);
    tableDichVu->setHorizontalHeaderLabels({"Mã DV", "Tên DV", "Loại DV", "Giá", "Trạng Thái"});
    tableDichVu->horizontalHeader()->setStretchLastSection(true);
    tableDichVu->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableDichVu->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addLayout(btnLayout);
    layout->addWidget(tableDichVu);

    // Connections
    connect(btnThemDV, &QPushButton::clicked, this, &MainWindow::onThemDichVu);
    connect(btnSuaDV, &QPushButton::clicked, this, &MainWindow::onSuaDichVu);
    connect(btnXoaDV, &QPushButton::clicked, this, &MainWindow::onXoaDichVu);

    return widget;
}

QWidget *MainWindow::createNhanVienPage()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnThemNV = new QPushButton("➕ Thêm Nhân Viên");
    btnSuaNV = new QPushButton("✏️ Sửa Nhân Viên");
    btnXoaNV = new QPushButton("❌ Xóa Nhân Viên");
    btnXoaNV->setProperty("class", "delete");

    btnLayout->addWidget(btnThemNV);
    btnLayout->addWidget(btnSuaNV);
    btnLayout->addWidget(btnXoaNV);
    btnLayout->addStretch();

    // Table
    tableNhanVien = new QTableWidget();
    tableNhanVien->setColumnCount(6);
    tableNhanVien->setHorizontalHeaderLabels({"Mã NV", "Họ Tên", "SĐT", "Vị Trí", "Lương", "Trạng Thái"});
    tableNhanVien->horizontalHeader()->setStretchLastSection(true);
    tableNhanVien->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableNhanVien->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addLayout(btnLayout);
    layout->addWidget(tableNhanVien);

    // Connections
    connect(btnThemNV, &QPushButton::clicked, this, &MainWindow::onThemNhanVien);
    connect(btnSuaNV, &QPushButton::clicked, this, &MainWindow::onSuaNhanVien);
    connect(btnXoaNV, &QPushButton::clicked, this, &MainWindow::onXoaNhanVien);

    return widget;
}

QWidget *MainWindow::createThongKePage()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    // Group box thống kê
    QGroupBox *groupBox = new QGroupBox("📈 Thống Kê Hệ Thống");
    QGridLayout *grid = new QGridLayout(groupBox);

    // Labels
    lblDoanhThuNgay = new QLabel("0 VNĐ");
    lblDoanhThuThang = new QLabel("0 VNĐ");
    lblTongSan = new QLabel("0");
    lblTongKhach = new QLabel("0");

    lblDoanhThuNgay->setStyleSheet("font-size: 24px; font-weight: bold; color: #4CAF50;");
    lblDoanhThuThang->setStyleSheet("font-size: 24px; font-weight: bold; color: #2196F3;");
    lblTongSan->setStyleSheet("font-size: 24px; font-weight: bold; color: #FF9800;");
    lblTongKhach->setStyleSheet("font-size: 24px; font-weight: bold; color: #9C27B0;");

    grid->addWidget(new QLabel("💰 Doanh Thu Hôm Nay:"), 0, 0);
    grid->addWidget(lblDoanhThuNgay, 0, 1);

    grid->addWidget(new QLabel("💰 Doanh Thu Tháng Này:"), 1, 0);
    grid->addWidget(lblDoanhThuThang, 1, 1);

    grid->addWidget(new QLabel("⚽ Tổng Số Sân:"), 2, 0);
    grid->addWidget(lblTongSan, 2, 1);

    grid->addWidget(new QLabel("👥 Tổng Khách Hàng:"), 3, 0);
    grid->addWidget(lblTongKhach, 3, 1);

    layout->addWidget(groupBox);
    layout->addStretch();

    return widget;
}

void MainWindow::updateSanBongTable()
{
    tableSanBong->setRowCount(0);
    const MangDong<SanBong> &ds = quanLy->getDsSanBong();

    for (int i = 0; i < ds.getKichThuoc(); i++)
    {
        const SanBong &san = ds[i];
        int row = tableSanBong->rowCount();
        tableSanBong->insertRow(row);

        tableSanBong->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(san.getMaSan())));
        tableSanBong->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(san.getTenSan())));
        tableSanBong->setItem(row, 2, new QTableWidgetItem(QString("Sân %1 người").arg(san.getLoaiSan())));
        tableSanBong->setItem(row, 3, new QTableWidgetItem(QString::number(san.getGiaThue(), 'f', 0) + " VNĐ"));
        tableSanBong->setItem(row, 4, new QTableWidgetItem(san.getBaoTri() ? "Đang Bảo Trì" : "Hoạt Động"));
    }
}

void MainWindow::updateKhachHangTable()
{
    tableKhachHang->setRowCount(0);
    const MangDong<KhachHang> &ds = quanLy->getDsKhachHang();

    for (int i = 0; i < ds.getKichThuoc(); i++)
    {
        const KhachHang &kh = ds[i];
        int row = tableKhachHang->rowCount();
        tableKhachHang->insertRow(row);

        tableKhachHang->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(kh.getMaKH())));
        tableKhachHang->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(kh.getHoTen())));
        tableKhachHang->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(kh.getSdt())));
        tableKhachHang->setItem(row, 3, new QTableWidgetItem(QString::number(kh.getDiemTichLuy())));
        tableKhachHang->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(kh.getTenCapDo())));
    }
}

void MainWindow::updateLichDatTable()
{
    tableLichDat->setRowCount(0);
    const MangDong<LichDatSan> &ds = quanLy->getDsLichDatSan();

    for (int i = 0; i < ds.getKichThuoc(); i++)
    {
        const LichDatSan &lich = ds[i];
        int row = tableLichDat->rowCount();
        tableLichDat->insertRow(row);

        tableLichDat->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(lich.getMaLichDat())));
        tableLichDat->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(lich.getMaKH())));
        tableLichDat->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(lich.getMaSan())));

        QString batDau = QString::fromStdString(formatTime(lich.getThoiGianBatDau()));
        QString ketThuc = QString::fromStdString(formatTime(lich.getThoiGianKetThuc()));

        tableLichDat->setItem(row, 3, new QTableWidgetItem(batDau));
        tableLichDat->setItem(row, 4, new QTableWidgetItem(ketThuc));
        tableLichDat->setItem(row, 5, new QTableWidgetItem(QString::number(lich.getTongTien(), 'f', 0) + " VNĐ"));
        tableLichDat->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(lich.getTrangThaiDat())));
        tableLichDat->setItem(row, 7, new QTableWidgetItem(QString::fromStdString(lich.getTrangThaiTT())));
    }
}

void MainWindow::updateDichVuTable()
{
    tableDichVu->setRowCount(0);
    const MangDong<DichVu> &ds = quanLy->getDsDichVu();

    for (int i = 0; i < ds.getKichThuoc(); i++)
    {
        const DichVu &dv = ds[i];
        int row = tableDichVu->rowCount();
        tableDichVu->insertRow(row);

        tableDichVu->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(dv.getMaDV())));
        tableDichVu->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(dv.getTenDV())));
        tableDichVu->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(dv.getLoaiDV())));
        tableDichVu->setItem(row, 3, new QTableWidgetItem(QString::number(dv.getGiaDV(), 'f', 0) + " VNĐ"));
        tableDichVu->setItem(row, 4, new QTableWidgetItem(dv.getConHang() ? "Còn Hàng" : "Hết Hàng"));
    }
}

void MainWindow::updateNhanVienTable()
{
    tableNhanVien->setRowCount(0);
    const MangDong<NhanVien> &ds = quanLy->getDsNhanVien();

    for (int i = 0; i < ds.getKichThuoc(); i++)
    {
        const NhanVien &nv = ds[i];
        int row = tableNhanVien->rowCount();
        tableNhanVien->insertRow(row);

        tableNhanVien->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(nv.getMaNV())));
        tableNhanVien->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(nv.getHoTen())));
        tableNhanVien->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(nv.getSdt())));
        tableNhanVien->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(nv.getViTri())));
        tableNhanVien->setItem(row, 4, new QTableWidgetItem(QString::number(nv.getLuongCoBan(), 'f', 0) + " VNĐ"));
        tableNhanVien->setItem(row, 5, new QTableWidgetItem(nv.getDangLam() ? "Đang Làm" : "Đã Nghỉ"));
    }
}

void MainWindow::updateThongKeDisplay()
{
    // Doanh thu hôm nay
    time_t now = time(nullptr);
    double doanhThuNgay = quanLy->tinhDoanhThuNgay(now);
    lblDoanhThuNgay->setText(QString::number(doanhThuNgay, 'f', 0) + " VNĐ");

    // Doanh thu tháng này
    tm *t = localtime(&now);
    double doanhThuThang = quanLy->tinhDoanhThuThang(t->tm_mon + 1, t->tm_year + 1900);
    lblDoanhThuThang->setText(QString::number(doanhThuThang, 'f', 0) + " VNĐ");

    // Tổng số sân
    lblTongSan->setText(QString::number(quanLy->getDsSanBong().getKichThuoc()));

    // Tổng khách hàng
    lblTongKhach->setText(QString::number(quanLy->getDsKhachHang().getKichThuoc()));
}

// Slot implementations (continue in next part due to length)
void MainWindow::onThemSanBong()
{
    SanBongDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        if (quanLy->themSanBong(dialog.getMaSan(), dialog.getTenSan(),
                                dialog.getLoaiSan(), dialog.getGiaThue()))
        {
            updateSanBongTable();
            QMessageBox::information(this, "Thành công", "Đã thêm sân bóng!");
        }
        else
        {
            QMessageBox::warning(this, "Lỗi", "Mã sân đã tồn tại!");
        }
    }
}

void MainWindow::onSuaSanBong()
{
    int row = tableSanBong->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn sân cần sửa!");
        return;
    }

    QString maSan = tableSanBong->item(row, 0)->text();
    SanBong *san = quanLy->timSanBong(maSan.toStdString());

    if (san)
    {
        SanBongDialog dialog(this, san);
        if (dialog.exec() == QDialog::Accepted)
        {
            if (quanLy->suaSanBong(maSan.toStdString(), dialog.getTenSan(),
                                   dialog.getLoaiSan(), dialog.getGiaThue()))
            {
                updateSanBongTable();
                QMessageBox::information(this, "Thành công", "Đã cập nhật sân bóng!");
            }
        }
    }
}

void MainWindow::onXoaSanBong()
{
    int row = tableSanBong->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn sân cần xóa!");
        return;
    }

    QString maSan = tableSanBong->item(row, 0)->text();

    auto reply = QMessageBox::question(this, "Xác nhận",
                                       "Bạn có chắc muốn xóa sân này?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (quanLy->xoaSanBong(maSan.toStdString()))
        {
            updateSanBongTable();
            QMessageBox::information(this, "Thành công", "Đã xóa sân bóng!");
        }
    }
}

void MainWindow::onBaoTriSan()
{
    int row = tableSanBong->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn sân!");
        return;
    }

    QString maSan = tableSanBong->item(row, 0)->text();
    SanBong *san = quanLy->timSanBong(maSan.toStdString());

    if (san)
    {
        if (san->getBaoTri())
        {
            quanLy->tatBaoTriSan(maSan.toStdString());
            QMessageBox::information(this, "Thành công", "Đã tắt chế độ bảo trì!");
        }
        else
        {
            quanLy->batBaoTriSan(maSan.toStdString());
            QMessageBox::information(this, "Thành công", "Đã bật chế độ bảo trì!");
        }
        updateSanBongTable();
    }
}

void MainWindow::onRefreshSanBong()
{
    updateSanBongTable();
}

// Similar implementations for other slot methods...
void MainWindow::onThemKhachHang()
{
    KhachHangDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        if (quanLy->themKhachHang(dialog.getMaKH(), dialog.getHoTen(), dialog.getSdt()))
        {
            updateKhachHangTable();
            QMessageBox::information(this, "Thành công", "Đã thêm khách hàng!");
        }
        else
        {
            QMessageBox::warning(this, "Lỗi", "Mã khách hàng đã tồn tại!");
        }
    }
}

void MainWindow::onSuaKhachHang()
{
    int row = tableKhachHang->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn khách hàng cần sửa!");
        return;
    }

    QString maKH = tableKhachHang->item(row, 0)->text();
    KhachHang *kh = quanLy->timKhachHang(maKH.toStdString());

    if (kh)
    {
        KhachHangDialog dialog(this, kh);
        if (dialog.exec() == QDialog::Accepted)
        {
            if (quanLy->suaKhachHang(maKH.toStdString(), dialog.getHoTen(), dialog.getSdt()))
            {
                updateKhachHangTable();
                QMessageBox::information(this, "Thành công", "Đã cập nhật khách hàng!");
            }
        }
    }
}

void MainWindow::onXoaKhachHang()
{
    int row = tableKhachHang->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn khách hàng cần xóa!");
        return;
    }

    QString maKH = tableKhachHang->item(row, 0)->text();

    auto reply = QMessageBox::question(this, "Xác nhận",
                                       "Bạn có chắc muốn xóa khách hàng này?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (quanLy->xoaKhachHang(maKH.toStdString()))
        {
            updateKhachHangTable();
            QMessageBox::information(this, "Thành công", "Đã xóa khách hàng!");
        }
    }
}

void MainWindow::onRefreshKhachHang()
{
    updateKhachHangTable();
}

void MainWindow::onDatSanTrucQuan()
{
    SanBookingDialog dialog(quanLy, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        updateLichDatTable();
        updateThongKeDisplay();
        QMessageBox::information(this, "Thành công", "Đã đặt sân thành công!");
    }
}

void MainWindow::onHuyLich()
{
    int row = tableLichDat->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn lịch cần hủy!");
        return;
    }

    QString maLich = tableLichDat->item(row, 0)->text();

    auto reply = QMessageBox::question(this, "Xác nhận",
                                       "Bạn có chắc muốn hủy lịch đặt này?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (quanLy->huyLichDat(maLich.toStdString()))
        {
            updateLichDatTable();
            QMessageBox::information(this, "Thành công", "Đã hủy lịch đặt!");
        }
    }
}

void MainWindow::onThanhToan()
{
    int row = tableLichDat->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn lịch cần thanh toán!");
        return;
    }

    QString maLich = tableLichDat->item(row, 0)->text();
    double tongTien, giamGia;

    if (quanLy->thanhToan(maLich.toStdString(), tongTien, giamGia))
    {
        QString msg = QString("Thanh toán thành công!\n\n"
                              "Tổng tiền: %1 VNĐ\n"
                              "Giảm giá: %2 VNĐ\n"
                              "Thành tiền: %3 VNĐ")
                          .arg(tongTien + giamGia, 0, 'f', 0)
                          .arg(giamGia, 0, 'f', 0)
                          .arg(tongTien, 0, 'f', 0);

        QMessageBox::information(this, "Thành công", msg);
        updateLichDatTable();
        updateThongKeDisplay();
    }
    else
    {
        QMessageBox::warning(this, "Lỗi", "Không thể thanh toán lịch này!");
    }
}

void MainWindow::onRefreshLichDat()
{
    updateLichDatTable();
}

void MainWindow::onThemDichVu()
{
    DichVuDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        if (quanLy->themDichVu(dialog.getMaDV(), dialog.getTenDV(),
                               dialog.getLoaiDV(), dialog.getGiaDV()))
        {
            updateDichVuTable();
            QMessageBox::information(this, "Thành công", "Đã thêm dịch vụ!");
        }
        else
        {
            QMessageBox::warning(this, "Lỗi", "Mã dịch vụ đã tồn tại!");
        }
    }
}

void MainWindow::onSuaDichVu()
{
    int row = tableDichVu->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn dịch vụ cần sửa!");
        return;
    }

    QString maDV = tableDichVu->item(row, 0)->text();
    DichVu *dv = quanLy->timDichVu(maDV.toStdString());

    if (dv)
    {
        DichVuDialog dialog(this, dv);
        if (dialog.exec() == QDialog::Accepted)
        {
            if (quanLy->suaDichVu(maDV.toStdString(), dialog.getTenDV(),
                                  dialog.getLoaiDV(), dialog.getGiaDV()))
            {
                updateDichVuTable();
                QMessageBox::information(this, "Thành công", "Đã cập nhật dịch vụ!");
            }
        }
    }
}

void MainWindow::onXoaDichVu()
{
    int row = tableDichVu->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn dịch vụ cần xóa!");
        return;
    }

    QString maDV = tableDichVu->item(row, 0)->text();

    auto reply = QMessageBox::question(this, "Xác nhận",
                                       "Bạn có chắc muốn xóa dịch vụ này?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (quanLy->xoaDichVu(maDV.toStdString()))
        {
            updateDichVuTable();
            QMessageBox::information(this, "Thành công", "Đã xóa dịch vụ!");
        }
    }
}

void MainWindow::onRefreshDichVu()
{
    updateDichVuTable();
}

void MainWindow::onThemNhanVien()
{
    NhanVienDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        if (quanLy->themNhanVien(dialog.getMaNV(), dialog.getHoTen(), dialog.getSdt(),
                                 dialog.getViTri(), dialog.getLuong(),
                                 dialog.getTaiKhoan(), dialog.getMatKhau()))
        {
            updateNhanVienTable();
            QMessageBox::information(this, "✅ Thành công",
                                     "Đã thêm nhân viên!\n\n"
                                     "Tài khoản: " +
                                         QString::fromStdString(dialog.getTaiKhoan()) + "\n"
                                                                                        "Mật khẩu: " +
                                         QString::fromStdString(dialog.getMatKhau()));
        }
        else
        {
            QMessageBox::warning(this, "❌ Lỗi", "Mã nhân viên đã tồn tại!");
        }
    }
}

void MainWindow::onSuaNhanVien()
{
    int row = tableNhanVien->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "⚠️ Cảnh báo", "Vui lòng chọn nhân viên cần sửa!");
        return;
    }

    QString maNV = tableNhanVien->item(row, 0)->text();
    NhanVien *nv = quanLy->timNhanVien(maNV.toStdString());

    if (nv)
    {
        NhanVienDialog dialog(this, nv);
        if (dialog.exec() == QDialog::Accepted)
        {
            if (quanLy->suaNhanVien(maNV.toStdString(), dialog.getHoTen(), dialog.getSdt(),
                                    dialog.getViTri(), dialog.getLuong(),
                                    dialog.getTaiKhoan(), dialog.getMatKhau()))
            {
                updateNhanVienTable();
                QMessageBox::information(this, "✅ Thành công", "Đã cập nhật nhân viên!");
            }
        }
    }
}

void MainWindow::onXoaNhanVien()
{
    int row = tableNhanVien->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn nhân viên cần xóa!");
        return;
    }

    QString maNV = tableNhanVien->item(row, 0)->text();

    auto reply = QMessageBox::question(this, "Xác nhận",
                                       "Bạn có chắc muốn xóa nhân viên này?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (quanLy->xoaNhanVien(maNV.toStdString()))
        {
            updateNhanVienTable();
            QMessageBox::information(this, "Thành công", "Đã xóa nhân viên!");
        }
    }
}

void MainWindow::onRefreshNhanVien()
{
    updateNhanVienTable();
}

void MainWindow::onXemThongKe()
{
    updateThongKeDisplay();
}

// ===== AUTHENTICATION METHODS =====

void MainWindow::applyAccessControl()
{
    bool isAdmin = authManager->hasFullAccess();

    // User info removed from sidebar - no longer displaying user details

    // Nhân viên không được:
    // 1. Xóa sân bóng, khách hàng, dịch vụ
    // 2. Quản lý nhân viên

    if (!isAdmin)
    {
        // Ẩn/vô hiệu hóa các nút xóa
        if (btnXoaSan)
        {
            btnXoaSan->setEnabled(false);
            btnXoaSan->setStyleSheet("background-color: #999; color: #666;");
            btnXoaSan->setToolTip("⚠️ Chỉ Admin mới có quyền xóa sân");
        }

        if (btnXoaKH)
        {
            btnXoaKH->setEnabled(false);
            btnXoaKH->setStyleSheet("background-color: #999; color: #666;");
            btnXoaKH->setToolTip("⚠️ Chỉ Admin mới có quyền xóa khách hàng");
        }

        if (btnXoaDV)
        {
            btnXoaDV->setEnabled(false);
            btnXoaDV->setStyleSheet("background-color: #999; color: #666;");
            btnXoaDV->setToolTip("⚠️ Chỉ Admin mới có quyền xóa dịch vụ");
        }

        // Ẩn menu Nhân viên
        if (btnMenuNhanVien)
        {
            btnMenuNhanVien->setVisible(false);
        }

        // Ẩn page Nhân viên (page index 4)
        if (stackedWidget && stackedWidget->count() > 4)
        {
            // Không ẩn page, chỉ ẩn button menu
        }
    }
}

void MainWindow::onLogout()
{
    auto reply = QMessageBox::question(this, "🚪 Đăng Xuất",
                                       "Bạn có chắc muốn đăng xuất?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // Lưu dữ liệu trước khi đăng xuất
        quanLy->luuDuLieu();
        authManager->logout();

        // Emit signal to return to login instead of closing app
        emit logoutRequested();

        // Close this window
        close();
    }
}

void MainWindow::onChangePassword()
{
    if (!authManager->hasFullAccess())
    {
        QMessageBox::warning(this, "⚠️ Không có quyền",
                             "Chỉ Admin mới có thể đổi mật khẩu!");
        return;
    }

    bool ok;
    QString oldPass = QInputDialog::getText(this, "🔑 Đổi Mật Khẩu Admin",
                                            "Nhập mật khẩu hiện tại:",
                                            QLineEdit::Password, "", &ok);

    if (!ok || oldPass.isEmpty())
        return;

    QString newPass = QInputDialog::getText(this, "🔑 Đổi Mật Khẩu Admin",
                                            "Nhập mật khẩu mới:",
                                            QLineEdit::Password, "", &ok);

    if (!ok || newPass.isEmpty())
        return;

    QString confirmPass = QInputDialog::getText(this, "🔑 Đổi Mật Khẩu Admin",
                                                "Xác nhận mật khẩu mới:",
                                                QLineEdit::Password, "", &ok);

    if (!ok || confirmPass.isEmpty())
        return;

    if (newPass != confirmPass)
    {
        QMessageBox::warning(this, "❌ Lỗi", "Mật khẩu xác nhận không khớp!");
        return;
    }

    if (authManager->changeAdminPassword(oldPass.toStdString(), newPass.toStdString()))
    {
        QMessageBox::information(this, "✅ Thành công",
                                 "Đã đổi mật khẩu Admin thành công!");
    }
    else
    {
        QMessageBox::critical(this, "❌ Thất bại",
                              "Mật khẩu hiện tại không đúng!");
    }
}
