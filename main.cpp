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

#include "scatter.h"

int main(int argc, char *argv[])
{ 
    QApplication app(argc, argv);
    Q3DScatter *graph = new Q3DScatter();
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

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);

    widget->setWindowTitle(QStringLiteral("Vector field visualization"));

    QPushButton *toggleRotationButton = new QPushButton(widget);
    toggleRotationButton->setText(QStringLiteral("Toggle animation"));
    QPushButton *toggleSunButton = new QPushButton(widget);
    toggleSunButton->setText(QStringLiteral("Toggle Sun"));

    QSlider *fieldLinesSlider = new QSlider(Qt::Horizontal, widget);
    fieldLinesSlider->setTickInterval(1);
    fieldLinesSlider->setMinimum(1);
    fieldLinesSlider->setValue(12);
    fieldLinesSlider->setMaximum(128);

    QSlider *arrowsSlider = new QSlider(Qt::Horizontal, widget);
    arrowsSlider->setTickInterval(1);
    arrowsSlider->setMinimum(8);
    arrowsSlider->setValue(16);
    arrowsSlider->setMaximum(32);

    //
    QVBoxLayout *vTopLayout = new QVBoxLayout();
    QHBoxLayout *hXLayout = new QHBoxLayout();
    QHBoxLayout *hYLayout = new QHBoxLayout();
    QHBoxLayout *hZLayout = new QHBoxLayout();

    vLayout->addLayout(vTopLayout);
    vLayout->addLayout(hXLayout);
    vLayout->addLayout(hYLayout);
    vLayout->addLayout(hZLayout);

    QLineEdit *xRange1 = new QLineEdit(widget);
    xRange1->setPlaceholderText(QString("x1"));
    QLineEdit *xRange2 = new QLineEdit(widget);
    xRange2->setPlaceholderText(QString("x2"));

    QLineEdit *yRange1 = new QLineEdit(widget);
    yRange1->setPlaceholderText(QString("y1"));
    QLineEdit *yRange2 = new QLineEdit(widget);
    yRange2->setPlaceholderText(QString("y2"));

    QLineEdit *zRange1 = new QLineEdit(widget);
    zRange1->setPlaceholderText(QString("z1"));
    QLineEdit *zRange2 = new QLineEdit(widget);
    zRange2->setPlaceholderText(QString("z2"));

    hXLayout->addWidget(xRange1);
    hXLayout->addWidget(xRange2);

    hYLayout->addWidget(yRange1);
    hYLayout->addWidget(yRange2);

    hZLayout->addWidget(zRange1);
    hZLayout->addWidget(zRange2);

    //
    vTopLayout->addWidget(toggleRotationButton);
    vTopLayout->addWidget(toggleSunButton);
    vTopLayout->addWidget(new QLabel(QStringLiteral("Field Lines (1 - 128):")));
    vTopLayout->addWidget(fieldLinesSlider);
    vTopLayout->addWidget(new QLabel(QStringLiteral("Arrows per line (8 - 32):")));
    vTopLayout->addWidget(arrowsSlider, 1, Qt::AlignTop);
    //

    Scatter *modifier = new Scatter(graph);

    QObject::connect(toggleRotationButton, &QPushButton::clicked, modifier,
                     &Scatter::toggleRotation);
    QObject::connect(toggleSunButton, &QPushButton::clicked, modifier,
                     &Scatter::toggleSun);
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

    widget->show();
    return app.exec();
}
