#include "mainwindow.h"
#include "ui_mainwindow.h"

// #define REF_LENGTH_X 300
// #define REF_LENGTH_Y 400
// #define REF_LENGTH_Z 183

// #define REF_LENGTH_X 620
// #define REF_LENGTH_Y 284
// #define REF_LENGTH_Z 224

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->installEventFilter( this );

    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(ui->label);
    setCentralWidget(scrollArea);

    img_scale = 1.0;
    img_scale_min = 1.0;

    getVanish_mode_x = false;
    getVanish_mode_y = false;
    getVanish_mode_z = false;
    getVanish_mode_o = false;
    setReference_x = false;
    setReference_y = false;
    setReference_z = false;
    refx_m = false;
    refy_m = false;
    refz_m = false;
    get3d_mode = false;

}

MainWindow::~MainWindow()
{
    delete ui;
}

// on_action functions
void MainWindow::on_actionOpen_Image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("Open Image"), ".", tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
    /*
    bool is_gif = fileName.contains("gif");

    if (is_gif) {
        Magick::InitializeMagick("");
        Magick::Image mimage(fileName.toStdString());
        int w = mimage.columns();
        int h = mimage.rows();

        // Make OpenCV Mat of same size with 8-bit and 3 channels
        image = cv::Mat::zeros(h, w, CV_8UC3);
        // Unpack Magick++ pixels into OpenCV Mat structure
        mimage.write(0, 0, w, h,"BGR",Magick::CharPixel,image.data);

    } else {
        image = cv::imread(fileName.toLatin1().data());
    }
    */
    image = cv::imread(fileName.toLatin1().data());

    // convert cv::Mat to QImage
    cv::cvtColor(image, image, CV_BGR2RGB);

    int width_im = image.cols;
    int height_im = image.rows;
    double width_m = ui->label->width();
    double height_m = ui->label->height();

    if (width_im < width_m || height_im < height_m) {
        img_scale = (width_m / width_im) > (height_m / height_im) ? (width_m / width_im) : (height_m / height_im);
        img_scale_min = img_scale;
    }

    contour_image = image.clone();

    QImage Q_img = QImage((const unsigned char*)(image.data), image.cols, image.rows, image.step, QImage::Format_RGB888);

    ui->label->setPixmap(QPixmap::fromImage(Q_img).scaled(QPixmap::fromImage(Q_img).width()*img_scale,
                                                          QPixmap::fromImage(Q_img).height()*img_scale, Qt::KeepAspectRatio));
}

void MainWindow::display_image(){
    cv::Mat myImage;
    if(getVanish_mode_x || getVanish_mode_y || getVanish_mode_z || getVanish_mode_o || refx_m || refy_m || refz_m )
        myImage = contour_image;
    else
        myImage = image;

    QImage Q_img = QImage((const unsigned char*)(myImage.data), myImage.cols, myImage.rows, myImage.step, QImage::Format_RGB888);
    QPixmap p = QPixmap::fromImage(Q_img);
    ui->label->setPixmap( p.scaled(p.width()*img_scale, p.height()*img_scale, Qt::KeepAspectRatio) );
}

void MainWindow::on_actionZoom_in_triggered(){
    img_scale += 0.1;
    display_image();
}
void MainWindow::on_actionZoom_out_triggered(){
    img_scale -= 0.1;
    display_image();
}

void MainWindow::on_actionReset_all_triggered(){
    this->resetAll();
}

void MainWindow::resetAll(){
    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = getVanish_mode_z = getVanish_mode_o = false;
    setReference_x = setReference_y = setReference_z = false;
    refx_m = refy_m = refz_m = false;
    ui->actionGet_3D_point->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = getVanish_mode_z = getVanish_mode_o =  get3d_mode = false;
}

void MainWindow::on_actionGet_vanish_x_triggered(bool checked){
    getVanish_mode_x = checked;

    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    ui->actionset_reference_x->setChecked(false);
    ui->actionset_reference_y->setChecked(false);
    ui->actionset_reference_z->setChecked(false);
    getVanish_mode_y = getVanish_mode_z = getVanish_mode_o = setReference_x = setReference_y = setReference_z = false;
}
void MainWindow::on_actionGet_vanish_y_triggered(bool checked){
    getVanish_mode_y = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    ui->actionset_reference_x->setChecked(false);
    ui->actionset_reference_y->setChecked(false);
    ui->actionset_reference_z->setChecked(false);
    getVanish_mode_x = getVanish_mode_z = getVanish_mode_o = setReference_x = setReference_y = setReference_z = false;
}
void MainWindow::on_actionGet_vanish_z_triggered(bool checked){
    getVanish_mode_z = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    ui->actionset_reference_x->setChecked(false);
    ui->actionset_reference_y->setChecked(false);
    ui->actionset_reference_z->setChecked(false);
    getVanish_mode_y = getVanish_mode_x = getVanish_mode_o = setReference_x = setReference_y = setReference_z = false;
}
void MainWindow::on_actionGet_origin_triggered(bool checked){
    getVanish_mode_o = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    ui->actionset_reference_x->setChecked(false);
    ui->actionset_reference_y->setChecked(false);
    ui->actionset_reference_z->setChecked(false);
    getVanish_mode_y = getVanish_mode_z = getVanish_mode_x = setReference_x = setReference_y = setReference_z = false;
}

void MainWindow::on_actionset_reference_x_triggered(bool checked){
    setReference_x = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    ui->actionset_reference_y->setChecked(false);
    ui->actionset_reference_z->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = getVanish_mode_z = getVanish_mode_o = setReference_y = setReference_z = false;
}

void MainWindow::on_actionset_reference_y_triggered(bool checked){
    setReference_y = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    ui->actionset_reference_x->setChecked(false);
    ui->actionset_reference_z->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = getVanish_mode_z = getVanish_mode_o = setReference_x = setReference_z = false;
}

void MainWindow::on_actionset_reference_z_triggered(bool checked){
    setReference_z = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    ui->actionGet_origin->setChecked(false);
    ui->actionset_reference_y->setChecked(false);
    ui->actionset_reference_x->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = getVanish_mode_z = getVanish_mode_o = setReference_y = setReference_x = false;
}

void MainWindow::on_actionGet_3D_point_triggered(bool checked){
    get3d_mode = checked;

    ui->actionGet_vanish_x->setChecked(false);
    ui->actionGet_vanish_y->setChecked(false);
    ui->actionGet_vanish_z->setChecked(false);
    getVanish_mode_x = getVanish_mode_y = getVanish_mode_z = false;
}

void MainWindow::on_actionDraw_vanish_triggered(){
    cout << "size of vanish x: " << vanish_x.size() << endl;
    cout << "size of vanish y: " << vanish_y.size() << endl;
    cout << "size of vanish z: " << vanish_z.size() << endl;

    if( vanish_x.size()%2 != 0 || vanish_y.size()%2 != 0 || vanish_z.size()%2 != 0 ){
        cout << "size of vanish_ is not even! " << endl;
        return;
    }

    if(vanish_x.size()>0)
        for(uint i=0; i<vanish_x.size()/4; i++){
            cv::line(contour_image, cv::Point(vanish_x[4*i], vanish_x[4*i+1]),
                                    cv::Point(vanish_x[4*i+2], vanish_x[4*i+3]), CV_RGB(255,0,0), 4); // blue
        }
    if(vanish_y.size()>0)
        for(uint i=0; i<vanish_y.size()/4; i++){
            cv::line(contour_image, cv::Point(vanish_y[4*i], vanish_y[4*i+1]),
                                    cv::Point(vanish_y[4*i+2], vanish_y[4*i+3]), CV_RGB(0,255,0), 4); // green
        }
    if(vanish_z.size()>0)
        for(uint i=0; i<vanish_z.size()/4; i++){
            cv::line(contour_image, cv::Point(vanish_z[4*i], vanish_z[4*i+1]),
                                    cv::Point(vanish_z[4*i+2], vanish_z[4*i+3]), CV_RGB(0,0,255), 4); // red
        }

    display_image();

    calVanishingPt();
    calProjectionMatrix();
    getTextureMap();
    cal3DPosition();
    generateVRML(string("sony"));

}


// event filter
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {

    // getVanish_mode + left click:
    if( (event->type() == QEvent::MouseButtonPress) &&
        (strcmp(watched->metaObject()->className(), "MainWindow")) == 0 )
    {
        QMouseEvent* me = static_cast<QMouseEvent*> (event);
        QPoint p = ui->label->mapFrom(this, me->pos());
        p /= img_scale;

        cout << p.x() << "\t" << p.y() << endl;

        if(getVanish_mode_x){
            vanish_x.push_back( p.x() );
            vanish_x.push_back( p.y() );
        }
        else if(getVanish_mode_y){
            vanish_y.push_back( p.x() );
            vanish_y.push_back( p.y() );
        }
        else if(getVanish_mode_z){
            vanish_z.push_back( p.x() );
            vanish_z.push_back( p.y() );
        }
        else if(getVanish_mode_o){
            Origin = cv::Point3d(p.x(), p.y(), 1);
            cv::circle(contour_image, cv::Point(p.x(),p.y()), 1, CV_RGB(128,128,128), 5);
            origin_m = true;
        }
        else if (setReference_x) {
            refx = cv::Point3d(p.x() , p.y() , 1);
            cv::circle(contour_image, cv::Point(p.x(),p.y()), 1, CV_RGB(128,128,128), 5);
            refx_m = true;
        } else if (setReference_y) {
            refy = cv::Point3d(p.x() , p.y() , 1);
            cv::circle(contour_image, cv::Point(p.x(),p.y()), 1, CV_RGB(128,128,128), 5);
            refy_m = true;
        } else if (setReference_z) {
            refz = cv::Point3d(p.x() , p.y() , 1);
            cv::circle(contour_image, cv::Point(p.x(),p.y()), 1, CV_RGB(128,128,128), 5);
            refz_m = true;
        }
        else if(get3d_mode){

        }
        else
            return false;

        display_image();
    }

    return false;
}


// Actual functions start here
// step 1: Calculate vanishing points, Bob Collin's method
void MainWindow::calVanishingPt(){
    cv::Mat M, EigenVector, EigenValue;
    cv::Mat temp_M;
    cv::Point3d point1, point2;
    cv::Point3d line;

    // M = (cv::Mat_<double>(3,3) << 1,2,3, 4,5,6, 7,8,9.1);
    // temp_M = (cv::Mat_<double>(3,3) << 1,0,0, 0,0,0, 0,0,0);
    // cout << "temp_M is:  " << temp_M << endl;

    // get vanishing point x
    M = (cv::Mat_<double>(3,3) << 0,0,0, 0,0,0, 0,0,0);
    for(uint i=0; i<vanish_x.size()/4; i++){
        point1 = cv::Point3d(vanish_x[4*i], vanish_x[4*i+1], 1);
        point2 = cv::Point3d(vanish_x[4*i+2], vanish_x[4*i+3], 1);
        line = point1.cross(point2);

        temp_M = (cv::Mat_<double>(3,3) << line.x*line.x, line.x*line.y, line.x*line.z,
                                          line.x*line.y, line.y*line.y, line.y*line.z,
                                          line.x*line.z, line.y*line.z, line.z*line.z);
        M = M + temp_M;
    }

    cv::eigen(M, EigenValue, EigenVector);
    cout << "eigen values x: " << EigenValue << endl;
    cout << "eigen vectors x: " << EigenVector << endl << endl;

    vanishPt_x = cv::Point3d(EigenVector.at<double>(2,0)/EigenVector.at<double>(2,2),
                             EigenVector.at<double>(2,1)/EigenVector.at<double>(2,2), 1);
    cout << "Vanishing Point x: " << vanishPt_x << endl;


    // get vanishing point y
    M = (cv::Mat_<double>(3,3) << 0,0,0, 0,0,0, 0,0,0);
    for(uint i=0; i<vanish_y.size()/4; i++){
        point1 = cv::Point3d(vanish_y[4*i], vanish_y[4*i+1], 1);
        point2 = cv::Point3d(vanish_y[4*i+2], vanish_y[4*i+3], 1);
        line = point1.cross(point2);

        temp_M = (cv::Mat_<double>(3,3) << line.x*line.x, line.x*line.y, line.x*line.z,
                                          line.x*line.y, line.y*line.y, line.y*line.z,
                                          line.x*line.z, line.y*line.z, line.z*line.z);
        M = M + temp_M;
    }

    cv::eigen(M, EigenValue, EigenVector);
    cout << "eigen values y: " << EigenValue << endl;
    cout << "eigen vectors y: " << EigenVector << endl << endl;

    vanishPt_y = cv::Point3d(EigenVector.at<double>(2,0)/EigenVector.at<double>(2,2),
                             EigenVector.at<double>(2,1)/EigenVector.at<double>(2,2), 1);
    cout << "Vanishing Point y: " << vanishPt_y << endl;


    // get vanishing point z
    M = (cv::Mat_<double>(3,3) << 0,0,0, 0,0,0, 0,0,0);
    for(uint i=0; i<vanish_z.size()/4; i++){
        point1 = cv::Point3d(vanish_z[4*i], vanish_z[4*i+1], 1);
        point2 = cv::Point3d(vanish_z[4*i+2], vanish_z[4*i+3], 1);
        line = point1.cross(point2);

        temp_M = (cv::Mat_<double>(3,3) << line.x*line.x, line.x*line.y, line.x*line.z,
                                          line.x*line.y, line.y*line.y, line.y*line.z,
                                          line.x*line.z, line.y*line.z, line.z*line.z);
        M = M + temp_M;
    }

    cv::eigen(M, EigenValue, EigenVector);
    cout << "eigen values z: " << EigenValue << endl;
    cout << "eigen vectors z: " << EigenVector << endl << endl;

    vanishPt_z = cv::Point3d(EigenVector.at<double>(2,0)/EigenVector.at<double>(2,2),
                             EigenVector.at<double>(2,1)/EigenVector.at<double>(2,2), 1);
    cout << "Vanishing Point z: " << vanishPt_z << endl << endl;

}


// step 2: Calculate Projection Matrix
void MainWindow::calProjectionMatrix(){

    // Choose the Origin First
    if (!origin_m) {
        cout << "not selected origin" << endl;
        return;
    }
    if (!refx_m) {
        cout << "not selected x-axis" << endl;
        return;
    }
    if (!refy_m) {
        cout << "not selected y-axis" << endl;
        return;
    }
    if (!refz_m) {
        cout << "not selected z-axis" << endl;
        return;
    }

    REF_LENGTH_X = sqrt((refx.x - Origin.x)*(refx.x - Origin.x) + (refx.y - Origin.y)*(refx.y - Origin.y));
    REF_LENGTH_Y = sqrt((refy.x - Origin.x)*(refy.x - Origin.x) + (refy.y - Origin.y)*(refy.y - Origin.y));
    REF_LENGTH_Z = sqrt((refz.x - Origin.x)*(refz.x - Origin.x) + (refz.y - Origin.y)*(refz.y - Origin.y));

    cout << REF_LENGTH_X << " " << REF_LENGTH_Y << " " << REF_LENGTH_Z << endl;


//    scale_x = (0.5 * (refx.x - Origin.x)/(vanishPt_x.x - refx.x)
//             + 0.5 * (refx.y - Origin.y)/(vanishPt_x.y - refx.y)) / REF_LENGTH_X;

//    scale_y = (0.5 * (refy.x - Origin.x)/(vanishPt_y.x - refy.x)
//             + 0.5 * (refy.y - Origin.y)/(vanishPt_y.y - refy.y)) / REF_LENGTH_Y;
//    scale_x = ((refx.x - Origin.x)/(vanishPt_x.x - refx.x)) / REF_LENGTH_X;
//    scale_y = ((refy.y - Origin.y)/(vanishPt_y.y - refy.y)) / REF_LENGTH_Y;

//    if(abs(refz.x - Origin.x) <= 1){
//            scale_z = ((refz.y - Origin.y)/(vanishPt_z.y - refz.y)) / REF_LENGTH_Z;
//        }
//        else{
//            scale_z = (0.5 * (refz.x - Origin.x)/(vanishPt_z.x - refz.x)
//                     + 0.5 * (refz.y - Origin.y)/(vanishPt_z.y - refz.y)) / REF_LENGTH_Z;
//        }
    cv::Mat right = (cv::Mat_<double>(2,1) << refx.x - Origin.x, refx.y - Origin.y);
    cv::Mat left = (cv::Mat_<double>(2,1) << vanishPt_x.x - refx.x, vanishPt_x.y - refx.y);
    cv::Mat ans;
    cv::solve(left, right, ans, DECOMP_NORMAL);
    scale_x = ans.at<double>(0) / REF_LENGTH_X;
    cout << "scale_x is: " << scale_x << endl;

    right = (cv::Mat_<double>(2,1) << refy.x - Origin.x, refy.y - Origin.y);
    left = (cv::Mat_<double>(2,1) << vanishPt_y.x - refy.x, vanishPt_y.y - refy.y);
    cv::solve(left, right, ans, DECOMP_NORMAL);
    scale_y = ans.at<double>(0) / REF_LENGTH_Y;
    cout << "scale_y is: " << scale_y << endl;

    right = (cv::Mat_<double>(2,1) << refz.x - Origin.x, refz.y - Origin.y);
    left = (cv::Mat_<double>(2,1) << vanishPt_z.x - refz.x, vanishPt_z.y - refz.y);
    cv::solve(left, right, ans, DECOMP_NORMAL);
    scale_z = ans.at<double>(0) / REF_LENGTH_Z;
    cout << "scale_z is: " << scale_z << endl << endl;

    ProjMatrix = (cv::Mat_<double>(3,4) << scale_x*vanishPt_x.x, scale_y*vanishPt_y.x, scale_z*vanishPt_z.x, Origin.x,
                                          scale_x*vanishPt_x.y, scale_y*vanishPt_y.y, scale_z*vanishPt_z.y, Origin.y,
                                          scale_x*vanishPt_x.z, scale_y*vanishPt_y.z, scale_z*vanishPt_z.z, Origin.z);

    cout << "Projection Matrix is: " << ProjMatrix << endl << endl;

}


// step 3: Use homography matrix to get texture map
void MainWindow::getTextureMap(){

    cv::Mat Hxy = (cv::Mat_<double>(3,3) << scale_x*vanishPt_x.x, scale_y*vanishPt_y.x, Origin.x,
                                           scale_x*vanishPt_x.y, scale_y*vanishPt_y.y, Origin.y,
                                           scale_x*vanishPt_x.z, scale_y*vanishPt_y.z, Origin.z);

    cv::Mat Hxz = (cv::Mat_<double>(3,3) << scale_x*vanishPt_x.x, scale_z*vanishPt_z.x, Origin.x,
                                           scale_x*vanishPt_x.y, scale_z*vanishPt_z.y, Origin.y,
                                           scale_x*vanishPt_x.z, scale_z*vanishPt_z.z, Origin.z);

    cv::Mat Hyz = (cv::Mat_<double>(3,3) << scale_y*vanishPt_y.x, scale_z*vanishPt_z.x, Origin.x,
                                           scale_y*vanishPt_y.y, scale_z*vanishPt_z.y, Origin.y,
                                           scale_y*vanishPt_y.z, scale_z*vanishPt_z.z, Origin.z);



    /*
    cv::warpPerspective(image, dstImage, Hxy.inv(), tempImage.size());
    QImage Q_img = QImage ((const unsigned char*)(dstImage.data), dstImage.cols, dstImage.rows, dstImage.step, QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(Q_img).scaled(QPixmap::fromImage(Q_img).width()*img_scale,
                                                          QPixmap::fromImage(Q_img).height()*img_scale, Qt::KeepAspectRatio));
    */

    cv::Mat dstImage;
    cv::Mat perspective_matrix;
    cv::Mat tempImage;

    Point2f v1 = Point2f(162, 227); // yz, xy
    Point2f v2 = Point2f(174, 366); // yz
    Point2f v3 = Point2f(391, 542); // xz, yz
    Point2f v4 = Point2f(392, 399); // xz, yz, xy
    Point2f v5 = Point2f(607, 430); // xz
    Point2f v6 = Point2f(618, 290); // xy, xz
    Point2f v7 = Point2f(379, 139); // xy
    Point2f v8 = Point2f(382, 277);

//    Point2f v1 = Point2f(770, 1100); // yz, xy
//    Point2f v2 = Point2f(787, 1677); // yz
//    Point2f v3 = Point2f(1393, 2173); // xz, yz
//    Point2f v4 = Point2f(1387, 1430); // xz, yz, xy
//    Point2f v5 = Point2f(2660, 1580); // xz
//    Point2f v6 = Point2f(2737, 1063); // xy, xz
//    Point2f v7 = Point2f(2003, 890); // xy
//        //Point2f v8 = Point2f(382, 277);

    // patch xy
    Point2f pts1[] = {v1, v7, v4, v6};
    Point2f pts2[] = {Point2f(0,0), Point2f(REF_LENGTH_X,0), Point2f(0,REF_LENGTH_Y), Point2f(REF_LENGTH_X,REF_LENGTH_Y)};

    perspective_matrix = cv::getPerspectiveTransform(pts1, pts2);
    tempImage = cv::Mat(REF_LENGTH_Y, REF_LENGTH_X, image.type());

    cv::warpPerspective(image, dstImage, perspective_matrix, tempImage.size(), INTER_LINEAR);
    cv::resize(dstImage, dstImage, cv::Size(), 1/img_scale, 1/img_scale);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    imwrite("../../../../SingleViewModeling/SingleViewModel/temp/xy_patch.jpg", dstImage);
    // imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/xy_patch.jpg", dstImage);


    // patch xz
    Point2f pts3[] = {v4, v6,v3, v5};
    Point2f pts4[] = {Point2f(0,0), Point2f(REF_LENGTH_X,0), Point2f(0,REF_LENGTH_Z), Point2f(REF_LENGTH_X,REF_LENGTH_Z)};

    perspective_matrix = cv::getPerspectiveTransform(pts3, pts4);
    tempImage = cv::Mat(REF_LENGTH_Z, REF_LENGTH_X, image.type());

    cv::warpPerspective(image, dstImage, perspective_matrix, tempImage.size(), INTER_LINEAR);
    cv::resize(dstImage, dstImage, cv::Size(), 1/img_scale, 1/img_scale);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    imwrite("../../../../SingleViewModeling/SingleViewModel/temp/xz_patch.jpg", dstImage);
    // imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/xz_patch.jpg", dstImage);


    // patch yz
    Point2f pts5[] = {v4, v1, v3, v2};
    Point2f pts6[] = {Point2f(0,0), Point2f(REF_LENGTH_Y,0), Point2f(0,REF_LENGTH_Z), Point2f(REF_LENGTH_Y,REF_LENGTH_Z)};

    perspective_matrix = cv::getPerspectiveTransform(pts5, pts6);
    tempImage = cv::Mat(REF_LENGTH_Z, REF_LENGTH_Y, image.type());

    cv::warpPerspective(image, dstImage, perspective_matrix, tempImage.size(), INTER_LINEAR);
    cv::resize(dstImage, dstImage, cv::Size(), 1/img_scale, 1/img_scale);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    imwrite("../../../../SingleViewModeling/SingleViewModel/temp/yz_patch.jpg", dstImage);
    // imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/yz_patch.jpg", dstImage);


    /*
    cv::Mat tempImage = cv::Mat(image.size().height*2, image.size().width*2, image.type());

    cv::warpPerspective(image, dstImage, Hxy.inv(), tempImage.size(), INTER_LINEAR);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/Hxy_image.jpg", dstImage);
    cv::warpPerspective(dstImage, invImage, Hxy, tempImage.size(), INTER_LINEAR);
    imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/Inv_Hxy_image.jpg", invImage);


    cv::warpPerspective(image, dstImage, Hxz.inv(), tempImage.size(), INTER_LINEAR);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/Hxz_image.jpg", dstImage);
    cv::warpPerspective(dstImage, invImage, Hxz, tempImage.size(), INTER_LINEAR);
    imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/Inv_Hxz_image.jpg", invImage);


    cv::warpPerspective(image, dstImage, Hyz.inv(), tempImage.size(), INTER_LINEAR);
    cv::cvtColor(dstImage, dstImage, CV_BGR2RGB);
    imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/Hyz_image.jpg", dstImage);
    cv::warpPerspective(dstImage, invImage, Hyz, tempImage.size(), INTER_LINEAR);
    imwrite("/home/jguoaj/Desktop/SingleViewModel/temp/Inv_Hyz_image.jpg", invImage);
    */

}


// step 4: Mark interesting points
inline double norml2(Point3d p){
    return sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
}

double MainWindow::getRefHeight(Point3d r, Point3d b){

    // we want to find Reference height R
    double R;
    double H = REF_LENGTH_Z;
    Point3d b0, v, t, temp;
    b0 = Point3d(Origin.x, Origin.y, 1);
    temp = ( b.cross(b0) ).cross( vanishPt_x.cross(vanishPt_y) );
    v = Point3d(temp.x/temp.z, temp.y/temp.z, 1);
    temp = ( v.cross(refz) ).cross( r.cross(b) );
    t = Point3d(temp.x/temp.z, temp.y/temp.z, 1);

    R = H * norml2(r-b) * norml2(vanishPt_z-t) / ( norml2(t-b) * norml2(vanishPt_z-r) );
    return R;
}

Point3d MainWindow::get3dCoor(Point3d r, Point3d b){

    double z0 = getRefHeight(r, b);
    //cout << "\n 3D height z0 is:  " << z0 << endl;

    Mat point_3d;
    Mat point_2d = ( cv::Mat_<double>(3,1) << r.x, r.y, r.z );
    Mat Hz = (cv::Mat_<double>(3,3) << scale_x*vanishPt_x.x, scale_y*vanishPt_y.x, scale_z*z0*vanishPt_z.x + Origin.x,
                                      scale_x*vanishPt_x.y, scale_y*vanishPt_y.y, scale_z*z0*vanishPt_z.y + Origin.y,
                                      scale_x*vanishPt_x.z, scale_y*vanishPt_y.z, scale_z*z0*vanishPt_z.z + Origin.z);

    point_3d = Hz.inv() * point_2d;

    Point3d p = Point3d(point_3d.at<double>(0,0)/point_3d.at<double>(2,0),
                        point_3d.at<double>(1,0)/point_3d.at<double>(2,0), z0);
    return p;
}

// function overload, http://sklin93.github.io/svm.html
// continue mark the interesting points on the same height
Point3d MainWindow::get3dCoor(Point3d r, double h){

    double z0 = h;
    Mat point_3d;
    Mat point_2d = ( cv::Mat_<double>(3,1) << r.x, r.y, r.z );
    Mat Hz = (cv::Mat_<double>(3,3) << scale_x*vanishPt_x.x, scale_y*vanishPt_y.x, scale_z*z0*vanishPt_z.x + Origin.x,
                                      scale_x*vanishPt_x.y, scale_y*vanishPt_y.y, scale_z*z0*vanishPt_z.y + Origin.y,
                                      scale_x*vanishPt_x.z, scale_y*vanishPt_y.z, scale_z*z0*vanishPt_z.z + Origin.z);

    point_3d = Hz.inv() * point_2d;

    Point3d p = Point3d(point_3d.at<double>(0,0)/point_3d.at<double>(2,0),
                        point_3d.at<double>(1,0)/point_3d.at<double>(2,0), z0);
    return p;
}


void MainWindow::cal3DPosition(){

    /* 3d world coordinate, v5 and v6 example
    Point3d p = get3dCoor(v6, v5);
    cout << "v6 coordinate is: " << p << endl;
    */

    /* 2d image coordinate */
//    Old Box
    Point3d v1 = Point3d(162, 227, 1);
    Point3d v2 = Point3d(174, 366, 1);
    Point3d v3 = Point3d(391, 542, 1);
    Point3d v4 = Point3d(392, 399, 1);
    Point3d v5 = Point3d(607, 430, 1);
    Point3d v6 = Point3d(618, 290, 1);
    Point3d v7 = Point3d(379, 139, 1);
    Point3d v8 = Point3d(382, 278, 1);

    // New Box
//    Point3d v1 = Point3d(770, 1100, 1); // yz, xy
//    Point3d v2 = Point3d(787, 1677, 1); // yz
//    Point3d v3 = Point3d(1393, 2173, 1); // xz, yz
//    Point3d v4 = Point3d(1387, 1430, 1); // xz, yz, xy
//    Point3d v5 = Point3d(2660, 1580, 1); // xz
//    Point3d v6 = Point3d(2737, 1063, 1); // xy, xz
//    Point3d v7 = Point3d(2003, 890, 1); // xy
//    Point3d v8 = Point3d(382, 278, 1);

    Point3d p1, p2, p3, p4, p5, p6, p7, p8;
    // Old Box
    p1 = get3dCoor(v1, REF_LENGTH_Z);
    p6 = get3dCoor(v6, REF_LENGTH_Z);

    p2 = get3dCoor(v2, 0);
    p3 = get3dCoor(v3, 0);
    p5 = get3dCoor(v5, 0);
    p8 = get3dCoor(v8, 0);

    p4 = get3dCoor(v4, REF_LENGTH_Z);
    p7 = get3dCoor(v7, REF_LENGTH_Z);

//    // New Box
//    p1 = get3dCoor(v1, v2);
//    p6 = get3dCoor(v6, v5);

//    p2 = get3dCoor(v2, 0);
//    p3 = get3dCoor(v3, 0);
//    p5 = get3dCoor(v5, 0);
//    p8 = get3dCoor(v8, 0);

//    p4 = get3dCoor(v4, v3);
//    p7 = get3dCoor(v7, REF_LENGTH_Z);

    cout << "\n\n";
    cout << "v1 coordinate is: " << p1 << endl;
    cout << "v2 coordinate is: " << p2 << endl;
    cout << "v3 coordinate is: " << p3 << endl;
    cout << "v4 coordinate is: " << p4 << endl;
    cout << "v5 coordinate is: " << p5 << endl;
    cout << "v6 coordinate is: " << p6 << endl;
    cout << "v7 coordinate is: " << p7 << endl;
    cout << "v8 coordinate is: " << p8 << endl;

    Face face1;
    face1.coor3d.push_back(p4);
    face1.coor3d.push_back(p6);
    face1.coor3d.push_back(p7);
    face1.coor3d.push_back(p1);
    face1.textureFileName = string("xy_patch.jpg");
    faces.push_back(face1);

    Face face2;
    face2.coor3d.push_back(p3);
    face2.coor3d.push_back(p5);
    face2.coor3d.push_back(p6);
    face2.coor3d.push_back(p4);
    face2.textureFileName = string("xz_patch.jpg");
    faces.push_back(face2);

    Face face3;
    face3.coor3d.push_back(p3);
    face3.coor3d.push_back(p2);
    face3.coor3d.push_back(p1);
    face3.coor3d.push_back(p4);
    face3.textureFileName = string("yz_patch.jpg");
    faces.push_back(face3);

}


// step 5: Generate 3D vrml models
void MainWindow::generateVRML(const string &prefix)
{
    string fname = prefix + ".wrl";
    ofstream ofile(fname.c_str());
    ofile << "#VRML V2.0 utf8" << endl << endl;
    ofile << "Collision {" << endl;
    ofile << "    collide FALSE" << endl;
    ofile << "    children [" << endl;

    for(uint i=0; i<faces.size(); i++){

        Face face = faces[i];
        ofile << "Shape {" << endl;
        ofile << "    appearance  Appearance {" << endl;
        ofile << "        texture  ImageTexture {" << endl;
        ofile << "            url \"" << face.textureFileName << "\"" << endl;
        ofile << "        }" << endl;
        ofile << "    }" << endl;
        ofile << "    geometry IndexedFaceSet {" << endl;
        ofile << "        coord Coordinate {" << endl;
        ofile << "            point [" << endl;

        vector<Point3d> Coor3 = face.coor3d;
        for(uint j=0; j<Coor3.size(); j++){
            Point3d p = Coor3[j];
            ofile << "                   ";
            ofile << p.x << " " << p.y << " " << p.z << "," << endl;
        }

        ofile << "           ]" << endl;
        ofile << "       }" << endl;
        ofile << "       coordIndex [" << endl;
        ofile << "               0, 1, 2, 3, -1" << endl;
        ofile << "       ]" << endl;
        ofile << "       texCoord TextureCoordinate {" << endl;
        ofile << "           point [" << endl;
        ofile << "                     0  0, " << endl;
        ofile << "                     1  0, " << endl;
        ofile << "                     1  1, " << endl;
        ofile << "                     0  1, " << endl;
        ofile << "           ]" << endl;
        ofile << "       }" << endl;
        ofile << "       texCoordIndex [" << endl;
        ofile << "               0, 1, 2, 3, -1" << endl;
        ofile << "       ]" << endl;
        ofile << "       solid FALSE" << endl;
        ofile << "    }" << endl;
        ofile << "}" << endl;
    }
    ofile << "    ]" << endl;
    ofile << "}" << endl;

}



/*
void SingleViewModel::generateVRMLCode(const string &prefix)
{
    for(int i=0;i<faces.size();i++)
    {
        Face *face=faces[i];
        char fID[20];
        sprintf(fID,"_%.3d.png",face->ID());
        string fname(fID);
        fname=prefix+fname;
        face->Texture().save(fname.c_str());
        face->textureFileName=fname;
    }
    string fname=prefix+".wrl";
    ofstream ofile(fname.c_str());
    ofile<<"#VRML V2.0 utf8"<<endl;
    ofile<<"Transform {"<<endl;
    ofile<<"  translation "<<camCenter.x<<" "<<camCenter.y<<" "<<camCenter.z<<endl;
    ofile<<"  children ["<<endl;
    ofile<<"    Shape {"<<endl;
    ofile<<"      geometry Sphere {"<<endl;
    ofile<<"        radius 0.15"<<endl;
    ofile<<"      }"<<endl;
    ofile<<"    }"<<endl;
    ofile<<"  ]"<<endl;
    ofile<<"}"<<endl;
    for(int i=0;i<faces.size();i++)
    {
        Face *face=faces[i];
        ofile<<"Shape{"<<endl;
        ofile<<"    appearance  Appearance{"<<endl;
        ofile<<"        texture  ImageTexture{"<<endl;
        ofile<<"           url \""<<face->TexFileName()<<"\""<<endl;
        ofile<<"        }"<<endl;
        ofile<<"    }"<<endl;
        ofile<<"    geometry IndexedFaceSet {"<<endl;
        ofile<<"       coord Coordinate {"<<endl;
        ofile<<"         point[";
        for(int j=3;j>0;j--)
        {
            cv::Point3d p=face->realvertexs[j]->Coor3d();
            ofile<<p.x<<" "<<p.y<<" "<<p.z<<", ";
        }
        cv::Point3d p=face->realvertexs[0]->Coor3d();
        ofile<<p.x<<" "<<p.y<<" "<<p.z<<"]"<<endl;
        ofile<<"       }"<<endl;
        ofile<<"       coordIndex [0,1,2,3,-1]"<<endl;
        ofile<<"       ccw TRUE"<<endl;
        ofile<<"       solid FALSE"<<endl;
        ofile<<"       texCoord TextureCoordinate {"<<endl;
        ofile<<"       point [0  0, 1  0, 1  1, 0  1]"<<endl;
        ofile<<"       }"<<endl;
        ofile<<"       texCoordIndex[0 1 2 3 -1]"<<endl;
        ofile<<"    }"<<endl;
        ofile<<"}"<<endl;
    }
}
*/




















