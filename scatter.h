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

    void setXRange(const QString& x);
    void setYRange(const QString& x);
    void setZRange(const QString& x);

private:
    Q3DScatter *m_graph;
    QTimer m_rotationTimer;
    int m_fieldLines;
    int m_arrowsPerLine;
    QScatter3DSeries *m_magneticField;
    QCustom3DItem *m_sun;
    QCustom3DItem *m_vec;
    QScatterDataArray *m_magneticFieldArray;
    float m_angleOffset;
    float m_angleStep;

private:
    QPair<float, float> m_xRange;
    QPair<float, float> m_yRange;
    QPair<float, float> m_zRange;

    int m_xSegments;
    int m_ySegments;
    int m_zSegments;
};
