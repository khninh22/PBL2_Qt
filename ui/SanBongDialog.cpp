#include "SanBongDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>

SanBongDialog::SanBongDialog(QWidget *parent, SanBong *san)
    : QDialog(parent)
{
    setWindowTitle(san ? "Sửa Sân Bóng" : "Thêm Sân Bóng Mới");
    setMinimumWidth(400);

    QFormLayout *formLayout = new QFormLayout();

    txtMaSan = new QLineEdit();
    txtTenSan = new QLineEdit();
    cboLoaiSan = new QComboBox();
    spnGiaThue = new QDoubleSpinBox();

    cboLoaiSan->addItems({"5", "7", "11"});
    spnGiaThue->setRange(0, 10000000);
    spnGiaThue->setSuffix(" VNĐ");
    spnGiaThue->setDecimals(0);

    if (san)
    {
        txtMaSan->setText(QString::fromStdString(san->getMaSan()));
        txtMaSan->setReadOnly(true);
        txtTenSan->setText(QString::fromStdString(san->getTenSan()));
        cboLoaiSan->setCurrentText(QString::number(san->getLoaiSan()));
        spnGiaThue->setValue(san->getGiaThue());
    }

    formLayout->addRow("Mã Sân:", txtMaSan);
    formLayout->addRow("Tên Sân:", txtTenSan);
    formLayout->addRow("Loại Sân:", cboLoaiSan);
    formLayout->addRow("Giá Thuê:", spnGiaThue);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
}
