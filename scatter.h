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
    void setFieldLines(int lines);
    void setArrowsPerLine(int arrows);
    void toggleRotation();
    void triggerRotation();
    void toggleSun();

private:
    Q3DScatter *m_graph;
    QTimer m_rotationTimer;
    int m_fieldLines;
    int m_arrowsPerLine;
    QScatter3DSeries *m_magneticField;
    QCustom3DItem *m_sun;
    QScatterDataArray *m_magneticFieldArray;
    float m_angleOffset;
    float m_angleStep;
};
