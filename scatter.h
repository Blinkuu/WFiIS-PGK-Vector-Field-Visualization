#pragma once

#include <QtDataVisualization/q3dscatter.h>
#include <QtDataVisualization/qscatterdataproxy.h>
#include <QtCore/QTimer>

using namespace QtDataVisualization;

class Scatter : public QObject
{
    Q_OBJECT
public:
    explicit Scatter(Q3DScatter *scatter);
    ~Scatter();

    void generateData();

public Q_SLOTS:
    void setXFirst(const QString& x);
    void setXSecond(const QString& x);
    void setYFirst(const QString& y);
    void setYSecond(const QString& y);
    void setZFirst(const QString& z);
    void setZSecond(const QString& z);
    void setA(const QString& a);
    void setB(const QString& b);
    void setC(const QString& c);

    void setXRange(const QString& x);
    void setYRange(const QString& y);
    void setZRange(const QString& z);

    void setArrowsLength(int arrowLength);
    void functionboxItemChanged(int index);
    void lengthboxItemChanged(int index);
    void themeboxItemChanged(int index);

    void setCutByPlain(bool checked);
    void setPlainA(const QString& A);
    void setPlainB(const QString& B);
    void setPlainC(const QString& C);
    void setPlainD(const QString& D);

    void handleButton();
private:
    Q3DScatter *m_graph;

private:
    bool isAbovePlain(float x, float y, float z);

    std::function<QVector3D(const QVector3D&&, float, float, float)> m_function;

    QPair<float, float> m_xRange;
    QPair<float, float> m_yRange;
    QPair<float, float> m_zRange;
    float m_a = 1.0f;
    float m_b = 1.0f;
    float m_c = 1.0f;
    float m_plainA = 1.0f;
    float m_plainB = 1.0f;
    float m_plainC = 1.0f;
    float m_plainD = 1.0f;

    int m_lenghtOption = 0;

    bool m_cutByPlain = false;

    int m_arrowLength;
};
