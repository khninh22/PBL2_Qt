#include "KhachHangDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>

KhachHangDialog::KhachHangDialog(QWidget *parent, KhachHang *kh)
    : QDialog(parent)
{
    setWindowTitle(kh ? "Sửa Khách Hàng" : "Thêm Khách Hàng Mới");
    setMinimumWidth(400);
    
    // ✅ Cải thiện UI
    setStyleSheet(
        "QDialog { background-color: #f5f5f5; }"
        "QPushButton {"
        "    background-color: #2196F3; color: white; border: none;"
        "    padding: 10px 24px; border-radius: 5px; font-weight: bold;"
        "    font-size: 13px; min-width: 100px;"
        "}"
        "QPushButton:hover { background-color: #1976D2; }"
        "QLineEdit {"
        "    padding: 8px; border: 2px solid #BDBDBD; border-radius: 4px;"
        "    background-color: white; font-size: 13px;"
        "}"
        "QLineEdit:focus { border: 2px solid #2196F3; }"
    );

    QFormLayout *formLayout = new QFormLayout();

    txtMaKH = new QLineEdit();
    txtHoTen = new QLineEdit();
    txtSdt = new QLineEdit();

    if (kh)
    {
        txtMaKH->setText(QString::fromStdString(kh->getMaKH()));
        txtMaKH->setReadOnly(true);
        txtHoTen->setText(QString::fromStdString(kh->getHoTen()));
        txtSdt->setText(QString::fromStdString(kh->getSdt()));
    }

    formLayout->addRow("Mã Khách Hàng:", txtMaKH);
    formLayout->addRow("Họ Tên:", txtHoTen);
    formLayout->addRow("Số Điện Thoại:", txtSdt);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
}
