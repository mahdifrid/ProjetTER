#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

namespace Ui {
class frmMain;
}

class frmMain : public QMainWindow
{
    Q_OBJECT

public slots:
    void processFrameAndUpdateGUI();                //  prototype fonction

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();


private slots:
    void on_btnPauseOrResume_clicked();
    void on_actionCapture_triggered();

private:
    Ui::frmMain *ui;    
    cv::VideoCapture capWebcam;             // orondre un  object pou l'utiliser avec la webcam
    cv::VideoCapture capture;
    QTimer* qtimer;                 // timer objet
    QImage frmMain::convertOpenCVMatToQtQImage(cv::Mat mat);
    QList<CvScalar>  colors;
    QPixmap*         pixmap;
    CvHaarClassifierCascade* cascade;
    CvMemStorage*   storage;
    IplImage*  imgOriginal;

    void frmMain::exitProgram();                    // function prototype

};

#endif // FRMMAIN_H
