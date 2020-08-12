#ifndef EDITABLEPLOT_H
#define EDITABLEPLOT_H

/*!
 * \file EditablePlot.h
 * \brief Plot with a curve that can be edited by the user with the mouse
 */

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <vector>

/*! \class EditablePlot
* \brief Class that represents the plot editable with the mouse
* It overloads QwtPlot to add the functionality of moving points with the mouse
* The user can click and hold near to a point to move it up and down
* It requires the installation of Qwt : https://qwt.sourceforge.io/qwtinstall.html
*/
class QwtEditablePlot : public QwtPlot {
        Q_OBJECT
public:
    QwtEditablePlot(double minimumX=0,double minimumY=-10,double maximumX=10,double maximumY=10);
    /*!
    *  \brief Get the points placed in the graph by the user
    *
    *  \return A vector with the current QPointF
    */
    std::vector<QPointF> getPoints();

public slots:
    /*!
    *  \brief Handle the mouse movement on the graph
    *   This needs to be called each time a new position of the mouse is detected
    *  \param pos Position of the mouse on the graph
    */
    void onSelected(const QPointF& pos);
    /*!
    *  \brief Change the curve style
    *
    *  \param style 0 Steps, 1 Lines
    */
    void changeStyle(int style);
    /*!
    *  \brief Change the sample time
    *
    *  \param sampleTime ms
    */
    void setSampleTime(int sampleT);
    /*!
    *  \brief Detects mouse press
    *  Right click is use to change the sample period
    */
    void mousePressEvent(QMouseEvent *eventPress);

private:
    QwtPlotCurve* curve; /*!< Curve object inside the plot */
    QVector<QPointF> points; /*!< List of points on the plot */
    QPointF newPoint; /*!< New point that will replace the one moved */
    int sampleTime; /*!< Current sample period */
    int indexPeriod; /*!< Index of the current sample period */
    std::vector<int> availableSamplePeriod={1000,500,100}; /*!< Sample periods that can be selected */
    QwtPlotMagnifier* zoomer; /*!< Object that handle zoom with middle button of the mouse */
    QwtPlotPanner* panner; /*!< Object that handle movement on the graph with middle button of the mouse */
    double minX,minY,maxX,maxY;

};

#endif // EDITABLEPLOT_H
