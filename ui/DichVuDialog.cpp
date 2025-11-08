#include "DichVuDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>

DichVuDialog::DichVuDialog(QuanLyThueSan *ql, QWidget *parent, DichVu *dv)
    : QDialog(parent), quanLy(ql)
{
    setWindowTitle(dv ? "Sửa Dịch Vụ" : "Thêm Dịch Vụ Mới");
    setMinimumWidth(400);
    
    // ✅ Cải thiện UI dialog
    setStyleSheet(
        "QDialog {"
        "    background-color: #f5f5f5;"
        "}"
        "QPushButton {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "    padding: 10px 24px;"
        "    border-radius: 5px;"
        "    font-weight: bold;"
        "    font-size: 13px;"
        "    min-width: 100px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
        "QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {"
        "    padding: 8px;"
        "    border: 2px solid #BDBDBD;"
        "    border-radius: 4px;"
        "    background-color: white;"
        "    font-size: 13px;"
        "}"
        "QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus {"
        "    border: 2px solid #2196F3;"
        "}"
    );

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
    else
    {
        // ✅ Auto-generate mã dịch vụ mới
        QString suggestedMa = generateMaDV();
        txtMaDV->setText(suggestedMa);
        txtMaDV->setPlaceholderText("VD: DV001");
        
        // ✅ Gợi ý mã
        lblSuggestion = new QLabel(QString("💡 Gợi ý: <b>%1</b>").arg(suggestedMa));
        lblSuggestion->setStyleSheet("color: #1976D2; font-size: 12px;");
    }

    formLayout->addRow("Mã Dịch Vụ:", txtMaDV);
    if (!dv && lblSuggestion)
        formLayout->addRow("", lblSuggestion);
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

QString DichVuDialog::generateMaDV()
{
    const MangDong<DichVu> &dsDV = quanLy->getDsDichVu();
    int maxNum = 0;

    for (int i = 0; i < dsDV.getKichThuoc(); i++)
    {
        QString maDV = QString::fromStdString(dsDV[i].getMaDV());
        if (maDV.startsWith("DV"))
        {
            bool ok;
            int num = maDV.mid(2).toInt(&ok);
            if (ok && num > maxNum)
                maxNum = num;
        }
    }

    return QString("DV%1").arg(maxNum + 1, 3, 10, QChar('0'));
}
