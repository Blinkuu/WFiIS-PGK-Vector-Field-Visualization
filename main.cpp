#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLineEdit>
#include <QtGui/QScreen>
#include <QApplication>
#include <QPointer>

#include "scatter.h"

int main(int argc, char *argv[])
{ 
    QApplication app(argc, argv);
    QPointer<Q3DScatter> graph = new Q3DScatter();
    QWidget *container = QWidget::createWindowContainer(graph);

    if (!graph->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return -1;
    }

    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 8, screenSize.height() / 6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    QPointer<QWidget> widget = new QWidget;
    QPointer<QHBoxLayout> hLayout = new QHBoxLayout(widget);
    QPointer<QVBoxLayout> vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);


    widget->setWindowTitle(QStringLiteral("Vector field visualization"));

    QPointer<QSlider> fieldLinesSlider = new QSlider(Qt::Horizontal, widget);
    fieldLinesSlider->setTickInterval(1);
    fieldLinesSlider->setMinimum(1);
    fieldLinesSlider->setValue(12);
    fieldLinesSlider->setMaximum(128);

    QPointer<QSlider> arrowsSlider = new QSlider(Qt::Horizontal, widget);
    arrowsSlider->setTickInterval(1);
    arrowsSlider->setMinimum(8);
    arrowsSlider->setValue(16);
    arrowsSlider->setMaximum(32);

    // Layout
    QPointer<QVBoxLayout> vTopLayout = new QVBoxLayout();
    QPointer<QHBoxLayout> hXLayout = new QHBoxLayout();
    QPointer<QHBoxLayout> hYLayout = new QHBoxLayout();
    QPointer<QHBoxLayout> hZLayout = new QHBoxLayout();
    QPointer<QHBoxLayout> hSegLayout = new QHBoxLayout();

    vLayout->addLayout(vTopLayout);
    vTopLayout->addWidget(new QLabel(QStringLiteral("Przedział zmienności argumentów:")));
    vTopLayout->addLayout(hXLayout);
    vTopLayout->addLayout(hYLayout);
    vTopLayout->addLayout(hZLayout);
    vTopLayout->addWidget(new QLabel(QStringLiteral("Ilość podprzedziałów:")));
    vTopLayout->addLayout(hSegLayout);
    //

    // Set ranges
    QPointer<QLineEdit> xRange1 = new QLineEdit(widget);
    xRange1->setPlaceholderText(QString("x1"));
    xRange1->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    xRange1->setMaximumSize(50,200);
    QPointer<QLineEdit> xRange2 = new QLineEdit(widget);
    xRange2->setPlaceholderText(QString("x2"));
    xRange2->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    xRange2->setMaximumSize(50,200);

    QPointer<QLineEdit> yRange1 = new QLineEdit(widget);
    yRange1->setPlaceholderText(QString("y1"));
    yRange1->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    yRange1->setMaximumSize(50,200);
    QPointer<QLineEdit> yRange2 = new QLineEdit(widget);
    yRange2->setPlaceholderText(QString("y2"));
    yRange2->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    yRange2->setMaximumSize(50,200);

    QPointer<QLineEdit> zRange1 = new QLineEdit(widget);
    zRange1->setPlaceholderText(QString("z1"));
    zRange1->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    zRange1->setMaximumSize(50,200);
    QPointer<QLineEdit> zRange2 = new QLineEdit(widget);
    zRange2->setPlaceholderText(QString("z2"));
    zRange2->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    zRange2->setMaximumSize(50,200);
    //

    // Set segments
    QPointer<QLineEdit> xSeg = new QLineEdit(widget);
    xSeg->setPlaceholderText(QString("10"));
    xSeg->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    xSeg->setMaximumSize(50,200);
    QPointer<QLineEdit> ySeg = new QLineEdit(widget);
    ySeg->setPlaceholderText(QString("10"));
    ySeg->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    ySeg->setMaximumSize(50,200);
    QPointer<QLineEdit> zSeg = new QLineEdit(widget);
    zSeg->setPlaceholderText(QString("10"));
    zSeg->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    zSeg->setMaximumSize(50,200);
    //

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

    // Top layout
    vTopLayout->addWidget(new QLabel(QStringLiteral("Field Lines (1 - 128):")));
    vTopLayout->addWidget(fieldLinesSlider);
    vTopLayout->addWidget(new QLabel(QStringLiteral("Arrows per line (8 - 32):")));
    vTopLayout->addWidget(arrowsSlider, 1, Qt::AlignTop);
    //

    QPointer<Scatter> modifier = new Scatter(graph);

    QObject::connect(fieldLinesSlider, &QSlider::valueChanged, modifier,
                     &Scatter::setFieldLines);
    QObject::connect(arrowsSlider, &QSlider::valueChanged, modifier,
                     &Scatter::setArrowsPerLine);

    QObject::connect(xRange1, SIGNAL(textChanged(QString)), modifier, SLOT(setXFirst(QString)));
    QObject::connect(xRange2, SIGNAL(textChanged(QString)), modifier, SLOT(setXSecond(QString)));
    QObject::connect(yRange1, SIGNAL(textChanged(QString)), modifier, SLOT(setYFirst(QString)));
    QObject::connect(yRange2, SIGNAL(textChanged(QString)), modifier, SLOT(setYSecond(QString)));
    QObject::connect(zRange1, SIGNAL(textChanged(QString)), modifier, SLOT(setZFirst(QString)));
    QObject::connect(zRange2, SIGNAL(textChanged(QString)), modifier, SLOT(setZSecond(QString)));

    QObject::connect(xSeg, SIGNAL(textChanged(QString)), modifier, SLOT(setXRange(QString)));
    QObject::connect(ySeg, SIGNAL(textChanged(QString)), modifier, SLOT(setYRange(QString)));
    QObject::connect(zSeg, SIGNAL(textChanged(QString)), modifier, SLOT(setZRange(QString)));

    widget->show();
    return app.exec();
}
