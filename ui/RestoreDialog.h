#ifndef RESTOREDIALOG_H
#define RESTOREDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "../core/managers/BackupManager.h"
#include "../core/QuanLyThueSan.h"

/**
 * @brief Dialog to select and restore from backups
 */
class RestoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RestoreDialog(QuanLyThueSan *quanLy, QWidget *parent = nullptr);
    QString getSelectedBackupName() const;

private slots:
    void onRestore();
    void onRefresh();

private:
    void setupUI();
    void loadBackupList();

    QuanLyThueSan *quanLy;
    QListWidget *listBackups;
    QPushButton *btnRestore;
    QPushButton *btnRefresh;
    QPushButton *btnCancel;
    QLabel *lblInfo;
    
    QString selectedBackup;
};

#endif // RESTOREDIALOG_H
