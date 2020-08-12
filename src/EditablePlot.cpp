#include "../../includes/EditablePlot.h"
#include <QtWidgets>
#include <map>
#include <iostream>
#include <qwt_plot.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

QwtEditablePlot::QwtEditablePlot(double minimumX,double minimumY,double maximumX,double maximumY)
    : QwtPlot(),minX(minimumX),minY(minimumY),maxX(maximumX),maxY(maximumY){
    curve =  new QwtPlotCurve();
    QwtPlotPicker *picker = new QwtPlotPicker(QwtPlot::xBottom,
                                              QwtPlot::yLeft,
                                              QwtPicker::CrossRubberBand,
                                              QwtPicker::ActiveOnly,
                                              this->canvas());
    picker->setStateMachine(new QwtPickerDragPointMachine());
    connect(picker, SIGNAL(moved(const QPointF&)), this, SLOT(onSelected(const QPointF&)));
    indexPeriod = 0;
    setSampleTime(0); // 1000 ms
    curve->setStyle(QwtPlotCurve::Steps);
    curve->setCurveAttribute(QwtPlotCurve::Inverted);
    curve->setSamples( points );
    this->setTitle( "QwtEditablePlot" );
    this->setCanvasBackground( Qt::white );
    this->setAxisScale( QwtPlot::xBottom, minX, maxX);
    this->setAxisScale( QwtPlot::yLeft, minY, maxY);
    this->setToolTip("Left click and hold near to a point to move this point \nRight click to change sample period \nUse the middle button to zoom or pan");

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->attach( this );

    curve->setPen( Qt::blue, 4 ),
    curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
                                       QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 8, 8 ) );
    curve->setSymbol( symbol );

    curve->attach( this );

    zoomer = new QwtPlotMagnifier(this->canvas());
    panner = new QwtPlotPanner(this->canvas());
    panner->setMouseButton(Qt::MidButton);
}

void QwtEditablePlot::onSelected(const QPointF& p){
    std::cout << "onSelected (x , y)" << p.x() << " , " << p.y() << " toPoint " << p.toPoint().x() <<" "<< p.toPoint().y() << std::endl;

    int closePoint = round((double)(p.x()-minX)*(points.size()-1)/(maxX-minX)); // Index in the tab of the closest point from the mouse
    if(closePoint >=0 && closePoint<=points.size()-1 && p.toPoint().y()>minY && p.toPoint().y()<maxY){
        newPoint = points.at(closePoint);
        std::cout << "closePoint " << closePoint << newPoint.x() << " , " << newPoint.y() ;
        newPoint.setY(p.y());
        points.replace(closePoint,newPoint);

        std::cout << " newPoint " << newPoint.x() << " , " << newPoint.y() << std::endl;
        curve->setSamples( points );
        this->replot();
    }
}

void QwtEditablePlot::changeStyle(int style){
    switch(style){
    case 0:
        curve->setStyle(QwtPlotCurve::Steps);
        break;
    case 1:
        curve->setStyle(QwtPlotCurve::Lines);
        break;
    default:
        break;
    }
    this->replot();
}

void QwtEditablePlot::setSampleTime(int index){
    sampleTime = availableSamplePeriod[index];
    points.clear();
    int nPoints = ((maxX-minX)*1000)/sampleTime; // Max time (10s) over sample period
    for(int i=0;i<=nPoints;i++){
        points << QPointF((i*(double)(maxX-minX)/nPoints)+minX,0.0);
    }
    curve->setSamples( points );
    this->replot();
    this->setFooter("Sample period : "+QString::number(sampleTime)+" ms");
}

std::vector<QPointF> QwtEditablePlot::getPoints(){
    std::vector<QPointF> values;
    for(QPointF p : points){
        values.push_back(p);
    }
    return values;
}

void QwtEditablePlot::mousePressEvent(QMouseEvent *eventPress){
    if(eventPress->button() == Qt::RightButton){
        setSampleTime((++indexPeriod)%availableSamplePeriod.size());
    }
}
