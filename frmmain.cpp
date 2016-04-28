#include "frmmain.h"
#include "ui_frmmain.h"
#include<QtCore>
#include<QMessageBox>


// constructor //

frmMain::frmMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::frmMain) {
    ui->setupUi(this);
    QLabel* lblCapture;
    capture.open(0);
    cascade = (CvHaarClassifierCascade*)cvLoad("C:\\Users\\frid\\Documents\\CannyStill\\haarcascade_frontalface_alt2.xml");
    storage = cvCreateMemStorage(0);
    colors << cvScalar(0.0,0.0,255.0) << cvScalar(0.0,128.0,255.0)
                << cvScalar(0.0,255.0,255.0) << cvScalar(0.0,255.0,0.0)
                << cvScalar(255.0,128.0,0.0) << cvScalar(255.0,255.0,0.0)
                << cvScalar(255.0,0.0,0.0) << cvScalar(255.0,0.0,255.0);
    capWebcam.open(0);                  // associer l'objet de capture à la webcam par défaut

    if(capWebcam.isOpened() == false) {                 // if (false)
        QMessageBox::information(this, "", "error: Webcam pas accessible \n\n Quitter l'interface \n");        //  errors messages
        exitProgram();                                  //  quitter le programme
        return;                                         //
    }

    qtimer = new QTimer(this);                          //instancier l'objet  timer
    connect(qtimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));     // ascocier l'objet timer au  processFrameAndUpdateGUI
    qtimer->start(20);                                  // lancer timer, le cycle se defile toutes les 20 msec (50x par sec)
}



// destructor //
frmMain::~frmMain() {
    cvReleaseImage(&imgOriginal);
    cvReleaseCapture(&capture);
    delete ui;
}

void frmMain::exitProgram() {
    if(qtimer->isActive()) qtimer->stop();          // if timer(true)  arreter le  timer
    QApplication::quit();                            //  quitter le programme
}

void frmMain::processFrameAndUpdateGUI() {
    cv::Mat imgOriginal;                    // original image
    cv::Mat imgHSV;
    cv::Mat imgThreshLow;
    cv::Mat imgThreshHigh;
    cv::Mat imgThresh;

    bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);                    // obtenir la suivante   frame de the webcam

    if (!blnFrameReadSuccessfully || imgOriginal.empty()) {                            // if frame(false) probleme avaec l'image
        QMessageBox::information(this, "", "incapable de lire de webcam \ n \ n sortant programme \ n");        // errors message box
        exitProgram();                                                             //  quitter le programme
        return;                                                                     //
    }

    cv::cvtColor(imgOriginal, imgHSV, CV_BGR2HSV);
    cv::inRange(imgHSV, cv::Scalar(0, 155, 155), cv::Scalar(18, 255, 255), imgThreshLow);
    cv::inRange(imgHSV, cv::Scalar(165, 155, 155), cv::Scalar(179, 255, 255), imgThreshHigh);
    cv::add(imgThreshLow, imgThreshHigh, imgThresh);
    cv::GaussianBlur(imgThresh, imgThresh, cv::Size(3, 3), 0);
    cv::Mat structuringElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::dilate(imgThresh, imgThresh, structuringElement);
    cv::erode(imgThresh, imgThresh, structuringElement);

    std::vector<cv::Vec3f> v3fCircles;    // déclarer les variables cercles on consider le visage de la personne comme un cercle a fin de le detecter
    cv::HoughCircles(imgThresh, v3fCircles, CV_HOUGH_GRADIENT, 2, imgThresh.rows / 4, 100, 50, 10, 400);      // remplir les cercles variables avec tous les cercles dans l'image traitée


    for(unsigned int i = 0; i < v3fCircles.size(); i++) {                                                                                       // for each circle
        ui->txtXYRadius->appendPlainText(QString("ball position x =") + QString::number(v3fCircles[i][0]).rightJustified(4, ' ') +              //afficher la posistion du cercle et son rayon
                                                                         QString(", y =") + QString::number(v3fCircles[i][1]).rightJustified(4, ' ') +
                                                                         QString(", radius =") + QString::number(v3fCircles[i][2], 'f', 3).rightJustified(7, ' '));

        cv::circle(imgOriginal, cv::Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]), (int)v3fCircles[i][2], cv::Scalar(0, 0, 255), 3);      // dessiner un cercle rouge autour de l'objet détecté qui est le visage
        cv::circle(imgOriginal, cv::Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]), 3, cv::Scalar(0, 255, 0), CV_FILLED);                  // dessiner petit cercle vert au centre du l'objet détecté qui est normallement le nez
    }

    QImage qimgOriginal = convertOpenCVMatToQtQImage(imgOriginal);                         // convertir Mat OpenCV à Qt QImage
    QImage qimgThresh = convertOpenCVMatToQtQImage(imgThresh);                       //

    ui->lblOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));           // montrent des images dela cam  avec les étiquettes de formulaire
    ui->lblThresh->setPixmap(QPixmap::fromImage(qimgThresh));         //
}


QImage frmMain::convertOpenCVMatToQtQImage(cv::Mat mat) {
    if(mat.channels() == 1) {                                   // si canal(1) de l'image (niveaux de gris ou noir et blanc)
        return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);     // return QImage
    } else if(mat.channels() == 3) {
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);       // return QImage
    } else {
        qDebug() << "si  l'image n'a pas été 1 canal ou 3 canaux, ne devrait jamais arriver ici";
    }
    return QImage();        //return frame noir QImage
}

void frmMain::on_btnPauseOrResume_clicked() {
    if(qtimer->isActive() == true) {
        qtimer->stop();
        ui->btnPauseOrResume->setText(" Reprondre ");
    } else {
        qtimer->start(20);
        ui->btnPauseOrResume->setText(" Pause ");
    }
}
void frmMain::on_actionCapture_triggered(){
// Query camera for next frame
imgOriginal = cvQueryFrame( capture );


if (imgOriginal)
{
    // Detect objects
    cvClearMemStorage( storage );


    CvSeq* objects = cvHaarDetectObjects(imgOriginal,
                                         cascade,
                                         storage,
                                         1.1,
                                         3,
                                         CV_HAAR_DO_CANNY_PRUNING,
                                         cvSize( 100, 100 ));


    int n = (objects ? objects->total : 0);


    CvRect* r;
    // Loop through objects and draw boxes
    for( int i = 0; i < n; i++ )
    {
        r = ( CvRect* )cvGetSeqElem( objects, i );
        cvRectangle( imgOriginal,
                     cvPoint( r->x, r->y ),
                     cvPoint( r->x + r->width, r->y + r->height ),
                     colors[i%8]
                    );
    }
    // Convert IplImage to QImage
    QImage qimgOriginal = QImage((const uchar *)imgOriginal->imageData,
                                         imgOriginal->width,
                                         imgOriginal->height,
                                         QImage::Format_RGB888).rgbSwapped();
    ui->lblOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));

}
}
