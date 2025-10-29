#include "KhachHangDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>

KhachHangDialog::KhachHangDialog(QWidget *parent, KhachHang *kh)
    : QDialog(parent)
{
    setWindowTitle(kh ? "Sửa Khách Hàng" : "Thêm Khách Hàng Mới");
    setMinimumWidth(400);

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
