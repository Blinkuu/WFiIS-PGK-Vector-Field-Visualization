#include "scatter.h"
#include <QtCore/qmath.h>
#include <QtDataVisualization/QCustom3DItem>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtDataVisualization/qscatter3dseries.h>
#include <QtDataVisualization/qscatterdataproxy.h>
#include <QtDataVisualization/qvalue3daxis.h>
#include <Qt3DCore/QTransform>

#include <iostream>

using namespace QtDataVisualization;

constexpr float verticalRange = 10.0f;
constexpr float horizontalRange = verticalRange;
constexpr float doublePi = static_cast<float>(M_PI) * 2.0f;
constexpr float radiansToDegrees = 360.0f / doublePi;

float minimum(float a, float b, float c){
    if(a < b){
        if(a < c){
            return a;
        }
    }
    if (b < c){
        return b;
    } else {
        return c;
    }
}


Scatter::Scatter(Q3DScatter *scatter)
 : m_graph(scatter),
   m_function([](const QVector3D&& vec, float , float, float){ return QVector3D(vec.x(), vec.y(), vec.z()); }),
   m_xRange(-horizontalRange, horizontalRange),
   m_yRange(-verticalRange, verticalRange),
   m_zRange(-horizontalRange, horizontalRange),
   m_arrowLength(50) {

  m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
  m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);
  m_graph->customItems().clear();
  m_graph->activeTheme()->setType(Q3DTheme::ThemeQt);
  m_graph->axisX()->setRange(m_xRange.first, m_xRange.second);
  m_graph->axisY()->setRange(m_yRange.first, m_yRange.second);
  m_graph->axisZ()->setRange(m_zRange.first, m_zRange.second);
  m_graph->axisX()->setSegmentCount(static_cast<int>(horizontalRange));
  m_graph->axisZ()->setSegmentCount(static_cast<int>(horizontalRange));

  generateData();
}

Scatter::~Scatter() {
  m_graph->removeCustomItems();
  delete m_graph;
}

void Scatter::generateData() {

  m_graph->removeCustomItems();
  m_graph->clearSelection();

  QImage sunColor = QImage(2, 2, QImage::Format_RGB32);
  sunColor.fill(QColor(0xff, 0xbb, 0x00));

  float min = 0.;
  float max = 0.;
  std::vector<float> lengths;
  QValue3DAxis *axisX = m_graph->axisX();
  QValue3DAxis *axisY = m_graph->axisY();
  QValue3DAxis *axisZ = m_graph->axisZ();
  for (float xr = m_xRange.first; xr <= m_xRange.second; xr += (m_xRange.second - m_xRange.first) / axisX->segmentCount()) {
    for (float yr = m_yRange.first; yr <= m_yRange.second; yr += (m_yRange.second - m_yRange.first) / axisY->segmentCount()) {
      for (float zr = m_zRange.first; zr <= m_zRange.second; zr += (m_zRange.second - m_zRange.first) / axisZ->segmentCount()) {
        auto vec = m_function(QVector3D(xr, yr, zr), m_a, m_b, m_c);
        lengths.push_back(vec.lengthSquared());
        if (vec.lengthSquared() > max) {
          max = vec.lengthSquared();
        } else if (vec.lengthSquared() < min) {
          min = vec.lengthSquared();
        }
      }
    }
  }

  float stepx = (m_xRange.second - m_xRange.first) / axisX->segmentCount();
  float stepy = (m_yRange.second - m_yRange.first) / axisY->segmentCount();
  float stepz = (m_zRange.second - m_zRange.first) / axisZ->segmentCount();
  int i = 0;
  for (float xr = m_xRange.first; xr <= m_xRange.second; xr += stepx) {
    for (float yr = m_yRange.first; yr <= m_yRange.second; yr += stepy) {
      for (float zr = m_zRange.first; zr <= m_zRange.second; zr += stepz) {

        auto vec = m_function(QVector3D(xr, yr, zr), m_a, m_b, m_c);
        auto item = new QCustom3DItem();
        if(m_lenghtOption == 0){
            item->setScaling(QVector3D(0.05f, vec.lengthSquared() / max * minimum(stepx, stepy, stepz) / 10, 0.05f));
        } else if (m_lenghtOption == 1) {
            item->setScaling(QVector3D(0.07f, 0.12f, 0.07f));
        } else {
            item->setScaling(QVector3D(0.05f, m_arrowLength / 300.0f * vec.lengthSquared() / max, 0.05f));
        }

        item->setMeshFile(QStringLiteral(":/arrow.obj"));
        auto out = static_cast<unsigned char>(abs((lengths[i] - min) * 255 / (max - min)));
        QImage sunColor = QImage(2, 2, QImage::Format_RGB32);
        sunColor.fill(QColor(static_cast<int>(out), 0, static_cast<int>(255 - out)));
        item->setTextureImage(sunColor);
        i++;

        // rotation
        auto up = QVector3D(0, 1, 0);
        auto angle = qAcos(static_cast<double>(QVector3D::dotProduct(up, vec) / vec.length()));
        auto axis = QVector3D::crossProduct(up, vec);
        auto rot = QQuaternion::fromAxisAndAngle(axis, angle * static_cast<double>(radiansToDegrees));
        auto roty = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, (xr >= 0.0f && zr >= 0.0f) || (xr <= 0.0f && zr <= 0.0f) ? 90.0f : -90.0f);
        if (xr == 0.0f) {
          roty = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 180.0f);
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

  x.toFloat() > m_xRange.second ? m_xRange.first = -horizontalRange : m_xRange.first = x.toFloat();

  axis->setRange(m_xRange.first, m_xRange.second);
  generateData();
}

void Scatter::setXSecond(const QString &x) {
  QValue3DAxis *axis = m_graph->axisX();

  x.toFloat() < m_xRange.first ? m_xRange.second = horizontalRange : m_xRange.second = x.toFloat();

  axis->setRange(m_xRange.first, m_xRange.second);
  generateData();
}

void Scatter::setYFirst(const QString &y) {
  QValue3DAxis *axis = m_graph->axisY();

  y.toFloat() > m_yRange.second ? m_yRange.first = -horizontalRange : m_yRange.first = y.toFloat();

  axis->setRange(m_yRange.first, m_yRange.second);
  generateData();
}

void Scatter::setYSecond(const QString &y) {
  QValue3DAxis *axis = m_graph->axisY();

  y.toFloat() < m_yRange.first ? m_yRange.second = horizontalRange : m_yRange.second = y.toFloat();

  axis->setRange(m_yRange.first, m_yRange.second);
  generateData();
}
void Scatter::setZFirst(const QString &z) {
  QValue3DAxis *axis = m_graph->axisZ();

  z.toFloat() > m_zRange.second ? m_zRange.first = -horizontalRange : m_zRange.first = z.toFloat();

  axis->setRange(m_zRange.first, m_zRange.second);
  generateData();
}

void Scatter::setZSecond(const QString &z) {
  QValue3DAxis *axis = m_graph->axisZ();

  z.toFloat() < m_zRange.first ? m_zRange.second = horizontalRange : m_zRange.second = z.toFloat();

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

void Scatter::setArrowsLength(int arrowLength) {
    m_arrowLength = arrowLength;
    generateData();
}

void Scatter::functionboxItemChanged(int index) {
    if(index == 0)
        m_function = [](const QVector3D&& vec, float a = 1, float b = 1, float c = 1){ return QVector3D(a * vec.x(), b * vec.y(), c * vec.z()); };
    else if(index == 1)
        m_function = [](const QVector3D&& vec, float a = 1, float b = 1, float c = 1){ return QVector3D(a * vec.y() * vec.z(), b * vec.x() * vec.z(), c * vec.x() * vec.y()); };
    else if(index == 2){
        m_function = [](const QVector3D&& vec, float a = 1, float b = 1, float c = 1){ return QVector3D(qSin(a * vec.x()), b * qSin(vec.y()), c * qSin(vec.z())); };
    } else if(index == 3){
        m_function = [](const QVector3D&& vec, float a = 1, float b = 1, float c = 1){ return QVector3D(a * qTan(vec.x()), b * qTan(vec.y()), c * qTan(vec.z())); };
    }
    generateData();
}

void Scatter::themeboxItemChanged(int index) {
    if(index == 0)
        m_graph->activeTheme()->setType(Q3DTheme::ThemeQt);
    else if(index == 1)
        m_graph->activeTheme()->setType(Q3DTheme::ThemeEbony);
}

void Scatter::setA(const QString& a){
    m_a = a.toInt();
    generateData();
}

void Scatter::setB(const QString& b){
    m_b = b.toInt();
    generateData();
}

void Scatter::setC(const QString& c){
    m_c = c.toInt();
    generateData();
}

void Scatter::lengthboxItemChanged(int index){
    m_lenghtOption = index;
    generateData();
}
