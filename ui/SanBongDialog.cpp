#include "SanBongDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMessageBox>

SanBongDialog::SanBongDialog(QuanLyThueSan *ql, QWidget *parent, SanBong *san)
    : QDialog(parent), quanLy(ql)
{
    setWindowTitle(san ? "Sửa Sân Bóng" : "Thêm Sân Bóng Mới");
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
        "QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {"
        "    padding: 8px; border: 2px solid #BDBDBD; border-radius: 4px;"
        "    background-color: white; font-size: 13px;"
        "}"
        "QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus {"
        "    border: 2px solid #2196F3;"
        "}"
    );

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
    else
    {
        // ✅ Auto-generate mã sân mới
        QString suggestedMa = generateMaSan();
        txtMaSan->setText(suggestedMa);
        txtMaSan->setPlaceholderText("VD: SB001");
        
        // ✅ Gợi ý mã
        lblSuggestion = new QLabel(QString("💡 Gợi ý: <b>%1</b>").arg(suggestedMa));
        lblSuggestion->setStyleSheet("color: #1976D2; font-size: 12px;");
    }

    formLayout->addRow("Mã Sân:", txtMaSan);
    if (!san && lblSuggestion)
        formLayout->addRow("", lblSuggestion);
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

QString SanBongDialog::generateMaSan()
{
    const MangDong<SanBong> &dsSan = quanLy->getDsSanBong();
    int maxNum = 0;

    for (int i = 0; i < dsSan.getKichThuoc(); i++)
    {
        QString maSan = QString::fromStdString(dsSan[i].getMaSan());
        if (maSan.startsWith("SB"))
        {
            bool ok;
            int num = maSan.mid(2).toInt(&ok);
            if (ok && num > maxNum)
                maxNum = num;
        }
    }

    return QString("SB%1").arg(maxNum + 1, 3, 10, QChar('0'));
}
