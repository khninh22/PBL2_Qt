#include "ThongKeDialog.h"
#include <QMessageBox>

ThongKeDialog::ThongKeDialog(QuanLyThueSan *quanLy, QWidget *parent)
    : QDialog(parent), quanLy(quanLy)
{
    setWindowTitle("📊 Thống Kê Nâng Cao");
    resize(1200, 800);
    
    setupUI();
    
    // Load thống kê mặc định (tháng này)
    onThongKeClicked();
}

void ThongKeDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // ========== PHẦN 1: CHỌN KHOẢNG THỜI GIAN ==========
    QGroupBox *groupFilter = new QGroupBox("📅 Chọn Khoảng Thời Gian Thống Kê");
    QHBoxLayout *filterLayout = new QHBoxLayout(groupFilter);
    
    cboKhoangThoiGian = new QComboBox();
    cboKhoangThoiGian->addItems({"Hôm Nay", "7 Ngày Qua", "Tháng Này", "Quý Này", "Năm Này", "Tùy Chỉnh"});
    cboKhoangThoiGian->setCurrentIndex(2); // Mặc định: Tháng này
    cboKhoangThoiGian->setFixedWidth(150);
    
    dateFrom = new QDateEdit(QDate::currentDate().addDays(-30));
    dateFrom->setCalendarPopup(true);
    dateFrom->setDisplayFormat("dd/MM/yyyy");
    dateFrom->setEnabled(false);
    
    dateTo = new QDateEdit(QDate::currentDate());
    dateTo->setCalendarPopup(true);
    dateTo->setDisplayFormat("dd/MM/yyyy");
    dateTo->setEnabled(false);
    
    btnThongKe = new QPushButton("📊 Xem Thống Kê");
    btnThongKe->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 10px 25px; border-radius: 5px;");
    
    filterLayout->addWidget(new QLabel("Khoảng:"));
    filterLayout->addWidget(cboKhoangThoiGian);
    filterLayout->addWidget(new QLabel("     Từ:"));
    filterLayout->addWidget(dateFrom);
    filterLayout->addWidget(new QLabel("Đến:"));
    filterLayout->addWidget(dateTo);
    filterLayout->addWidget(btnThongKe);
    filterLayout->addStretch();
    
    mainLayout->addWidget(groupFilter);
    
    // ========== PHẦN 2: TỔNG QUAN ==========
    QGroupBox *groupTongQuan = new QGroupBox("📈 Tổng Quan");
    QGridLayout *gridTongQuan = new QGridLayout(groupTongQuan);
    
    lblDoanhThuKhoang = new QLabel("0 VNĐ");
    lblTongBooking = new QLabel("0");
    lblDoanhThuTB = new QLabel("0 VNĐ");
    lblTyLeHuy = new QLabel("0%");
    lblDoanhThuNgay = new QLabel("0 VNĐ");
    lblDoanhThuThang = new QLabel("0 VNĐ");
    lblDoanhThuQuy = new QLabel("0 VNĐ");
    lblDoanhThuNam = new QLabel("0 VNĐ");
    
    QString labelStyle = "font-size: 16px; font-weight: bold; color: #2196F3; padding: 5px; border: 2px solid #E3F2FD; border-radius: 5px; background: #F5F5F5;";
    QString labelStyleGreen = "font-size: 20px; font-weight: bold; color: #4CAF50; padding: 8px; border: 2px solid #C8E6C9; border-radius: 5px; background: #F1F8E9;";
    
    lblDoanhThuKhoang->setStyleSheet(labelStyleGreen);
    lblDoanhThuNgay->setStyleSheet(labelStyle);
    lblDoanhThuThang->setStyleSheet(labelStyle);
    lblDoanhThuQuy->setStyleSheet(labelStyle);
    lblDoanhThuNam->setStyleSheet(labelStyle);
    lblTongBooking->setStyleSheet(labelStyle);
    lblDoanhThuTB->setStyleSheet(labelStyle);
    lblTyLeHuy->setStyleSheet(labelStyle);
    
    int row = 0;
    gridTongQuan->addWidget(new QLabel("<b>💎 DOANH THU KHOẢNG:</b>"), row, 0);
    gridTongQuan->addWidget(lblDoanhThuKhoang, row, 1);
    gridTongQuan->addWidget(new QLabel("<b>📋 Tổng Booking:</b>"), row, 2);
    gridTongQuan->addWidget(lblTongBooking, row, 3);
    
    row++;
    gridTongQuan->addWidget(new QLabel("<b>💵 DT TB/Booking:</b>"), row, 0);
    gridTongQuan->addWidget(lblDoanhThuTB, row, 1);
    gridTongQuan->addWidget(new QLabel("<b>❌ Tỷ Lệ Hủy:</b>"), row, 2);
    gridTongQuan->addWidget(lblTyLeHuy, row, 3);
    
    row++;
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    gridTongQuan->addWidget(line, row, 0, 1, 4);
    
    row++;
    gridTongQuan->addWidget(new QLabel("<b>💰 DT Hôm Nay:</b>"), row, 0);
    gridTongQuan->addWidget(lblDoanhThuNgay, row, 1);
    gridTongQuan->addWidget(new QLabel("<b>💰 DT Tháng Này:</b>"), row, 2);
    gridTongQuan->addWidget(lblDoanhThuThang, row, 3);
    
    row++;
    gridTongQuan->addWidget(new QLabel("<b>💰 DT Quý Này:</b>"), row, 0);
    gridTongQuan->addWidget(lblDoanhThuQuy, row, 1);
    gridTongQuan->addWidget(new QLabel("<b>💰 DT Năm Này:</b>"), row, 2);
    gridTongQuan->addWidget(lblDoanhThuNam, row, 3);
    
    mainLayout->addWidget(groupTongQuan);
    
    // ========== PHẦN 3: TABS CHI TIẾT ==========
    QTabWidget *detailTabs = new QTabWidget();
    detailTabs->setStyleSheet("QTabWidget::pane { border: 1px solid #ccc; } QTabBar::tab:selected { background: #4CAF50; color: white; }");
    
    // Tab 1: Thống kê sân
    QWidget *tabSan = new QWidget();
    QVBoxLayout *layoutSan = new QVBoxLayout(tabSan);
    QLabel *titleSan = new QLabel("<h3 style='color: #4CAF50;'>🏟️ Top Sân Hot Nhất (Theo Tần Suất & Doanh Thu)</h3>");
    layoutSan->addWidget(titleSan);
    
    tableThongKeSan = new QTableWidget();
    tableThongKeSan->setColumnCount(5);
    tableThongKeSan->setHorizontalHeaderLabels({"Mã Sân", "Tên Sân", "Số Lần Đặt", "Doanh Thu (VNĐ)", "Tỷ Lệ Lấp Đầy (%)"});
    tableThongKeSan->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableThongKeSan->setSelectionBehavior(QTableWidget::SelectRows);
    tableThongKeSan->setEditTriggers(QTableWidget::NoEditTriggers);
    tableThongKeSan->setAlternatingRowColors(true);
    layoutSan->addWidget(tableThongKeSan);
    detailTabs->addTab(tabSan, "🏟️ Thống Kê Sân");
    
    // Tab 2: Khung giờ
    QWidget *tabGio = new QWidget();
    QVBoxLayout *layoutGio = new QVBoxLayout(tabGio);
    QLabel *titleGio = new QLabel("<h3 style='color: #FF9800;'>⏰ Phân Tích Khung Giờ Đông Khách</h3>");
    layoutGio->addWidget(titleGio);
    
    tableKhungGio = new QTableWidget();
    tableKhungGio->setColumnCount(3);
    tableKhungGio->setHorizontalHeaderLabels({"Khung Giờ", "Số Lần Đặt", "Doanh Thu (VNĐ)"});
    tableKhungGio->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableKhungGio->setSelectionBehavior(QTableWidget::SelectRows);
    tableKhungGio->setEditTriggers(QTableWidget::NoEditTriggers);
    tableKhungGio->setAlternatingRowColors(true);
    layoutGio->addWidget(tableKhungGio);
    detailTabs->addTab(tabGio, "⏰ Khung Giờ");
    
    // Tab 3: Top khách VIP
    QWidget *tabKhach = new QWidget();
    QVBoxLayout *layoutKhach = new QVBoxLayout(tabKhach);
    QLabel *titleKhach = new QLabel("<h3 style='color: #9C27B0;'>👑 Top 10 Khách Hàng VIP (Chi Tiêu Nhiều Nhất)</h3>");
    layoutKhach->addWidget(titleKhach);
    
    tableTopKhach = new QTableWidget();
    tableTopKhach->setColumnCount(5);
    tableTopKhach->setHorizontalHeaderLabels({"Hạng", "Mã KH", "Tên Khách Hàng", "Số Lần Đặt", "Tổng Chi Tiêu (VNĐ)"});
    tableTopKhach->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableTopKhach->setSelectionBehavior(QTableWidget::SelectRows);
    tableTopKhach->setEditTriggers(QTableWidget::NoEditTriggers);
    tableTopKhach->setAlternatingRowColors(true);
    layoutKhach->addWidget(tableTopKhach);
    detailTabs->addTab(tabKhach, "👑 Top Khách VIP");
    
    mainLayout->addWidget(detailTabs);
    
    // Connect signals
    connect(cboKhoangThoiGian, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &ThongKeDialog::onKhoangThoiGianChanged);
    connect(btnThongKe, &QPushButton::clicked, this, &ThongKeDialog::onThongKeClicked);
}

void ThongKeDialog::onKhoangThoiGianChanged(int index)
{
    bool isCustom = (index == 5); // "Tùy Chỉnh"
    dateFrom->setEnabled(isCustom);
    dateTo->setEnabled(isCustom);
    
    if (!isCustom)
    {
        QDate today = QDate::currentDate();
        QDate from, to = today;
        
        switch(index)
        {
        case 0: // Hôm nay
            from = today;
            break;
        case 1: // 7 ngày qua
            from = today.addDays(-6);
            break;
        case 2: // Tháng này
            from = QDate(today.year(), today.month(), 1);
            break;
        case 3: // Quý này
            {
                int quy = (today.month() - 1) / 3;
                int thangDau = quy * 3 + 1;
                from = QDate(today.year(), thangDau, 1);
            }
            break;
        case 4: // Năm này
            from = QDate(today.year(), 1, 1);
            break;
        }
        
        dateFrom->setDate(from);
        dateTo->setDate(to);
    }
}

void ThongKeDialog::onThongKeClicked()
{
    QDate from = dateFrom->date();
    QDate to = dateTo->date();
    
    if (from > to)
    {
        QMessageBox::warning(this, "Lỗi", "Ngày bắt đầu phải nhỏ hơn ngày kết thúc!");
        return;
    }
    
    // Chuyển QDate sang time_t
    QDateTime dtFrom(from, QTime(0, 0, 0));
    QDateTime dtTo(to, QTime(23, 59, 59));
    
    time_t tuNgay = dtFrom.toSecsSinceEpoch();
    time_t denNgay = dtTo.toSecsSinceEpoch();
    
    capNhatThongKe(tuNgay, denNgay);
}

void ThongKeDialog::capNhatThongKe(time_t tuNgay, time_t denNgay)
{
    // ========== TỔNG QUAN ==========
    double doanhThuKhoang = quanLy->tinhDoanhThuKhoang(tuNgay, denNgay);
    int tongBooking = quanLy->demTongLichDat(tuNgay, denNgay);
    double doanhThuTB = quanLy->tinhDoanhThuTrungBinh(tuNgay, denNgay);
    double tyLeHuy = quanLy->tinhTyLeHuyLich(tuNgay, denNgay);
    
    lblDoanhThuKhoang->setText(QString::number(doanhThuKhoang, 'f', 0) + " VNĐ");
    lblTongBooking->setText(QString::number(tongBooking));
    lblDoanhThuTB->setText(QString::number(doanhThuTB, 'f', 0) + " VNĐ");
    lblTyLeHuy->setText(QString::number(tyLeHuy, 'f', 1) + "%");
    
    // Doanh thu theo kỳ
    time_t now = time(nullptr);
    tm *t = localtime(&now);
    
    double dtNgay = quanLy->tinhDoanhThuNgay(now);
    double dtThang = quanLy->tinhDoanhThuThang(t->tm_mon + 1, t->tm_year + 1900);
    int quy = (t->tm_mon / 3) + 1;
    double dtQuy = quanLy->tinhDoanhThuQuy(quy, t->tm_year + 1900);
    double dtNam = quanLy->tinhDoanhThuNam(t->tm_year + 1900);
    
    lblDoanhThuNgay->setText(QString::number(dtNgay, 'f', 0) + " VNĐ");
    lblDoanhThuThang->setText(QString::number(dtThang, 'f', 0) + " VNĐ");
    lblDoanhThuQuy->setText(QString::number(dtQuy, 'f', 0) + " VNĐ");
    lblDoanhThuNam->setText(QString::number(dtNam, 'f', 0) + " VNĐ");
    
    // ========== THỐNG KÊ SÂN ==========
    MangDong<ThongKeSan> tkSan = quanLy->thongKeTanSuatSan(tuNgay, denNgay);
    tableThongKeSan->setRowCount(0);
    
    for (int i = 0; i < tkSan.getKichThuoc(); i++)
    {
        const ThongKeSan &tk = tkSan[i];
        int row = tableThongKeSan->rowCount();
        tableThongKeSan->insertRow(row);
        
        tableThongKeSan->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(tk.maSan)));
        tableThongKeSan->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(tk.tenSan)));
        tableThongKeSan->setItem(row, 2, new QTableWidgetItem(QString::number(tk.soLanDat)));
        tableThongKeSan->setItem(row, 3, new QTableWidgetItem(QString::number(tk.doanhThu, 'f', 0)));
        tableThongKeSan->setItem(row, 4, new QTableWidgetItem(QString::number(tk.tyLelapDay, 'f', 1) + "%"));
        
        // Highlight sân hot (tỷ lệ lấp đầy > 50%)
        if (tk.tyLelapDay > 50.0)
        {
            for (int col = 0; col < 5; col++)
                tableThongKeSan->item(row, col)->setBackground(QBrush(QColor(255, 235, 59, 100)));
        }
    }
    
    // ========== THỐNG KÊ KHUNG GIỜ ==========
    MangDong<ThongKeKhungGio> tkGio = quanLy->thongKeKhungGio(tuNgay, denNgay);
    tableKhungGio->setRowCount(0);
    
    for (int i = 0; i < tkGio.getKichThuoc(); i++)
    {
        const ThongKeKhungGio &tk = tkGio[i];
        int row = tableKhungGio->rowCount();
        tableKhungGio->insertRow(row);
        
        QString khungGio = QString("%1:00 - %2:00").arg(tk.gio).arg(tk.gio + 1);
        tableKhungGio->setItem(row, 0, new QTableWidgetItem(khungGio));
        tableKhungGio->setItem(row, 1, new QTableWidgetItem(QString::number(tk.soLanDat)));
        tableKhungGio->setItem(row, 2, new QTableWidgetItem(QString::number(tk.doanhThu, 'f', 0)));
        
        // Highlight giờ vàng (> 10 lần đặt)
        if (tk.soLanDat > 10)
        {
            for (int col = 0; col < 3; col++)
                tableKhungGio->item(row, col)->setBackground(QBrush(QColor(76, 175, 80, 100)));
        }
    }
    
    // ========== TOP KHÁCH VIP ==========
    MangDong<ThongKeKhachHang> topKH = quanLy->topKhachHangVIP(10, tuNgay, denNgay);
    tableTopKhach->setRowCount(0);
    
    for (int i = 0; i < topKH.getKichThuoc(); i++)
    {
        const ThongKeKhachHang &tk = topKH[i];
        int row = tableTopKhach->rowCount();
        tableTopKhach->insertRow(row);
        
        QString medal = (i == 0) ? "🥇" : (i == 1) ? "🥈" : (i == 2) ? "🥉" : QString::number(i + 1);
        
        tableTopKhach->setItem(row, 0, new QTableWidgetItem(medal));
        tableTopKhach->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(tk.maKH)));
        tableTopKhach->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(tk.hoTen)));
        tableTopKhach->setItem(row, 3, new QTableWidgetItem(QString::number(tk.soLanDat)));
        tableTopKhach->setItem(row, 4, new QTableWidgetItem(QString::number(tk.tongChiTieu, 'f', 0)));
        
        // Highlight top 3
        if (i < 3)
        {
            QColor color = (i == 0) ? QColor(255, 215, 0, 100) :  // Gold
                          (i == 1) ? QColor(192, 192, 192, 100) : // Silver
                                    QColor(205, 127, 50, 100);    // Bronze
            for (int col = 0; col < 5; col++)
                tableTopKhach->item(row, col)->setBackground(QBrush(color));
        }
    }
}
