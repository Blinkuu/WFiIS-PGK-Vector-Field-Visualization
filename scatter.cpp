#include "scatter.h"
#include <QtCore/qmath.h>
#include <QtDataVisualization/QCustom3DItem>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtDataVisualization/qscatter3dseries.h>
#include <QtDataVisualization/qscatterdataproxy.h>
#include <QtDataVisualization/qvalue3daxis.h>
#include <iostream>

#include <Qt3DCore/QTransform>

using namespace QtDataVisualization;

static constexpr float verticalRange = 10.0f;
static constexpr float horizontalRange = verticalRange;
static constexpr float doublePi = static_cast<float>(M_PI) * 2.0f;
static constexpr float animationFrames = 30.0f;
static constexpr float radiansToDegrees = 360.0f / doublePi;

Scatter::Scatter(Q3DScatter *scatter)
    : m_graph(scatter), m_fieldLines(12), m_arrowsPerLine(16),
      m_magneticField(new QScatter3DSeries), m_sun(new QCustom3DItem),
      m_vec(new QCustom3DItem), m_magneticFieldArray(nullptr),
      m_angleOffset(0.0f),
      m_angleStep(doublePi / m_arrowsPerLine / animationFrames),
      m_xRange(-horizontalRange, horizontalRange),
      m_yRange(-verticalRange, verticalRange),
      m_zRange(-horizontalRange, horizontalRange),
      m_xSegments(static_cast<int>(horizontalRange)),
      m_ySegments(static_cast<int>(verticalRange)),
      m_zSegments(static_cast<int>(horizontalRange)) {
  m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
  m_graph->scene()->activeCamera()->setCameraPreset(
      Q3DCamera::CameraPresetFront);

  // Magnetic field lines use custom narrow arrow
  m_magneticField->setItemSize(0.2f);
  //! [3]
  m_magneticField->setMesh(QAbstract3DSeries::MeshUserDefined);
  m_magneticField->setUserDefinedMesh(QStringLiteral(":/arrow.obj"));
  //! [3]
  //! [4]
  QLinearGradient fieldGradient(0, 0, 16, 1024);
  fieldGradient.setColorAt(0.0, Qt::yellow);
  fieldGradient.setColorAt(1.0, Qt::red);
  m_magneticField->setBaseGradient(fieldGradient);
  m_magneticField->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
  //! [4]

  m_graph->customItems().clear();
  m_graph->activeTheme()->setType(Q3DTheme::ThemeEbony);

  // Configure the axes according to the data
  m_graph->axisX()->setRange(m_xRange.first, m_xRange.second);
  m_graph->axisY()->setRange(m_yRange.first, m_yRange.second);
  m_graph->axisZ()->setRange(m_zRange.first, m_zRange.second);
  m_graph->axisX()->setSegmentCount(static_cast<int>(horizontalRange));
  m_graph->axisZ()->setSegmentCount(static_cast<int>(horizontalRange));

  generateData();
}

Scatter::~Scatter() { delete m_graph; }

void Scatter::generateData() {
  m_graph->removeCustomItems();

  QImage sunColor = QImage(2, 2, QImage::Format_RGB32);
  sunColor.fill(QColor(0xff, 0xbb, 0x00));
  //    m_sun->setTextureImage(sunColor);

  float min = 0.;
  float max = 0.;
  std::vector<float> lengths;
  QValue3DAxis *axisX = m_graph->axisX();
  QValue3DAxis *axisY = m_graph->axisY();
  QValue3DAxis *axisZ = m_graph->axisZ();
  for (float xr = m_xRange.first; xr <= m_xRange.second;
       xr += (m_xRange.second - m_xRange.first) / axisX->segmentCount()) {
    for (float yr = m_yRange.first; yr <= m_yRange.second;
         yr += (m_yRange.second - m_yRange.first) / axisY->segmentCount()) {
      for (float zr = m_zRange.first; zr <= m_zRange.second;
           zr += (m_zRange.second - m_zRange.first) / axisZ->segmentCount()) {
        auto vec = QVector3D(xr, yr, zr);
        lengths.push_back(vec.lengthSquared());
        if (vec.lengthSquared() > max) {
          max = vec.lengthSquared();
        } else if (vec.lengthSquared() < min) {
          min = vec.lengthSquared();
        }
      }
    }
  }

  auto fun1 = [](const QVector3D &&vec) {
    return QVector3D{vec.x(), vec.y(), vec.z()};
  };

  auto fun2 = [](const QVector3D &&vec) {
    return QVector3D{vec.y() * vec.z(), vec.x() * vec.z(), vec.x() * vec.y()};
  };

  int i = 0;
  for (float xr = m_xRange.first; xr <= m_xRange.second;
       xr += (m_xRange.second - m_xRange.first) / axisX->segmentCount()) {
    for (float yr = m_yRange.first; yr <= m_yRange.second;
         yr += (m_yRange.second - m_yRange.first) / axisY->segmentCount()) {
      for (float zr = m_zRange.first; zr <= m_zRange.second;
           zr += (m_zRange.second - m_zRange.first) / axisZ->segmentCount()) {

        auto vec = fun1(QVector3D(xr, yr, zr));
        auto item = new QCustom3DItem();
        auto end = QVector3D(xr, yr, zr) + vec;
        item->setScaling(QVector3D(0.05f, 0.05, 0.05f));
        item->setMeshFile(QStringLiteral(":/arrow.obj"));
        auto out = static_cast<unsigned char>(
            fabs((lengths[i] - min) * 255 / (max - min)));
        QImage sunColor = QImage(2, 2, QImage::Format_RGB32);
        sunColor.fill(
            QColor(static_cast<int>(out), 0, static_cast<int>(255 - out)));
        item->setTextureImage(sunColor);
        i++;

        Qt3DCore::QTransform rotateTransform;
        auto up = QVector3D(0, 1, 0);

        // rotation
        auto angle = qAcos(QVector3D::dotProduct(up, vec) / vec.length());
        auto axis = QVector3D::crossProduct(up, vec);
        auto rot = QQuaternion::fromAxisAndAngle(axis, angle * radiansToDegrees);
        auto roty = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, (xr >= 0.0f && zr >= 0.0f) || (xr <= 0.0f && zr <= 0.0f) ? 90.0f : -90.0f);
        if (xr == 0.0f) { roty = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 180.0f);
          item->setRotation(roty * rot);
        }else if (zr == 0.0f) {
          item->setRotation(rot);
        } else {
          item->setRotation(roty * rot);
        }

        item->setPosition(QVector3D(xr, yr, zr));
        m_graph->addCustomItem(item);
      }
    }
  }
}

void Scatter::setXFirst(const QString &x) {
  QValue3DAxis *axis = m_graph->axisX();

  !x.isEmpty() && x.toFloat() < 0.0f ? m_xRange.first = x.toFloat()
                                     : m_xRange.first = -10.0f;

  axis->setRange(m_xRange.first, m_xRange.second);
  generateData();
}

void Scatter::setXSecond(const QString &x) {
  QValue3DAxis *axis = m_graph->axisX();

  !x.isEmpty() && x.toFloat() > 0.0f ? m_xRange.second = x.toFloat()
                                     : m_xRange.second = 10.0f;

  axis->setRange(m_xRange.first, m_xRange.second);
  generateData();
}

void Scatter::setYFirst(const QString &y) {
  QValue3DAxis *axis = m_graph->axisY();

  !y.isEmpty() && y.toFloat() < 0.0f ? m_yRange.first = y.toFloat()
                                     : m_yRange.first = -10.0f;

  axis->setRange(m_yRange.first, m_yRange.second);
  generateData();
}

void Scatter::setYSecond(const QString &y) {
  QValue3DAxis *axis = m_graph->axisY();

  !y.isEmpty() && y.toFloat() > 0.0f ? m_yRange.second = y.toFloat()
                                     : m_yRange.second = 10.0f;

  axis->setRange(m_yRange.first, m_yRange.second);
  generateData();
}
void Scatter::setZFirst(const QString &z) {
  QValue3DAxis *axis = m_graph->axisZ();

  !z.isEmpty() && z.toFloat() < 0.0f ? m_zRange.first = z.toFloat()
                                     : m_zRange.first = -10.0f;

  axis->setRange(m_zRange.first, m_zRange.second);
  generateData();
}

void Scatter::setZSecond(const QString &z) {
  QValue3DAxis *axis = m_graph->axisZ();

  !z.isEmpty() && z.toFloat() > 0.0f ? m_zRange.second = z.toFloat()
                                     : m_zRange.second = 10.0f;

  axis->setRange(m_zRange.first, m_zRange.second);
  generateData();
}

void Scatter::setXRange(const QString &x) {
  QValue3DAxis *axis = m_graph->axisX();

  !x.isEmpty() && x.toInt() > 0
      ? axis->setSegmentCount(x.toInt())
      : axis->setSegmentCount(static_cast<int>(horizontalRange));
  generateData();
}

void Scatter::setYRange(const QString &x) {
  QValue3DAxis *axis = m_graph->axisY();

  !x.isEmpty() && x.toInt() > 0
      ? axis->setSegmentCount(x.toInt())
      : axis->setSegmentCount(static_cast<int>(horizontalRange));
  generateData();
}

void Scatter::setZRange(const QString &x) {
  QValue3DAxis *axis = m_graph->axisZ();

  !x.isEmpty() && x.toInt() > 0
      ? axis->setSegmentCount(x.toInt())
      : axis->setSegmentCount(static_cast<int>(horizontalRange));
  generateData();
}
