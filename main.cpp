#include <QApplication>
#include <QPointer>
#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "scatter.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QPointer<Q3DScatter> graph = new Q3DScatter();
  QPointer<QWidget> container = QWidget::createWindowContainer(graph);

  if (!graph->hasContext()) {
    QMessageBox msgBox;
    msgBox.setText("Couldn't initialize the OpenGL context.");
    msgBox.exec();
    return -1;
  }

  QSize screenSize = graph->screen()->size();
  container->setMinimumSize(QSize(800, 600));
  container->setMaximumSize(screenSize);
  container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  container->setFocusPolicy(Qt::StrongFocus);

  // Main frame and layout
  QPointer<QWidget> widget = new QWidget;

  QPointer<QHBoxLayout> hLayout = new QHBoxLayout(widget);
  QPointer<QVBoxLayout> vLayout = new QVBoxLayout();

  hLayout->addWidget(container, 1);
  hLayout->addLayout(vLayout);

  widget->setWindowTitle(QStringLiteral("Vector field visualization"));

  // Additional layouts
  QPointer<QHBoxLayout> hXLayout = new QHBoxLayout();
  QPointer<QHBoxLayout> hYLayout = new QHBoxLayout();
  QPointer<QHBoxLayout> hZLayout = new QHBoxLayout();
  QPointer<QHBoxLayout> hSegLayout = new QHBoxLayout();
  QPointer<QHBoxLayout> hParLayout = new QHBoxLayout();
  QPointer<QHBoxLayout> hPlainLayout = new QHBoxLayout();

  //vLayout->addLayout(vTopLayout);
  vLayout->addWidget(new QLabel(QStringLiteral("Przedział zmienności argumentów:")));
  vLayout->addLayout(hXLayout);
  vLayout->addLayout(hYLayout);
  vLayout->addLayout(hZLayout);
  vLayout->addWidget(new QLabel(QStringLiteral("Ilość podprzedziałów:")));
  vLayout->addLayout(hSegLayout);
  //

  // Set ranges
  QPointer<QLineEdit> xRange1 = new QLineEdit(widget);
  xRange1->setPlaceholderText(QString("x1"));
  xRange1->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  xRange1->setMaximumSize(50, 200);
  QPointer<QLineEdit> xRange2 = new QLineEdit(widget);
  xRange2->setPlaceholderText(QString("x2"));
  xRange2->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  xRange2->setMaximumSize(50, 200);

  QPointer<QLineEdit> yRange1 = new QLineEdit(widget);
  yRange1->setPlaceholderText(QString("y1"));
  yRange1->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  yRange1->setMaximumSize(50, 200);
  QPointer<QLineEdit> yRange2 = new QLineEdit(widget);
  yRange2->setPlaceholderText(QString("y2"));
  yRange2->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  yRange2->setMaximumSize(50, 200);

  QPointer<QLineEdit> zRange1 = new QLineEdit(widget);
  zRange1->setPlaceholderText(QString("z1"));
  zRange1->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  zRange1->setMaximumSize(50, 200);
  QPointer<QLineEdit> zRange2 = new QLineEdit(widget);
  zRange2->setPlaceholderText(QString("z2"));
  zRange2->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  zRange2->setMaximumSize(50, 200);
  //

  // Set segments
  QPointer<QLineEdit> xSeg = new QLineEdit(widget);
  xSeg->setPlaceholderText(QString("10"));
  xSeg->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  xSeg->setMaximumSize(50, 200);
  QPointer<QLineEdit> ySeg = new QLineEdit(widget);
  ySeg->setPlaceholderText(QString("10"));
  ySeg->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  ySeg->setMaximumSize(50, 200);
  QPointer<QLineEdit> zSeg = new QLineEdit(widget);
  zSeg->setPlaceholderText(QString("10"));
  zSeg->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  zSeg->setMaximumSize(50, 200);
  //

  //vec length option
  QPointer<QComboBox> lengthOptions = new QComboBox();
  lengthOptions->addItem("Automatycznie");
  lengthOptions->addItem("Stała");
  lengthOptions->addItem("Podana przez użytkownika");
  // Set slider
  QPointer<QSlider> arrowsLengthSlider = new QSlider(Qt::Horizontal, widget);
  arrowsLengthSlider->setTickInterval(1);
  arrowsLengthSlider->setMinimum(1);
  arrowsLengthSlider->setValue(50);
  arrowsLengthSlider->setMaximum(100);
  vLayout->addWidget(new QLabel(QStringLiteral("Długość wektora:")));
  vLayout->addWidget(lengthOptions);
  vLayout->addWidget(arrowsLengthSlider);
  //

  // Function combobox
  QPointer<QComboBox> functionComboBox = new QComboBox();
  functionComboBox->addItem("F(x,y,z) = v(a*x, b*y, c*z)");
  functionComboBox->addItem("F(x,y,z) = v(a*y*z, b*x*z, c*x*y)");
  functionComboBox->addItem("F(x, y, z) = v(a * sin(x), b * sin(y), c * sin(z))");
  functionComboBox->addItem("F(x, y, z) = v(a * sin(x), b * sin(y), c * sin(z))");
  vLayout->addWidget(new QLabel(QStringLiteral("Wybierz funkcję:")));
  vLayout->addWidget(functionComboBox);
  //Set a,b,c params
  QPointer<QLineEdit> a = new QLineEdit(widget);
  a->setPlaceholderText(QString("1"));
  a->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  a->setMaximumSize(50, 200);
  QPointer<QLineEdit> b = new QLineEdit(widget);
  b->setPlaceholderText(QString("1"));
  b->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  b->setMaximumSize(50, 200);
  QPointer<QLineEdit> c = new QLineEdit(widget);
  c->setPlaceholderText(QString("1"));
  c->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  c->setMaximumSize(50, 200);

  hParLayout->addWidget(new QLabel(QStringLiteral(" a    =")));
  hParLayout->addWidget(a);
  hParLayout->addWidget(new QLabel(QStringLiteral(" b    =")));
  hParLayout->addWidget(b);
  hParLayout->addWidget(new QLabel(QStringLiteral(" c    =")));
  hParLayout->addWidget(c);
  vLayout->addLayout(hParLayout);

  // Theme combobox
  QPointer<QComboBox> themeComboBox = new QComboBox();
  themeComboBox->addItem("Biały motyw");
  themeComboBox->addItem("Czarny motyw");
  vLayout->addWidget(new QLabel(QStringLiteral("Wybierz motyw:")));
  vLayout->addWidget(themeComboBox);
  //

  // Inputs of plain, which vector shall be limited by
  QPointer<QCheckBox> plainLimiterCheckBox = new QCheckBox;
  plainLimiterCheckBox->setText("Włącz odcięcie płaszczyzną");
  vLayout->addWidget(plainLimiterCheckBox);

  QPointer<QLineEdit> xMax = new QLineEdit(widget);
  xMax->setPlaceholderText(QString("1"));
  xMax->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  xMax->setMaximumSize(50, 200);
  QPointer<QLineEdit> yMax = new QLineEdit(widget);
  yMax->setPlaceholderText(QString("1"));
  yMax->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  yMax->setMaximumSize(50, 200);
  QPointer<QLineEdit> zMax = new QLineEdit(widget);
  zMax->setPlaceholderText(QString("1"));
  zMax->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
  zMax->setMaximumSize(50, 200);

  hPlainLayout->addWidget(new QLabel(QStringLiteral(" xMax    =")));
  hPlainLayout->addWidget(xMax);
  hPlainLayout->addWidget(new QLabel(QStringLiteral(" yMax    =")));
  hPlainLayout->addWidget(yMax);
  hPlainLayout->addWidget(new QLabel(QStringLiteral(" zMax    =")));
  hPlainLayout->addWidget(zMax);
  vLayout->addLayout(hPlainLayout);

  // Bottom layout
  hSegLayout->addWidget(xSeg);
  hSegLayout->addWidget(ySeg);
  hSegLayout->addWidget(zSeg);

  hXLayout->addWidget(xRange1);
  hXLayout->addWidget(xRange2);

  hYLayout->addWidget(yRange1);
  hYLayout->addWidget(yRange2);

  hZLayout->addWidget(zRange1);
  hZLayout->addWidget(zRange2);
  //

  QPointer<Scatter> modifier = new Scatter(graph);

  QObject::connect(xRange1, SIGNAL(textChanged(QString)), modifier,
                   SLOT(setXFirst(QString)));
  QObject::connect(xRange2, SIGNAL(textChanged(QString)), modifier,
                   SLOT(setXSecond(QString)));
  QObject::connect(yRange1, SIGNAL(textChanged(QString)), modifier,
                   SLOT(setYFirst(QString)));
  QObject::connect(yRange2, SIGNAL(textChanged(QString)), modifier,
                   SLOT(setYSecond(QString)));
  QObject::connect(zRange1, SIGNAL(textChanged(QString)), modifier,
                   SLOT(setZFirst(QString)));
  QObject::connect(zRange2, SIGNAL(textChanged(QString)), modifier,
                   SLOT(setZSecond(QString)));

  QObject::connect(xSeg, SIGNAL(textChanged(QString)), modifier,
                   SLOT(setXRange(QString)));
  QObject::connect(ySeg, SIGNAL(textChanged(QString)), modifier,
                   SLOT(setYRange(QString)));
  QObject::connect(zSeg, SIGNAL(textChanged(QString)), modifier,
                   SLOT(setZRange(QString)));

  QObject::connect(a, SIGNAL(textChanged(QString)), modifier,
                   SLOT(setA(QString)));
  QObject::connect(b, SIGNAL(textChanged(QString)), modifier,
                   SLOT(setB(QString)));
  QObject::connect(c, SIGNAL(textChanged(QString)), modifier,
                   SLOT(setC(QString)));

  QObject::connect(arrowsLengthSlider, &QSlider::valueChanged, modifier,
                   &Scatter::setArrowsLength);

  QObject::connect(functionComboBox, SIGNAL(currentIndexChanged(int)), modifier,
                   SLOT(functionboxItemChanged(int)));
  QObject::connect(lengthOptions, SIGNAL(currentIndexChanged(int)), modifier,
                   SLOT(lengthboxItemChanged(int)));

  QObject::connect(themeComboBox, SIGNAL(currentIndexChanged(int)), modifier,
                   SLOT(themeboxItemChanged(int)));

  QObject::connect(plainLimiterCheckBox, SIGNAL(clicked(bool)), modifier,
                   SLOT(setCutByPlain(bool)));
  QObject::connect(xMax, SIGNAL(textChanged(QString)), modifier,
                   SLOT(setMaxX(QString)));
  QObject::connect(yMax, SIGNAL(textChanged(QString)), modifier,
                   SLOT(setMaxY(QString)));
  QObject::connect(zMax, SIGNAL(textChanged(QString)), modifier,
                   SLOT(setMaxZ(QString)));

  widget->show();
  return app.exec();
}
