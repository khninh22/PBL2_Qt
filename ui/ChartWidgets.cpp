#include "ChartWidgets.h"
#include <QPainter>
#include <QPainterPath>
#include <QFontMetrics>
#include <cmath>

// ========== PIE CHART ==========
PieChartWidget::PieChartWidget(QWidget *parent)
    : QWidget(parent)
{
    // Màu sắc đẹp cho biểu đồ tròn
    m_colors = {
        QColor(76, 175, 80),   // Green
        QColor(33, 150, 243),  // Blue
        QColor(255, 152, 0),   // Orange
        QColor(156, 39, 176),  // Purple
        QColor(244, 67, 54),   // Red
        QColor(255, 235, 59),  // Yellow
        QColor(0, 188, 212),   // Cyan
        QColor(121, 85, 72),   // Brown
        QColor(158, 158, 158), // Grey
        QColor(255, 193, 7)    // Amber
    };
    
    setMinimumSize(400, 400);
}

void PieChartWidget::setData(const QVector<QPair<QString, double>> &data)
{
    m_data = data;
    update();
}

void PieChartWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Vẽ tiêu đề
    if (!m_title.isEmpty())
    {
        QFont titleFont = painter.font();
        titleFont.setPointSize(14);
        titleFont.setBold(true);
        painter.setFont(titleFont);
        painter.drawText(rect().adjusted(0, 10, 0, 0), Qt::AlignHCenter | Qt::AlignTop, m_title);
    }
    
    if (m_data.isEmpty())
    {
        painter.drawText(rect(), Qt::AlignCenter, "Không có dữ liệu");
        return;
    }
    
    // Tính tổng
    double total = 0;
    for (const auto &item : m_data)
        total += item.second;
    
    if (total == 0)
    {
        painter.drawText(rect(), Qt::AlignCenter, "Không có dữ liệu");
        return;
    }
    
    // Vùng vẽ biểu đồ tròn
    int diameter = qMin(width(), height() - 100);
    int centerX = width() / 2;
    int centerY = (height() - 100) / 2 + 40;
    int radius = diameter / 2 - 20;
    
    QRect pieRect(centerX - radius, centerY - radius, radius * 2, radius * 2);
    
    // Vẽ các phần của biểu đồ tròn
    int startAngle = 0;
    for (int i = 0; i < m_data.size(); i++)
    {
        double value = m_data[i].second;
        int spanAngle = static_cast<int>((value / total) * 360 * 16);  // Qt uses 1/16th of a degree
        
        QColor color = m_colors[i % m_colors.size()];
        painter.setBrush(color);
        painter.setPen(QPen(Qt::white, 2));
        
        painter.drawPie(pieRect, startAngle, spanAngle);
        
        // Vẽ phần trăm ở giữa mỗi phần
        if (spanAngle > 500)  // Chỉ vẽ % nếu phần đủ lớn
        {
            double angle = (startAngle + spanAngle / 2.0) / 16.0;
            double radian = angle * M_PI / 180.0;
            int textX = centerX + static_cast<int>(radius * 0.7 * cos(radian));
            int textY = centerY - static_cast<int>(radius * 0.7 * sin(radian));
            
            QString percentText = QString::number(value / total * 100, 'f', 1) + "%";
            
            painter.setPen(Qt::white);
            QFont font = painter.font();
            font.setBold(true);
            font.setPointSize(10);
            painter.setFont(font);
            
            QRect textRect(textX - 30, textY - 10, 60, 20);
            painter.drawText(textRect, Qt::AlignCenter, percentText);
        }
        
        startAngle += spanAngle;
    }
    
    // Vẽ chú thích
    int legendX = 20;
    int legendY = height() - 80;
    int boxSize = 15;
    int spacing = 20;
    
    QFont legendFont = painter.font();
    legendFont.setPointSize(9);
    painter.setFont(legendFont);
    
    for (int i = 0; i < m_data.size(); i++)
    {
        QColor color = m_colors[i % m_colors.size()];
        painter.setBrush(color);
        painter.setPen(Qt::black);
        
        int y = legendY + i * spacing;
        painter.drawRect(legendX, y, boxSize, boxSize);
        
        QString label = m_data[i].first;
        double percent = (m_data[i].second / total) * 100;
        QString text = QString("%1 (%2%)").arg(label).arg(percent, 0, 'f', 1);
        
        painter.setPen(Qt::black);
        painter.drawText(legendX + boxSize + 8, y + 12, text);
    }
}

// ========== BAR CHART ==========
BarChartWidget::BarChartWidget(QWidget *parent)
    : QWidget(parent), m_barColor(76, 175, 80)
{
    setMinimumSize(600, 400);
}

void BarChartWidget::setData(const QVector<QPair<QString, double>> &data)
{
    m_data = data;
    update();
}

void BarChartWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Vẽ tiêu đề
    if (!m_title.isEmpty())
    {
        QFont titleFont = painter.font();
        titleFont.setPointSize(14);
        titleFont.setBold(true);
        painter.setFont(titleFont);
        painter.drawText(rect().adjusted(0, 10, 0, 0), Qt::AlignHCenter | Qt::AlignTop, m_title);
    }
    
    if (m_data.isEmpty())
    {
        painter.drawText(rect(), Qt::AlignCenter, "Không có dữ liệu");
        return;
    }
    
    // Tìm giá trị max
    double maxValue = 0;
    for (const auto &item : m_data)
        maxValue = qMax(maxValue, item.second);
    
    if (maxValue == 0)
    {
        painter.drawText(rect(), Qt::AlignCenter, "Không có dữ liệu");
        return;
    }
    
    // Vùng vẽ
    int marginTop = 60;
    int marginBottom = 80;
    int marginLeft = 60;
    int marginRight = 40;
    
    int chartWidth = width() - marginLeft - marginRight;
    int chartHeight = height() - marginTop - marginBottom;
    
    // Vẽ trục
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(marginLeft, marginTop, marginLeft, height() - marginBottom);  // Trục Y
    painter.drawLine(marginLeft, height() - marginBottom, width() - marginRight, height() - marginBottom);  // Trục X
    
    // Vẽ các cột
    int barCount = m_data.size();
    int barWidth = chartWidth / barCount - 10;
    
    for (int i = 0; i < barCount; i++)
    {
        double value = m_data[i].second;
        int barHeight = static_cast<int>((value / maxValue) * chartHeight);
        
        int x = marginLeft + 5 + i * (chartWidth / barCount);
        int y = height() - marginBottom - barHeight;
        
        // Vẽ cột với gradient
        QLinearGradient gradient(x, y, x, y + barHeight);
        gradient.setColorAt(0, m_barColor.lighter(120));
        gradient.setColorAt(1, m_barColor);
        
        painter.setBrush(gradient);
        painter.setPen(QPen(m_barColor.darker(120), 1));
        painter.drawRect(x, y, barWidth, barHeight);
        
        // Vẽ giá trị trên cột
        painter.setPen(Qt::black);
        QFont valueFont = painter.font();
        valueFont.setPointSize(8);
        valueFont.setBold(true);
        painter.setFont(valueFont);
        
        QString valueText = QString::number(value, 'f', 0);
        if (value >= 1000000)
            valueText = QString::number(value / 1000000.0, 'f', 1) + "M";
        else if (value >= 1000)
            valueText = QString::number(value / 1000.0, 'f', 0) + "K";
        
        painter.drawText(x, y - 5, barWidth, 15, Qt::AlignCenter, valueText);
        
        // Vẽ nhãn dưới cột
        painter.setPen(Qt::black);
        QFont labelFont = painter.font();
        labelFont.setPointSize(8);
        painter.setFont(labelFont);
        
        painter.save();
        painter.translate(x + barWidth / 2, height() - marginBottom + 10);
        painter.rotate(45);
        painter.drawText(0, 0, m_data[i].first);
        painter.restore();
    }
    
    // Vẽ các mức giá trị trên trục Y
    painter.setPen(Qt::gray);
    QFont axisFont = painter.font();
    axisFont.setPointSize(8);
    painter.setFont(axisFont);
    
    for (int i = 0; i <= 5; i++)
    {
        double value = (maxValue / 5) * i;
        int y = height() - marginBottom - static_cast<int>((value / maxValue) * chartHeight);
        
        // Vẽ đường kẻ ngang
        painter.setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
        painter.drawLine(marginLeft, y, width() - marginRight, y);
        
        // Vẽ giá trị
        painter.setPen(Qt::black);
        QString text = QString::number(value, 'f', 0);
        if (value >= 1000000)
            text = QString::number(value / 1000000.0, 'f', 1) + "M";
        else if (value >= 1000)
            text = QString::number(value / 1000.0, 'f', 0) + "K";
        
        painter.drawText(5, y - 5, marginLeft - 10, 20, Qt::AlignRight | Qt::AlignVCenter, text);
    }
}
