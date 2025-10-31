#include "DichVuDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>

DichVuDialog::DichVuDialog(QWidget *parent, DichVu *dv)
    : QDialog(parent)
{
    setWindowTitle(dv ? "Sửa Dịch Vụ" : "Thêm Dịch Vụ Mới");
    setMinimumWidth(400);

    QFormLayout *formLayout = new QFormLayout();

    txtMaDV = new QLineEdit();
    txtTenDV = new QLineEdit();
    cboLoaiDV = new QComboBox();
    spnGiaDV = new QDoubleSpinBox();

    cboLoaiDV->addItems({"Đồ Uống", "Đồ Ăn", "Khác"});
    spnGiaDV->setRange(0, 1000000);
    spnGiaDV->setSuffix(" VNĐ");
    spnGiaDV->setDecimals(0);

    if (dv)
    {
        txtMaDV->setText(QString::fromStdString(dv->getMaDV()));
        txtMaDV->setReadOnly(true);
        txtTenDV->setText(QString::fromStdString(dv->getTenDV()));
        cboLoaiDV->setCurrentText(QString::fromStdString(dv->getLoaiDV()));
        spnGiaDV->setValue(dv->getGiaDV());
    }

    formLayout->addRow("Mã Dịch Vụ:", txtMaDV);
    formLayout->addRow("Tên Dịch Vụ:", txtTenDV);
    formLayout->addRow("Loại Dịch Vụ:", cboLoaiDV);
    formLayout->addRow("Giá:", spnGiaDV);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
}
