#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h" 
#include <QtCore>
#include <ctime>
#include <stdio.h>
#include "qutag_adq.h"
#include "qutag_anl.h"
#include "dbcontrol.h"
#include "socket_com.h"
#include "qkd_param.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include "swabian.h"

#define MAX_QUBITS 1000
#define HDF5TIMEINTEGRATION 3

namespace Ui {
class MainWindow;
}

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    void run() override {
        int i;
        int channels[18];

        for (i = 0; i < 18; i++)
            channels[i] = i+1;
        
        while (true) {
            if (!s->t) {
                sleep(1000);
                continue;
            }
            double *rates = new double[18];

            printf("getting rates\n");
            s->get_count_rates(channels,rates,18);

            emit(rates_ready(rates));
        }
    }
    WorkerThread(Swabian *s_) {
        s = s_;
    }
    Swabian *s;
signals:
    void rates_ready(double *rates);
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:


  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void closeEvent(QCloseEvent *event);

  void setupPlotA(QCustomPlot *customPlot);
  void setupratePlot(QCustomPlot *customPlot);

  void setupsignalslot();
  void setupHistoPlot(QCustomPlot *customPlot);

  void setup_histolines_Teleport();
  
  void setupDefaultRanges();
  
  void setup_plot_qkd_results(QCustomPlot *scope);
  void setup_plot_qkd_stats(QCustomPlot *scope);

private slots:
    void refreshButton();
    void connectButton();
    void parametersChanged();
    void show_rates(double *rates);
  
  void plotRates(char AoB, int event, double key);

  void changeStartchan(int starchan){this->in_startChan=starchan;}

  void histoplot(const vectorDouble &dat1, const vectorDouble &dat2, const vectorDouble &dat3);

  void LinePlot();

  void BegA1(int val){Plot_Win_BoE[0][0][0]=val;LinePlot();}
  void BegA2(int val){Plot_Win_BoE[0][1][0]=val;LinePlot();}
  void BegA3(int val){Plot_Win_BoE[0][2][0]=val;LinePlot();}
  void EndA1(int val){Plot_Win_BoE[0][0][1]=val;LinePlot();}
  void EndA2(int val){Plot_Win_BoE[0][1][1]=val;LinePlot();}
  void EndA3(int val){Plot_Win_BoE[0][2][1]=val;LinePlot();}

  void BegB1(int val){Plot_Win_BoE[1][0][0]=val;LinePlot();}
  void BegB2(int val){Plot_Win_BoE[1][1][0]=val;LinePlot();}
  void BegB3(int val){Plot_Win_BoE[1][2][0]=val;LinePlot();}
  void EndB1(int val){Plot_Win_BoE[1][0][1]=val;LinePlot();}
  void EndB2(int val){Plot_Win_BoE[1][1][1]=val;LinePlot();}
  void EndB3(int val){Plot_Win_BoE[1][2][1]=val;LinePlot();}

  void BegC1(int val){Plot_Win_BoE[2][0][0]=val;LinePlot();}
  void BegC2(int val){Plot_Win_BoE[2][1][0]=val;LinePlot();}
  void BegC3(int val){Plot_Win_BoE[2][2][0]=val;LinePlot();}
  void EndC1(int val){Plot_Win_BoE[2][0][1]=val;LinePlot();}
  void EndC2(int val){Plot_Win_BoE[2][1][1]=val;LinePlot();}
  void EndC3(int val){Plot_Win_BoE[2][2][1]=val;LinePlot();}

  void Chang_in_binsinplot(int val){this->in_binsinplot=val;}
  void Chang_in_histStart(int val){this->in_histStart=val;}
  void Chang_in_histEnd(int val){this->in_histEnd=val;}

  void Chang_in_adqtime(double val){this->in_adqtime=val;}

  void Chang_in_PlotAChn1(int val){this->in_PlotACh1=val;}
  void Chang_in_PlotAChn2(int val){this->in_PlotACh2=val;}
  void Chang_in_PlotBChn1(int val){this->in_PlotBCh1=val;}
  void Chang_in_PlotBChn2(int val){this->in_PlotBCh2=val;}
  void Chang_in_PlotCChn1(int val){this->in_PlotCCh1=val;}
  void Chang_in_PlotCChn2(int val){this->in_PlotCCh2=val;}

  void Chang_track1(bool val){this->P_T[0]=val;trackRateChang =true;}
  void Chang_track2(bool val){this->P_T[1]=val;trackRateChang =true;}
  void Chang_track3(bool val){this->P_T[2]=val;trackRateChang =true;}
  void Chang_track4(bool val){this->P_T[3]=val;trackRateChang =true;}
  void Chang_track5(bool val){this->P_T[4]=val;trackRateChang =true;}
  void Chang_track6(bool val){this->P_T[5]=val;trackRateChang =true;}
  void Chang_track7(bool val){this->P_T[6]=val;trackRateChang =true;}
  void Chang_track8(bool val){this->P_T[7]=val;trackRateChang =true;}
  void Chang_track9(bool val){this->P_T[8]=val;trackRateChang =true;}

  void CombinationChange(bool val){CombiChang =val;}

  void Chang_adqtime_2(double val){in_adqtime_2=val;}

  void turnONDB(int val);

  void SaveState(bool a);
  void LoadState(bool a);

  void Chang_delayline(int val){in_delayline=val;}

  void resetdelay(){in_delayline=0;prev_homscan=0;}
  void chang_in_max_del(int val){in_Max_delay=val;}

  void createQKDLinesA();
  void createQKDLinesB();
  void createQKDLinesC();


  void setup_histolines_QKD();

  /*void chang_QKD_time(int val){QubitTime=val;}
  void chang_QKD_phase(int val){Phasetime=val;}
  void chang_QKD_nuqub(int val){NoQubits=val;}
  void chang_QKD_PxQ(int val){PeaksQubit=val;}hidelines(val)
  void chang_QKD_iw(int val){QKD_intWind=val;}
  void chang_QKD_zero(int val){QKD_zero=val;}*/

  void chang_QKD_timeA(double val){in_QKD_timeA=val;if(initR)createQKDLinesA();}
  void chang_QKD_phA(int val){in_QKD_phA=val;if(initR)createQKDLinesA();}
  void chang_QKD_numbA(int val){if(in_QKD_numbA>val)hidelinesA(val);in_QKD_numbA=val;if(initR)createQKDLinesA();}
  void chang_QKD_pxqA(int val){in_QKD_pxqA=val;if(initR)createQKDLinesA();}
  void chang_QKD_iwA(int val){in_QKD_iwA=val;if(initR)createQKDLinesA();}
  void chang_QKD_zeroA(int val){in_QKD_zeroA =val;if(initR)createQKDLinesA();}

  void chang_QKD_timeB(double val){in_QKD_timeB=val;if(initR)createQKDLinesB();}
  void chang_QKD_phB(int val){in_QKD_phB=val;if(initR)createQKDLinesB();}
  void chang_QKD_numbB(int val){if(in_QKD_numbB>val)hidelinesB(val);in_QKD_numbB=val;if(initR)createQKDLinesB();}
  void chang_QKD_pxqB(int val){in_QKD_pxqB=val;if(initR)createQKDLinesB();}
  void chang_QKD_iwB(int val){in_QKD_iwB=val;if(initR)createQKDLinesB();}
  void chang_QKD_zeroB(int val){in_QKD_zeroB =val;if(initR)createQKDLinesB();}

  void chang_QKD_timeC(double val){in_QKD_timeC=val;if(initR)createQKDLinesC();}
  void chang_QKD_phC(int val){in_QKD_phC=val;if(initR)createQKDLinesC();}
  void chang_QKD_numbC(int val){if(in_QKD_numbC>val)hidelinesC(val);in_QKD_numbC=val;if(initR)createQKDLinesC();}
  void chang_QKD_pxqC(int val){in_QKD_pxqC=val;if(initR)createQKDLinesC();}
  void chang_QKD_iwC(int val){in_QKD_iwC=val;if(initR)createQKDLinesC();}
  void chang_QKD_zeroC(int val){in_QKD_zeroC =val;if(initR)createQKDLinesC();}

  /*int in_QKD_timeA, in_QKD_numbA, in_QKD_phA, in_QKD_iwA, in_QKD_pxqA, in_QKD_zeroA;
  int in_QKD_timeB, in_QKD_numbB, in_QKD_phB, in_QKD_iwB, in_QKD_pxqB, in_QKD_zeroB;
  int in_QKD_timeC, in_QKD_numbC, in_QKD_phC, in_QKD_iwC, in_QKD_pxqC, in_QKD_zeroC;*/

  void hidelinesA(int val);
  void hidelinesB(int val);
  void hidelinesC(int val);

  void set_qkd_datafromDB(const boolvector2d &dat,int qkdcolumns, int qkdrows);

  void plot_qkd_results_det(double okA,double errA,double randA,double bkgndA,double okB,double errB,double randB,double bkgndB,double okC,double errC,double randC,double bkgndC, double key);
  void plot_qkd_results_QB(double okE,double errE,double randE,double bkgndE,double okL,double errL,double randL, double bkgndL,double okP,double errP,double randP,double bkgndP, double key);

  void plot_qkd_stats(double sifted_time, double sifted_phase, double error_time, double error_phase, double key);

  void hdf5savefile();

  void fillTablesNames(QStringList tables_names);
  void tableSelected(QAction* action){emit tableQKDtoDB(action->text());}

  void chang_QKD_turnONDB(int val){QKD_DB_ON=val;}

private:
  Ui::MainWindow *ui;
  qutagadq adq;
  Swabian s;
  qutaganl anl;
  DBControl dbc;
  socket_com udpcom;
  QKD_param qkdparam;
  QString demoName;
  QTimer dataTimer;
  QCPItemTracer *itemDemoPhaseTracer;
  bool Teleport0_or_QKD1=1;
  int currentDemoIndex;
  double prom;
  QButtonGroup *buttonGroup1 ;
  QButtonGroup *buttonGroup2 ;
  bool trackRateChang =false, CombiChang =false;
  float in_adqtime_2;
  QVector<int> datach1;
  QVector<int> datacali;
  bool dbrunning=false;

  QCPItemStraightLine *infLine[18];

  double lastPointKey_tab1;
  double lastPointKey_tab3;
  double lastPointKey_tab4;
  double qkd_prevKey;

  bool initR=false;
  ////first tab//////
  QMap<QString, int>windows;
  double prevbinwidth;

  ///general Configs////
  int in_binsinplot, in_startChan, in_histStart, in_histEnd;
  double in_adqtime;
  int in_PlotACh1, in_PlotACh2, in_PlotBCh1, in_PlotBCh2,in_PlotCCh1,in_PlotCCh2;
  /////first plot////
  int P_R[9]={0};
  bool P_T[9]={0};
  int Plot_Win_BoE[3][3][2]={{{0}}};

  ////SECOND tab////


  int in_delayline=0;

  bool in_homscan=false;
  int in_homscan_time;
  int prev_homscan=0;
  int xrange = 120;
  int in_Max_delay=500;
  bool firstscan=false;
  int in_stepduration;
  double del_key, del_previouskey;

//////QKD///////////////

  QCPItemStraightLine *LinesPlotA1[MAX_QUBITS];
  QCPItemStraightLine *LinesPlotA2[MAX_QUBITS];
  QCPItemStraightLine *LinesPlotA3[MAX_QUBITS];
  QCPItemStraightLine *LinesPlotA4[MAX_QUBITS];

  QCPItemStraightLine *LinesPlotB1[MAX_QUBITS];
  QCPItemStraightLine *LinesPlotB2[MAX_QUBITS];
  QCPItemStraightLine *LinesPlotB3[MAX_QUBITS];
  QCPItemStraightLine *LinesPlotB4[MAX_QUBITS];

  QCPItemStraightLine *LinesPlotC1[MAX_QUBITS];
  QCPItemStraightLine *LinesPlotC2[MAX_QUBITS];
  QCPItemStraightLine *LinesPlotC3[MAX_QUBITS];
  QCPItemStraightLine *LinesPlotC4[MAX_QUBITS];

  int QubitTime, Phasetime, NoQubits, PeaksQubit, QKD_intWind, QKD_zero;

  int  in_QKD_numbA=30, in_QKD_phA=50, in_QKD_iwA=45, in_QKD_pxqA=3, in_QKD_zeroA=1;
  int  in_QKD_numbB=30, in_QKD_phB=50, in_QKD_iwB=45, in_QKD_pxqB=3, in_QKD_zeroB=1;
  int  in_QKD_numbC=30, in_QKD_phC=50, in_QKD_iwC=45, in_QKD_pxqC=3, in_QKD_zeroC=1;
  double in_QKD_timeA=200, in_QKD_timeB=200, in_QKD_timeC=200;

  QVector<QVector<bool>> in_qkdfromDB;
  int in_qubnumindb, in_qkddbcolumns;
  bool qubitsfromDBloaded = false;
  int QKD_DB_ON=0;

  QVector<double> datah5okA;
  QVector<double> datah5errA;
  QVector<double> datah5randA;
  QVector<double> datah5bkgndA;
  QVector<double> datah5okB;
  QVector<double> datah5errB;
  QVector<double> datah5randB;
  QVector<double> datah5bkgndB;
  QVector<double> datah5okC;
  QVector<double> datah5errC;
  QVector<double> datah5randC;
  QVector<double> datah5bkgndC;

  /*QFile savejasonFile;
  QJsonArray jasonhistoA, jasonhistoB, jasonhistoC;*/

  bool HDF5File_created=false;
signals:
    void main_SaveAndValues(int and1, int and2, int and3, int orgate, int bsm1, int bsm2, float andTime, int delayline);
    void main_SaveRateValues( int Ra1, int Ra2, int Ra3, int Rb1, int Rb2, int Rb3, int Rc1, int Rc2, int Rc3, float hist_adqtime);

    void main_SaveQKDresults(double okA, double errA,double  randA, double bkgndA, double okB, double errB, double randB, double bkgndB, double okC, double errC, double randC, double bkgndC);
    void main_SaveQKDstats(int sifted_time, int sifted_phase, double error_time, double error_phase);
    void MW_savehdf5(QString mensaje);
    void saveH5datafromMW(const intvector datah5okA, const intvector datah5errA, const intvector datah5randA, const intvector datah5bkgndA, const intvector datah5okB, const intvector datah5errB, const intvector datah5randB, const intvector datah5bkgndB, const intvector datah5okC, const intvector datah5errC, const intvector datah5randC, const intvector datah5bkgndC);
    void tableQKDtoDB(QString text);
};

#endif // MAINWINDOW_H
