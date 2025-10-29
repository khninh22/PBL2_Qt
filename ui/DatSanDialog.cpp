#include "DatSanDialog.h"
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QDateTime>

DatSanDialog::DatSanDialog(QuanLyThueSan *ql, QWidget *parent)
    : QDialog(parent), quanLy(ql)
{
    setWindowTitle("Đặt Sân Mới");
    setMinimumWidth(500);

    QFormLayout *formLayout = new QFormLayout();

    // Khách hàng
    cboKhachHang = new QComboBox();
    const MangDong<KhachHang> &dsKH = quanLy->getDsKhachHang();
    for (int i = 0; i < dsKH.getKichThuoc(); i++)
    {
        QString item = QString("%1 - %2")
                           .arg(QString::fromStdString(dsKH[i].getMaKH()))
                           .arg(QString::fromStdString(dsKH[i].getHoTen()));
        cboKhachHang->addItem(item, QString::fromStdString(dsKH[i].getMaKH()));
    }

    // Loại sân
    cboLoaiSan = new QComboBox();
    cboLoaiSan->addItems({"5", "7", "11"});

    // Thời gian
    dteBatDau = new QDateTimeEdit();
    dteBatDau->setDateTime(QDateTime::currentDateTime());
    dteBatDau->setDisplayFormat("dd/MM/yyyy HH:mm");

    dteKetThuc = new QDateTimeEdit();
    dteKetThuc->setDateTime(QDateTime::currentDateTime().addSecs(3600));
    dteKetThuc->setDisplayFormat("dd/MM/yyyy HH:mm");

    // Sân trống
    cboSanTrong = new QComboBox();

    formLayout->addRow("Khách Hàng:", cboKhachHang);
    formLayout->addRow("Loại Sân:", cboLoaiSan);
    formLayout->addRow("Thời Gian Bắt Đầu:", dteBatDau);
    formLayout->addRow("Thời Gian Kết Thúc:", dteKetThuc);

    QPushButton *btnTimSan = new QPushButton("🔍 Tìm Sân Trống");
    formLayout->addRow("", btnTimSan);
    formLayout->addRow("Chọn Sân:", cboSanTrong);

    QPushButton *btnDatSan = new QPushButton("✅ Đặt Sân");
    QPushButton *btnHuy = new QPushButton("❌ Hủy");

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnDatSan);
    btnLayout->addWidget(btnHuy);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);

    connect(btnTimSan, &QPushButton::clicked, this, &DatSanDialog::onTimSanTrong);
    connect(btnDatSan, &QPushButton::clicked, this, &DatSanDialog::onDatSan);
    connect(btnHuy, &QPushButton::clicked, this, &QDialog::reject);
}

void DatSanDialog::onTimSanTrong()
{
    cboSanTrong->clear();

    int loaiSan = cboLoaiSan->currentText().toInt();
    time_t batDau = dteBatDau->dateTime().toSecsSinceEpoch();
    time_t ketThuc = dteKetThuc->dateTime().toSecsSinceEpoch();

    MangDong<SanBong> sanTrong = quanLy->timSanTrong(loaiSan, batDau, ketThuc);

    if (sanTrong.getKichThuoc() == 0)
    {
        QMessageBox::information(this, "Thông báo", "Không có sân trống trong khung giờ này!");
        return;
    }

    for (int i = 0; i < sanTrong.getKichThuoc(); i++)
    {
        QString item = QString("%1 - %2 (%3 VNĐ)")
                           .arg(QString::fromStdString(sanTrong[i].getMaSan()))
                           .arg(QString::fromStdString(sanTrong[i].getTenSan()))
                           .arg(sanTrong[i].getGiaThue(), 0, 'f', 0);
        cboSanTrong->addItem(item, QString::fromStdString(sanTrong[i].getMaSan()));
    }
}

void DatSanDialog::onDatSan()
{
    if (cboSanTrong->count() == 0)
    {
        QMessageBox::warning(this, "Cảnh báo", "Vui lòng tìm sân trống trước!");
        return;
    }

    std::string maKH = cboKhachHang->currentData().toString().toStdString();
    std::string maSan = cboSanTrong->currentData().toString().toStdString();
    time_t batDau = dteBatDau->dateTime().toSecsSinceEpoch();
    time_t ketThuc = dteKetThuc->dateTime().toSecsSinceEpoch();

    std::string maLichMoi;
    if (quanLy->datSan(maKH, maSan, batDau, ketThuc, maLichMoi))
    {
        accept();
    }
    else
    {
        QMessageBox::warning(this, "Lỗi", "Không thể đặt sân!");
    }
}
