#include "SanBookingDialog.h"
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <QPainterPath>
#include <QStandardItemModel>
#include <ctime>

// ============================================================================
// TimeSlotWidget Implementation
// ============================================================================

TimeSlotWidget::TimeSlotWidget(const QString &maSan, int gioStart, double duration,
                               bool isBooked, QWidget *parent)
    : QWidget(parent), maSan(maSan), gioStart(gioStart), duration(duration),
      booked(isBooked), selected(false), hovered(false)
{
    setMinimumSize(85, 80); // ✅ Giảm chiều rộng để gọn gàng hơn
    setCursor(booked ? Qt::ForbiddenCursor : Qt::PointingHandCursor);
    setToolTip(QString("%1 - %2:%3").arg(maSan).arg(gioStart).arg("00"));
}

void TimeSlotWidget::setBooked(bool book)
{
    booked = book;
    setCursor(booked ? Qt::ForbiddenCursor : Qt::PointingHandCursor);
    update();
}

void TimeSlotWidget::setSelected(bool sel)
{
    selected = sel;
    update();
}

void TimeSlotWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect = this->rect().adjusted(2, 2, -2, -2);

    // Vẽ hình sân bóng
    QPainterPath fieldPath;
    fieldPath.addRoundedRect(rect, 8, 8);

    // Màu nền
    QColor bgColor;
    if (booked)
    {
        bgColor = QColor(239, 83, 80); // Đỏ - Đã đặt
    }
    else if (selected)
    {
        bgColor = QColor(255, 193, 7); // Vàng - Đang chọn
    }
    else if (hovered)
    {
        bgColor = QColor(102, 187, 106); // Xanh nhạt - Hover
    }
    else
    {
        bgColor = QColor(76, 175, 80); // Xanh - Trống
    }

    painter.fillPath(fieldPath, bgColor);

    // Vẽ viền
    painter.setPen(QPen(Qt::white, 2));
    painter.drawRoundedRect(rect, 8, 8);

    // Vẽ các đường kẻ sân như hình bạn gửi
    int centerX = rect.center().x();
    int centerY = rect.center().y();
    int width = rect.width();
    int height = rect.height();

    painter.setPen(QPen(Qt::white, 1.5));

    // Vẽ đường giữa
    painter.drawLine(centerX, rect.top() + 5, centerX, rect.bottom() - 5);

    // Vẽ vòng tròn giữa
    painter.drawEllipse(QPoint(centerX, centerY), height / 6, height / 6);
    painter.drawPoint(centerX, centerY);

    // Vẽ khu vực phạt đền (penalty areas)
    int penaltyWidth = width / 6;
    int penaltyHeight = height / 3;

    // Khu vực phạt đền trái
    painter.drawRect(rect.left() + 5, centerY - penaltyHeight / 2, penaltyWidth, penaltyHeight);

    // Khu vực phạt đền phải
    painter.drawRect(rect.right() - penaltyWidth - 5, centerY - penaltyHeight / 2, penaltyWidth, penaltyHeight);

    // Vẽ cầu môn (goal)
    int goalWidth = penaltyWidth / 3;
    int goalHeight = height / 5;

    // Cầu môn trái
    painter.drawRect(rect.left() + 2, centerY - goalHeight / 2, goalWidth, goalHeight);

    // Cầu môn phải
    painter.drawRect(rect.right() - goalWidth - 2, centerY - goalHeight / 2, goalWidth, goalHeight);

    // ✅ Vẽ trạng thái (không vẽ giờ)
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setBold(true);
    font.setPointSize(9);
    painter.setFont(font);

    QString status = booked ? "ĐÃ ĐẶT" : (selected ? "ĐANG CHỌN" : "TRỐNG");
    painter.drawText(rect.adjusted(0, 0, 0, -5), Qt::AlignBottom | Qt::AlignHCenter, status);
}

void TimeSlotWidget::mousePressEvent(QMouseEvent *event)
{
    if (!booked && event->button() == Qt::LeftButton)
    {
        emit clicked(this);
    }
}

void TimeSlotWidget::enterEvent(QEnterEvent *event)
{
    if (!booked)
    {
        hovered = true;
        update();
    }
}

void TimeSlotWidget::leaveEvent(QEvent *event)
{
    hovered = false;
    update();
}

// ============================================================================
// SanBookingDialog Implementation
// ============================================================================

SanBookingDialog::SanBookingDialog(QuanLyThueSan *ql, QWidget *parent)
    : QDialog(parent), quanLy(ql), currentDate(QDate::currentDate()),
      loaiSanFilter(0), gioLocBatDau(0), gioLocKetThuc(0) // ✅ Khởi tạo bộ lọc giờ
{
    setWindowTitle("📅 Đặt Sân - Lịch Trực Quan");
    setMinimumSize(1400, 800); // Tăng kích thước tối thiểu
    setupUI();
    loadSanBookingGrid();

    // ✅ Mở full màn hình (maximized)
    showMaximized();
}

void SanBookingDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // ===== HEADER: Filters =====
    QHBoxLayout *filterLayout = new QHBoxLayout();

    filterLayout->addWidget(new QLabel("📅 Ngày:"));
    dateEdit = new QDateEdit(currentDate);
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setCalendarPopup(true);
    filterLayout->addWidget(dateEdit);

    filterLayout->addSpacing(20);

    filterLayout->addWidget(new QLabel("⚽ Loại Sân:"));
    cboLoaiSan = new QComboBox();
    cboLoaiSan->addItem("Tất cả", 0);
    cboLoaiSan->addItem("Sân 5 người", 5);
    cboLoaiSan->addItem("Sân 7 người", 7);
    cboLoaiSan->addItem("Sân 11 người", 11);
    filterLayout->addWidget(cboLoaiSan);

    filterLayout->addSpacing(20);

    // ✅ MỚI: Lọc theo giờ
    filterLayout->addWidget(new QLabel("🕐 Lọc Giờ:"));
    cboGioLoc = new QComboBox();
    cboGioLoc->addItem("Tất cả (6:00 - 22:00)", 0);
    cboGioLoc->addItem("────────────────", -1); // Separator
    cboGioLoc->addItem("06:00 - 07:00", 6);
    cboGioLoc->addItem("07:00 - 08:00", 7);
    cboGioLoc->addItem("08:00 - 09:00", 8);
    cboGioLoc->addItem("09:00 - 10:00", 9);
    cboGioLoc->addItem("10:00 - 11:00", 10);
    cboGioLoc->addItem("11:00 - 12:00", 11);
    cboGioLoc->addItem("12:00 - 13:00", 12);
    cboGioLoc->addItem("13:00 - 14:00", 13);
    cboGioLoc->addItem("14:00 - 15:00", 14);
    cboGioLoc->addItem("15:00 - 16:00", 15);
    cboGioLoc->addItem("16:00 - 17:00", 16);
    cboGioLoc->addItem("17:00 - 18:00", 17);
    cboGioLoc->addItem("18:00 - 19:00", 18);
    cboGioLoc->addItem("19:00 - 20:00", 19);
    cboGioLoc->addItem("20:00 - 21:00", 20);
    cboGioLoc->addItem("21:00 - 22:00", 21);

    // Vô hiệu hóa item phân cách
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(cboGioLoc->model());
    if (model)
    {
        QStandardItem *separator = model->item(1); // "────────────────"
        if (separator)
        {
            separator->setFlags(separator->flags() & ~Qt::ItemIsEnabled);
            separator->setData(QColor(150, 150, 150), Qt::ForegroundRole);
        }
    }

    filterLayout->addWidget(cboGioLoc);

    filterLayout->addSpacing(20);

    // ✅ Chọn thời lượng đặt sân
    filterLayout->addWidget(new QLabel("⏱️ Thời lượng:"));
    cboThoiLuong = new QComboBox();
    cboThoiLuong->addItem("1 giờ", 1.0);
    cboThoiLuong->addItem("1.5 giờ", 1.5);
    cboThoiLuong->addItem("2 giờ", 2.0);
    cboThoiLuong->setCurrentIndex(0); // Mặc định: 1 giờ
    cboThoiLuong->setToolTip("Chọn thời lượng muốn đặt sân (tối thiểu 1 giờ).\nKhi click vào ô bắt đầu, hệ thống sẽ tự động chọn các ô liên tiếp.");
    filterLayout->addWidget(cboThoiLuong);

    filterLayout->addSpacing(20);

    QPushButton *btnRefresh = new QPushButton("🔄 Làm mới");
    btnRefresh->setStyleSheet("background-color: #2196F3; color: white; padding: 8px 15px; border-radius: 5px;");
    filterLayout->addWidget(btnRefresh);

    filterLayout->addStretch();

    mainLayout->addLayout(filterLayout);

    // ===== LEGEND =====
    QHBoxLayout *legendLayout = new QHBoxLayout();
    legendLayout->addWidget(new QLabel("🟢 <b>Xanh</b>: Trống"));
    legendLayout->addWidget(new QLabel("🔴 <b>Đỏ</b>: Đã đặt"));
    legendLayout->addWidget(new QLabel("🟡 <b>Vàng</b>: Đang chọn"));
    legendLayout->addStretch();
    mainLayout->addLayout(legendLayout);

    // ===== BOOKING GRID =====
    bookingGrid = new QTableWidget();
    bookingGrid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bookingGrid->setSelectionMode(QAbstractItemView::NoSelection);

    // ✅ Tối ưu scroll mượt mà
    bookingGrid->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    bookingGrid->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    // ✅ Resize mode cho header
    bookingGrid->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    bookingGrid->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    // ✅ Tối ưu kích thước
    bookingGrid->setMinimumHeight(450);
    bookingGrid->setMinimumWidth(800);

    // ✅ Đổi màu header + tối ưu hiển thị
    bookingGrid->setStyleSheet(
        "QTableWidget {"
        "    gridline-color: #ddd;"
        "    background-color: white;"
        "}"
        "QHeaderView::section {"
        "    background-color: #34495e;"
        "    color: white;"
        "    padding: 6px;"
        "    border: 1px solid #2c3e50;"
        "    font-weight: bold;"
        "    font-size: 11px;"
        "}"
        "QHeaderView::section:horizontal {"
        "    background-color: #2c3e50;"
        "}"
        "QHeaderView::section:vertical {"
        "    background-color: #34495e;"
        "}");

    mainLayout->addWidget(bookingGrid);

    // ===== BOOKING INFO =====
    QHBoxLayout *infoLayout = new QHBoxLayout();

    infoLayout->addWidget(new QLabel("👤 Khách hàng:"));
    cboKhachHang = new QComboBox();
    const MangDong<KhachHang> &dsKH = quanLy->getDsKhachHang();
    for (int i = 0; i < dsKH.getKichThuoc(); i++)
    {
        QString item = QString("%1 - %2")
                           .arg(QString::fromStdString(dsKH[i].getMaKH()))
                           .arg(QString::fromStdString(dsKH[i].getHoTen()));
        cboKhachHang->addItem(item, QString::fromStdString(dsKH[i].getMaKH()));
    }
    infoLayout->addWidget(cboKhachHang);

    infoLayout->addSpacing(20);

    lblThongTin = new QLabel("Chưa chọn khung giờ nào");
    lblThongTin->setStyleSheet("color: #666; font-style: italic;");
    infoLayout->addWidget(lblThongTin);

    infoLayout->addStretch();

    lblChiPhi = new QLabel("Tổng: 0 VNĐ");
    lblChiPhi->setStyleSheet("font-size: 16px; font-weight: bold; color: #4CAF50;");
    infoLayout->addWidget(lblChiPhi);

    mainLayout->addLayout(infoLayout);

    // ===== BUTTONS =====
    QHBoxLayout *btnLayout = new QHBoxLayout();

    btnClearSelection = new QPushButton("🗑️ Xóa chọn");
    btnClearSelection->setEnabled(false);
    btnClearSelection->setStyleSheet("background-color: #FF9800; color: white; padding: 10px 20px; border-radius: 5px;");
    btnLayout->addWidget(btnClearSelection);

    btnLayout->addStretch();

    btnDatSan = new QPushButton("✅ Xác Nhận Đặt Sân");
    btnDatSan->setEnabled(false);
    btnDatSan->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px 30px; border-radius: 5px; font-weight: bold;");
    btnLayout->addWidget(btnDatSan);

    QPushButton *btnHuy = new QPushButton("❌ Hủy");
    btnHuy->setStyleSheet("background-color: #f44336; color: white; padding: 10px 20px; border-radius: 5px;");
    btnLayout->addWidget(btnHuy);

    mainLayout->addLayout(btnLayout);

    // ===== CONNECTIONS =====
    connect(dateEdit, &QDateEdit::dateChanged, this, &SanBookingDialog::onDateChanged);
    connect(cboLoaiSan, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SanBookingDialog::onLoaiSanChanged);
    connect(cboGioLoc, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SanBookingDialog::onGioLocChanged); // ✅ MỚI
    connect(btnRefresh, &QPushButton::clicked, this, &SanBookingDialog::onRefresh);
    connect(btnClearSelection, &QPushButton::clicked, this, &SanBookingDialog::onClearSelection);
    connect(btnDatSan, &QPushButton::clicked, this, &SanBookingDialog::onDatSan);
    connect(btnHuy, &QPushButton::clicked, this, &QDialog::reject);
}

void SanBookingDialog::loadSanBookingGrid()
{
    bookingGrid->clear();
    selectedSlots.clear();

    // Lọc sân theo loại
    const MangDong<SanBong> &dsSan = quanLy->getDsSanBong();
    dsSanHienThi = MangDong<SanBong>();

    for (int i = 0; i < dsSan.getKichThuoc(); i++)
    {
        if (!dsSan[i].getBaoTri())
        {
            if (loaiSanFilter == 0 || dsSan[i].getLoaiSan() == loaiSanFilter)
            {
                dsSanHienThi.them(dsSan[i]);
            }
        }
    }

    if (dsSanHienThi.getKichThuoc() == 0)
    {
        QMessageBox::information(this, "Thông báo", "Không có sân nào khả dụng!");
        return;
    }

    // ✅ Tính toán số cột dựa trên bộ lọc giờ (mỗi giờ có 2 ô: xx:00 và xx:30)
    int gioStart = (gioLocBatDau == 0) ? GIO_BAT_DAU : gioLocBatDau;
    int gioEnd = (gioLocKetThuc == 0) ? GIO_KET_THUC : gioLocKetThuc;
    int soKhungGioHienThi = (gioEnd - gioStart) * 2; // x2 vì mỗi giờ có 2 ô (00 và 30 phút)

    // Setup table
    int numSans = dsSanHienThi.getKichThuoc();
    bookingGrid->setRowCount(numSans);
    bookingGrid->setColumnCount(soKhungGioHienThi);

    // Header cho cột (các khung giờ 30 phút)
    QStringList headers;
    for (int h = gioStart; h < gioEnd; h++)
    {
        headers << QString("%1:00-%2:30").arg(h).arg(h);
        headers << QString("%1:30-%2:00").arg(h).arg(h + 1);
    }
    bookingGrid->setHorizontalHeaderLabels(headers);

    // Header cho hàng (tên sân)
    QStringList rowHeaders;
    for (int i = 0; i < numSans; i++)
    {
        QString label = QString("%1\n%2\n(%3 người)")
                            .arg(QString::fromStdString(dsSanHienThi[i].getMaSan()))
                            .arg(QString::fromStdString(dsSanHienThi[i].getTenSan()))
                            .arg(dsSanHienThi[i].getLoaiSan());
        rowHeaders << label;
    }
    bookingGrid->setVerticalHeaderLabels(rowHeaders);

    // Tạo các TimeSlotWidget (mỗi giờ có 2 ô: xx:00 và xx:30)
    for (int row = 0; row < numSans; row++)
    {
        const SanBong &san = dsSanHienThi[row];
        QString maSan = QString::fromStdString(san.getMaSan());

        int col = 0;
        for (int h = gioStart; h < gioEnd; h++)
        {
            // Tạo 2 ô cho mỗi giờ: xx:00 và xx:30
            for (int minute = 0; minute < 60; minute += 30)
            {
                // Kiểm tra xem khung giờ này đã được đặt chưa
                bool isBooked = false;

                // Chuyển đổi ngày + giờ sang time_t
                QDateTime startDT(currentDate, QTime(h, minute));
                QDateTime endDT(currentDate, QTime(h, minute + 30));
                if (minute == 30) // Trường hợp xx:30, endTime là giờ tiếp theo
                {
                    endDT = QDateTime(currentDate, QTime(h + 1, 0));
                }
                time_t startTime = startDT.toSecsSinceEpoch();
                time_t endTime = endDT.toSecsSinceEpoch();

                // Kiểm tra xem có lịch đặt nào trùng không
                const MangDong<LichDatSan> &dsLich = quanLy->getDsLichDatSan();
                for (int i = 0; i < dsLich.getKichThuoc(); i++)
                {
                    if (dsLich[i].getMaSan() == san.getMaSan())
                    {
                        time_t lichStart = dsLich[i].getThoiGianBatDau();
                        time_t lichEnd = dsLich[i].getThoiGianKetThuc();

                        // Kiểm tra overlap
                        if ((startTime >= lichStart && startTime < lichEnd) ||
                            (endTime > lichStart && endTime <= lichEnd) ||
                            (startTime <= lichStart && endTime >= lichEnd))
                        {
                            isBooked = true;
                            break;
                        }
                    }
                }

                // Tạo widget với thông tin giờ và phút
                TimeSlotWidget *widget = new TimeSlotWidget(maSan, h, 0.5, isBooked);
                widget->setProperty("minute", minute); // Lưu thông tin phút

                // ✅ Cập nhật tooltip với thông tin phút
                QString timeRange = QString("%1:%2-%3:%4")
                                        .arg(h, 2, 10, QChar('0'))
                                        .arg(minute, 2, 10, QChar('0'))
                                        .arg(minute == 0 ? h : h + 1, 2, 10, QChar('0'))
                                        .arg(minute == 0 ? 30 : 0, 2, 10, QChar('0'));
                widget->setToolTip(QString("%1 - %2").arg(maSan).arg(timeRange));

                bookingGrid->setCellWidget(row, col, widget);

                connect(widget, &TimeSlotWidget::clicked, this, &SanBookingDialog::onTimeSlotClicked);

                col++;
            }
        }
    }

    bookingGrid->verticalHeader()->setDefaultSectionSize(85); // ✅ Giảm chiều cao hàng để gọn hơn

    // ✅ Đặt chiều rộng cột nhỏ gọn hơn
    for (int col = 0; col < soKhungGioHienThi; col++)
    {
        bookingGrid->setColumnWidth(col, 90); // Giảm từ 130 xuống 90 để vừa màn hình
    }

    // ✅ Đặt chiều rộng cho vertical header (tên sân)
    bookingGrid->verticalHeader()->setDefaultSectionSize(85);
    bookingGrid->verticalHeader()->setMinimumWidth(120);
}

void SanBookingDialog::onDateChanged()
{
    currentDate = dateEdit->date();
    loadSanBookingGrid();
}

void SanBookingDialog::onLoaiSanChanged()
{
    loaiSanFilter = cboLoaiSan->currentData().toInt();
    loadSanBookingGrid();
}

// ✅ MỚI: Xử lý lọc theo giờ
void SanBookingDialog::onGioLocChanged()
{
    int option = cboGioLoc->currentData().toInt();

    // Xử lý item phân cách (không làm gì)
    if (option == -1)
    {
        return;
    }

    if (option == 0)
    {
        // Tất cả (6-22)
        gioLocBatDau = 0;
        gioLocKetThuc = 0;
    }
    else if (option >= 6 && option <= 21)
    {
        // Theo giờ cụ thể (6 → 6-7, 7 → 7-8, ..., 21 → 21-22)
        gioLocBatDau = option;
        gioLocKetThuc = option + 1;
    }
    else
    {
        // Mặc định: Tất cả
        gioLocBatDau = 0;
        gioLocKetThuc = 0;
    }

    loadSanBookingGrid();
}

void SanBookingDialog::onRefresh()
{
    loadSanBookingGrid();
    QMessageBox::information(this, "Thông báo", "Đã làm mới dữ liệu!");
}

void SanBookingDialog::onTimeSlotClicked(TimeSlotWidget *widget)
{
    // Toggle selection
    if (widget->isBooked())
    {
        return; // Không thể chọn ô đã đặt
    }

    bool wasSelected = selectedSlots.contains(widget);

    if (wasSelected)
    {
        // Bỏ chọn
        selectedSlots.removeOne(widget);
        widget->setSelected(false);
    }
    else
    {
        // ✅ Tự động chọn các ô liên tiếp theo thời lượng đã chọn
        double thoiLuongDat = cboThoiLuong->currentData().toDouble();

        QString maSan = widget->getMaSan();
        int startHour = widget->getGioStart();
        int startMinute = widget->property("minute").toInt();
        int slotsNeeded = static_cast<int>(thoiLuongDat * 2); // Mỗi slot = 0.5 giờ (30 phút)

        QList<TimeSlotWidget *> slotsToSelect;

        // Tìm tất cả các widget trong bảng
        for (int row = 0; row < bookingGrid->rowCount(); row++)
        {
            for (int col = 0; col < bookingGrid->columnCount(); col++)
            {
                QWidget *cellWidget = bookingGrid->cellWidget(row, col);
                if (!cellWidget)
                    continue;

                TimeSlotWidget *slot = qobject_cast<TimeSlotWidget *>(cellWidget);
                if (!slot)
                    continue;

                // Chỉ chọn các ô cùng sân
                if (slot->getMaSan() != maSan)
                    continue;

                // Tính toán thứ tự của slot (bắt đầu từ ô được click)
                int slotHour = slot->getGioStart();
                int slotMinute = slot->property("minute").toInt();

                // Chuyển đổi sang số slot (mỗi giờ = 2 slots)
                int startSlotIndex = startHour * 2 + (startMinute / 30);
                int currentSlotIndex = slotHour * 2 + (slotMinute / 30);
                int slotDiff = currentSlotIndex - startSlotIndex;

                // Kiểm tra xem slot này có nằm trong khoảng cần chọn không
                if (slotDiff >= 0 && slotDiff < slotsNeeded)
                {
                    // Kiểm tra ô này đã bị đặt chưa
                    if (!slot->isBooked())
                    {
                        slotsToSelect.append(slot);
                    }
                    else
                    {
                        // Nếu có ô đã bị đặt trong khoảng thời gian, hiển thị cảnh báo
                        QMessageBox::warning(this, "Cảnh báo",
                                             QString("Không thể đặt sân %1 từ %2:%3 với thời lượng %4 giờ\n"
                                                     "Khung giờ %5:%6 đã có người đặt!")
                                                 .arg(maSan)
                                                 .arg(startHour, 2, 10, QChar('0'))
                                                 .arg(startMinute, 2, 10, QChar('0'))
                                                 .arg(thoiLuongDat)
                                                 .arg(slotHour, 2, 10, QChar('0'))
                                                 .arg(slotMinute, 2, 10, QChar('0')));
                        return;
                    }
                }
            }
        }

        // Kiểm tra đủ số slot cần thiết chưa
        if (slotsToSelect.size() < slotsNeeded)
        {
            QMessageBox::warning(this, "Cảnh báo",
                                 QString("Không thể đặt sân %1 từ %2:%3 với thời lượng %4 giờ\n"
                                         "Không đủ khung giờ trống liên tiếp hoặc vượt quá giờ hoạt động (6:00-22:00)")
                                     .arg(maSan)
                                     .arg(startHour, 2, 10, QChar('0'))
                                     .arg(startMinute, 2, 10, QChar('0'))
                                     .arg(thoiLuongDat));
            return;
        }

        // Chọn tất cả các slot
        for (TimeSlotWidget *slot : slotsToSelect)
        {
            if (!selectedSlots.contains(slot))
            {
                selectedSlots.append(slot);
                slot->setSelected(true);
            }
        }
    }

    // Update UI
    if (selectedSlots.isEmpty())
    {
        lblThongTin->setText("Chưa chọn khung giờ nào");
        lblChiPhi->setText("Tổng: 0 VNĐ");
        btnDatSan->setEnabled(false);
        btnClearSelection->setEnabled(false);
    }
    else
    {
        // Tính tổng chi phí
        double tongTien = 0;
        double tongGio = 0;
        QString sanInfo;
        QMap<QString, int> sanToSlotCount; // Group by sân, đếm số slot

        for (TimeSlotWidget *slot : selectedSlots)
        {
            QString maSan = slot->getMaSan();
            sanToSlotCount[maSan]++;
            tongGio += slot->getDuration();

            // Tìm giá sân
            for (int i = 0; i < dsSanHienThi.getKichThuoc(); i++)
            {
                if (QString::fromStdString(dsSanHienThi[i].getMaSan()) == maSan)
                {
                    tongTien += dsSanHienThi[i].getGiaThue() * slot->getDuration();
                    break;
                }
            }
        }

        // Format thông tin
        QStringList infoList;
        for (auto it = sanToSlotCount.begin(); it != sanToSlotCount.end(); ++it)
        {
            double gioCount = it.value() * 0.5; // Mỗi slot = 0.5 giờ
            infoList << QString("%1 (%2 giờ)").arg(it.key()).arg(gioCount);
        }

        lblThongTin->setText(QString("Đã chọn: %1 - Tổng %2 giờ")
                                 .arg(infoList.join(", "))
                                 .arg(tongGio));
        lblChiPhi->setText(QString("Tổng: %L1 VNĐ").arg(tongTien, 0, 'f', 0));
        btnDatSan->setEnabled(true);
        btnClearSelection->setEnabled(true);
    }
}

void SanBookingDialog::onClearSelection()
{
    for (TimeSlotWidget *widget : selectedSlots)
    {
        widget->setSelected(false);
    }
    selectedSlots.clear();

    lblThongTin->setText("Chưa chọn khung giờ nào");
    lblChiPhi->setText("Tổng: 0 VNĐ");
    btnDatSan->setEnabled(false);
    btnClearSelection->setEnabled(false);
}

void SanBookingDialog::onDatSan()
{
    if (selectedSlots.isEmpty())
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn ít nhất một khung giờ!");
        return;
    }

    QString maKH = cboKhachHang->currentData().toString();

    if (maKH.isEmpty())
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng chọn khách hàng!");
        return;
    }

    // ✅ MỚI: Xác nhận thông tin khách hàng trước khi đặt
    if (!xacNhanThongTinKhachHang())
    {
        return; // User hủy hoặc thông tin không đúng
    }

    // Group theo sân để đặt từng lịch
    QMap<QString, QList<TimeSlotWidget *>> sanToSlots;
    for (TimeSlotWidget *slot : selectedSlots)
    {
        sanToSlots[slot->getMaSan()].append(slot);
    }

    int successCount = 0;
    QStringList errors;

    QMapIterator<QString, QList<TimeSlotWidget *>> it(sanToSlots);
    while (it.hasNext())
    {
        it.next();
        QString maSan = it.key();
        QList<TimeSlotWidget *> slotList = it.value();

        // Sort theo giờ
        std::sort(slotList.begin(), slotList.end(), [](TimeSlotWidget *a, TimeSlotWidget *b)
                  { return a->getGioStart() < b->getGioStart(); });

        // Gộp các slot liên tiếp thành một booking
        int i = 0;
        while (i < slotList.size())
        {
            int startHour = slotList[i]->getGioStart();
            int endHour = startHour + 1;
            int j = i + 1;

            // Tìm các slot liên tiếp
            while (j < slotList.size() && slotList[j]->getGioStart() == endHour)
            {
                endHour++;
                j++;
            }

            // Tạo booking từ startHour đến endHour
            QDateTime startDT(currentDate, QTime(startHour, 0));
            QDateTime endDT(currentDate, QTime(endHour, 0));

            std::string maLichMoi;
            bool success = quanLy->datSan(
                maKH.toStdString(),
                maSan.toStdString(),
                startDT.toSecsSinceEpoch(),
                endDT.toSecsSinceEpoch(),
                maLichMoi);

            if (success)
            {
                successCount++;
            }
            else
            {
                // ✅ THÊM: Thông báo lỗi chi tiết hơn
                QString errorMsg;

                // Kiểm tra các lỗi có thể xảy ra
                time_t now = time(nullptr);
                time_t startTime = startDT.toSecsSinceEpoch();

                if (startTime < (now - 300))
                {
                    errorMsg = QString("%1: %2:00-%3:00 (Thời gian đã qua)")
                                   .arg(maSan)
                                   .arg(startHour)
                                   .arg(endHour);
                }
                else if (startHour < 6 || endHour > 22)
                {
                    errorMsg = QString("%1: %2:00-%3:00 (Ngoài giờ hoạt động 6:00-22:00)")
                                   .arg(maSan)
                                   .arg(startHour)
                                   .arg(endHour);
                }
                else if (endHour - startHour > 12)
                {
                    errorMsg = QString("%1: %2:00-%3:00 (Vượt quá 12 giờ)")
                                   .arg(maSan)
                                   .arg(startHour)
                                   .arg(endHour);
                }
                else
                {
                    errorMsg = QString("%1: %2:00-%3:00 (Sân đã được đặt hoặc không khả dụng)")
                                   .arg(maSan)
                                   .arg(startHour)
                                   .arg(endHour);
                }

                errors << errorMsg;
            }

            i = j;
        }
    }

    if (successCount > 0)
    {
        QString msg = QString("✅ Đã đặt thành công %1 lịch!").arg(successCount);
        if (!errors.isEmpty())
        {
            msg += QString("\n\n⚠️ Không đặt được:\n%1").arg(errors.join("\n"));
        }
        QMessageBox::information(this, "Thông báo", msg);

        if (errors.isEmpty())
        {
            accept();
        }
        else
        {
            loadSanBookingGrid(); // Refresh
        }
    }
    else
    {
        QString errorDetail = "Không thể đặt sân!\n\n";
        errorDetail += "Chi tiết lỗi:\n";
        errorDetail += errors.join("\n");
        QMessageBox::critical(this, "❌ Đặt Sân Thất Bại", errorDetail);
    }
}

QString SanBookingDialog::formatTime(int hour, int minute) const
{
    return QString("%1:%2").arg(hour, 2, 10, QChar('0')).arg(minute, 2, 10, QChar('0'));
}

// ✅ MỚI: Xác nhận thông tin khách hàng trước khi đặt sân
bool SanBookingDialog::xacNhanThongTinKhachHang()
{
    QString maKH = cboKhachHang->currentData().toString();

    // Tìm thông tin khách hàng
    KhachHang *kh = quanLy->timKhachHang(maKH.toStdString());
    if (!kh)
    {
        QMessageBox::critical(this, "❌ Lỗi", "Không tìm thấy thông tin khách hàng!");
        return false;
    }

    // Tính tổng tiền và số giờ đặt
    double tongTien = 0;
    int tongGio = selectedSlots.size();
    QStringList danhSachSan;
    QMap<QString, int> sanCount;

    for (TimeSlotWidget *slot : selectedSlots)
    {
        QString maSan = slot->getMaSan();
        sanCount[maSan]++;

        // Tìm giá sân
        for (int i = 0; i < dsSanHienThi.getKichThuoc(); i++)
        {
            if (QString::fromStdString(dsSanHienThi[i].getMaSan()) == maSan)
            {
                tongTien += dsSanHienThi[i].getGiaThue() * slot->getDuration();
                break;
            }
        }
    }

    // Format danh sách sân
    QMapIterator<QString, int> it(sanCount);
    while (it.hasNext())
    {
        it.next();
        danhSachSan << QString("  • %1: %2 giờ").arg(it.key()).arg(it.value());
    }

    // Tạo message xác nhận
    QString confirmMsg = "📋 XÁC NHẬN THÔNG TIN ĐẶT SÂN\n\n";
    confirmMsg += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    confirmMsg += "👤 KHÁCH HÀNG:\n";
    confirmMsg += QString("  • Mã KH: %1\n").arg(QString::fromStdString(kh->getMaKH()));
    confirmMsg += QString("  • Họ tên: %1\n").arg(QString::fromStdString(kh->getHoTen()));
    confirmMsg += QString("  • SĐT: %1\n").arg(QString::fromStdString(kh->getSdt()));
    confirmMsg += QString("  • Điểm tích lũy: %1 điểm\n").arg(kh->getDiemTichLuy());
    confirmMsg += QString("  • Cấp độ: %1\n\n").arg(QString::fromStdString(kh->getTenCapDo()));

    confirmMsg += "📅 THÔNG TIN ĐẶT SÂN:\n";
    confirmMsg += QString("  • Ngày: %1\n").arg(currentDate.toString("dd/MM/yyyy"));
    confirmMsg += QString("  • Tổng: %1 giờ\n").arg(tongGio);
    confirmMsg += "  • Sân:\n";
    confirmMsg += danhSachSan.join("\n") + "\n\n";

    confirmMsg += "💰 CHI PHÍ:\n";
    confirmMsg += QString("  • Tổng tiền: %L1 VNĐ\n\n").arg(tongTien, 0, 'f', 0);

    confirmMsg += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    confirmMsg += "⚠️ Vui lòng kiểm tra kỹ thông tin!\n";
    confirmMsg += "Bạn có chắc chắn muốn đặt sân?";

    // Hiển thị dialog xác nhận
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("✅ Xác Nhận Đặt Sân");
    msgBox.setText(confirmMsg);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.button(QMessageBox::Yes)->setText("✅ Xác Nhận Đặt");
    msgBox.button(QMessageBox::No)->setText("❌ Hủy");
    msgBox.setDefaultButton(QMessageBox::No); // Mặc định là No để tránh click nhầm

    int result = msgBox.exec();
    return (result == QMessageBox::Yes);
}
