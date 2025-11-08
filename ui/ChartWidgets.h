#ifndef CHARTWIDGETS_H
#define CHARTWIDGETS_H

#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QPair>
#include <QString>

// ========== BIỂU ĐỒ TRÒN ==========
class PieChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PieChartWidget(QWidget *parent = nullptr);
    
    void setData(const QVector<QPair<QString, double>> &data);
    void setTitle(const QString &title) { m_title = title; update(); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPair<QString, double>> m_data;  // <Label, Value>
    QString m_title;
    QVector<QColor> m_colors;
};

// ========== BIỂU ĐỒ CỘT ==========
class BarChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BarChartWidget(QWidget *parent = nullptr);
    
    void setData(const QVector<QPair<QString, double>> &data);
    void setTitle(const QString &title) { m_title = title; update(); }
    void setColor(const QColor &color) { m_barColor = color; update(); }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPair<QString, double>> m_data;  // <Label, Value>
    QString m_title;
    QColor m_barColor;
};

#endif // CHARTWIDGETS_H
