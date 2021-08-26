#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <iostream>//entradas y salidas por consola
#include <fstream>//archivos.txt
#include <vector>//min_element//max_exelement//HD
#include <QApplication>
#include <QtCore>
#include <algorithm>
#include <H5Cpp.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){

    ui->setupUi(this);
    setGeometry(200, 200, 1500, 800);
    setupsignalslot();
    setWindowTitle(QString("INQNET TDC"));
    Teleport0_or_QKD1=1;

//setup style of the histograms and plots
    setupHistoPlot(ui->PlotB);
    setupHistoPlot(ui->PlotA);
    setupHistoPlot(ui->PlotC);
    setupratePlot(ui->PlotTrack);
    setup_plot_qkd_results(ui->QKD_H1_results);
    setup_plot_qkd_results(ui->QKD_H2_results);
    setup_plot_qkd_results(ui->QKD_H3_results);
    setup_plot_qkd_stats(ui->qkd_errorplot);
    setup_plot_qkd_stats(ui->qkd_siftedplot);
    setup_plot_qkd_results(ui->Early_results);
    setup_plot_qkd_results(ui->Late_results);
    setup_plot_qkd_results(ui->Phase_results);

    if(!Teleport0_or_QKD1){
        setup_histolines_Teleport();
        setupDefaultRanges();
    }
    else {
        setup_histolines_QKD();
    }
    


    ui->thch1->setValue(0.1);
    ui->thch2->setValue(-0.08);
    ui->thch3->setValue(-0.06);
    ui->thch4->setValue(-0.08);
    ui->delaych1->setValue(0);
    ui->delaych2->setValue(0);
    ui->delaych3->setValue(0);
    ui->delaych4->setValue(0);

    ui->histStart->setValue(1);
    ui->histEnd->setValue(50001);

    ui->binsinplot->setValue(10000);
    ui->adqtime->setValue(2);//update rate Adq time

    ui->PlotAChn1->setValue(1);
    ui->PlotAChn2->setValue(3);
    ui->PlotBChn1->setValue(1);
    ui->PlotBChn2->setValue(2);
    ui->PlotCChn1->setValue(1);
    ui->PlotCChn2->setValue(4);
    ui->startChan->setValue(1);

    lastPointKey_tab1 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    lastPointKey_tab3 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    lastPointKey_tab4 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    qkd_prevKey = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    ui->rof1->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof2->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof3->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof4->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof5->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof6->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof7->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof8->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof9->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof10->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof11->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof12->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof13->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof14->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof15->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof16->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof17->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->rof18->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");

    ui->rof1->addItem(tr("Rise"));
    ui->rof1->addItem(tr("Fall"));
    ui->rof2->addItem(tr("Rise"));
    ui->rof2->addItem(tr("Fall"));
    ui->rof3->addItem(tr("Rise"));
    ui->rof3->addItem(tr("Fall"));
    ui->rof4->addItem(tr("Rise"));
    ui->rof4->addItem(tr("Fall"));
    ui->rof5->addItem(tr("Rise"));
    ui->rof5->addItem(tr("Fall"));
    ui->rof6->addItem(tr("Rise"));
    ui->rof6->addItem(tr("Fall"));
    ui->rof7->addItem(tr("Rise"));
    ui->rof7->addItem(tr("Fall"));
    ui->rof8->addItem(tr("Rise"));
    ui->rof8->addItem(tr("Fall"));
    ui->rof9->addItem(tr("Rise"));
    ui->rof9->addItem(tr("Fall"));
    ui->rof10->addItem(tr("Rise"));
    ui->rof10->addItem(tr("Fall"));
    ui->rof11->addItem(tr("Rise"));
    ui->rof11->addItem(tr("Fall"));
    ui->rof12->addItem(tr("Rise"));
    ui->rof12->addItem(tr("Fall"));
    ui->rof13->addItem(tr("Rise"));
    ui->rof13->addItem(tr("Fall"));
    ui->rof14->addItem(tr("Rise"));
    ui->rof14->addItem(tr("Fall"));
    ui->rof15->addItem(tr("Rise"));
    ui->rof15->addItem(tr("Fall"));
    ui->rof16->addItem(tr("Rise"));
    ui->rof16->addItem(tr("Fall"));
    ui->rof17->addItem(tr("Rise"));
    ui->rof17->addItem(tr("Fall"));
    ui->rof18->addItem(tr("Rise"));
    ui->rof18->addItem(tr("Fall"));

    ui->rof1->setCurrentText("Fall");
    ui->rof2->setCurrentText("Fall");
    ui->rof3->setCurrentText("Fall");
    ui->rof4->setCurrentText("Fall");
    ui->rof5->setCurrentText("Fall");
    ui->rof6->setCurrentText("Fall");
    ui->rof7->setCurrentText("Fall");
    ui->rof8->setCurrentText("Fall");
    ui->rof9->setCurrentText("Fall");
    ui->rof10->setCurrentText("Fall");
    ui->rof11->setCurrentText("Fall");
    ui->rof12->setCurrentText("Fall");
    ui->rof13->setCurrentText("Fall");
    ui->rof14->setCurrentText("Fall");
    ui->rof15->setCurrentText("Fall");
    ui->rof16->setCurrentText("Fall");
    ui->rof17->setCurrentText("Fall");
    ui->rof18->setCurrentText("Fall");

    /* FIXME: Should be some way to do this globally using stylesheets, but I
     * wasn't able to get it to work. */
    ui->test1->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test2->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test3->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test4->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test5->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test6->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test7->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test8->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test9->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test10->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test11->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test12->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test13->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test14->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test15->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test16->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test17->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");
    ui->test18->setStyleSheet("QComboBox { background-color: darkGray }" "QListView { color: white; }");

    ui->test1->addItem(tr("Enable"));
    ui->test1->addItem(tr("Disable"));
    ui->test2->addItem(tr("Enable"));
    ui->test2->addItem(tr("Disable"));
    ui->test3->addItem(tr("Enable"));
    ui->test3->addItem(tr("Disable"));
    ui->test4->addItem(tr("Enable"));
    ui->test4->addItem(tr("Disable"));
    ui->test5->addItem(tr("Enable"));
    ui->test5->addItem(tr("Disable"));
    ui->test6->addItem(tr("Enable"));
    ui->test6->addItem(tr("Disable"));
    ui->test7->addItem(tr("Enable"));
    ui->test7->addItem(tr("Disable"));
    ui->test8->addItem(tr("Enable"));
    ui->test8->addItem(tr("Disable"));
    ui->test9->addItem(tr("Enable"));
    ui->test9->addItem(tr("Disable"));
    ui->test10->addItem(tr("Enable"));
    ui->test10->addItem(tr("Disable"));
    ui->test11->addItem(tr("Enable"));
    ui->test11->addItem(tr("Disable"));
    ui->test12->addItem(tr("Enable"));
    ui->test12->addItem(tr("Disable"));
    ui->test13->addItem(tr("Enable"));
    ui->test13->addItem(tr("Disable"));
    ui->test14->addItem(tr("Enable"));
    ui->test14->addItem(tr("Disable"));
    ui->test15->addItem(tr("Enable"));
    ui->test15->addItem(tr("Disable"));
    ui->test16->addItem(tr("Enable"));
    ui->test16->addItem(tr("Disable"));
    ui->test17->addItem(tr("Enable"));
    ui->test17->addItem(tr("Disable"));
    ui->test18->addItem(tr("Enable"));
    ui->test18->addItem(tr("Disable"));

    ui->test1->setCurrentText("Disable");
    ui->test2->setCurrentText("Disable");
    ui->test3->setCurrentText("Disable");
    ui->test4->setCurrentText("Disable");
    ui->test5->setCurrentText("Disable");
    ui->test6->setCurrentText("Disable");
    ui->test7->setCurrentText("Disable");
    ui->test8->setCurrentText("Disable");
    ui->test9->setCurrentText("Disable");
    ui->test10->setCurrentText("Disable");
    ui->test11->setCurrentText("Disable");
    ui->test12->setCurrentText("Disable");
    ui->test13->setCurrentText("Disable");
    ui->test14->setCurrentText("Disable");
    ui->test15->setCurrentText("Disable");
    ui->test16->setCurrentText("Disable");
    ui->test17->setCurrentText("Disable");
    ui->test18->setCurrentText("Disable");

    /* Set connected label to Disconnected. */
    ui->connected_label->setText("Disconnected");


    //while(1){
    //    udpcom.sendDataToClient(6.66);
    //    sleep(1);
    //}

   /* QubitTime=200;
    Phasetime=50;
    NoQubits=30;
    PeaksQubit=3;
    QKD_intWind = 45;
    QKD_zero = 1;*/

   /* ui->QKD_time->setValue(200);//QubitTime
    ui->QKD_phase->setValue(50);//Phasetime
    ui->QKD_nuqub->setValue(30);//NoQubits
    ui->QKD_PxQ->setValue(3);//PeaksQubit
    ui->QKD_iw->setValue(45);//QKD_intWind
    ui->QKD_zero->setValue(1);//QKD_zero*/

    qkdparam.QKD_setDefault();

/*    savejasonFile.setFileName(QDateTime::currentDateTime().toString("yyyy-MM-dd'_'hh:mm:ss'.json'"));
    if (!savejasonFile.open(QIODevice::ReadWrite)) {
        qWarning("Couldn't open save file.");
    }
*/
    //dbc.start();
    //adq.start();
    //anl.start();

    adq.initdone = 1;
    initR=true;

    createQKDLinesA();
    createQKDLinesB();
    createQKDLinesC();



}


//////////////////////////////////////////////////////////
///////////////////setups///////////////////////////
///////////////////////////////////////////////////////////

void MainWindow::setupDefaultRanges(){
   
    ui->BegA1->setValue(70700);
    ui->BegA2->setValue(72600);
    ui->BegA3->setValue(74600);
    ui->EndA1->setValue(72100);
    ui->EndA2->setValue(73900);
    ui->EndA3->setValue(75900);

    ui->BegB1->setValue(71600);
    ui->BegB2->setValue(73600);
    ui->BegB3->setValue(75600);
    ui->EndB1->setValue(72800);
    ui->EndB2->setValue(74800);
    ui->EndB3->setValue(76800);

    ui->BegC1->setValue(71600);
    ui->BegC2->setValue(73600);
    ui->BegC3->setValue(75600);
    ui->EndC1->setValue(72800);
    ui->EndC2->setValue(74800);
    ui->EndC3->setValue(76800);
    
}
void MainWindow::setup_histolines_Teleport(){
        for (int i=0;i<18;i++) {
            if(i<6)infLine[i] = new QCPItemStraightLine(ui->PlotA);
            if((i>5) && (i<12)) infLine[i] = new QCPItemStraightLine(ui->PlotB);
            if(i>11)infLine[i] = new QCPItemStraightLine(ui->PlotC);
        }
    }
void MainWindow::setup_histolines_QKD(){
    //QVector<QCPItemStraightLine> LinesPlotA1(2000,ui->PlotA);

    for (int i = 0 ; i<MAX_QUBITS ; i++) {
        LinesPlotA1[i] = new QCPItemStraightLine(ui->PlotA);
        LinesPlotA1[i]->setPen(QPen(Qt::red));
        LinesPlotA1[i]->setVisible(0);

        LinesPlotA2[i] = new QCPItemStraightLine(ui->PlotA);
        LinesPlotA2[i]->setPen(QPen(Qt::green));
        LinesPlotA2[i]->setVisible(0);

        LinesPlotA3[i] = new QCPItemStraightLine(ui->PlotA);
        LinesPlotA3[i]->setPen(QPen(Qt::yellow));
        LinesPlotA3[i]->setVisible(0);
        LinesPlotA4[i] = new QCPItemStraightLine(ui->PlotA);
        LinesPlotA4[i]->setPen(QPen(Qt::white));
        LinesPlotA4[i]->setVisible(0);

        LinesPlotB1[i] = new QCPItemStraightLine(ui->PlotB);
        LinesPlotB1[i]->setPen(QPen(Qt::red));
        LinesPlotB1[i]->setVisible(0);

        LinesPlotB2[i] = new QCPItemStraightLine(ui->PlotB);
        LinesPlotB2[i]->setPen(QPen(Qt::green));
        LinesPlotB2[i]->setVisible(0);

        LinesPlotB3[i] = new QCPItemStraightLine(ui->PlotB);
        LinesPlotB3[i]->setPen(QPen(Qt::yellow));
        LinesPlotB3[i]->setVisible(0);

        LinesPlotB4[i] = new QCPItemStraightLine(ui->PlotB);
        LinesPlotB4[i]->setPen(QPen(Qt::white));
        LinesPlotB4[i]->setVisible(0);

        LinesPlotC1[i] = new QCPItemStraightLine(ui->PlotC);
        LinesPlotC1[i]->setPen(QPen(Qt::red));
        LinesPlotC1[i]->setVisible(0);

        LinesPlotC2[i] = new QCPItemStraightLine(ui->PlotC);
        LinesPlotC2[i]->setPen(QPen(Qt::green));
        LinesPlotC2[i]->setVisible(0);

        LinesPlotC3[i] = new QCPItemStraightLine(ui->PlotC);
        LinesPlotC3[i]->setPen(QPen(Qt::yellow));
        LinesPlotC3[i]->setVisible(0);

        LinesPlotC4[i] = new QCPItemStraightLine(ui->PlotC);
        LinesPlotC4[i]->setPen(QPen(Qt::white));
        LinesPlotC4[i]->setVisible(0);
    }
}
void MainWindow::setupratePlot(QCustomPlot *scope){


scope->plotLayout()->clear();

QCPAxisRect *wideAxisRect = new QCPAxisRect(scope);


  wideAxisRect->setupFullAxesBox(true);
  wideAxisRect->axis(QCPAxis::atRight, 0)->setTickLabels(true);
  //wideAxisRect->axis(QCPAxis::atTop, 0)->setTickLabels(true);

  wideAxisRect->axis(QCPAxis::atRight, 0)->setTickLabelColor(Qt::white);
  //wideAxisRect->axis(QCPAxis::atTop, 0)->setTickLabelColor(Qt::white);
  wideAxisRect->axis(QCPAxis::atLeft, 0)->setTickLabelColor(Qt::white);
  wideAxisRect->axis(QCPAxis::atBottom, 0)->setTickLabelColor(Qt::white);

  wideAxisRect->axis(QCPAxis::atRight, 0)->setBasePen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atTop, 0)->setBasePen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atLeft, 0)->setBasePen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atBottom, 0)->setBasePen(QPen(Qt::white, 1));

  wideAxisRect->axis(QCPAxis::atRight, 0)->setTickPen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atTop, 0)->setTickPen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atLeft, 0)->setTickPen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atBottom, 0)->setTickPen(QPen(Qt::white, 1));

  wideAxisRect->axis(QCPAxis::atRight, 0)->setSubTickPen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atTop, 0)->setSubTickPen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atLeft, 0)->setSubTickPen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atBottom, 0)->setSubTickPen(QPen(Qt::white, 1));

  wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
  wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
  wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setSubGridVisible(true);
  wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setSubGridVisible(true);
  //wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setVisible(false);//
  //wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setVisible(false);//
  wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setZeroLinePen(Qt::NoPen);
  wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setZeroLinePen(Qt::NoPen);
  wideAxisRect->axis(QCPAxis::atLeft, 0)->setUpperEnding(QCPLineEnding::esSpikeArrow);
  wideAxisRect->axis(QCPAxis::atBottom, 0)->setUpperEnding(QCPLineEnding::esSpikeArrow);

  //wideAxisRect->axis(QCPAxis::atLeft, 0)->setLabel("Counts");
  //wideAxisRect->axis(QCPAxis::atBottom, 0)->setLabel("Time");

  wideAxisRect->axis(QCPAxis::atLeft, 0)->setLabelColor(Qt::white);
  wideAxisRect->axis(QCPAxis::atBottom, 0)->setLabelColor(Qt::white);

  wideAxisRect->setRangeZoom(Qt::Vertical);


 //scope->plotLayout()->addElement(0, 0, title1);
 scope->plotLayout()->addElement(0, 0, wideAxisRect);

  QCPGraph *graph1 = scope->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
  graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1), QBrush(Qt::white),4));
  graph1->setPen(QPen(QColor(200, 0, 0), 2));

  QCPGraph *graph2 = scope->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
  graph2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1), QBrush(Qt::red),4));
  graph2->setPen(QPen(QColor(0, 200, 0), 2));

  QCPGraph *graph3 = scope->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
  graph3->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1), QBrush(Qt::blue),4));
  graph3->setPen(QPen(QColor(200, 200, 0), 2));



  QLinearGradient plotGradient;
  plotGradient.setStart(0, 0);
  plotGradient.setFinalStop(0, 350);
  plotGradient.setColorAt(0, QColor(80, 80, 80));
  plotGradient.setColorAt(1, QColor(50, 50, 50));
  scope->setBackground(plotGradient);
  QLinearGradient axisRectGradient;
  axisRectGradient.setStart(0, 0);
  axisRectGradient.setFinalStop(0, 350);
  axisRectGradient.setColorAt(0, QColor(80, 80, 80));
  axisRectGradient.setColorAt(1, QColor(30, 30, 30));
  scope->axisRect()->setBackground(axisRectGradient);


  //scope->yAxis->setRange(0, 2);

  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
  timeTicker->setTimeFormat("%h:%m:%s");
  scope->xAxis->setTicker(timeTicker);
  scope->rescaleAxes();
  scope->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void MainWindow::setupHistoPlot(QCustomPlot *histograma){

  histograma->plotLayout()->clear();

  QCPAxisRect *wideAxisRect = new QCPAxisRect(histograma);


  wideAxisRect->setupFullAxesBox(true);
  wideAxisRect->axis(QCPAxis::atRight, 0)->setTickLabels(true);
  wideAxisRect->axis(QCPAxis::atTop, 0)->setTickLabels(true);

  wideAxisRect->axis(QCPAxis::atRight, 0)->setTickLabelColor(Qt::white);
  wideAxisRect->axis(QCPAxis::atTop, 0)->setTickLabelColor(Qt::white);
  wideAxisRect->axis(QCPAxis::atLeft, 0)->setTickLabelColor(Qt::white);
  wideAxisRect->axis(QCPAxis::atBottom, 0)->setTickLabelColor(Qt::white);

  wideAxisRect->axis(QCPAxis::atRight, 0)->setBasePen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atTop, 0)->setBasePen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atLeft, 0)->setBasePen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atBottom, 0)->setBasePen(QPen(Qt::white, 1));

  wideAxisRect->axis(QCPAxis::atRight, 0)->setTickPen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atTop, 0)->setTickPen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atLeft, 0)->setTickPen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atBottom, 0)->setTickPen(QPen(Qt::white, 1));

  wideAxisRect->axis(QCPAxis::atRight, 0)->setSubTickPen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atTop, 0)->setSubTickPen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atLeft, 0)->setSubTickPen(QPen(Qt::white, 1));
  wideAxisRect->axis(QCPAxis::atBottom, 0)->setSubTickPen(QPen(Qt::white, 1));

  wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
  wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
  wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
  wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setSubGridVisible(true);
  wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setSubGridVisible(true);
  //wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setVisible(false);//
  //wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setVisible(false);//
  wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setZeroLinePen(Qt::NoPen);
  wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setZeroLinePen(Qt::NoPen);
  wideAxisRect->axis(QCPAxis::atLeft, 0)->setUpperEnding(QCPLineEnding::esSpikeArrow);
  wideAxisRect->axis(QCPAxis::atBottom, 0)->setUpperEnding(QCPLineEnding::esSpikeArrow);

  //wideAxisRect->axis(QCPAxis::atLeft, 0)->setLabel("Cuentas");
  //wideAxisRect->axis(QCPAxis::atBottom, 0)->setLabel("Energia");

  wideAxisRect->axis(QCPAxis::atLeft, 0)->setLabelColor(Qt::white);
  wideAxisRect->axis(QCPAxis::atBottom, 0)->setLabelColor(Qt::white);

  wideAxisRect->setRangeZoom(Qt::Horizontal);



  
/*QCPPlotTitle *title1 = new QCPPlotTitle(histograma);
title1->setText("histo");
title1->setFont(QFont("sans", 12, QFont::Bold));
title1->setTextColor(Qt::white);

*/


  //histograma->plotLayout()->addElement(0, 0, title1);
  histograma->plotLayout()->addElement(0, 0, wideAxisRect);


  QCPGraph *graph1 = histograma->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
  //QCPGraph *graph2 = histograma->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));

  graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1), QBrush(Qt::white),4));
  graph1->setPen(QPen(QColor(100, 100, 100), 2));
  graph1->setLineStyle((QCPGraph::LineStyle)4);

  histograma->xAxis->setRange(0, 10000);

  //histograma->addGraph();
  //histograma->graph(0)->setPen(QPen(Qt::red));

  QLinearGradient plotGradient;
  plotGradient.setStart(0, 0);
  plotGradient.setFinalStop(0, 350);
  plotGradient.setColorAt(0, QColor(80, 80, 80));
  plotGradient.setColorAt(1, QColor(50, 50, 50));
  histograma->setBackground(plotGradient);
  QLinearGradient axisRectGradient;
  axisRectGradient.setStart(0, 0);
  axisRectGradient.setFinalStop(0, 350);
  axisRectGradient.setColorAt(0, QColor(80, 80, 80));
  axisRectGradient.setColorAt(1, QColor(30, 30, 30));
  histograma->axisRect()->setBackground(axisRectGradient);



 // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
  histograma->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);


}
void MainWindow::setup_plot_qkd_results(QCustomPlot *scope){

    scope->plotLayout()->clear();

    QCPAxisRect *wideAxisRect = new QCPAxisRect(scope);


      wideAxisRect->setupFullAxesBox(true);
      wideAxisRect->axis(QCPAxis::atRight, 0)->setTickLabels(true);
      //wideAxisRect->axis(QCPAxis::atTop, 0)->setTickLabels(true);

      wideAxisRect->axis(QCPAxis::atRight, 0)->setTickLabelColor(Qt::white);
      //wideAxisRect->axis(QCPAxis::atTop, 0)->setTickLabelColor(Qt::white);
      wideAxisRect->axis(QCPAxis::atLeft, 0)->setTickLabelColor(Qt::white);
      wideAxisRect->axis(QCPAxis::atBottom, 0)->setTickLabelColor(Qt::white);

      wideAxisRect->axis(QCPAxis::atRight, 0)->setBasePen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atTop, 0)->setBasePen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atLeft, 0)->setBasePen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atBottom, 0)->setBasePen(QPen(Qt::white, 1));

      wideAxisRect->axis(QCPAxis::atRight, 0)->setTickPen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atTop, 0)->setTickPen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atLeft, 0)->setTickPen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atBottom, 0)->setTickPen(QPen(Qt::white, 1));

      wideAxisRect->axis(QCPAxis::atRight, 0)->setSubTickPen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atTop, 0)->setSubTickPen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atLeft, 0)->setSubTickPen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atBottom, 0)->setSubTickPen(QPen(Qt::white, 1));

      wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
      wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
      wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
      wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
      wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setSubGridVisible(true);
      wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setSubGridVisible(true);
      //wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setVisible(false);//
      //wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setVisible(false);//
      wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setZeroLinePen(Qt::NoPen);
      wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setZeroLinePen(Qt::NoPen);
      wideAxisRect->axis(QCPAxis::atLeft, 0)->setUpperEnding(QCPLineEnding::esSpikeArrow);
      wideAxisRect->axis(QCPAxis::atBottom, 0)->setUpperEnding(QCPLineEnding::esSpikeArrow);

      //wideAxisRect->axis(QCPAxis::atLeft, 0)->setLabel("Counts");
      //wideAxisRect->axis(QCPAxis::atBottom, 0)->setLabel("Time");

      wideAxisRect->axis(QCPAxis::atLeft, 0)->setLabelColor(Qt::white);
      wideAxisRect->axis(QCPAxis::atBottom, 0)->setLabelColor(Qt::white);

      wideAxisRect->setRangeZoom(Qt::Vertical);


     //scope->plotLayout()->addElement(0, 0, title1);
     scope->plotLayout()->addElement(0, 0, wideAxisRect);

      QCPGraph *graph1 = scope->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
      graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1), QBrush(Qt::white),4));
      graph1->setPen(QPen(QColor(0, 0, 0), 2));

      QCPGraph *graph2 = scope->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
      graph2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1), QBrush(Qt::red),4));
      graph2->setPen(QPen(QColor(200, 0, 0), 2));

      QCPGraph *graph3 = scope->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
      graph3->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1), QBrush(Qt::blue),4));
      graph3->setPen(QPen(QColor(0, 0, 200), 2));

      QCPGraph *graph4 = scope->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
      graph4->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1), QBrush(Qt::yellow),4));
      graph4->setPen(QPen(QColor(200, 200, 0), 2));


      QLinearGradient plotGradient;
      plotGradient.setStart(0, 0);
      plotGradient.setFinalStop(0, 350);
      plotGradient.setColorAt(0, QColor(80, 80, 80));
      plotGradient.setColorAt(1, QColor(50, 50, 50));
      scope->setBackground(plotGradient);
      QLinearGradient axisRectGradient;
      axisRectGradient.setStart(0, 0);
      axisRectGradient.setFinalStop(0, 350);
      axisRectGradient.setColorAt(0, QColor(80, 80, 80));
      axisRectGradient.setColorAt(1, QColor(30, 30, 30));
      scope->axisRect()->setBackground(axisRectGradient);


      //scope->yAxis->setRange(0, 2);

      QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
      timeTicker->setTimeFormat("%h:%m:%s");
      scope->xAxis->setTicker(timeTicker);
      scope->rescaleAxes();
      scope->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}
void MainWindow::setup_plot_qkd_stats(QCustomPlot *scope){

    scope->plotLayout()->clear();

    QCPAxisRect *wideAxisRect = new QCPAxisRect(scope);


      wideAxisRect->setupFullAxesBox(true);
      wideAxisRect->axis(QCPAxis::atRight, 0)->setTickLabels(true);
      //wideAxisRect->axis(QCPAxis::atTop, 0)->setTickLabels(true);

      wideAxisRect->axis(QCPAxis::atRight, 0)->setTickLabelColor(Qt::white);
      //wideAxisRect->axis(QCPAxis::atTop, 0)->setTickLabelColor(Qt::white);
      wideAxisRect->axis(QCPAxis::atLeft, 0)->setTickLabelColor(Qt::white);
      wideAxisRect->axis(QCPAxis::atBottom, 0)->setTickLabelColor(Qt::white);

      wideAxisRect->axis(QCPAxis::atRight, 0)->setBasePen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atTop, 0)->setBasePen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atLeft, 0)->setBasePen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atBottom, 0)->setBasePen(QPen(Qt::white, 1));

      wideAxisRect->axis(QCPAxis::atRight, 0)->setTickPen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atTop, 0)->setTickPen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atLeft, 0)->setTickPen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atBottom, 0)->setTickPen(QPen(Qt::white, 1));

      wideAxisRect->axis(QCPAxis::atRight, 0)->setSubTickPen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atTop, 0)->setSubTickPen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atLeft, 0)->setSubTickPen(QPen(Qt::white, 1));
      wideAxisRect->axis(QCPAxis::atBottom, 0)->setSubTickPen(QPen(Qt::white, 1));

      wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
      wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
      wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
      wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
      wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setSubGridVisible(true);
      wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setSubGridVisible(true);
      //wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setVisible(false);//
      //wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setVisible(false);//
      wideAxisRect->axis(QCPAxis::atLeft, 0)->grid()->setZeroLinePen(Qt::NoPen);
      wideAxisRect->axis(QCPAxis::atBottom, 0)->grid()->setZeroLinePen(Qt::NoPen);
      wideAxisRect->axis(QCPAxis::atLeft, 0)->setUpperEnding(QCPLineEnding::esSpikeArrow);
      wideAxisRect->axis(QCPAxis::atBottom, 0)->setUpperEnding(QCPLineEnding::esSpikeArrow);

      //wideAxisRect->axis(QCPAxis::atLeft, 0)->setLabel("Counts");
      //wideAxisRect->axis(QCPAxis::atBottom, 0)->setLabel("Time");

      wideAxisRect->axis(QCPAxis::atLeft, 0)->setLabelColor(Qt::white);
      wideAxisRect->axis(QCPAxis::atBottom, 0)->setLabelColor(Qt::white);

      wideAxisRect->setRangeZoom(Qt::Vertical);


     //scope->plotLayout()->addElement(0, 0, title1);
     scope->plotLayout()->addElement(0, 0, wideAxisRect);

      QCPGraph *graph1 = scope->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
      graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1), QBrush(Qt::white),4));
      graph1->setPen(QPen(QColor(0, 0, 0), 2));

      QCPGraph *graph2 = scope->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
      graph2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1), QBrush(Qt::red),4));
      graph2->setPen(QPen(QColor(200, 0, 0), 2));

      QLinearGradient plotGradient;
      plotGradient.setStart(0, 0);
      plotGradient.setFinalStop(0, 350);
      plotGradient.setColorAt(0, QColor(80, 80, 80));
      plotGradient.setColorAt(1, QColor(50, 50, 50));
      scope->setBackground(plotGradient);
      QLinearGradient axisRectGradient;
      axisRectGradient.setStart(0, 0);
      axisRectGradient.setFinalStop(0, 350);
      axisRectGradient.setColorAt(0, QColor(80, 80, 80));
      axisRectGradient.setColorAt(1, QColor(30, 30, 30));
      scope->axisRect()->setBackground(axisRectGradient);


      //scope->yAxis->setRange(0, 2);

      QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
      timeTicker->setTimeFormat("%h:%m:%s");
      scope->xAxis->setTicker(timeTicker);
      scope->rescaleAxes();
      scope->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void MainWindow::refreshButton()
{
    int i;

    std::vector<std::string> devices = s.check_for_devices();

    ui->taggers->clear();
    for (i = 0; i < devices.size(); i++) {
        ui->taggers->addItem(tr(devices[i].c_str()));
    }
}

void MainWindow::connectButton()
{
}

void MainWindow::setupsignalslot(){


    //QTimer::singleShot(0, this, SLOT(showFullScreen()));


    qRegisterMetaType<vectorInt64>("vectorInt64");
    qRegisterMetaType<vectorInt32>("vectorInt32");
    qRegisterMetaType<vectorInt8>("vectorInt8");
    qRegisterMetaType<vectorDouble>("vectorDouble");
    qRegisterMetaType<boolvector2d>("boolvector2d");
    qRegisterMetaType<intvector>("intvector");



    QObject::connect(ui->connect_button, &QPushButton::released, this, &MainWindow::connectButton);
    QObject::connect(ui->refresh_button, &QPushButton::released, this, &MainWindow::refreshButton);

    QObject::connect(ui->startChan, SIGNAL(valueChanged(int)), &adq, SLOT(Chang_in_startChan(int)));
    QObject::connect(ui->startChan, SIGNAL(valueChanged(int)), &anl, SLOT(Chang_in_startChan(int)));
    QObject::connect(ui->startChan, SIGNAL(valueChanged(int)), this, SLOT(changeStartchan(int)));

    QObject::connect(ui->adqtime, SIGNAL(valueChanged(double)), this, SLOT(Chang_in_adqtime(double)));
    QObject::connect(ui->adqtime, SIGNAL(valueChanged(double)), &adq, SLOT(Chang_in_adqtime(double)));



    QObject::connect(ui->histStart, SIGNAL(valueChanged(int)), &anl, SLOT(Chang_in_histStart(int)));
    QObject::connect(ui->histStart, SIGNAL(valueChanged(int)), this, SLOT(Chang_in_histStart(int)));
    QObject::connect(ui->histStart, SIGNAL(valueChanged(int)), &adq, SLOT(Chang_in_histStart(int)));


    QObject::connect(ui->histEnd, SIGNAL(valueChanged(int)), &anl, SLOT(Chang_in_histEnd(int)));
    QObject::connect(ui->histEnd, SIGNAL(valueChanged(int)), this, SLOT(Chang_in_histEnd(int)));
    QObject::connect(ui->histEnd, SIGNAL(valueChanged(int)), &adq, SLOT(Chang_in_histEnd(int)));

    QObject::connect(ui->binsinplot, SIGNAL(valueChanged(int)), &anl, SLOT(Chang_in_binsinplot(int)));
    QObject::connect(ui->binsinplot, SIGNAL(valueChanged(int)), this, SLOT(Chang_in_binsinplot(int)));
    QObject::connect(ui->binsinplot, SIGNAL(valueChanged(int)), &adq, SLOT(Chang_in_binsinplot(int)));

    QObject::connect(ui->PlotAChn1, SIGNAL(valueChanged(int)), this, SLOT(Chang_in_PlotAChn1(int)));
    QObject::connect(ui->PlotAChn2, SIGNAL(valueChanged(int)), this, SLOT(Chang_in_PlotAChn2(int)));
    QObject::connect(ui->PlotBChn1, SIGNAL(valueChanged(int)), this, SLOT(Chang_in_PlotBChn1(int)));
    QObject::connect(ui->PlotBChn2, SIGNAL(valueChanged(int)), this, SLOT(Chang_in_PlotBChn2(int)));
    QObject::connect(ui->PlotCChn1, SIGNAL(valueChanged(int)), this, SLOT(Chang_in_PlotCChn1(int)));
    QObject::connect(ui->PlotCChn2, SIGNAL(valueChanged(int)), this, SLOT(Chang_in_PlotCChn2(int)));

    QObject::connect(ui->PlotAChn1, SIGNAL(valueChanged(int)), &adq, SLOT(Chang_in_PlotAChn1(int)));
    QObject::connect(ui->PlotAChn2, SIGNAL(valueChanged(int)), &adq, SLOT(Chang_in_PlotAChn2(int)));
    QObject::connect(ui->PlotBChn1, SIGNAL(valueChanged(int)), &adq, SLOT(Chang_in_PlotBChn1(int)));
    QObject::connect(ui->PlotBChn2, SIGNAL(valueChanged(int)), &adq, SLOT(Chang_in_PlotBChn2(int)));
    QObject::connect(ui->PlotCChn1, SIGNAL(valueChanged(int)), &adq, SLOT(Chang_in_PlotCChn1(int)));
    QObject::connect(ui->PlotCChn2, SIGNAL(valueChanged(int)), &adq, SLOT(Chang_in_PlotCChn2(int)));

    QObject::connect(ui->PlotAChn1, SIGNAL(valueChanged(int)), &anl, SLOT(Chang_in_PlotAChn1(int)));
    QObject::connect(ui->PlotAChn2, SIGNAL(valueChanged(int)), &anl, SLOT(Chang_in_PlotAChn2(int)));
    QObject::connect(ui->PlotBChn1, SIGNAL(valueChanged(int)), &anl, SLOT(Chang_in_PlotBChn1(int)));
    QObject::connect(ui->PlotBChn2, SIGNAL(valueChanged(int)), &anl, SLOT(Chang_in_PlotBChn2(int)));
    QObject::connect(ui->PlotCChn1, SIGNAL(valueChanged(int)), &anl, SLOT(Chang_in_PlotCChn1(int)));
    QObject::connect(ui->PlotCChn2, SIGNAL(valueChanged(int)), &anl, SLOT(Chang_in_PlotCChn2(int)));




    QObject::connect(ui->BegA1, SIGNAL(valueChanged(int)), this, SLOT(BegA1(int)));
    QObject::connect(ui->BegA2, SIGNAL(valueChanged(int)), this, SLOT(BegA2(int)));
    QObject::connect(ui->BegA3, SIGNAL(valueChanged(int)), this, SLOT(BegA3(int)));

    QObject::connect(ui->EndA1, SIGNAL(valueChanged(int)), this, SLOT(EndA1(int)));
    QObject::connect(ui->EndA2, SIGNAL(valueChanged(int)), this, SLOT(EndA2(int)));
    QObject::connect(ui->EndA3, SIGNAL(valueChanged(int)), this, SLOT(EndA3(int)));

    QObject::connect(ui->BegB1, SIGNAL(valueChanged(int)), this, SLOT(BegB1(int)));
    QObject::connect(ui->BegB2, SIGNAL(valueChanged(int)), this, SLOT(BegB2(int)));
    QObject::connect(ui->BegB3, SIGNAL(valueChanged(int)), this, SLOT(BegB3(int)));

    QObject::connect(ui->EndB1, SIGNAL(valueChanged(int)), this, SLOT(EndB1(int)));
    QObject::connect(ui->EndB2, SIGNAL(valueChanged(int)), this, SLOT(EndB2(int)));
    QObject::connect(ui->EndB3, SIGNAL(valueChanged(int)), this, SLOT(EndB3(int)));

    QObject::connect(ui->BegC1, SIGNAL(valueChanged(int)), this, SLOT(BegC1(int)));
    QObject::connect(ui->BegC2, SIGNAL(valueChanged(int)), this, SLOT(BegC2(int)));
    QObject::connect(ui->BegC3, SIGNAL(valueChanged(int)), this, SLOT(BegC3(int)));

    QObject::connect(ui->EndC1, SIGNAL(valueChanged(int)), this, SLOT(EndC1(int)));
    QObject::connect(ui->EndC2, SIGNAL(valueChanged(int)), this, SLOT(EndC2(int)));
    QObject::connect(ui->EndC3, SIGNAL(valueChanged(int)), this, SLOT(EndC3(int)));

    QObject::connect(ui->BegA1, SIGNAL(valueChanged(int)), &anl, SLOT(BegA1(int)));
    QObject::connect(ui->BegA2, SIGNAL(valueChanged(int)), &anl, SLOT(BegA2(int)));
    QObject::connect(ui->BegA3, SIGNAL(valueChanged(int)), &anl, SLOT(BegA3(int)));

    QObject::connect(ui->EndA1, SIGNAL(valueChanged(int)), &anl, SLOT(EndA1(int)));
    QObject::connect(ui->EndA2, SIGNAL(valueChanged(int)), &anl, SLOT(EndA2(int)));
    QObject::connect(ui->EndA3, SIGNAL(valueChanged(int)), &anl, SLOT(EndA3(int)));

    QObject::connect(ui->BegB1, SIGNAL(valueChanged(int)), &anl, SLOT(BegB1(int)));
    QObject::connect(ui->BegB2, SIGNAL(valueChanged(int)), &anl, SLOT(BegB2(int)));
    QObject::connect(ui->BegB3, SIGNAL(valueChanged(int)), &anl, SLOT(BegB3(int)));

    QObject::connect(ui->EndB1, SIGNAL(valueChanged(int)), &anl, SLOT(EndB1(int)));
    QObject::connect(ui->EndB2, SIGNAL(valueChanged(int)), &anl, SLOT(EndB2(int)));
    QObject::connect(ui->EndB3, SIGNAL(valueChanged(int)), &anl, SLOT(EndB3(int)));

    QObject::connect(ui->BegC1, SIGNAL(valueChanged(int)), &anl, SLOT(BegC1(int)));
    QObject::connect(ui->BegC2, SIGNAL(valueChanged(int)), &anl, SLOT(BegC2(int)));
    QObject::connect(ui->BegC3, SIGNAL(valueChanged(int)), &anl, SLOT(BegC3(int)));

    QObject::connect(ui->EndC1, SIGNAL(valueChanged(int)), &anl, SLOT(EndC1(int)));
    QObject::connect(ui->EndC2, SIGNAL(valueChanged(int)), &anl, SLOT(EndC2(int)));
    QObject::connect(ui->EndC3, SIGNAL(valueChanged(int)), &anl, SLOT(EndC3(int)));

    QObject::connect(ui->trackA1, SIGNAL(toggled(bool)), this, SLOT(Chang_track1(bool)));
    QObject::connect(ui->trackA2, SIGNAL(toggled(bool)), this, SLOT(Chang_track2(bool)));
    QObject::connect(ui->trackA3, SIGNAL(toggled(bool)), this, SLOT(Chang_track3(bool)));
    QObject::connect(ui->trackB1, SIGNAL(toggled(bool)), this, SLOT(Chang_track4(bool)));
    QObject::connect(ui->trackB2, SIGNAL(toggled(bool)), this, SLOT(Chang_track5(bool)));
    QObject::connect(ui->trackB3, SIGNAL(toggled(bool)), this, SLOT(Chang_track6(bool)));
    QObject::connect(ui->trackC1, SIGNAL(toggled(bool)), this, SLOT(Chang_track7(bool)));
    QObject::connect(ui->trackC2, SIGNAL(toggled(bool)), this, SLOT(Chang_track8(bool)));
    QObject::connect(ui->trackC3, SIGNAL(toggled(bool)), this, SLOT(Chang_track9(bool)));

    QObject::connect(&anl, SIGNAL(Chang_anlAvilable(bool)), &adq, SLOT(Chang_anlAvilable(bool)));

    QObject::connect(&anl, SIGNAL(CombinationChange(bool)), this, SLOT(CombinationChange(bool)));
   // QObject::connect(&anl, SIGNAL(CombinationChange(bool)), this, SLOT(CombinationChange(bool)));

    QObject::connect(&adq, SIGNAL(dataready(vectorInt64, vectorInt8, int)), &anl, SLOT(timestampANL(vectorInt64, vectorInt8, int)),Qt::QueuedConnection);


    QObject::connect(&anl, SIGNAL(anlongoing(bool)), &adq, SLOT(adqpausechange(bool)));

    QObject::connect(&adq, SIGNAL(qutaghist(vectorDouble, vectorDouble, vectorDouble)), this, SLOT(histoplot(vectorDouble, vectorDouble, vectorDouble)),Qt::QueuedConnection);

    QObject::connect(ui->thch1, SIGNAL(valueChanged(double)), &adq, SLOT(change_trigger_level_ch1(double)));
    QObject::connect(ui->thch2, SIGNAL(valueChanged(double)), &adq, SLOT(change_trigger_level_ch2(double)));
    QObject::connect(ui->thch3, SIGNAL(valueChanged(double)), &adq, SLOT(change_trigger_level_ch3(double)));
    QObject::connect(ui->thch4, SIGNAL(valueChanged(double)), &adq, SLOT(change_trigger_level_ch4(double)));
    QObject::connect(ui->delaych1, SIGNAL(valueChanged(double)), &adq, SLOT(change_delay_ch1(int)));
    QObject::connect(ui->delaych2, SIGNAL(valueChanged(double)), &adq, SLOT(change_delay_ch2(int)));
    QObject::connect(ui->delaych3, SIGNAL(valueChanged(double)), &adq, SLOT(change_delay_ch3(int)));
    QObject::connect(ui->delaych4, SIGNAL(valueChanged(double)), &adq, SLOT(change_delay_ch4(int)));

    QObject::connect(ui->actionSave_state, SIGNAL(triggered(bool)), this, SLOT(SaveState(bool)));
    QObject::connect(ui->actionLoad_state, SIGNAL(triggered(bool)), this, SLOT(LoadState(bool)));

    QObject::connect(this, SIGNAL(main_SaveAndValues(int, int, int , int , int, int , float , int )), &dbc, SLOT(SaveAndValues(int, int, int , int , int, int , float , int )));

    QObject::connect(this, SIGNAL(main_SaveRateValues( int, int , int , int , int , int , int , int , int , float)), &dbc, SLOT(SaveRateValues( int, int , int , int , int , int , int , int , int , float)));

    QObject::connect(ui->rof1, SIGNAL(currentTextChanged(QString)), &adq, SLOT(Chang_rof1(QString)));
    QObject::connect(ui->rof2, SIGNAL(currentTextChanged(QString)), &adq, SLOT(Chang_rof2(QString)));
    QObject::connect(ui->rof3, SIGNAL(currentTextChanged(QString)), &adq, SLOT(Chang_rof3(QString)));
    QObject::connect(ui->rof4, SIGNAL(currentTextChanged(QString)), &adq, SLOT(Chang_rof4(QString)));
    //QObject::connect(ui->rof5, SIGNAL(currentTextChanged(QString)), &adq, SLOT(Chang_rof1(QString)));
    //QObject::connect(ui->rof6, SIGNAL(currentTextChanged(QString)), &adq, SLOT(Chang_rof2(QString)));
    //QObject::connect(ui->rof7, SIGNAL(currentTextChanged(QString)), &adq, SLOT(Chang_rof3(QString)));
    //QObject::connect(ui->rof8, SIGNAL(currentTextChanged(QString)), &adq, SLOT(Chang_rof4(QString)));

    QObject::connect(ui->actionQKD, SIGNAL(triggered()), &qkdparam, SLOT(show()));

    QObject::connect(&qkdparam, SIGNAL(sig_QKD_timeA(double)), this, SLOT(chang_QKD_timeA(double)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_timeB(double)), this, SLOT(chang_QKD_timeB(double)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_timeC(double)), this, SLOT(chang_QKD_timeC(double)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_numbA(int)), this, SLOT(chang_QKD_numbA(int)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_numbB(int)), this, SLOT(chang_QKD_numbB(int)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_numbC(int)), this, SLOT(chang_QKD_numbC(int)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_phA(int)), this, SLOT(chang_QKD_phA(int)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_phB(int)), this, SLOT(chang_QKD_phB(int)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_phC(int)), this, SLOT(chang_QKD_phC(int)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_iwA(int)), this, SLOT(chang_QKD_iwA(int)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_iwB(int)), this, SLOT(chang_QKD_iwB(int)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_iwC(int)), this, SLOT(chang_QKD_iwC(int)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_pxqA(int)), this, SLOT(chang_QKD_pxqA(int)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_pxqB(int)), this, SLOT(chang_QKD_pxqB(int)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_pxqC(int)), this, SLOT(chang_QKD_pxqC(int)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_zeroA(int)), this, SLOT(chang_QKD_zeroA(int)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_zeroB(int)), this, SLOT(chang_QKD_zeroB(int)));
    QObject::connect(&qkdparam, SIGNAL(sig_QKD_zeroC(int)), this, SLOT(chang_QKD_zeroC(int)));

    QObject::connect(&dbc, SIGNAL(MYtables(QStringList)), this, SLOT(fillTablesNames(QStringList)));


    QObject::connect(&dbc, SIGNAL(qubitsfromDB(boolvector2d, int, int)), this, SLOT(set_qkd_datafromDB(boolvector2d , int, int)),Qt::QueuedConnection);

    QObject::connect(&qkdparam, SIGNAL(savehdf5()), this, SLOT(hdf5savefile()));
    QObject::connect(this, SIGNAL(MW_savehdf5(QString)), &dbc, SLOT(createHDF5forQKDdata(QString)));

    QObject::connect(this, SIGNAL(saveH5datafromMW(intvector,intvector,intvector,intvector,intvector,intvector,intvector,intvector,intvector,intvector,intvector,intvector)), &dbc, SLOT(appendQKDdata2HDF5(intvector,intvector,intvector,intvector,intvector,intvector,intvector,intvector,intvector,intvector,intvector,intvector)));

    QObject::connect(this, SIGNAL(main_SaveQKDresults(double,double, double, double ,double, double,double,double,double, double, double, double)),&dbc, SLOT(SaveQKDresults(double,double, double, double ,double, double,double,double,double, double, double, double)));
    QObject::connect(this, SIGNAL(main_SaveQKDstats(int, int, double, double)),&dbc, SLOT(SaveQKDstats(int, int, double, double)));

    QObject::connect(ui->menuLoad_Qubits, SIGNAL(triggered(QAction*)), this, SLOT(tableSelected(QAction*)));
    QObject::connect(this , SIGNAL(tableQKDtoDB(QString)), &dbc, SLOT(readQubits(QString)));

    QObject::connect(&qkdparam, SIGNAL(sig_turnONDB(int)), this, SLOT(chang_QKD_turnONDB(int)));
    ;
}



//////////////////////////////////////////////////////////
///////////////////plotting///////////////////////////
///////////////////////////////////////////////////////////

void MainWindow::plotRates(char AoBoC, int event, double key){


    double value1 = event; 

   if(AoBoC=='A'){

        ui->PlotTrack->graph(0)->addData(key-lastPointKey_tab1, value1);
    // remove data of lines that's outside visible range:key, value1
    //ui->PlotTrack->graph(0)->removeDataBefore(key-55);
    // rescale value (vertical) axis to fit the current data:
        //ui->PlotTrack->graph(0)->rescaleValueAxis(true);
      //  lastPointKey_tab1 = key;
 // make key axis range scroll with the data (at a constant range size of 8):
       // ui->PlotTrack->graph(0)->keyAxis()->setRange(key+0.25, 50, Qt::AlignRight);
        //ui->PlotTrack->xAxis->setRange(key, 8, Qt::AlignRight);
        //ui->PlotTrack->replot();
    }
   if(AoBoC=='B'){


        ui->PlotTrack->graph(1)->addData(key-lastPointKey_tab1, value1);


    }
   if(AoBoC=='C'){
        ui->PlotTrack->graph(2)->addData(key-lastPointKey_tab1, value1);

    }

    ui->PlotTrack->xAxis->setRange(key-lastPointKey_tab1, 120, Qt::AlignRight);
   //ui->PlotTrack->yAxis->rescale();

    ui->PlotTrack->replot();

    if(trackRateChang && AoBoC=='A'){
        ui->PlotTrack->graph(0)->data()->clear();
        trackRateChang=false;
    }
    if(trackRateChang && AoBoC=='B'){
        ui->PlotTrack->graph(1)->data()->clear();
        trackRateChang=false;
    }
    if(trackRateChang && AoBoC=='C'){
        ui->PlotTrack->graph(2)->data()->clear();
        trackRateChang=false;
    }
   //prom=0;
   //promedio.prepend(event);
   //for(int i=0; i<promedio.size(); i++)prom=prom+promedio[i];
   //prom=prom/promedio.size();
   //ui->mediamovil->display(prom);
   //if(promedio.size()>50)promedio.resize(50);
    //lastPointKey_tab1 = key;

}

void MainWindow::plot_qkd_results_det(double okA,double errA,double randA,double bkgndA,double okB,double errB,double randB,double bkgndB,double okC,double errC,double randC,double bkgndC, double key){

//std::cout<<"randA: "<<randA<<std::endl;

    ui->QKD_H1_results->graph(0)->addData(key-lastPointKey_tab3, okA);
    ui->QKD_H1_results->graph(1)->addData(key-lastPointKey_tab3, errA);
    ui->QKD_H1_results->graph(2)->addData(key-lastPointKey_tab3, randA);
    ui->QKD_H1_results->graph(3)->addData(key-lastPointKey_tab3, bkgndA);

    ui->QKD_H2_results->graph(0)->addData(key-lastPointKey_tab3, okB);
    ui->QKD_H2_results->graph(1)->addData(key-lastPointKey_tab3, errB);
    ui->QKD_H2_results->graph(2)->addData(key-lastPointKey_tab3, randB);
    ui->QKD_H2_results->graph(3)->addData(key-lastPointKey_tab3, bkgndB);

    ui->QKD_H3_results->graph(0)->addData(key-lastPointKey_tab3, okC);
    ui->QKD_H3_results->graph(1)->addData(key-lastPointKey_tab3, errC);
    ui->QKD_H3_results->graph(2)->addData(key-lastPointKey_tab3, randC);
    ui->QKD_H3_results->graph(3)->addData(key-lastPointKey_tab3, bkgndC);



    ui->QKD_H1_results->xAxis->setRange(key-lastPointKey_tab1, 120, Qt::AlignRight);
    ui->QKD_H2_results->xAxis->setRange(key-lastPointKey_tab1, 120, Qt::AlignRight);
    ui->QKD_H3_results->xAxis->setRange(key-lastPointKey_tab1, 120, Qt::AlignRight);
   //ui->PlotTrack->yAxis->rescale();

    ui->QKD_H1_results->replot();
    ui->QKD_H2_results->replot();
    ui->QKD_H3_results->replot();



}
void MainWindow::plot_qkd_results_QB(double okE,double errE,double randE,double bkgndE,double okL,double errL,double randL, double bkgndL,double okP,double errP,double randP,double bkgndP, double key){

//std::cout<<"randA: "<<randA<<std::endl;

    ui->Early_results->graph(0)->addData(key-lastPointKey_tab3, okE);
    ui->Early_results->graph(1)->addData(key-lastPointKey_tab3, errE);
    ui->Early_results->graph(2)->addData(key-lastPointKey_tab3, randE);
    ui->Early_results->graph(3)->addData(key-lastPointKey_tab3, bkgndE);

    ui->Late_results->graph(0)->addData(key-lastPointKey_tab3, okL);
    ui->Late_results->graph(1)->addData(key-lastPointKey_tab3, errL);
    ui->Late_results->graph(2)->addData(key-lastPointKey_tab3, randL);
    ui->Late_results->graph(3)->addData(key-lastPointKey_tab3, bkgndL);

    ui->Phase_results->graph(0)->addData(key-lastPointKey_tab3, okP);
    ui->Phase_results->graph(1)->addData(key-lastPointKey_tab3, errP);
    ui->Phase_results->graph(2)->addData(key-lastPointKey_tab3, randP);
    ui->Phase_results->graph(3)->addData(key-lastPointKey_tab3, bkgndP);



    ui->Early_results->xAxis->setRange(key-lastPointKey_tab1, 120, Qt::AlignRight);
    ui->Late_results->xAxis->setRange(key-lastPointKey_tab1, 120, Qt::AlignRight);
    ui->Phase_results->xAxis->setRange(key-lastPointKey_tab1, 120, Qt::AlignRight);
   //ui->PlotTrack->yAxis->rescale();

    ui->Early_results->replot();
    ui->Late_results->replot();
    ui->Phase_results->replot();



}

void MainWindow::plot_qkd_stats(double sifted_time, double sifted_phase, double error_time, double error_phase, double key){
    //std::cout<<sifted_time<<"   "<<sifted_phase<< "     "<<error_time<< "     "<< error_phase<<"   "<<key<<std::endl;

    ui->qkd_errorplot->graph(0)->addData(key-lastPointKey_tab3, error_time);
    ui->qkd_errorplot->graph(1)->addData(key-lastPointKey_tab3, error_phase);

    ui->qkd_siftedplot->graph(0)->addData(key-lastPointKey_tab3, sifted_time);
    ui->qkd_siftedplot->graph(1)->addData(key-lastPointKey_tab3, sifted_phase);




    ui->qkd_errorplot->xAxis->setRange(key-lastPointKey_tab1, 120, Qt::AlignRight);
    ui->qkd_siftedplot->xAxis->setRange(key-lastPointKey_tab1, 120, Qt::AlignRight);

   //ui->PlotTrack->yAxis->rescale();

    ui->qkd_errorplot->replot();
    ui->qkd_siftedplot->replot();

}
//////////////////////////////////////////////////////////
///////////////////histograms///////////////////////////
///////////////////////////////////////////////////////////


void MainWindow::histoplot(const vectorDouble &datA, const vectorDouble &datB, const vectorDouble &datC){
    //std::cout<<" histoplot hermanitititototot"<< datB.size()<<"   "<<datC.size() <<std::endl;
    double binwidth=((in_histEnd-in_histStart)/in_binsinplot);

  //  std::cout<<"histogram size   "<<datA.size()<<std::endl;
//std::cout<<"binwidth   "<<binwidth<<std::endl;
    QVector<double> x(datA.size());
//for (int i=in_histStart; i<in_histEnd; i++){
    for (int i=0; i<datA.size(); ++i){
        x[i] = binwidth*i+in_histStart;

    }

  ui->PlotA->graph(0)->data()->clear();
  // pass data points to graphs:
  ui->PlotA->graph(0)->setData(x, datA);
  //let the ranges scale themselves so graph 0 fits perfectly in the visible area:
  if(binwidth!=prevbinwidth)ui->PlotA->graph(0)->rescaleAxes();
  ui->PlotA->graph(0)->rescaleValueAxis();
  ui->PlotA->replot();

  ui->PlotB->graph(0)->data()->clear();
  // pass data points to graphs:
  ui->PlotB->graph(0)->setData(x, datB);
  //let the ranges scale themselves so graph 0 fits perfectly in the visible area:
  if(binwidth!=prevbinwidth)ui->PlotB->graph(0)->rescaleAxes();
  ui->PlotB->graph(0)->rescaleValueAxis();
  ui->PlotB->replot();

  ui->PlotC->graph(0)->data()->clear();
  // pass data points to graphs:
  ui->PlotC->graph(0)->setData(x, datC);
  //let the ranges scale themselves so graph 0 fits perfectly in the visible area:
  if(binwidth!=prevbinwidth)ui->PlotC->graph(0)->rescaleAxes();
  ui->PlotC->graph(0)->rescaleValueAxis();
  ui->PlotC->replot();
  prevbinwidth=binwidth;

  if(!Teleport0_or_QKD1){
      for (int D=0; D<datA.size(); D++) {
          for (int i=0; i<9; i++) {
              if(i<3){
                  if(x[D]>Plot_Win_BoE[0][i][0] && x[D]<Plot_Win_BoE[0][i][1])P_R[i]+=datA[D];
              }
              if(i>2 && i<6) {
                  if(x[D]>Plot_Win_BoE[1][i-3][0] && x[D]<Plot_Win_BoE[1][i-3][1])P_R[i]+=datB[D];
              }
              if(i>5) {
                  if(x[D]>Plot_Win_BoE[2][i-6][0] && x[D]<Plot_Win_BoE[2][i-6][1])P_R[i]+=datC[D];
              }

           }

      }
      ui->countA1->display(P_R[0]);
      ui->countA2->display(P_R[1]);
      ui->countA3->display(P_R[2]);
      ui->countB1->display(P_R[3]);
      ui->countB2->display(P_R[4]);
      ui->countB3->display(P_R[5]);
      ui->countC1->display(P_R[6]);
      ui->countC2->display(P_R[7]);
      ui->countC3->display(P_R[8]);

      if(dbrunning)emit main_SaveRateValues(P_R[0], P_R[1],  P_R[2], P_R[3], P_R[4], P_R[5], P_R[6], P_R[7], P_R[8], float(in_adqtime));

      double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
      for (int i=0;i<9;i++){
          char AoBoC = char(NULL);
          if(P_T[i]){
              if(i<3)AoBoC='A';
              if(i>2 && i<6) AoBoC='B';
              if(i>5) AoBoC='C';
              plotRates(AoBoC, P_R[i], key);
          }
      }
      for (int i=0;i<9;i++)P_R[i]=0;
  }


  //////////////////
  ////QKD HISTO/////
  /// //////////////

  if(Teleport0_or_QKD1 && qubitsfromDBloaded){

      double resultAok=0, resultAerr=0, resultArand=0, resultAbkgnd=0;
      double resultBok=0, resultBerr=0, resultBrand=0, resultBbkgnd=0;
      double resultCok=0, resultCerr=0, resultCrand=0, resultCbkgnd=0;
      double tempresultAok=0, tempresultAerr=0, tempresultArand=0, tempresultAbkgnd=0;
      double tempresultBok=0, tempresultBerr=0, tempresultBrand=0, tempresultBbkgnd=0;
      double tempresultCok=0, tempresultCerr=0, tempresultCrand=0, tempresultCbkgnd=0;

      double resultEok=0, resultEerr=0, resultErand=0;
      double resultLok=0, resultLerr=0, resultLrand=0;
      double resultPok=0, resultPerr=0, resultPrand=0;
      double totalBkgnd=0;
     /* double tempresultEok=0, tempresultEerr=0, tempresultErand=0;
      double tempresultLok=0, tempresultLerr=0, tempresultLrand=0;
      double tempresultPok=0, tempresultPerr=0, tempresultPrand=0;*/

      int error_ok_time = 0, error_ok_phase = 0, error_time = 0, error_phase = 0;


      //////////////Histo A//////////////
       // std::cout<<"in_QKD_numbA: "<<in_QKD_numbA<<std::endl;
      for(int q=0; (q<in_QKD_numbA && q<in_qkdfromDB.size()); q++){
          for (int D = int((q*int(in_QKD_timeA)+in_QKD_zeroA)/binwidth); D<=int((q*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA)/binwidth); D++) {//first win
              if(q*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA<datA.size()){
                  tempresultAok+=in_qkdfromDB[q][0]*datA[D];
                  tempresultAerr+=in_qkdfromDB[q][1]*datA[D];
                  tempresultArand+=in_qkdfromDB[q][2]*datA[D];

              }
          }

          for (int D = int((q*int(in_QKD_timeA)+in_QKD_zeroA+in_QKD_phA)/binwidth); D<=int((q*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA+in_QKD_phA)/binwidth); D++) {//second win
              if(q*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA+in_QKD_phA<datA.size()){
                  tempresultAok+=in_qkdfromDB[q][3]*datA[D];
                  tempresultAerr+=in_qkdfromDB[q][4]*datA[D];
                  tempresultArand+=in_qkdfromDB[q][5]*datA[D];
               }
          }

          for (int D = int((q*int(in_QKD_timeA)+in_QKD_zeroA+2*in_QKD_phA)/binwidth); D<=int((q*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA+2*in_QKD_phA)/binwidth); D++) {//third win
              if(q*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA+2*in_QKD_phA<datA.size()){
                  tempresultAok+=in_qkdfromDB[q][6]*datA[D];
                  tempresultAerr+=in_qkdfromDB[q][7]*datA[D];
                  tempresultArand+=in_qkdfromDB[q][8]*datA[D];
              }
          }

         //if(q==0)std::cout<<q*int(in_QKD_timeA)+in_QKD_zeroA+3*in_QKD_phA<<"       "<<q*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA+3*in_QKD_phA<<std::endl;

          for (int D = int((q*int(in_QKD_timeA)+in_QKD_zeroA+3*in_QKD_phA)/binwidth); D<=int((q*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA+3*in_QKD_phA)/binwidth); D++) {//last win (background)
              if(q*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA+3*in_QKD_phA<datA.size())tempresultAbkgnd+=datA[D];
          }
   // std::cout<<tempresultAbkgnd<<std::endl;
          if(in_qkdfromDB[q][2]&&in_qkdfromDB[q][8]){//phase calc  R -- * -- R
              error_ok_phase += tempresultAok+tempresultAerr;
              error_phase += tempresultAerr;

              resultPok+= tempresultAok;
              resultPerr+= tempresultAerr;
              resultPrand+= tempresultArand;
          }
          if(in_qkdfromDB[q][5]&&!in_qkdfromDB[q][2]&&!in_qkdfromDB[q][8]){//time calc    !R -- R -- !R
              error_ok_time += tempresultAok+tempresultAerr;
              error_time += tempresultAerr;
          }

          if(in_qkdfromDB[q][0]&&in_qkdfromDB[q][5]&&in_qkdfromDB[q][7]){//time calc E    O -- R -- E
              resultEok+= tempresultAok;
              resultEerr+= tempresultAerr;
              resultErand+= tempresultArand;
          }
          if(in_qkdfromDB[q][1]&&in_qkdfromDB[q][5]&&in_qkdfromDB[q][6]){//time calc L  E -- R -- O
              resultLok+= tempresultAok;
              resultLerr+= tempresultAerr;
              resultLrand+= tempresultArand;
          }

         // if(q==0)std::cout<<in_qkdfromDB[q][8]<<"       "<<tempresultArand<<std::endl;
              /////cross qubit integration////
          resultAok+=tempresultAok;
          resultArand+=tempresultArand;
          resultAerr+=tempresultAerr;
          resultAbkgnd+=tempresultAbkgnd;

             /////cross histogram integration////

          if(q+1>datah5okA.size() && q+1>datah5errA.size() && q+1>datah5randA.size() && q+1>datah5bkgndA.size()){///if the array doesnt have the qubit
              datah5okA.append(tempresultAok);
              datah5randA.append(tempresultArand);
              datah5errA.append(tempresultAerr);
              datah5bkgndA.append(tempresultAbkgnd);
          }
          else{ ///if it have it, integrate
              datah5okA[q]+=tempresultAok;
              datah5randA[q]+=tempresultArand;
              datah5errA[q]+=tempresultAerr;
              datah5bkgndA[q]+=tempresultAbkgnd;
          }

          tempresultAok=0;
          tempresultAerr=0;
          tempresultArand=0;
          tempresultAbkgnd=0;
        // if(resultArand>200000)std::cout<<q<<std::endl;
      }

      ////////Histo B/////////////



      for(int q=0; (q<in_QKD_numbB && q<in_qkdfromDB.size()); q++){
          for (int D = q*int(in_QKD_timeB)+in_QKD_zeroB; D<=q*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB; D++) {//first win
              if(q*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB<datB.size()){
                  tempresultBok+=in_qkdfromDB[q][9]*datB[D];
                  tempresultBerr+=in_qkdfromDB[q][10]*datB[D];
                  tempresultBrand+=in_qkdfromDB[q][11]*datB[D];
              }
          }

          for (int D = q*int(in_QKD_timeB)+in_QKD_zeroB+in_QKD_phB; D<=q*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB+in_QKD_phB; D++) {//second win
              if(q*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB+in_QKD_phB<datB.size()){
                  tempresultBok+=in_qkdfromDB[q][12]*datB[D];
                  tempresultBerr+=in_qkdfromDB[q][13]*datB[D];
                  tempresultBrand+=in_qkdfromDB[q][14]*datB[D];
              }
          }

          for (int D = q*int(in_QKD_timeB)+in_QKD_zeroB+2*in_QKD_phB; D<=q*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB+2*in_QKD_phB; D++) {//third win
              if(q*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB+2*in_QKD_phB<datB.size()){
                  tempresultBok+=in_qkdfromDB[q][15]*datB[D];
                  tempresultBerr+=in_qkdfromDB[q][16]*datB[D];
                  tempresultBrand+=in_qkdfromDB[q][17]*datB[D];
              }
          }

          for (int D = q*int(in_QKD_timeB)+in_QKD_zeroB+3*in_QKD_phB; D<=q*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB+3*in_QKD_phB; D++) {//last win (background)
              if(q*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB+3*in_QKD_phB<datB.size())tempresultBbkgnd+=datB[D];
          }

          if(in_qkdfromDB[q][11]&&in_qkdfromDB[q][17]){//phase calc  R -- * -- R
              error_ok_phase += tempresultBok+tempresultBerr;
              error_phase += tempresultBerr;

              resultPok+= tempresultBok;
              resultPerr+= tempresultBerr;
              resultPrand+= tempresultBrand;

          }
          if(in_qkdfromDB[q][14]&&!in_qkdfromDB[q][11]&&!in_qkdfromDB[q][17]){//time calc    !R -- R -- !R
              error_ok_time += tempresultBok+tempresultBerr;
              error_time += tempresultBerr;
          }

          if(in_qkdfromDB[q][9]&&in_qkdfromDB[q][14]&&in_qkdfromDB[q][16]){//time calc E    O -- R -- E
              resultEok+= tempresultBok;
              resultEerr+= tempresultBerr;
              resultErand+= tempresultBrand;
          }
          if(in_qkdfromDB[q][10]&&in_qkdfromDB[q][14]&&in_qkdfromDB[q][15]){//time calc L  E -- R -- O
              resultLok+= tempresultBok;
              resultLerr+= tempresultBerr;
              resultLrand+= tempresultBrand;
          }

              /////cross qubit integration////
          resultBok+=tempresultBok;
          resultBrand+=tempresultBrand;
          resultBerr+=tempresultBerr;
          resultBbkgnd+=tempresultBbkgnd;

             /////cross histogram integration////

          if(q+1>datah5okB.size() && q+1>datah5errB.size() && q+1>datah5randB.size() && q+1>datah5bkgndB.size()){///if the array doesnt have the qubit
              datah5okB.append(tempresultBok);
              datah5randB.append(tempresultBrand);
              datah5errB.append(tempresultBerr);
              datah5bkgndB.append(tempresultBbkgnd);
          }
          else{ ///if it have it, integrate
              datah5okB[q]+=tempresultBok;
              datah5randB[q]+=tempresultBrand;
              datah5errB[q]+=tempresultBerr;
              datah5bkgndB[q]+=tempresultBbkgnd;
          }

          tempresultBok=0;
          tempresultBerr=0;
          tempresultBrand=0;
          tempresultBbkgnd=0;
      }



      ////////Histo C////////////


      for(int q=0; (q<in_QKD_numbC && q<in_qkdfromDB.size()); q++){
          for (int D = q*int(in_QKD_timeC)+in_QKD_zeroC; D<=q*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC; D++) {//first win
              if(q*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC<datC.size()){
                  tempresultCok+=in_qkdfromDB[q][18]*datC[D];
                  tempresultCerr+=in_qkdfromDB[q][19]*datC[D];
                  tempresultCrand+=in_qkdfromDB[q][20]*datC[D];
              }
          }

          for (int D = q*int(in_QKD_timeC)+in_QKD_zeroC+in_QKD_phC; D<=q*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC+in_QKD_phC; D++) {//second win
              if(q*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC+in_QKD_phC<datC.size()){
                  tempresultCok+=in_qkdfromDB[q][21]*datC[D];
                  tempresultCerr+=in_qkdfromDB[q][22]*datC[D];
                  tempresultCrand+=in_qkdfromDB[q][23]*datC[D];
              }
          }

          for (int D = q*int(in_QKD_timeC)+in_QKD_zeroC+2*in_QKD_phC; D<=q*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC+2*in_QKD_phC; D++) {//third win
              if(q*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC+2*in_QKD_phC<datC.size()){
                  tempresultCok+=in_qkdfromDB[q][24]*datC[D];
                  tempresultCerr+=in_qkdfromDB[q][25]*datC[D];
                  tempresultCrand+=in_qkdfromDB[q][26]*datC[D];
              }
          }

          for (int D = q*int(in_QKD_timeC)+in_QKD_zeroC+3*in_QKD_phC; D<=q*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC+3*in_QKD_phC; D++) {//last win (background)
              if(q*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC+3*in_QKD_phC<datC.size())tempresultCbkgnd+=datC[D];
          }

          if(in_qkdfromDB[q][20]&&in_qkdfromDB[q][26]){//phase calc  R -- * -- R
              error_ok_phase += tempresultCok+tempresultCerr;
              error_phase += tempresultCerr;

              resultPok+= tempresultCok;
              resultPerr+= tempresultCerr;
              resultPrand+= tempresultCrand;
          }
          if(in_qkdfromDB[q][23]&&!in_qkdfromDB[q][20]&&!in_qkdfromDB[q][26]){//time calc    !R -- R -- !R
              error_ok_time += tempresultCok+tempresultCerr;
              error_time += tempresultCerr;
          }

          if(in_qkdfromDB[q][18]&&in_qkdfromDB[q][23]&&in_qkdfromDB[q][25]){//time calc E    O -- R -- E
              resultEok+= tempresultCok;
              resultEerr+= tempresultCerr;
              resultErand+= tempresultCrand;
          }
          if(in_qkdfromDB[q][19]&&in_qkdfromDB[q][23]&&in_qkdfromDB[q][24]){//time calc L  E -- R -- O
              resultLok+= tempresultCok;
              resultLerr+= tempresultCerr;
              resultLrand+= tempresultCrand;
          }
              /////cross qubit integration////
          resultCok+=tempresultCok;
          resultCrand+=tempresultCrand;
          resultCerr+=tempresultCerr;
          resultCbkgnd+=tempresultCbkgnd;

             /////cross histogram integration////

          if(q+1>datah5okC.size() && q+1>datah5errC.size() && q+1>datah5randC.size() && q+1>datah5bkgndC.size()){///if the array doesnt have the qubit
              datah5okC.append(tempresultCok);
              datah5randC.append(tempresultCrand);
              datah5errC.append(tempresultCerr);
              datah5bkgndC.append(tempresultCbkgnd);
          }
          else{ ///if it have it, integrate
              datah5okC[q]+=tempresultCok;
              datah5randC[q]+=tempresultCrand;
              datah5errC[q]+=tempresultCerr;
              datah5bkgndC[q]+=tempresultCbkgnd;
          }

          tempresultCok=0;
          tempresultCerr=0;
          tempresultCrand=0;
          tempresultCbkgnd=0;
      }


      totalBkgnd = resultAbkgnd+resultBbkgnd+resultCbkgnd;
      ///////Data submitt//////////


      double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

     // std::cout<<"resultAok:"<<resultAok<<"  resultAerr:"<<resultAerr<<"   resultArand:"<<resultArand<<"   resultAbkgnd:"<< resultAbkgnd<<std::endl;
      plot_qkd_results_det(resultAok, resultAerr,resultArand, resultAbkgnd, resultBok, resultBerr, resultBrand, resultBbkgnd,resultCok, resultCerr, resultCrand, resultCbkgnd,key);
      plot_qkd_results_QB(resultEok, resultEerr,resultErand, totalBkgnd, resultLok, resultLerr, resultLrand, totalBkgnd,resultPok, resultPerr, resultPrand, totalBkgnd,key);

      if(QKD_DB_ON)emit main_SaveQKDresults(resultAok, resultAerr,resultArand, resultAbkgnd, resultBok, resultBerr, resultBrand, resultBbkgnd,resultCok, resultCerr, resultCrand, resultCbkgnd);

      double error_rate_time=0, error_rate_phase=0;

      if(error_ok_time>0){
          error_rate_time= double(error_time)/double(error_ok_time);
          //std::cout<<"error_time: "<<error_time<< "  ||  error_ok_time: "<<error_ok_time<<"  ||  error_rate_time: "<<error_rate_time<<std::endl;
      }
      if(error_ok_phase>0)error_rate_phase= double(error_phase)/double(error_ok_phase+2*resultPrand);
      int sift_phase = error_ok_phase+2*resultPrand;


      resultAok=0;resultAerr=0;resultArand=0;resultAbkgnd=0;resultBok=0;resultBerr=0;resultBrand=0;resultBbkgnd=0;resultCok=0;resultCerr=0;resultCrand=0;resultCbkgnd=0;
      resultEok=0;resultEerr=0;resultErand=0;resultLok=0;resultLerr=0;resultLrand=0;resultPok=0;resultPerr=0;resultPrand=0;


      //std::cout<<error_ok_time<<"   "<<error_ok_phase<< "     "<<error_time<< "     "<< error_phase<<"   "<<key<<std::endl;
      plot_qkd_stats(error_ok_time, sift_phase, error_rate_time, error_rate_phase, key);
      if(QKD_DB_ON)emit main_SaveQKDstats(error_ok_time, sift_phase, error_rate_time, error_rate_phase);
      udpcom.sendDataToClient(error_rate_phase);
      error_time = 0;
      error_phase = 0;
      error_rate_time=0;
      error_rate_phase=0;
      sift_phase = 0;



      /////HDF5 data submission/////////
      if(key-qkd_prevKey>HDF5TIMEINTEGRATION && HDF5File_created){

          //emit saveH5datafromMW(datah5okA, datah5errA, datah5randA, datah5bkgndA,datah5okB, datah5errB, datah5randB, datah5bkgndB,datah5okC, datah5errC, datah5randC, datah5bkgndC);

          datah5okA.clear();
          datah5errA.clear();
          datah5randA.clear();
          datah5bkgndA.clear();

          datah5okB.clear();
          datah5errB.clear();
          datah5randB.clear();
          datah5bkgndB.clear();

          datah5okC.clear();
          datah5errC.clear();
          datah5randC.clear();
          datah5bkgndC.clear();

          qkd_prevKey=key;

      }

  }//end of QKD


  x.clear();

	
}

/////////////////////////////////////
///////////lines plots///////////////
/////////////////////////////////////


void MainWindow::LinePlot(){

    if(!Teleport0_or_QKD1){

        for (int i=0; i<18; i++) {

            if(i==0 || i==3 || i==6 || i==9 || i==12 || i==15)infLine[i]->setPen(QPen(Qt::red));
            if(i==1 || i==4 || i==7 || i==10 || i==13 || i==16)infLine[i]->setPen(QPen(Qt::green));
            if(i==2 || i==5 || i==8 || i==11  || i==14 || i==17)infLine[i]->setPen(QPen(Qt::yellow));



            if(i<6){

                if(i<3)infLine[i]->point1->setCoords(Plot_Win_BoE[0][i][0], 0);
                    else infLine[i]->point1->setCoords(Plot_Win_BoE[0][i-3][1], 0);
                if(i<3)infLine[i]->point2->setCoords(Plot_Win_BoE[0][i][0], 1);
                    else infLine[i]->point2->setCoords(Plot_Win_BoE[0][i-3][1], 1);


                ui->PlotA->replot();

            }
            if(i>5 && i<12){
                if(i<9)infLine[i]->point1->setCoords(Plot_Win_BoE[1][i-6][0], 0);
                    else infLine[i]->point1->setCoords(Plot_Win_BoE[1][i-6-3][1], 0);
                if(i<9)infLine[i]->point2->setCoords(Plot_Win_BoE[1][i-6][0], 1);
                    else infLine[i]->point2->setCoords(Plot_Win_BoE[1][i-6-3][1], 1);

                ui->PlotB->replot();


            }
            if(i>11){
                if(i<15)infLine[i]->point1->setCoords(Plot_Win_BoE[2][i-12][0], 0);
                    else infLine[i]->point1->setCoords(Plot_Win_BoE[2][i-12-3][1], 0);
                if(i<15)infLine[i]->point2->setCoords(Plot_Win_BoE[2][i-12][0], 1);
                    else infLine[i]->point2->setCoords(Plot_Win_BoE[2][i-12-3][1], 1);

                ui->PlotC->replot();

                }
        }
    }
}

void MainWindow::createQKDLinesA(){
/*std::cout<<in_QKD_timeA<<std::endl;
std::cout<<in_QKD_zeroA<<std::endl;
std::cout<<in_QKD_iwA<<std::endl;*/

    for(int i=0; i<in_QKD_numbA*2; i++){
       if((i+1)%2){
            LinesPlotA1[i]->setVisible(1);
            LinesPlotA1[i]->point1->setCoords(i/2*in_QKD_timeA+in_QKD_zeroA,0);
            LinesPlotA1[i]->point2->setCoords(i/2*in_QKD_timeA+in_QKD_zeroA,1);
            LinesPlotA2[i]->setVisible(1);
            LinesPlotA2[i]->point1->setCoords(i/2*in_QKD_timeA+in_QKD_zeroA+in_QKD_phA,0);
            LinesPlotA2[i]->point2->setCoords(i/2*in_QKD_timeA+in_QKD_zeroA+in_QKD_phA,1);
            LinesPlotA3[i]->setVisible(1);
            LinesPlotA3[i]->point1->setCoords(i/2*in_QKD_timeA+in_QKD_zeroA+2*in_QKD_phA,0);
            LinesPlotA3[i]->point2->setCoords(i/2*in_QKD_timeA+in_QKD_zeroA+2*in_QKD_phA,1);
            LinesPlotA4[i]->setVisible(1);
            LinesPlotA4[i]->point1->setCoords(i/2*in_QKD_timeA+in_QKD_zeroA+3*in_QKD_phA,0);
            LinesPlotA4[i]->point2->setCoords(i/2*in_QKD_timeA+in_QKD_zeroA+3*in_QKD_phA,1);

            //std::cout<<"1";
       }
       else{
            LinesPlotA1[i]->setVisible(1);
            LinesPlotA1[i]->point1->setCoords((i-1)/2*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA,0);
            LinesPlotA1[i]->point2->setCoords((i-1)/2*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA,1);
            LinesPlotA2[i]->setVisible(1);
            LinesPlotA2[i]->point1->setCoords((i-1)/2*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA+in_QKD_phA,0);
            LinesPlotA2[i]->point2->setCoords((i-1)/2*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA+in_QKD_phA,1);
            LinesPlotA3[i]->setVisible(1);
            LinesPlotA3[i]->point1->setCoords((i-1)/2*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA+2*in_QKD_phA,0);
            LinesPlotA3[i]->point2->setCoords((i-1)/2*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA+2*in_QKD_phA,1);
            LinesPlotA4[i]->setVisible(1);
            LinesPlotA4[i]->point1->setCoords((i-1)/2*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA+3*in_QKD_phA,0);
            LinesPlotA4[i]->point2->setCoords((i-1)/2*in_QKD_timeA+in_QKD_zeroA+in_QKD_iwA+3*in_QKD_phA,1);
       }
    }

    ui->PlotA->replot();

}
void MainWindow::createQKDLinesB(){
    for(int i=0; i<in_QKD_numbB*2; i++){
       if((i+1)%2){
            LinesPlotB1[i]->setVisible(1);
            LinesPlotB1[i]->point1->setCoords(i/2*in_QKD_timeB+in_QKD_zeroB,0);
            LinesPlotB1[i]->point2->setCoords(i/2*in_QKD_timeB+in_QKD_zeroB,1);
            LinesPlotB2[i]->setVisible(1);
            LinesPlotB2[i]->point1->setCoords(i/2*in_QKD_timeB+in_QKD_zeroB+in_QKD_phB,0);
            LinesPlotB2[i]->point2->setCoords(i/2*in_QKD_timeB+in_QKD_zeroB+in_QKD_phB,1);
            LinesPlotB3[i]->setVisible(1);
            LinesPlotB3[i]->point1->setCoords(i/2*in_QKD_timeB+in_QKD_zeroB+2*in_QKD_phB,0);
            LinesPlotB3[i]->point2->setCoords(i/2*in_QKD_timeB+in_QKD_zeroB+2*in_QKD_phB,1);
            LinesPlotB4[i]->setVisible(1);
            LinesPlotB4[i]->point1->setCoords(i/2*in_QKD_timeB+in_QKD_zeroB+3*in_QKD_phB,0);
            LinesPlotB4[i]->point2->setCoords(i/2*in_QKD_timeB+in_QKD_zeroB+3*in_QKD_phB,1);
       }
       else{
            LinesPlotB1[i]->setVisible(1);
            LinesPlotB1[i]->point1->setCoords((i-1)/2*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB,0);
            LinesPlotB1[i]->point2->setCoords((i-1)/2*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB,1);
            LinesPlotB2[i]->setVisible(1);
            LinesPlotB2[i]->point1->setCoords((i-1)/2*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB+in_QKD_phB,0);
            LinesPlotB2[i]->point2->setCoords((i-1)/2*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB+in_QKD_phB,1);
            LinesPlotB3[i]->setVisible(1);
            LinesPlotB3[i]->point1->setCoords((i-1)/2*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB+2*in_QKD_phB,0);
            LinesPlotB3[i]->point2->setCoords((i-1)/2*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB+2*in_QKD_phB,1);
            LinesPlotB4[i]->setVisible(1);
            LinesPlotB4[i]->point1->setCoords((i-1)/2*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB+3*in_QKD_phB,0);
            LinesPlotB4[i]->point2->setCoords((i-1)/2*in_QKD_timeB+in_QKD_zeroB+in_QKD_iwB+3*in_QKD_phB,1);
       }
    }
    ui->PlotB->replot();
}
void MainWindow::createQKDLinesC(){
    for(int i=0; i<in_QKD_numbC*2; i++){
       if((i+1)%2){
            LinesPlotC1[i]->setVisible(1);
            LinesPlotC1[i]->point1->setCoords(i/2*in_QKD_timeC+in_QKD_zeroC,0);
            LinesPlotC1[i]->point2->setCoords(i/2*in_QKD_timeC+in_QKD_zeroC,1);
            LinesPlotC2[i]->setVisible(1);
            LinesPlotC2[i]->point1->setCoords(i/2*in_QKD_timeC+in_QKD_zeroC+in_QKD_phC,0);
            LinesPlotC2[i]->point2->setCoords(i/2*in_QKD_timeC+in_QKD_zeroC+in_QKD_phC,1);
            LinesPlotC3[i]->setVisible(1);
            LinesPlotC3[i]->point1->setCoords(i/2*in_QKD_timeC+in_QKD_zeroC+2*in_QKD_phC,0);
            LinesPlotC3[i]->point2->setCoords(i/2*in_QKD_timeC+in_QKD_zeroC+2*in_QKD_phC,1);
            LinesPlotC4[i]->setVisible(1);
            LinesPlotC4[i]->point1->setCoords(i/2*in_QKD_timeC+in_QKD_zeroC+3*in_QKD_phC,0);
            LinesPlotC4[i]->point2->setCoords(i/2*in_QKD_timeC+in_QKD_zeroC+3*in_QKD_phC,1);
       }
       else{
            LinesPlotC1[i]->setVisible(1);
            LinesPlotC1[i]->point1->setCoords((i-1)/2*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC,0);
            LinesPlotC1[i]->point2->setCoords((i-1)/2*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC,1);
            LinesPlotC2[i]->setVisible(1);
            LinesPlotC2[i]->point1->setCoords((i-1)/2*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC+in_QKD_phC,0);
            LinesPlotC2[i]->point2->setCoords((i-1)/2*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC+in_QKD_phC,1);
            LinesPlotC3[i]->setVisible(1);
            LinesPlotC3[i]->point1->setCoords((i-1)/2*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC+2*in_QKD_phC,0);
            LinesPlotC3[i]->point2->setCoords((i-1)/2*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC+2*in_QKD_phC,1);
            LinesPlotC4[i]->setVisible(1);
            LinesPlotC4[i]->point1->setCoords((i-1)/2*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC+3*in_QKD_phC,0);
            LinesPlotC4[i]->point2->setCoords((i-1)/2*in_QKD_timeC+in_QKD_zeroC+in_QKD_iwC+3*in_QKD_phC,1);
       }
    }
    ui->PlotC->replot();
}

void MainWindow::hidelinesA(int val){

    for (int i= 2*val;i<2*in_QKD_numbA;i++) {
         LinesPlotA1[i]->setVisible(0);
         LinesPlotA2[i]->setVisible(0);
         LinesPlotA3[i]->setVisible(0);
         LinesPlotA4[i]->setVisible(0);
    }
}
void MainWindow::hidelinesB(int val){

    for (int i= 2*val;i<2*in_QKD_numbB;i++) {
         LinesPlotB1[i]->setVisible(0);
         LinesPlotB2[i]->setVisible(0);
         LinesPlotB3[i]->setVisible(0);
         LinesPlotB4[i]->setVisible(0);
    }
}
void MainWindow::hidelinesC(int val){

    for (int i= 2*val;i<2*in_QKD_numbC;i++) {
         LinesPlotC1[i]->setVisible(0);
         LinesPlotC2[i]->setVisible(0);
         LinesPlotC3[i]->setVisible(0);
         LinesPlotC4[i]->setVisible(0);
    }
}

void MainWindow::turnONDB(int val){
    /*if(!dbc.isRunning() && dbrunning == 0 && val==1){
        dbc.run();
        dbrunning=val;
    }
    if(dbc.isRunning() && dbrunning == 1 && val==0){
        dbrunning = val;
        while(dbc.isRunning())usleep(100);
        dbc.~DBControl();
    }*/
    dbrunning=val;
}


void MainWindow::SaveState(bool a){
//std::cout<<"debug"<<std::cout;
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Current Configuration"), "",
            tr("Configuration (*.conf);;All Files (*)"));

   //QString fileName = "test";
    if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }

            QDataStream out(&file);

                    out.setVersion(QDataStream::Qt_4_5);
                    QMap<QString, int> mapint;
                    QMap<QString, double> mapdouble;
                    QString localstring;


                    for ( int i=0;i<3;i++) {
                           for (int j =0;j<3;j++) {
                               for (int k = 0;k<2;k++) {
                                   localstring = QString("Plot_Win_BoE[%1][%2][%3]").arg(i).arg(j).arg(k);
                                   //std::cout<< localstring.toStdString()<< std::endl;
                                    if(!mapint.contains(localstring))mapint.insert(localstring,Plot_Win_BoE[i][j][k]);
                                    else mapint.value(localstring, Plot_Win_BoE[i][j][k]);
                               }

                           }

                     }

                    mapint.insert("in_startChan",in_startChan);
                    mapint.insert("in_PlotACh1",in_PlotACh1);
                    mapint.insert("in_PlotACh2",in_PlotACh2);
                    mapint.insert("in_PlotBCh1",in_PlotBCh1);
                    mapint.insert("in_PlotBCh2",in_PlotBCh2);
                    mapint.insert("in_PlotCCh1",in_PlotCCh1);
                    mapint.insert("in_PlotCCh2",in_PlotCCh2);
                    mapint.insert("in_histStart",in_histStart);
                    mapint.insert("in_histEnd",in_histEnd);
                    mapint.insert("in_binsinplot",in_binsinplot);

                    mapdouble.insert("in_adqtime", in_adqtime);

                    /*QMapIterator<QString,int>ip(mapint);
                    while (ip.hasNext()) {
                        ip.next();
                        std::cout<< ip.key().toStdString()<< ": " << ip.value() << std::endl;
                    }*/
                    out<<mapint;
                    out<<mapdouble;
            }

   // LoadState(1);
}



void MainWindow::LoadState(bool a){
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Load Configuration"), "",
            tr("Configuration (*.conf);;All Files (*)"));
    //QString fileName = "test";
    if (fileName.isEmpty())
            return;
        else {

            QFile file(fileName);

            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }
            QMap<QString, int> mapintout;
            QMap<QString, double> mapdoubleout;
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_4_5);
            in>>mapintout;

            /*QString localstring;
            for ( int i=0;i<3;i++) {
                   for (int j =0;j<3;j++) {
                       for (int k = 0;k<2;k++) {
                           localstring = QString("Plot_Win_BoE[%1][%2][%3]").arg(i).arg(j).arg(k);
                            if(mapintout.contains(localstring))Plot_Win_BoE[i][j][k]= mapintout.value(localstring);
                       }
                   }
             }*/

            if(mapintout.contains("Plot_Win_BoE[0][0][0]"))ui->BegA1->setValue(mapintout.value("Plot_Win_BoE[0][0][0]"));
            if(mapintout.contains("Plot_Win_BoE[0][1][0]"))ui->BegA2->setValue(mapintout.value("Plot_Win_BoE[0][1][0]"));
            if(mapintout.contains("Plot_Win_BoE[0][2][0]"))ui->BegA3->setValue(mapintout.value("Plot_Win_BoE[0][2][0]"));
            if(mapintout.contains("Plot_Win_BoE[0][0][1]"))ui->EndA1->setValue(mapintout.value("Plot_Win_BoE[0][0][1]"));
            if(mapintout.contains("Plot_Win_BoE[0][1][1]"))ui->EndA2->setValue(mapintout.value("Plot_Win_BoE[0][1][1]"));
            if(mapintout.contains("Plot_Win_BoE[0][2][1]"))ui->EndA3->setValue(mapintout.value("Plot_Win_BoE[0][2][1]"));

            if(mapintout.contains("Plot_Win_BoE[1][0][0]"))ui->BegB1->setValue(mapintout.value("Plot_Win_BoE[1][0][0]"));
            if(mapintout.contains("Plot_Win_BoE[1][1][0]"))ui->BegB2->setValue(mapintout.value("Plot_Win_BoE[1][1][0]"));
            if(mapintout.contains("Plot_Win_BoE[1][2][0]"))ui->BegB3->setValue(mapintout.value("Plot_Win_BoE[1][2][0]"));
            if(mapintout.contains("Plot_Win_BoE[1][0][1]"))ui->EndB1->setValue(mapintout.value("Plot_Win_BoE[1][0][1]"));
            if(mapintout.contains("Plot_Win_BoE[1][1][1]"))ui->EndB2->setValue(mapintout.value("Plot_Win_BoE[1][1][1]"));
            if(mapintout.contains("Plot_Win_BoE[1][2][1]"))ui->EndB3->setValue(mapintout.value("Plot_Win_BoE[1][2][1]"));

            if(mapintout.contains("Plot_Win_BoE[2][0][0]"))ui->BegC1->setValue(mapintout.value("Plot_Win_BoE[2][0][0]"));
            if(mapintout.contains("Plot_Win_BoE[2][1][0]"))ui->BegC2->setValue(mapintout.value("Plot_Win_BoE[2][1][0]"));
            if(mapintout.contains("Plot_Win_BoE[2][2][0]"))ui->BegC3->setValue(mapintout.value("Plot_Win_BoE[2][2][0]"));
            if(mapintout.contains("Plot_Win_BoE[2][0][1]"))ui->EndC1->setValue(mapintout.value("Plot_Win_BoE[2][0][1]"));
            if(mapintout.contains("Plot_Win_BoE[2][1][1]"))ui->EndC2->setValue(mapintout.value("Plot_Win_BoE[2][1][1]"));
            if(mapintout.contains("Plot_Win_BoE[2][2][1]"))ui->EndC3->setValue(mapintout.value("Plot_Win_BoE[2][2][1]"));


            if(mapintout.contains("in_startChan"))ui->startChan->setValue(mapintout.value("in_startChan"));
            if(mapintout.contains("in_PlotACh1"))ui->PlotAChn1->setValue(mapintout.value("in_PlotACh1"));
            if(mapintout.contains("in_PlotACh2"))ui->PlotAChn2->setValue(mapintout.value("in_PlotACh2"));
            if(mapintout.contains("in_PlotBCh1"))ui->PlotBChn1->setValue(mapintout.value("in_PlotBCh1"));
            if(mapintout.contains("in_PlotBCh2"))ui->PlotBChn2->setValue(mapintout.value("in_PlotBCh2"));
            if(mapintout.contains("in_PlotCCh1"))ui->PlotCChn1->setValue(mapintout.value("in_PlotCCh1"));
            if(mapintout.contains("in_PlotCCh2"))ui->PlotCChn2->setValue(mapintout.value("in_PlotCCh2"));
            if(mapintout.contains("in_histStart"))ui->histStart->setValue(mapintout.value("in_histStart"));
            if(mapintout.contains("in_histEnd"))ui->histEnd->setValue(mapintout.value("in_histEnd"));
            if(mapintout.contains("in_binsinplot"))ui->binsinplot->setValue(mapintout.value("in_binsinplot"));

            if(mapdoubleout.contains("in_adqtime"))ui->adqtime->setValue(mapdoubleout.value("in_adqtime"));



               QMapIterator<QString,int>i(mapintout);
               while (i.hasNext()) {
                   i.next();
                   std::cout<< i.key().toStdString() <<  ": " << i.value() << std::endl;
               }
               in>>mapdoubleout;
               QMapIterator<QString,double>j(mapdoubleout);
               while (j.hasNext()) {
                   j.next();
                   std::cout<< j.key().toStdString() <<  ": " << j.value() << std::endl;
               }

    }

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::hdf5savefile(){



        bool ok;
        QString comentario = QInputDialog::getText(this, tr("Record DATA"),tr("Insert a name for HDF5 the file"), QLineEdit::Normal,QDir::home().dirName(), &ok);


   if (ok && !comentario.isEmpty()){
       HDF5File_created=true;
       emit MW_savehdf5(comentario);
   }

   /*    QString key = QDateTime::currentDateTime().toString("dd_MMM_yyyy_hh:mm" );
       key.append(".txt");

       data = new QFile(key);

       if (!data->open(QIODevice::WriteOnly | QIODevice::Text))return;

       QTextStream out(data);
       out<<"# "<<comentario<<"\n";
       out<<"Vch0\tVch1\tCch0\tCch1\tTime\n";
       recorddata=true;*/


}

void MainWindow::fillTablesNames(QStringList tables_names)
{
    if (tables_names.length() == 0)
        QMessageBox::warning(this,
                             "Tables",
                             "There are no tables to display in the database",
                             QMessageBox::Ok);
    else
    {
       /* ui->menuLoad_Qubits->addAction()
        ui->comboBox_table_name->addItems(tables_names);

        ui->comboBox_table_name->setEnabled(true);
        ui->comboBox_table_name->setFocus();*/

        for (int i = 0; i < tables_names.size(); ++i)ui->menuLoad_Qubits->addAction(tables_names.at(i));
                         //std::cout << tables_names.at(i).toLocal8Bit().constData() << std::endl;

    }
}


void MainWindow::set_qkd_datafromDB(const boolvector2d &dat,int qkdcolumns, int qkdrows){
   /* std::cout<<dat.size()<<std::endl;
    std::cout<<dat[0].size()<<std::endl;
   for (int i = 0;i<dat.size();i++) {
       for (int j =0;j<dat[i].size();j++) {
           std::cout<<dat[i][j];
       }
        std::cout<<std::endl;
    }*/
    this->in_qkdfromDB=dat;
    for (int i = 0;i<in_qkdfromDB.size();i++) {
        std::cout<<i<<" : ";
        for (int j =0;j<in_qkdfromDB[i].size();j++) {
            std::cout<<in_qkdfromDB[i][j];
        }
         std::cout<<std::endl;
}
    this->in_qubnumindb=qkdrows;
    this->in_qkddbcolumns=qkdcolumns;
    qubitsfromDBloaded=true;
}


void MainWindow::closeEvent (QCloseEvent *event)
{
        std::cout<<"going out"<<std::endl;
       adq.Break();
       usleep(100);
       //adq.~qutagadq();
       usleep(100e3);
       std::cout<<"destroy"<<std::endl;
       //adq.quit();
       //anl.quit();


	
       while(adq.isRunning() || anl.isRunning()|| dbc.isRunning())usleep(100);
       usleep(1000);
       //savejasonFile.close();

       event->accept();


    //else event->ignore();

    
}
