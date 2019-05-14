#include "scatter.h"
#include <QtDataVisualization/qscatterdataproxy.h>
#include <QtDataVisualization/qvalue3daxis.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/qscatter3dseries.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtDataVisualization/QCustom3DItem>
#include <QtCore/qmath.h>

using namespace QtDataVisualization;

static constexpr float verticalRange = 10.0f;
static constexpr float horizontalRange = verticalRange;
static constexpr float ellipse_a = horizontalRange / 3.0f;
static constexpr float ellipse_b = verticalRange;
static constexpr float doublePi = float(M_PI) * 2.0f;
static constexpr float radiansToDegrees = 360.0f / doublePi;
static constexpr float animationFrames = 30.0f;

Scatter::Scatter(Q3DScatter *scatter)
    : m_graph(scatter),
      m_fieldLines(12),
      m_arrowsPerLine(16),
      m_magneticField(new QScatter3DSeries),
      m_sun(new QCustom3DItem),
      m_magneticFieldArray(nullptr),
      m_angleOffset(0.0f),
      m_angleStep(doublePi / m_arrowsPerLine / animationFrames),
      m_xRange(-horizontalRange, horizontalRange),
      m_yRange(-verticalRange, verticalRange),
      m_zRange(-horizontalRange, horizontalRange)
{
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
    m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

    // Magnetic field lines use custom narrow arrow
    m_magneticField->setItemSize(0.2f);
    //! [3]
    m_magneticField->setMesh(QAbstract3DSeries::MeshUserDefined);
    m_magneticField->setUserDefinedMesh(QStringLiteral(":/arrow.obj"));
    //! [3]
    //! [4]
    QLinearGradient fieldGradient(0, 0, 16, 1024);
    fieldGradient.setColorAt(0.0, Qt::black);
    fieldGradient.setColorAt(1.0, Qt::white);
    m_magneticField->setBaseGradient(fieldGradient);
    m_magneticField->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    //! [4]

    // For 'sun' we use a custom large sphere
    m_sun->setScaling(QVector3D(0.07f, 0.07f, 0.07f));
    m_sun->setMeshFile(QStringLiteral(":/sphere.obj"));
    QImage sunColor = QImage(2, 2, QImage::Format_RGB32);
    sunColor.fill(QColor(0xff, 0xbb, 0x00));
    m_sun->setTextureImage(sunColor);

    m_graph->addSeries(m_magneticField);
    m_graph->addCustomItem(m_sun);

    // Configure the axes according to the data
    m_graph->axisX()->setRange(m_xRange.first, m_xRange.second);
    m_graph->axisY()->setRange(m_yRange.first, m_yRange.second);
    m_graph->axisZ()->setRange(m_zRange.first, m_zRange.second);
    m_graph->axisX()->setSegmentCount(int(horizontalRange));
    m_graph->axisZ()->setSegmentCount(int(horizontalRange));

    QObject::connect(&m_rotationTimer, &QTimer::timeout, this,
                     &Scatter::triggerRotation);

    toggleRotation();
    generateData();
}

Scatter::~Scatter()
{
    delete m_graph;
}

void Scatter::generateData()
{
    // Reusing existing array is computationally cheaper than always generating new array, even if
    // all data items change in the array, if the array size doesn't change.
    if (!m_magneticFieldArray)
        m_magneticFieldArray = new QScatterDataArray;

    int arraySize = m_fieldLines * m_arrowsPerLine;
    if (arraySize != m_magneticFieldArray->size())
        m_magneticFieldArray->resize(arraySize);

    QScatterDataItem *ptrToDataArray = &m_magneticFieldArray->first();

    for (float i = 0; i < m_fieldLines; i++) {
        float horizontalAngle = (doublePi * i) / m_fieldLines;
        float xCenter = ellipse_a * qCos(horizontalAngle);
        float zCenter = ellipse_a * qSin(horizontalAngle);

        // Rotate - arrow always tangential to origin
        //! [0]
        QQuaternion yRotation = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, horizontalAngle * radiansToDegrees);
        //! [0]

        for (float j = 0; j < m_arrowsPerLine; j++) {
            // Calculate point on ellipse centered on origin and parallel to x-axis
            float verticalAngle = ((doublePi * j) / m_arrowsPerLine) + m_angleOffset;
            float xUnrotated = ellipse_a * qCos(verticalAngle);
            float y = ellipse_b * qSin(verticalAngle);

            // Rotate the ellipse around y-axis
            float xRotated = xUnrotated * qCos(horizontalAngle);
            float zRotated = xUnrotated * qSin(horizontalAngle);

            // Add offset
            float x = xCenter + xRotated;
            float z = zCenter + zRotated;

            //! [1]
            QQuaternion zRotation = QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 1.0f, verticalAngle * radiansToDegrees);
            QQuaternion totalRotation = yRotation * zRotation;
            //! [1]

            ptrToDataArray->setPosition(QVector3D(x, y, z));
            //! [2]
            ptrToDataArray->setRotation(totalRotation);
            //! [2]
            ptrToDataArray++;
        }
    }

    if (m_graph->selectedSeries() == m_magneticField)
        m_graph->clearSelection();

    m_magneticField->dataProxy()->resetArray(m_magneticFieldArray);
}

void Scatter::setFieldLines(int lines)
{
    m_fieldLines = lines;
    generateData();
}

void Scatter::setArrowsPerLine(int arrows)
{
    m_angleOffset = 0.0f;
    m_angleStep = doublePi / m_arrowsPerLine / animationFrames;
    m_arrowsPerLine = arrows;
    generateData();
}

void Scatter::triggerRotation()
{
    m_angleOffset += m_angleStep;
    generateData();
}

void Scatter::toggleSun()
{
    m_sun->setVisible(!m_sun->isVisible());
}

void Scatter::toggleRotation()
{
    if (m_rotationTimer.isActive())
        m_rotationTimer.stop();
    else
        m_rotationTimer.start(15);
}

void Scatter::setXFirst(const QString& x) {
    QValue3DAxis* axis = m_graph->axisX();

    !x.isEmpty() && x.toFloat() < 0.0f ? m_xRange.first = x.toFloat() : m_xRange.first = -10.0f;

    axis->setRange(m_xRange.first, m_xRange.second);
}

void Scatter::setXSecond(const QString& x) {
    QValue3DAxis* axis = m_graph->axisX();

    !x.isEmpty() && x.toFloat() > 0.0f ? m_xRange.second = x.toFloat() : m_xRange.second = 10.0f;

    axis->setRange(m_xRange.first, m_xRange.second);
}

void Scatter::setYFirst(const QString& y) {
    QValue3DAxis* axis = m_graph->axisY();

    !y.isEmpty() && y.toFloat() < 0.0f ? m_yRange.first = y.toFloat() : m_yRange.first = -10.0f;

    axis->setRange(m_yRange.first, m_yRange.second);
}

void Scatter::setYSecond(const QString& y) {
    QValue3DAxis* axis = m_graph->axisY();

    !y.isEmpty() && y.toFloat() > 0.0f ? m_yRange.second = y.toFloat() : m_yRange.second = 10.0f;

    axis->setRange(m_yRange.first, m_yRange.second);
}
void Scatter::setZFirst(const QString& z) {
    QValue3DAxis* axis = m_graph->axisZ();

    !z.isEmpty() && z.toFloat() < 0.0f ? m_zRange.first = z.toFloat() : m_zRange.first = -10.0f;

    axis->setRange(m_zRange.first, m_zRange.second);
}

void Scatter::setZSecond(const QString& z) {
    QValue3DAxis* axis = m_graph->axisZ();

    !z.isEmpty() && z.toFloat() > 0.0f ? m_zRange.second = z.toFloat() : m_zRange.second = 10.0f;

    axis->setRange(m_zRange.first, m_zRange.second);
}
