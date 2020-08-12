#include "../../includes/EditablePlot.h"

#include <QApplication>
#include <QtWidgets>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QwtEditablePlot* plot = new QwtEditablePlot(-6,-8,6,8);

    QMainWindow w;
    QWidget* centralWidget = new QWidget();

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(plot);

    QComboBox *transitions = new QComboBox();
    transitions->addItem("Steps");
    transitions->addItem("Lines");
    transitions->setMaximumWidth(150);
    QObject::connect(transitions,SIGNAL(currentIndexChanged(int)),plot,SLOT(changeStyle(int)));
    vLayout->addWidget(transitions);

    QPushButton* getValues = new QPushButton("Get values");
    getValues->setMaximumWidth(150);
    QObject::connect(getValues,&QPushButton::clicked,
                     [=](){
        std::vector<QPointF> values = plot->getPoints();
        for(QPointF p : values)
            std::cout << "( "<<p.x()<<" , "<<p.y()<<" )" << std::endl;
    });
    vLayout->addWidget(getValues);

    centralWidget->setLayout(vLayout);
    w.setCentralWidget(centralWidget);
    w.setMinimumSize(800,600);
    w.show();

    return a.exec();
}
