#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QPixmap"
#include "QLabel"
#include <QColor>
#include <QFileInfo>
#include <QVector>
#include <QImage>
#include <QPainter>
#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <math.h>  // Dùng math.h thay vì QtMath
#include <QDebug>
#include "QMdiSubWindow"
#include "imagewidget.h"
#include "QFileDialog"
#include <cmath>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mdiArea = new QMdiArea (this);
    setCentralWidget (mdiArea);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    actionMedian_Filter_Color = new QAction(tr("Median Filter Color"), this);






    connect(actionMedian_Filter_Color, &QAction::triggered, this, &MainWindow::on_actionMedian_Filter_Color_triggered);
    connect(ui->actionDilation, &QAction::triggered, this, &MainWindow::on_actionDilation_triggered);
    QAction *action = new QAction(this);
    action->setIcon(QIcon(QPixmap::fromImage(currentImage)));

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_bt_Browse_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open File", "D:/", "*.* All Files;;*.bmp;;*.jpeg;;*.ppm;;*.png;;*.jpg");
    if(filename.isEmpty())
        return;
    ui->ln_filename->setText(filename);

    QPixmap pixmap(filename);
    QLabel *lable = new QLabel();
    lable->setPixmap(pixmap);
    lable->setWindowTitle(QFileInfo(filename).fileName());
    lable->show();
}

void MainWindow::DisplayImage(QImage &img, QString title) {
    QLabel *label = new QLabel();
    label->setPixmap(QPixmap::fromImage(img));
    label->setWindowTitle(title);
    label->show();
    QMdiSubWindow *subWindow = new QMdiSubWindow();
    subWindow->setAttribute (Qt:: WA_DeleteOnClose);
    mdiArea->addSubWindow (subWindow);
    subWindow->setWindowTitle (title);
    ImageWidget *imagewidget = new ImageWidget();
    imagewidget->setImage (img);
    subWindow->setWidget (imagewidget);
    subWindow->resize (img.size());
    subWindow->show();

}


void MainWindow::on_bt_HisSlide_Gray_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename); //Doc anh tu File
    QImage image_out (image_in.width(),image_in.height(),QImage:: Format_ARGB32);
    int c = ui->ln_c->text().toInt(); //Doi chuoi thanh so
    for (int x=0; x<image_in.width(); x++)
    {
    for (int y=0; y<image_in.height(); y++) {
    QRgb color = image_in.pixel (x, y); //Lay mau gia tri pixel (x,y)
    int gray_in = qGray (color); //Doi gia tri mau thanh muc xam
    int gray_out = gray_in + c; //Thay doi do sang toi
    if (gray_out>255) //Kiem tra gia muc xam 0...255
    gray_out=255;
    else if (gray_out<0)
    gray_out=0;
    QRgb color_out = qRgb (gray_out, gray_out, gray_out);//Anh xam (R=G-B)
    image_out.setPixel(x, y, color_out);
    }
    }
    DisplayImage (image_in, QFileInfo (filename).fileName());
    DisplayImage (image_out, QFileInfo (filename).fileName()+"_His_Slide");
}

void MainWindow::on_bt_HisSlide_Color_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out (image_in.width(), image_in.height(), QImage::Format_ARGB32);
    int c = ui->ln_c->text().toInt();
    for (int x=0; x<image_in.width(); x++)
        for (int y=0; y<image_in.height(); y++) {
            QRgb rgb = image_in.pixel (x, y);
            QColor color_in (rgb);
            int h, s, v;
            color_in.getHsv(&h, &s, &v); //Lay 3 thanh phan h, s, v tu color
            int v_out = v + c;
            if (v_out>255)
                v_out=255;
            else if (v_out<0)
                v_out=0;
            QColor color_out = QColor::fromHsv (h, s, v_out);
            image_out.setPixel(x, y, color_out.rgb());
        }
    DisplayImage (image_in, QFileInfo (filename).fileName());
    DisplayImage (image_out, QFileInfo (filename).fileName()+" His_Slide");
}

void MainWindow::on_bt_Contrast_Gray_clicked()
{
    QString filename = ui->ln_filename->text();
        QImage image_in(filename);
        QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
        int contrast = ui->ln_contrast->text().toInt();
        double factor = (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));

        for (int x = 0; x < image_in.width(); x++) {
            for (int y = 0; y < image_in.height(); y++) {
                QRgb color = image_in.pixel(x, y);
                int gray_in = qGray(color);
                int gray_out = qBound(0, int(factor * (gray_in - 128) + 128), 255);
                QRgb color_out = qRgb(gray_out, gray_out, gray_out);
                image_out.setPixel(x, y, color_out);
            }
        }
        DisplayImage(image_in, QFileInfo(filename).fileName());
        DisplayImage(image_out, QFileInfo(filename).fileName() + "_Gray_Contrast");
}

void MainWindow::on_bt_Contrast_Color_clicked()
{
    QString filename = ui->ln_filename->text();
       QImage image_in(filename);
       QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
       int contrast = ui->ln_contrast->text().toInt();
       double factor = (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));

       for (int x = 0; x < image_in.width(); x++) {
           for (int y = 0; y < image_in.height(); y++) {
               QRgb rgb = image_in.pixel(x, y);
               QColor color_in(rgb);
               int h, s, v;
               color_in.getHsv(&h, &s, &v);
               int v_out = qBound(0, int(factor * (v - 128) + 128), 255);
               QColor color_out = QColor::fromHsv(h, s, v_out);
               image_out.setPixel(x, y, color_out.rgb());
           }
       }
       DisplayImage(image_in, QFileInfo(filename).fileName());
       DisplayImage(image_out, QFileInfo(filename).fileName() + "_Color_Contrast");
}

void MainWindow::on_bt_Threshold_Gray_clicked()
{
    QString filename = ui->ln_filename->text();
        QImage image_in(filename);
        QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);
        int threshold = ui->ln_threshold->text().toInt();

        for (int x = 0; x < image_in.width(); x++) {
            for (int y = 0; y < image_in.height(); y++) {
                QRgb color = image_in.pixel(x, y);
                int gray_in = qGray(color);
                int gray_out = gray_in >= threshold ? 255 : 0;
                QRgb color_out = qRgb(gray_out, gray_out, gray_out);
                image_out.setPixel(x, y, color_out);
            }
        }
        DisplayImage(image_in, QFileInfo(filename).fileName());
        DisplayImage(image_out, QFileInfo(filename).fileName() + "_Gray_Threshold");
}

void MainWindow::on_bt_Sequential_Threshold_Gray_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    int width = image_in.width();
    int height = image_in.height();

    // Bước 1: Tạo ảnh xám
    QImage image_gray(image_in.size(), QImage::Format_ARGB32);
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            QRgb color = image_in.pixel(x, y);
            int gray = qGray(color);
            image_gray.setPixel(x, y, qRgb(gray, gray, gray));
        }
    }

    // Bước 2: Áp dụng giải thuật tuần tự
    int threshold = 128;  // Giá trị ngưỡng ban đầu
    int newThreshold;
    do {
        int sum1 = 0, sum2 = 0, count1 = 0, count2 = 0;

        // Tính trung bình của các pixel nhỏ hơn và lớn hơn ngưỡng
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                int grayValue = qGray(image_gray.pixel(x, y));
                if (grayValue > threshold) {
                    sum1 += grayValue;
                    count1++;
                } else {
                    sum2 += grayValue;
                    count2++;
                }
            }
        }

        // Tính ngưỡng mới
        int mean1 = count1 == 0 ? 0 : sum1 / count1;
        int mean2 = count2 == 0 ? 0 : sum2 / count2;
        newThreshold = (mean1 + mean2) / 2;

        if (abs(newThreshold - threshold) < 1)
            break;
        threshold = newThreshold;
    } while (true);

    // Bước 3: Phân ngưỡng dựa vào giá trị ngưỡng tìm được
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int grayValue = qGray(image_gray.pixel(x, y));
            int newValue = (grayValue > threshold) ? 255 : 0;
            image_out.setPixel(x, y, qRgb(newValue, newValue, newValue));
        }
    }

    DisplayImage(image_gray, "Gray Image");
    DisplayImage(image_out, "Sequential Thresholding Result");
}

void MainWindow::on_bt_Otsu_Threshold_Gray_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    int width = image_in.width();
    int height = image_in.height();

    // Bước 1: Tạo ảnh xám
    QImage image_gray(image_in.size(), QImage::Format_ARGB32);
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            QRgb color = image_in.pixel(x, y);
            int gray = qGray(color);
            image_gray.setPixel(x, y, qRgb(gray, gray, gray));
        }
    }

    // Bước 2: Tính histogram của ảnh
    int histogram[256] = {0};
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int grayValue = qGray(image_gray.pixel(x, y));
            histogram[grayValue]++;
        }
    }

    // Bước 3: Áp dụng giải thuật Otsu
    int totalPixels = width * height;
    int sumB = 0, wB = 0;
    int sum1 = 0;
    for (int i = 0; i < 256; i++) {
        sum1 += i * histogram[i];
    }

    float maxVariance = 0;
    int threshold = 0;
    int wF;
    float mB, mF, betweenVariance;

    for (int t = 0; t < 256; t++) {
        wB += histogram[t];  // Trọng số của nhóm nền (background)
        if (wB == 0) continue;

        wF = totalPixels - wB;  // Trọng số của nhóm đối tượng (foreground)
        if (wF == 0) break;

        sumB += t * histogram[t];

        mB = sumB / (float) wB;  // Trung bình của nền
        mF = (sum1 - sumB) / (float) wF;  // Trung bình của đối tượng

        // Tính phương sai giữa
        betweenVariance = (float) wB * (float) wF * (mB - mF) * (mB - mF);

        // Lưu ngưỡng tối ưu khi phương sai giữa là lớn nhất
        if (betweenVariance > maxVariance) {
            maxVariance = betweenVariance;
            threshold = t;
        }
    }

    // Bước 4: Phân ngưỡng dựa vào giá trị ngưỡng Otsu tìm được
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int grayValue = qGray(image_gray.pixel(x, y));
            int newValue = (grayValue > threshold) ? 255 : 0;
            image_out.setPixel(x, y, qRgb(newValue, newValue, newValue));
        }
    }

    DisplayImage(image_gray, "Gray Image");
    DisplayImage(image_out, "Otsu Thresholding Result");
}


void MainWindow::Histogram_GrayImage(const QImage &img, const int &his_height) {
    int h[256];
    for (int i = 0; i < 256; i++)
        h[i] = 0;
    QImage image_in(img);
    for (int x = 0; x < img.width(); x++)
        for (int y = 0; y < img.height(); y++) {
            QRgb color = img.pixel(x, y);
            int gray = qGray(color);
            QRgb color_in = qRgb(gray, gray, gray);
            image_in.setPixel(x, y, color_in);
            h[gray]++;
        }
    int max = 0;
    for (int i = 0; i < 256; i++)
        if (h[i] > max)
            max = h[i];
    QImage img_his = QImage(256, his_height, QImage::Format_RGB888);
    img_his.fill(Qt::white);
    int lineHeight;
    for (int i = 0; i < 256; i++) {
        // tinh tien anh de histogram co do lon nhu mong muon
        lineHeight = his_height * h[i] / max;
        for (int j = his_height - 1; j >= his_height - 1 - lineHeight; j--) // toa do 0,0 nam phia tren ben trai
            img_his.setPixel(i, j, qRgb(0, 0, 0));
    }
    //QImage image_out(img.width(),img.height(), QImage:: Format_RGB888);
    QLabel *label_in = new QLabel();
    label_in->setPixmap(QPixmap::fromImage(image_in));
    label_in->show();
    // hien thi histogram
    QLabel *label_out = new QLabel();
    label_out->setPixmap(QPixmap::fromImage(img_his));
    label_out->show();
}

void MainWindow::on_bt_Histogram_Gray_clicked()
{
    QImage img(ui->ln_filename->text());
    Histogram_GrayImage(img, 200);
}


void MainWindow::Histogram_ColorImage(const QImage &img, const int his_height) {
  int hr[256];
  int hg[256];
  int hb[256];
  for (int i = 0; i < 256; i++) {
    hr[i] = 0;
    hg[i] = 0;
    hb[i] = 0;
  }
  QImage image_in(img);
  for (int x = 0; x < img.width(); x++) {
    for (int y = 0; y < img.height(); y++) {
      QRgb color = img.pixel(x, y);
      int green = qGreen(color);
      int red = qRed(color);
      int blue = qBlue(color);
      hr[red]++;
      hg[green]++;
      hb[blue]++;
    }
  }
  int maxr = 0, mang = 0, maxb = 0;
  for (int i = 0; i < 256; i++) {
    if (hr[i] > maxr) maxr = hr[i];
    if (hg[i] > mang) mang = hg[i];
    if (hb[i] > maxb) maxb = hb[i];
  }
  QImage img_hisr = QImage(256, his_height, QImage::Format_RGB888);
  img_hisr.fill(Qt::white);
  QImage img_hisg = QImage(256, his_height, QImage::Format_RGB888);
  img_hisg.fill(Qt::white);
  QImage img_hisb = QImage(256, his_height, QImage::Format_RGB888);
  img_hisb.fill(Qt::white);
  int liner, lineg, lineb;
  for (int i = 0; i < 256; i++) {
    liner = his_height * hr[i] / maxr; // tính tiến ảnh để histogram có độ lớn như mong muốn
    lineg = his_height * hg[i] / mang;
    lineb = his_height * hb[i] / maxb;
    for (int j = his_height - 1; j >= his_height - 1 - liner; j--) // toa độ 0,0 nằm phía trên bên trái
      img_hisr.setPixel(i, j, qRgb(255, 0, 0));
    for (int j = his_height - 1; j >= his_height - 1 - lineg; j--) // toa độ 0,0 nằm phía trên bên trái
      img_hisg.setPixel(i, j, qRgb(0, 255, 0));
    for (int j = his_height - 1; j >= his_height - 1 - lineb; j--) // toa độ 0,0 nằm phía trên bên trái
      img_hisb.setPixel(i, j, qRgb(0, 0, 255));
  }
  // QImage image_out(img.width(),img.height(),QImage::Format_RGB888);
  QLabel* label_in = new QLabel();
  label_in->setPixmap(QPixmap::fromImage(image_in));
  label_in->show();
  // hien thi histogram
  QLabel* label_out1 = new QLabel();
  label_out1->setPixmap(QPixmap::fromImage(img_hisr));
  label_out1->show();
  QLabel* label_out2 = new QLabel();
  label_out2->setPixmap(QPixmap::fromImage(img_hisg));
  label_out2->show();
  QLabel* label_out3 = new QLabel();
  label_out3->setPixmap(QPixmap::fromImage(img_hisb));
  label_out3->show();
}

void MainWindow::on_bt_Histogram_Color_clicked()
{
    QImage img(ui->ln_filename->text());
    Histogram_ColorImage(img, 200);
}

void MainWindow::on_bt_BienDoiTuyenTinh_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);

    // Tìm max và min gray
    int max = 0;
    int min = 255;

    for (int x = 0; x < image_in.width(); x++)
    {
        for (int y = 0; y < image_in.height(); y++)
        {
            QRgb color = image_in.pixel(x, y);
            int gray = qGray(color);

            if (gray > max) max = gray;
            if (gray < min) min = gray;
        }
    }

    // Biến đổi tuyến tính
    for (int x = 0; x < image_in.width(); x++)
    {
        for (int y = 0; y < image_in.height(); y++)
        {
            QRgb color = image_in.pixel(x, y);
            int gray = qGray(color);
            int newgray = 0;

            // Tránh chia cho 0
            if (max != min) {
                newgray = ((gray - min) * 255) / (max - min);
            }

            QRgb color_out = qRgb(newgray, newgray, newgray);
            image_out.setPixel(x, y, color_out);
        }
    }

    // Hiển thị ảnh đầu vào và đầu ra
    DisplayImage(image_in, QFileInfo(filename).fileName());
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Linear_Transformation");
}




void MainWindow::on_bt_CanBangHistogram_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    // Tính histogram
    int histogram[256] = {0};
    int totalPixels = image_in.width() * image_in.height();

    for (int x = 0; x < image_in.width(); x++)
    {
        for (int y = 0; y < image_in.height(); y++)
        {
            QRgb color = image_in.pixel(x, y);
            int gray = qGray(color);
            histogram[gray]++;
        }
    }

    // Tính phân phối tích lũy
    int cumulativeHistogram[256] = {0};
    cumulativeHistogram[0] = histogram[0];

    for (int i = 1; i < 256; i++)
    {
        cumulativeHistogram[i] = cumulativeHistogram[i - 1] + histogram[i];
    }

    // Cân bằng histogram
    for (int x = 0; x < image_in.width(); x++)
    {
        for (int y = 0; y < image_in.height(); y++)
        {
            QRgb color = image_in.pixel(x, y);
            int gray = qGray(color);

            // Tính giá trị mới cho pixel
            int newgray = (cumulativeHistogram[gray] * 255) / totalPixels;
            QRgb color_out = qRgb(newgray, newgray, newgray);
            image_out.setPixel(x, y, color_out);
        }
    }

    // Hiển thị ảnh đầu vào và đầu ra
    DisplayImage(image_in, QFileInfo(filename).fileName());
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Histogram_Equalization");
}


void MainWindow::on_bt_Negative_Gray_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);

    for (int x = 0; x < image_in.width(); x++) {
        for (int y = 0; y < image_in.height(); y++) {
            QRgb color = image_in.pixel(x, y);
            int gray_in = qGray(color);
            int gray_out = 255 - gray_in;
            QRgb color_out = qRgb(gray_out, gray_out, gray_out);
            image_out.setPixel(x, y, color_out);
        }
    }
    DisplayImage(image_in, QFileInfo(filename).fileName());
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Gray_Negative");
}

void MainWindow::on_bt_Negative_Color_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.width(), image_in.height(), QImage::Format_ARGB32);

    for (int x = 0; x < image_in.width(); x++) {
        for (int y = 0; y < image_in.height(); y++) {
            QRgb color = image_in.pixel(x, y);
            int red = qRed(color);
            int green = qGreen(color);
            int blue = qBlue(color);
            int red_out = 255 - red;
            int green_out = 255 - green;
            int blue_out = 255 - blue;
            QRgb color_out = qRgb(red_out, green_out, blue_out);
            image_out.setPixel(x, y, color_out);
        }
    }
    DisplayImage(image_in, QFileInfo(filename).fileName());
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Color_Negative");
}

void MainWindow::on_bt_loctrungbinh_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    // Đọc kích thước mặt nạ từ ô nhập
    int maskSize = ui->ln_kichthuocmatna->text().toInt();
    if (maskSize % 2 == 0) {
        maskSize++;
    }

    int halfSize = maskSize / 2;

    for (int x = 0; x < image_in.width(); x++) {
        for (int y = 0; y < image_in.height(); y++) {
            int sumR = 0, sumG = 0, sumB = 0;
            int count = 0;

            for (int dx = -halfSize; dx <= halfSize; dx++) {
                for (int dy = -halfSize; dy <= halfSize; dy++) {
                    int newX = x + dx;
                    int newY = y + dy;

                    if (newX >= 0 && newX < image_in.width() && newY >= 0 && newY < image_in.height()) {
                        QRgb color = image_in.pixel(newX, newY);
                        sumR += qRed(color);
                        sumG += qGreen(color);
                        sumB += qBlue(color);
                        count++;
                    }
                }
            }

            QRgb color_out = qRgb(sumR / count, sumG / count, sumB / count);
            image_out.setPixel(x, y, color_out);
        }
    }

    DisplayImage(image_in, QFileInfo(filename).fileName());
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Average_Filter");

    // Hỏi người dùng có muốn lưu ảnh không
    if (QMessageBox::question(this, "Lưu ảnh", "Bạn có muốn lưu ảnh đã xử lý không?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString savePath = QFileDialog::getSaveFileName(this, "Lưu ảnh", "", "Images (*.png *.jpg *.bmp)");
        if (!savePath.isEmpty()) {
            image_out.save(savePath);
        }
    }
}

void MainWindow::on_bt_loctrungvi_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    int maskSize = ui->ln_kichthuocmatna->text().toInt();
    if (maskSize % 2 == 0) {
        maskSize++;
    }

    int halfSize = maskSize / 2;

    for (int x = 0; x < image_in.width(); x++) {
        for (int y = 0; y < image_in.height(); y++) {
            QVector<int> redValues, greenValues, blueValues;

            for (int dx = -halfSize; dx <= halfSize; dx++) {
                for (int dy = -halfSize; dy <= halfSize; dy++) {
                    int newX = x + dx;
                    int newY = y + dy;

                    if (newX >= 0 && newX < image_in.width() && newY >= 0 && newY < image_in.height()) {
                        QRgb color = image_in.pixel(newX, newY);
                        redValues.append(qRed(color));
                        greenValues.append(qGreen(color));
                        blueValues.append(qBlue(color));
                    }
                }
            }

            std::sort(redValues.begin(), redValues.end());
            std::sort(greenValues.begin(), greenValues.end());
            std::sort(blueValues.begin(), blueValues.end());

            int medianR = redValues[redValues.size() / 2];
            int medianG = greenValues[greenValues.size() / 2];
            int medianB = blueValues[blueValues.size() / 2];

            QRgb color_out = qRgb(medianR, medianG, medianB);
            image_out.setPixel(x, y, color_out);
        }
    }

    DisplayImage(image_in, QFileInfo(filename).fileName());
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Median_Filter");

    if (QMessageBox::question(this, "Lưu ảnh", "Bạn có muốn lưu ảnh đã xử lý không?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString savePath = QFileDialog::getSaveFileName(this, "Lưu ảnh", "", "Images (*.png *.jpg *.bmp)");
        if (!savePath.isEmpty()) {
            image_out.save(savePath);
        }
    }
}

void MainWindow::on_bt_themnhieumuoitieu_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_gray(image_in.size(), QImage::Format_ARGB32);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    // Chuyển đổi ảnh sang xám
    for (int x = 0; x < image_in.width(); x++) {
        for (int y = 0; y < image_in.height(); y++) {
            QRgb color = image_in.pixel(x, y);
            int gray = qGray(color);
            image_gray.setPixel(x, y, qRgb(gray, gray, gray));
        }
    }

    int noisePercentage = ui->ln_tylenhieu->text().toInt();
    int totalPixels = image_gray.width() * image_gray.height();
    int numNoisePixels = (totalPixels * noisePercentage) / 100;

    image_out = image_gray;

    // Thêm nhiễu muối tiêu
    for (int i = 0; i < numNoisePixels; i++) {
        int x = rand() % image_out.width();
        int y = rand() % image_out.height();

        if (rand() % 2 == 0) {
            image_out.setPixel(x, y, qRgb(255, 255, 255));
        } else {
            image_out.setPixel(x, y, qRgb(0, 0, 0));
        }
    }

    DisplayImage(image_gray, QFileInfo(filename).fileName() + "_Gray");
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Salt_Pepper_Noise");

    if (QMessageBox::question(this, "Lưu ảnh", "Bạn có muốn lưu ảnh đã thêm nhiễu không?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString savePath = QFileDialog::getSaveFileName(this, "Lưu ảnh", "", "Images (*.png *.jpg *.bmp)");
        if (!savePath.isEmpty()) {
            image_out.save(savePath);
        }
    }
}

void MainWindow::on_bt_nhieumuoitieuanhmau_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    int noisePercentage = ui->ln_tylenhieu->text().toInt();
    int totalPixels = image_in.width() * image_in.height();
    int numNoisePixels = (totalPixels * noisePercentage) / 100;

    image_out = image_in;

    // Thêm nhiễu muối tiêu
    for (int i = 0; i < numNoisePixels; i++) {
        int x = rand() % image_in.width();
        int y = rand() % image_in.height();

        if (rand() % 2 == 0) {
            image_out.setPixel(x, y, qRgb(255, 255, 255));
        } else {
            image_out.setPixel(x, y, qRgb(0, 0, 0));
        }
    }

    DisplayImage(image_in, QFileInfo(filename).fileName());
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Salt_Pepper_Noise");

    if (QMessageBox::question(this, "Lưu ảnh", "Bạn có muốn lưu ảnh đã thêm nhiễu không?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString savePath = QFileDialog::getSaveFileName(this, "Lưu ảnh", "", "Images (*.png *.jpg *.bmp)");
        if (!savePath.isEmpty()) {
            image_out.save(savePath);
        }
    }
}

void MainWindow::on_bt_LocGradient_AnhXam_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_gray(image_in.size(), QImage::Format_ARGB32);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    // Chuyển đổi ảnh sang xám
    for (int x = 0; x < image_in.width(); x++) {
        for (int y = 0; y < image_in.height(); y++) {
            QRgb color = image_in.pixel(x, y);
            int gray = qGray(color);
            image_gray.setPixel(x, y, qRgb(gray, gray, gray));
        }
    }

    // Ma trận Sobel để tính gradient theo trục X và Y
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    int width = image_gray.width();
    int height = image_gray.height();

    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            int sumX = 0;
            int sumY = 0;

            // Áp dụng bộ lọc Sobel
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int grayValue = qGray(image_gray.pixel(x + i, y + j));
                    sumX += Gx[i + 1][j + 1] * grayValue;
                    sumY += Gy[i + 1][j + 1] * grayValue;
                }
            }

            // Tính độ lớn gradient
            int magnitude = qBound(0, static_cast<int>(sqrt(sumX * sumX + sumY * sumY)), 255);

            image_out.setPixel(x, y, qRgb(magnitude, magnitude, magnitude));
        }
    }

    // Hiển thị và lưu ảnh
    DisplayImage(image_gray, QFileInfo(filename).fileName() + "_Gray");
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Gradient_Filter");

    if (QMessageBox::question(this, "Lưu ảnh", "Bạn có muốn lưu ảnh đã xử lý không?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString savePath = QFileDialog::getSaveFileName(this, "Lưu ảnh", "", "Images (*.png *.jpg *.bmp)");
        if (!savePath.isEmpty()) {
            image_out.save(savePath);
        }
    }
}

void MainWindow::on_bt_LocGradient_AnhMau_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    // Ma trận Sobel để tính gradient theo trục X và Y
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    int width = image_in.width();
    int height = image_in.height();

    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            int sumRx = 0, sumRy = 0;
            int sumGx = 0, sumGy = 0;
            int sumBx = 0, sumBy = 0;

            // Áp dụng bộ lọc Sobel cho mỗi kênh màu
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    QRgb color = image_in.pixel(x + i, y + j);

                    int red = qRed(color);
                    int green = qGreen(color);
                    int blue = qBlue(color);

                    sumRx += Gx[i + 1][j + 1] * red;
                    sumRy += Gy[i + 1][j + 1] * red;

                    sumGx += Gx[i + 1][j + 1] * green;
                    sumGy += Gy[i + 1][j + 1] * green;

                    sumBx += Gx[i + 1][j + 1] * blue;
                    sumBy += Gy[i + 1][j + 1] * blue;
                }
            }

            // Tính độ lớn gradient cho từng kênh màu
            int magnitudeR = qBound(0, static_cast<int>(sqrt(sumRx * sumRx + sumRy * sumRy)), 255);
            int magnitudeG = qBound(0, static_cast<int>(sqrt(sumGx * sumGx + sumGy * sumGy)), 255);
            int magnitudeB = qBound(0, static_cast<int>(sqrt(sumBx * sumBx + sumBy * sumBy)), 255);

            // Tạo màu mới từ các độ lớn gradient của từng kênh
            image_out.setPixel(x, y, qRgb(magnitudeR, magnitudeG, magnitudeB));
        }
    }

    // Hiển thị và lưu ảnh
    DisplayImage(image_in, QFileInfo(filename).fileName());
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Color_Gradient_Filter");

    if (QMessageBox::question(this, "Lưu ảnh", "Bạn có muốn lưu ảnh đã xử lý không?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString savePath = QFileDialog::getSaveFileName(this, "Lưu ảnh", "", "Images (*.png *.jpg *.bmp)");
        if (!savePath.isEmpty()) {
            image_out.save(savePath);
        }
    }
}

void MainWindow::on_pushButton_23_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_gray(image_in.size(), QImage::Format_ARGB32);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    // Chuyển đổi ảnh sang xám
    for (int x = 0; x < image_in.width(); x++) {
        for (int y = 0; y < image_in.height(); y++) {
            QRgb color = image_in.pixel(x, y);
            int gray = qGray(color);
            image_gray.setPixel(x, y, qRgb(gray, gray, gray));
        }
    }

    // Ma trận Robert để tính gradient theo trục X và Y
    int Gx[2][2] = {
        {1, 0},
        {0, -1}
    };

    int Gy[2][2] = {
        {0, 1},
        {-1, 0}
    };

    int width = image_gray.width();
    int height = image_gray.height();

    for (int x = 0; x < width - 1; x++) {
        for (int y = 0; y < height - 1; y++) {
            int sumX = 0;
            int sumY = 0;

            // Áp dụng bộ lọc Robert
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    int grayValue = qGray(image_gray.pixel(x + i, y + j));
                    sumX += Gx[i][j] * grayValue;
                    sumY += Gy[i][j] * grayValue;
                }
            }

            // Tính độ lớn gradient
            int magnitude = qBound(0, static_cast<int>(sqrt(sumX * sumX + sumY * sumY)), 255);

            image_out.setPixel(x, y, qRgb(magnitude, magnitude, magnitude));
        }
    }

    // Hiển thị và lưu ảnh
    DisplayImage(image_gray, QFileInfo(filename).fileName() + "_Gray");
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Robert_Filter");

    if (QMessageBox::question(this, "Lưu ảnh", "Bạn có muốn lưu ảnh đã xử lý không?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString savePath = QFileDialog::getSaveFileName(this, "Lưu ảnh", "", "Images (*.png *.jpg *.bmp)");
        if (!savePath.isEmpty()) {
            image_out.save(savePath);
        }
    }
}

void MainWindow::on_bt_LocRobert_AnhMau_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    // Ma trận Robert để tính gradient theo trục X và Y
    int Gx[2][2] = {
        {1, 0},
        {0, -1}
    };

    int Gy[2][2] = {
        {0, 1},
        {-1, 0}
    };

    int width = image_in.width();
    int height = image_in.height();

    for (int x = 0; x < width - 1; x++) {
        for (int y = 0; y < height - 1; y++) {
            int sumRx = 0, sumRy = 0;
            int sumGx = 0, sumGy = 0;
            int sumBx = 0, sumBy = 0;

            // Áp dụng bộ lọc Robert cho mỗi kênh màu
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    QRgb color = image_in.pixel(x + i, y + j);

                    int red = qRed(color);
                    int green = qGreen(color);
                    int blue = qBlue(color);

                    sumRx += Gx[i][j] * red;
                    sumRy += Gy[i][j] * red;

                    sumGx += Gx[i][j] * green;
                    sumGy += Gy[i][j] * green;

                    sumBx += Gx[i][j] * blue;
                    sumBy += Gy[i][j] * blue;
                }
            }

            // Tính độ lớn gradient cho từng kênh màu
            int magnitudeR = qBound(0, static_cast<int>(sqrt(sumRx * sumRx + sumRy * sumRy)), 255);
            int magnitudeG = qBound(0, static_cast<int>(sqrt(sumGx * sumGx + sumGy * sumGy)), 255);
            int magnitudeB = qBound(0, static_cast<int>(sqrt(sumBx * sumBx + sumBy * sumBy)), 255);

            // Tạo màu mới từ các độ lớn gradient của từng kênh
            image_out.setPixel(x, y, qRgb(magnitudeR, magnitudeG, magnitudeB));
        }
    }

    // Hiển thị và lưu ảnh
    DisplayImage(image_in, QFileInfo(filename).fileName());
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Color_Robert_Filter");

    if (QMessageBox::question(this, "Lưu ảnh", "Bạn có muốn lưu ảnh đã xử lý không?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString savePath = QFileDialog::getSaveFileName(this, "Lưu ảnh", "", "Images (*.png *.jpg *.bmp)");
        if (!savePath.isEmpty()) {
            image_out.save(savePath);
        }
    }
}

void MainWindow::on_pushButton_24_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_gray(image_in.size(), QImage::Format_ARGB32);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    // Chuyển đổi ảnh sang xám
    for (int x = 0; x < image_in.width(); x++) {
        for (int y = 0; y < image_in.height(); y++) {
            QRgb color = image_in.pixel(x, y);
            int gray = qGray(color);
            image_gray.setPixel(x, y, qRgb(gray, gray, gray));
        }
    }

    // Ma trận Sobel để tính gradient theo trục X và Y
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    int width = image_gray.width();
    int height = image_gray.height();

    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            int sumX = 0;
            int sumY = 0;

            // Áp dụng bộ lọc Sobel
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int grayValue = qGray(image_gray.pixel(x + i, y + j));
                    sumX += Gx[i + 1][j + 1] * grayValue;
                    sumY += Gy[i + 1][j + 1] * grayValue;
                }
            }

            // Tính độ lớn gradient
            int magnitude = qBound(0, static_cast<int>(sqrt(sumX * sumX + sumY * sumY)), 255);

            image_out.setPixel(x, y, qRgb(magnitude, magnitude, magnitude));
        }
    }

    // Hiển thị và lưu ảnh
    DisplayImage(image_gray, QFileInfo(filename).fileName() + "_Gray");
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Sobel_Filter");

    if (QMessageBox::question(this, "Lưu ảnh", "Bạn có muốn lưu ảnh đã xử lý không?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString savePath = QFileDialog::getSaveFileName(this, "Lưu ảnh", "", "Images (*.png *.jpg *.bmp)");
        if (!savePath.isEmpty()) {
            image_out.save(savePath);
        }
    }
}

void MainWindow::on_bt_LocSobel_AnhMau_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    // Ma trận Sobel để tính gradient theo trục X và Y
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    int width = image_in.width();
    int height = image_in.height();

    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            int sumRx = 0, sumRy = 0;
            int sumGx = 0, sumGy = 0;
            int sumBx = 0, sumBy = 0;

            // Áp dụng bộ lọc Sobel cho từng kênh màu
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    QRgb color = image_in.pixel(x + i, y + j);

                    int red = qRed(color);
                    int green = qGreen(color);
                    int blue = qBlue(color);

                    sumRx += Gx[i + 1][j + 1] * red;
                    sumRy += Gy[i + 1][j + 1] * red;

                    sumGx += Gx[i + 1][j + 1] * green;
                    sumGy += Gy[i + 1][j + 1] * green;

                    sumBx += Gx[i + 1][j + 1] * blue;
                    sumBy += Gy[i + 1][j + 1] * blue;
                }
            }

            // Tính độ lớn gradient cho từng kênh màu
            int magnitudeR = qBound(0, static_cast<int>(sqrt(sumRx * sumRx + sumRy * sumRy)), 255);
            int magnitudeG = qBound(0, static_cast<int>(sqrt(sumGx * sumGx + sumGy * sumGy)), 255);
            int magnitudeB = qBound(0, static_cast<int>(sqrt(sumBx * sumBx + sumBy * sumBy)), 255);

            // Tạo màu mới từ các độ lớn gradient của từng kênh
            image_out.setPixel(x, y, qRgb(magnitudeR, magnitudeG, magnitudeB));
        }
    }

    // Hiển thị và lưu ảnh
    DisplayImage(image_in, QFileInfo(filename).fileName());
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Color_Sobel_Filter");

    if (QMessageBox::question(this, "Lưu ảnh", "Bạn có muốn lưu ảnh đã xử lý không?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString savePath = QFileDialog::getSaveFileName(this, "Lưu ảnh", "", "Images (*.png *.jpg *.bmp)");
        if (!savePath.isEmpty()) {
            image_out.save(savePath);
        }
    }
}

void MainWindow::on_pushButton_25_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_gray(image_in.size(), QImage::Format_ARGB32);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    // Chuyển đổi ảnh sang xám
    for (int x = 0; x < image_in.width(); x++) {
        for (int y = 0; y < image_in.height(); y++) {
            QRgb color = image_in.pixel(x, y);
            int gray = qGray(color);
            image_gray.setPixel(x, y, qRgb(gray, gray, gray));
        }
    }

    // Ma trận Prewitt để tính gradient theo trục X và Y
    int Gx[3][3] = {
        {-1, 0, 1},
        {-1, 0, 1},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -1, -1},
        { 0,  0,  0},
        { 1,  1,  1}
    };

    int width = image_gray.width();
    int height = image_gray.height();

    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            int sumX = 0;
            int sumY = 0;

            // Áp dụng bộ lọc Prewitt
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int grayValue = qGray(image_gray.pixel(x + i, y + j));
                    sumX += Gx[i + 1][j + 1] * grayValue;
                    sumY += Gy[i + 1][j + 1] * grayValue;
                }
            }

            // Tính độ lớn gradient
            int magnitude = qBound(0, static_cast<int>(sqrt(sumX * sumX + sumY * sumY)), 255);

            image_out.setPixel(x, y, qRgb(magnitude, magnitude, magnitude));
        }
    }

    // Hiển thị và lưu ảnh
    DisplayImage(image_gray, QFileInfo(filename).fileName() + "_Gray");
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Prewitt_Filter");

    if (QMessageBox::question(this, "Lưu ảnh", "Bạn có muốn lưu ảnh đã xử lý không?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString savePath = QFileDialog::getSaveFileName(this, "Lưu ảnh", "", "Images (*.png *.jpg *.bmp)");
        if (!savePath.isEmpty()) {
            image_out.save(savePath);
        }
    }
}

void MainWindow::on_bt_LocPrewitt_AnhMau_2_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    // Ma trận Prewitt để tính gradient theo trục X và Y
    int Gx[3][3] = {
        {-1, 0, 1},
        {-1, 0, 1},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -1, -1},
        { 0,  0,  0},
        { 1,  1,  1}
    };

    int width = image_in.width();
    int height = image_in.height();

    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            int sumRx = 0, sumRy = 0;
            int sumGx = 0, sumGy = 0;
            int sumBx = 0, sumBy = 0;

            // Áp dụng bộ lọc Prewitt cho từng kênh màu
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    QRgb color = image_in.pixel(x + i, y + j);

                    int red = qRed(color);
                    int green = qGreen(color);
                    int blue = qBlue(color);

                    sumRx += Gx[i + 1][j + 1] * red;
                    sumRy += Gy[i + 1][j + 1] * red;

                    sumGx += Gx[i + 1][j + 1] * green;
                    sumGy += Gy[i + 1][j + 1] * green;

                    sumBx += Gx[i + 1][j + 1] * blue;
                    sumBy += Gy[i + 1][j + 1] * blue;
                }
            }

            // Tính độ lớn gradient cho từng kênh màu
            int magnitudeR = qBound(0, static_cast<int>(sqrt(sumRx * sumRx + sumRy * sumRy)), 255);
            int magnitudeG = qBound(0, static_cast<int>(sqrt(sumGx * sumGx + sumGy * sumGy)), 255);
            int magnitudeB = qBound(0, static_cast<int>(sqrt(sumBx * sumBx + sumBy * sumBy)), 255);

            // Tạo màu mới từ các độ lớn gradient của từng kênh
            image_out.setPixel(x, y, qRgb(magnitudeR, magnitudeG, magnitudeB));
        }
    }

    // Hiển thị và lưu ảnh
    DisplayImage(image_in, QFileInfo(filename).fileName());
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Color_Prewitt_Filter");

    if (QMessageBox::question(this, "Lưu ảnh", "Bạn có muốn lưu ảnh đã xử lý không?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString savePath = QFileDialog::getSaveFileName(this, "Lưu ảnh", "", "Images (*.png *.jpg *.bmp)");
        if (!savePath.isEmpty()) {
            image_out.save(savePath);
        }
    }
}

void MainWindow::on_pushButton_26_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_gray(image_in.size(), QImage::Format_ARGB32);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    // Chuyển đổi ảnh sang xám
    for (int x = 0; x < image_in.width(); x++) {
        for (int y = 0; y < image_in.height(); y++) {
            QRgb color = image_in.pixel(x, y);
            int gray = qGray(color);
            image_gray.setPixel(x, y, qRgb(gray, gray, gray));
        }
    }

    // Ma trận Laplace
    int LaplaceKernel[3][3] = {
        { 0,  1,  0},
        { 1, -4,  1},
        { 0,  1,  0}
    };

    int width = image_gray.width();
    int height = image_gray.height();

    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            int sum = 0;

            // Áp dụng bộ lọc Laplace
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int grayValue = qGray(image_gray.pixel(x + i, y + j));
                    sum += LaplaceKernel[i + 1][j + 1] * grayValue;
                }
            }

            // Lấy giá trị tuyệt đối của tổng và đảm bảo trong khoảng [0, 255]
            int magnitude = qBound(0, abs(sum), 255);

            image_out.setPixel(x, y, qRgb(magnitude, magnitude, magnitude));
        }
    }

    // Hiển thị và lưu ảnh
    DisplayImage(image_gray, QFileInfo(filename).fileName() + "_Gray");
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Laplace_Filter");

    if (QMessageBox::question(this, "Lưu ảnh", "Bạn có muốn lưu ảnh đã xử lý không?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString savePath = QFileDialog::getSaveFileName(this, "Lưu ảnh", "", "Images (*.png *.jpg *.bmp)");
        if (!savePath.isEmpty()) {
            image_out.save(savePath);
        }
    }
}

void MainWindow::on_bt_LocLaplace_AnhMau_3_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    // Ma trận Laplace
    int LaplaceKernel[3][3] = {
        { 0,  1,  0},
        { 1, -4,  1},
        { 0,  1,  0}
    };

    int width = image_in.width();
    int height = image_in.height();

    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            int sumR = 0, sumG = 0, sumB = 0;

            // Áp dụng bộ lọc Laplace cho từng kênh màu
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    QRgb color = image_in.pixel(x + i, y + j);

                    int red = qRed(color);
                    int green = qGreen(color);
                    int blue = qBlue(color);

                    sumR += LaplaceKernel[i + 1][j + 1] * red;
                    sumG += LaplaceKernel[i + 1][j + 1] * green;
                    sumB += LaplaceKernel[i + 1][j + 1] * blue;
                }
            }

            // Lấy giá trị tuyệt đối của tổng cho từng kênh màu và giới hạn trong khoảng [0, 255]
            int magnitudeR = qBound(0, abs(sumR), 255);
            int magnitudeG = qBound(0, abs(sumG), 255);
            int magnitudeB = qBound(0, abs(sumB), 255);

            // Đặt giá trị mới cho pixel
            image_out.setPixel(x, y, qRgb(magnitudeR, magnitudeG, magnitudeB));
        }
    }

    // Hiển thị và lưu ảnh
    DisplayImage(image_in, QFileInfo(filename).fileName());
    DisplayImage(image_out, QFileInfo(filename).fileName() + "_Color_Laplace_Filter");

    if (QMessageBox::question(this, "Lưu ảnh", "Bạn có muốn lưu ảnh đã xử lý không?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QString savePath = QFileDialog::getSaveFileName(this, "Lưu ảnh", "", "Images (*.png *.jpg *.bmp)");
        if (!savePath.isEmpty()) {
            image_out.save(savePath);
        }
    }
}

void MainWindow::on_bt_AnMon_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    int width = image_in.width();
    int height = image_in.height();

    // Kernel 3x3 cho phép giãn nở
    int kernel[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };

    // Áp dụng phép giãn nở
    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            int max = 0;  // Giá trị pixel lớn nhất (cho phép giãn nở)
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int grayValue = qGray(image_in.pixel(x + i, y + j));
                    if (kernel[i + 1][j + 1] == 1) {
                        max = std::max(max, grayValue);
                    }
                }
            }
            image_out.setPixel(x, y, qRgb(max, max, max));
        }
    }

    DisplayImage(image_in, "Original Image");
    DisplayImage(image_out, "Dilated Image (Dilation Result)");
}

void MainWindow::on_bt_GianNo_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    int width = image_in.width();
    int height = image_in.height();

    // Kernel 3x3 cho phép ăn mòn
    int kernel[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };

    // Áp dụng phép ăn mòn
    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            int min = 255;  // Giá trị pixel nhỏ nhất (cho phép ăn mòn)
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int grayValue = qGray(image_in.pixel(x + i, y + j));
                    if (kernel[i + 1][j + 1] == 1) {
                        min = std::min(min, grayValue);
                    }
                }
            }
            image_out.setPixel(x, y, qRgb(min, min, min));
        }
    }

    DisplayImage(image_in, "Original Image");
    DisplayImage(image_out, "Eroded Image (Erosion Result)");
}

void MainWindow::on_bt_MoAnh_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_dilated(image_in.size(), QImage::Format_ARGB32);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    // Kernel 3x3
    int kernel[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };

    int width = image_in.width();
    int height = image_in.height();

    // Bước 1: Thực hiện phép giãn nở (Dilation)
    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            int max = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int grayValue = qGray(image_in.pixel(x + i, y + j));
                    if (kernel[i + 1][j + 1] == 1) {
                        max = std::max(max, grayValue);
                    }
                }
            }
            image_dilated.setPixel(x, y, qRgb(max, max, max));
        }
    }

    // Bước 2: Thực hiện phép ăn mòn (Erosion) trên ảnh đã giãn nở
    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            int min = 255;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int grayValue = qGray(image_dilated.pixel(x + i, y + j));
                    if (kernel[i + 1][j + 1] == 1) {
                        min = std::min(min, grayValue);
                    }
                }
            }
            image_out.setPixel(x, y, qRgb(min, min, min));
        }
    }

    DisplayImage(image_in, "Original");
    DisplayImage(image_out, "Close Result");
}

void MainWindow::on_bt_close_clicked()
{
    QString filename = ui->ln_filename->text();
    QImage image_in(filename);
    QImage image_eroded(image_in.size(), QImage::Format_ARGB32);
    QImage image_out(image_in.size(), QImage::Format_ARGB32);

    // Kernel 3x3
    int kernel[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };

    int width = image_in.width();
    int height = image_in.height();

    // Bước 1: Thực hiện phép ăn mòn (Erosion)
    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            int min = 255;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int grayValue = qGray(image_in.pixel(x + i, y + j));
                    if (kernel[i + 1][j + 1] == 1) {
                        min = std::min(min, grayValue);
                    }
                }
            }
            image_eroded.setPixel(x, y, qRgb(min, min, min));
        }
    }

    // Bước 2: Thực hiện phép giãn nở (Dilation) trên ảnh đã ăn mòn
    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            int max = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int grayValue = qGray(image_eroded.pixel(x + i, y + j));
                    if (kernel[i + 1][j + 1] == 1) {
                        max = std::max(max, grayValue);
                    }
                }
            }
            image_out.setPixel(x, y, qRgb(max, max, max));
        }
    }

    DisplayImage(image_in, "Original");
    DisplayImage(image_out, "Open Result");
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File",
    "C:/CVIPtools/images",
    "*.* All Files;;*.bmp;;*.jpeg;;*.ppm;;*.png;;*.jpg");
    QImage image (fileName);
    DisplayImage (image, QFileInfo (fileName).fileName());
}

void MainWindow::on_actionSave_triggered()
{
    QMdiSubWindow *activeWindow= mdiArea->currentSubWindow();
    if (activeWindow == 0)
    return;
    ImageWidget *activeWidget = (ImageWidget*) activeWindow->widget();
    QImage image = activeWidget->getImage();
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this, "Save image",
    "C:/CVIPtools/images",
    "BMP (*.bmp); PNG (*.png); JPEG (.jpg):: All files (*.*)",
    &selectedFilter);
    if (selectedFilter == "BMP (*.bmp)")
    image.save(fileName, "BMP");
    else if (selectedFilter == "PNG (*.png)")
    image.save(fileName, "PNG");
    else if (selectedFilter == "JPEG (.jpg)")
    image.save(fileName, "JPEG");
    else
    image.save(fileName);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

QImage MainWindow::draw_His_Gray(QImage &image_in)
{
const int HEIGHT = 128;//Chieu cao to chuc do
QImage histogram (256, HEIGHT, QImage:: Format_ARGB32);
histogram.fill(Qt::white);//Khoi tao to chuc do mau trang
int h[256];
for (int i=0; i<256; i++)//Khoi tao 256 mang luu tru so luong pixel
h[i]=0;
//tuong ung voi muc xam 0...255
for (int x=0; x<image_in.width(); x++)
for (int y=0; y<image_in.height(); y++) {
QRgb rgb = image_in.pixel (x, y);
int gray = qGray (rgb);
h[gray]++; //Dem so luong pixel tuong ung voi muc xam 0...255
}
int max=0;
for (int i=0; i<256; i++)
if (h[i]>max) max=h[i];
int lineheight = 0;
for (int x=0; x<256; x++) {
lineheight = HEIGHT*h [x]/max; //Tinh ty le
for (int y=HEIGHT-1; y>HEIGHT-1-lineheight; y--)
histogram.setPixel(x, y, qRgb(0, 0, 255)); //Ve to chuc do
}
return histogram;
}

void MainWindow::on_actionShow_Histogram_Gray_Image_triggered()
{
QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
if (activeWindow == 0)
return;
ImageWidget *activeWidget = (ImageWidget*) activeWindow->widget();
QImage image_in = activeWidget->getImage();
QImage image_out = draw_His_Gray (image_in);
DisplayImage (image_out, activeWindow->windowTitle()+"_Histogram");
}

QImage MainWindow::draw_His_Color (QImage &image_in)
{
const int HEIGHT = 128;
int h_red [256], h_blue [256], h_green [256];
for (int i=0; i<256; i++) {
h_red[i] = 0;
h_green [i] = 0;
h_blue [i] = 0;
}
for (int x=0; x<image_in.width(); x++)
for (int y=0; y<image_in.height(); y++) {
QColor color = image_in.pixel (x,y);
int red = color.red();
int green = color.green();
int blue = color.blue();
h_red [red]++;
h_green [green]++;
h_blue [blue]++;
}
int max = 0;
for (int i=0; i<256; i++) {
if (max < h_red[i]) max = h_red [i];
if (max < h_green[i]) max = h_green [i];
if (max < h_blue[i]) max = h_blue[i];
}
QImage histogram(256, HEIGHT * 3, QImage::Format_RGB32);

histogram.fill(Qt::white);
int lineHeight;
for (int i=0; i<256; i++) {
lineHeight = HEIGHT*h_red[i]/max;
for (int j=HEIGHT-1; j>HEIGHT-lineHeight; j--) histogram.setPixel(i,j,qRgb(255,0,0));
lineHeight = HEIGHT * h_green [i]/max;
for (int j=2*HEIGHT-1; j>2*HEIGHT-lineHeight; j--)
histogram.setPixel(i,j,qRgb(0,255,0));
lineHeight = HEIGHT * h_blue[i]/max;
for (int j=3*HEIGHT-1; j>3*HEIGHT-lineHeight; j--)
    histogram.setPixel(i,j,qRgb(0,0,255));
}
return histogram;
}

void MainWindow::on_actionShow_Histogram_Color_Image_triggered ()
{
QMdiSubWindow *activeWindow= mdiArea->currentSubWindow();
if (activeWindow== 0)
return;
ImageWidget *activeWidget = (ImageWidget*) activeWindow->widget();
QImage image_in = activeWidget->getImage();
QImage image_out = draw_His_Color(image_in);
DisplayImage (image_out, activeWindow->windowTitle()+"_Histogram");
}


void MainWindow::on_actionHistogram_Side_triggered()
{
    // Lấy cửa sổ con đang hoạt động
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if (activeWindow == 0) // Kiểm tra nếu không có cửa sổ con
        return;

    // Lấy widget chứa ảnh hiện tại
    ImageWidget *activeWidget = qobject_cast<ImageWidget *>(activeWindow->widget());
    if (!activeWidget)
        return;

    // Lấy ảnh hiện tại từ widget
    QImage image_in = activeWidget->getImage();

    // Kiểm tra loại ảnh và vẽ histogram tương ứng
    QImage image_out;
    if (image_in.isGrayscale()) // Kiểm tra nếu ảnh là ảnh xám
        image_out = draw_His_Gray(image_in);
    else // Nếu không thì xử lý histogram ảnh màu
        image_out = draw_His_Color(image_in);

    // Hiển thị histogram ở một bên
    DisplayImage(image_out, activeWindow->windowTitle() + " Histogram (Side)");
}


void MainWindow::on_actionHistogram_Stretch_triggered()
{
    // Lấy cửa sổ con đang hoạt động
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if (activeWindow == 0) // Kiểm tra nếu không có cửa sổ con
        return;

    // Lấy widget chứa ảnh hiện tại
    ImageWidget *activeWidget = qobject_cast<ImageWidget *>(activeWindow->widget());
    if (!activeWidget)
        return;

    // Lấy ảnh hiện tại từ widget
    QImage image_in = activeWidget->getImage();

    // Thực hiện Histogram Stretch
    QImage image_out = histogramStretch(image_in);

    // Hiển thị ảnh sau khi xử lý
    DisplayImage(image_out, activeWindow->windowTitle() + " Histogram Stretched");
}

QImage MainWindow::histogramStretch(const QImage &image_in)
{
    QImage image_out = image_in.copy(); // Tạo bản sao ảnh gốc để xử lý

    // Tìm giá trị mức xám nhỏ nhất và lớn nhất
    int minGray = 255, maxGray = 0;

    for (int x = 0; x < image_in.width(); ++x) {
        for (int y = 0; y < image_in.height(); ++y) {
            int gray = qGray(image_in.pixel(x, y)); // Lấy giá trị mức xám
            if (gray < minGray)
                minGray = gray;
            if (gray > maxGray)
                maxGray = gray;
        }
    }

    // Nếu minGray == maxGray thì không có gì để stretch
    if (minGray == maxGray)
        return image_out;

    // Thực hiện giãn giá trị mức xám
    for (int x = 0; x < image_out.width(); ++x) {
        for (int y = 0; y < image_out.height(); ++y) {
            QRgb pixel = image_in.pixel(x, y);
            int gray = qGray(pixel);

            // Tính giá trị mức xám mới
            int stretchedGray = (gray - minGray) * 255 / (maxGray - minGray);
            image_out.setPixel(x, y, qRgb(stretchedGray, stretchedGray, stretchedGray));
        }
    }

    return image_out;
}


void MainWindow::on_actionHistogram_Equalization_triggered()
{
    // Lấy cửa sổ hiện tại trong mdiArea
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if (activeWindow == 0) // Nếu không có cửa sổ đang mở
        return;

    // Lấy widget chứa ảnh
    ImageWidget *activeWidget = qobject_cast<ImageWidget *>(activeWindow->widget());
    if (!activeWidget)
        return;

    // Lấy ảnh gốc
    QImage image_in = activeWidget->getImage();

    // Thực hiện cân bằng histogram
    QImage image_out = histogramEqualization(image_in);

    // Hiển thị ảnh đã cân bằng histogram
    DisplayImage(image_out, activeWindow->windowTitle() + " - Equalized Histogram");
}

QImage MainWindow::histogramEqualization(const QImage &image_in)
{
    QImage image_out = image_in.copy(); // Tạo bản sao của ảnh đầu vào

    // Bước 1: Tính histogram gốc
    int histogram[256] = {0};
    for (int x = 0; x < image_in.width(); ++x) {
        for (int y = 0; y < image_in.height(); ++y) {
            int gray = qGray(image_in.pixel(x, y));
            histogram[gray]++;
        }
    }

    // Bước 2: Tính phân phối tích lũy (CDF)
    int cdf[256] = {0};
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; ++i) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    // Bước 3: Chuẩn hóa CDF để giá trị nằm trong [0, 255]
    int cdf_min = cdf[0];
    int total_pixels = image_in.width() * image_in.height();
    int lut[256] = {0}; // Bảng tra cứu (Lookup Table)
    for (int i = 0; i < 256; ++i) {
        lut[i] = qRound((float)(cdf[i] - cdf_min) / (total_pixels - cdf_min) * 255);
    }

    // Bước 4: Áp dụng LUT để tạo ảnh đầu ra
    for (int x = 0; x < image_out.width(); ++x) {
        for (int y = 0; y < image_out.height(); ++y) {
            int gray = qGray(image_in.pixel(x, y));
            int new_gray = lut[gray];
            image_out.setPixel(x, y, qRgb(new_gray, new_gray, new_gray));
        }
    }

    return image_out;
}


QImage MainWindow::linearModification(const QImage& inputImage, double alpha, int beta)
{
    // Tạo ảnh sao chép từ ảnh đầu vào
    QImage outputImage = inputImage.copy();

    // Áp dụng phép biến đổi tuyến tính
    for (int y = 0; y < inputImage.height(); ++y)
    {
        for (int x = 0; x < inputImage.width(); ++x)
        {
            // Lấy màu của pixel tại (x, y)
            QRgb color = inputImage.pixel(x, y);

            // Áp dụng phép biến đổi tuyến tính cho từng kênh màu
            int newRed = qBound(0, (int)(alpha * qRed(color) + beta), 255);
            int newGreen = qBound(0, (int)(alpha * qGreen(color) + beta), 255);
            int newBlue = qBound(0, (int)(alpha * qBlue(color) + beta), 255);

            // Thiết lập màu mới cho pixel
            outputImage.setPixel(x, y, qRgb(newRed, newGreen, newBlue));
        }
    }

    // Kiểm tra xem ảnh đầu ra có hợp lệ không và in ra thông báo debug
    qDebug() << "Image output is valid:" << !outputImage.isNull();

    return outputImage;
}


void MainWindow::on_actionLinear_Modification_triggered()
{
    // Đảm bảo bạn đã cung cấp đường dẫn đúng tới ảnh đầu vào
    QImage inputImage("C:/Users/thanh/Desktop/DA/image.jpg");  // Đảm bảo đường dẫn đúng tới ảnh của bạn
    if (inputImage.isNull()) {
        qDebug() << "Failed to load input image.";
        return;
    }

    // Gọi hàm linearModification
    double alpha = 1.2;  // Hệ số alpha (chỉnh độ sáng)
    int beta = 50;       // Hệ số beta (chỉnh độ sáng)

    QImage outputImage = linearModification(inputImage, alpha, beta);

    // Kiểm tra ảnh đầu ra có hợp lệ hay không
    if (outputImage.isNull()) {
        qDebug() << "Failed to process image.";
    } else {
        qDebug() << "Image processed successfully!";

        // Hiển thị ảnh đầu ra trong QLabel
        QLabel* label = new QLabel(this);  // Tạo QLabel mới
        label->setPixmap(QPixmap::fromImage(outputImage));  // Gán ảnh vào QLabel
        label->setAlignment(Qt::AlignCenter);  // Căn giữa ảnh
        label->resize(outputImage.size());  // Đảm bảo QLabel có kích thước phù hợp
        label->show();  // Hiển thị QLabel

        // Sau khi xử lý xong, bạn có thể lưu ảnh
        if (outputImage.save("C:/Users/thanh/Desktop/DA/output_image.jpg")) {
            qDebug() << "Image saved successfully.";
        } else {
            qDebug() << "Failed to save image.";
        }
    }
}

void MainWindow::on_actionGradient_Filter_triggered()
{
    // Đảm bảo bạn đã cung cấp đường dẫn đúng tới ảnh đầu vào
    QImage inputImage("C:/Users/thanh/Downloads/hinh.jpg");  // Đảm bảo đường dẫn đúng tới ảnh của bạn
    if (inputImage.isNull()) {
        qDebug() << "Failed to load input image.";
        return;
    }
    qDebug() << "Loaded image size: " << inputImage.size();

    // Áp dụng phép lọc Gradient (Sobel filter)
    QImage outputImage = applyGradientFilter(inputImage);

    // Kiểm tra ảnh đầu ra có hợp lệ hay không
    if (outputImage.isNull()) {
        qDebug() << "Failed to process image.";
    } else {
        qDebug() << "Gradient filter applied successfully!";
        qDebug() << "Output image is null: " << outputImage.isNull();
        qDebug() << "Output image size: " << outputImage.size();

        // Kiểm tra QPixmap từ QImage
        QPixmap pixmap = QPixmap::fromImage(outputImage);
        if (pixmap.isNull()) {
            qDebug() << "Failed to convert QImage to QPixmap.";
        } else {
            // Hiển thị ảnh đầu ra trong QLabel
            QLabel* label = new QLabel(this);  // Tạo QLabel mới
            label->setFixedSize(outputImage.size());  // Đảm bảo QLabel có kích thước phù hợp với ảnh
            qDebug() << "Setting QLabel size to: " << outputImage.size();
            label->setPixmap(pixmap);  // Gán ảnh vào QLabel
            label->setAlignment(Qt::AlignCenter);  // Căn giữa ảnh

            label->resize(outputImage.size());  // Đảm bảo QLabel có kích thước phù hợp
            label->show();  // Hiển thị QLabel

            // Sau khi xử lý xong, bạn có thể lưu ảnh
            if (outputImage.save("C:/Users/thanh/Desktop/DA/output_gradient_image.jpg")) {
                qDebug() << "Image saved successfully.";
            } else {
                qDebug() << "Failed to save image.";
            }
        }
    }
}

QImage MainWindow::applyGradientFilter(const QImage& inputImage)
{
    QImage outputImage = inputImage.copy();  // Tạo bản sao của ảnh đầu vào

    // Kiểm tra ảnh đầu vào
    if (inputImage.isNull()) {
        qDebug() << "Input image is null!";
        return QImage();  // Nếu ảnh đầu vào không hợp lệ, trả về ảnh null
    }

    // Logic để áp dụng bộ lọc gradient (Sobel hoặc giảm độ sáng mỗi pixel)
    for (int y = 0; y < inputImage.height(); ++y)
    {
        for (int x = 0; x < inputImage.width(); ++x)
        {
            QRgb color = inputImage.pixel(x, y);
            int red = qRed(color);
            int green = qGreen(color);
            int blue = qBlue(color);

            // Áp dụng gradient filter (ví dụ làm giảm độ sáng của mỗi pixel)
            int newRed = qBound(0, red - 50, 255);
            int newGreen = qBound(0, green - 50, 255);
            int newBlue = qBound(0, blue - 50, 255);

            outputImage.setPixel(x, y, qRgb(newRed, newGreen, newBlue));
        }
    }

    return outputImage;
}










void MainWindow::on_actionRobert_Filter_triggered()
{
    // Đảm bảo bạn đã cung cấp đường dẫn đúng tới ảnh đầu vào
    QImage inputImage("C:/Users/thanh/Downloads/hinh.jpg");  // Đảm bảo đường dẫn đúng tới ảnh của bạn
    if (inputImage.isNull()) {
        qDebug() << "Failed to load input image.";
        return;
    }
    qDebug() << "Loaded image size: " << inputImage.size();

    // Áp dụng phép lọc Robert (tính gradient theo hướng x và y)
    QImage outputImage = applyRobertFilter(inputImage);

    // Kiểm tra ảnh đầu ra có hợp lệ hay không
    if (outputImage.isNull()) {
        qDebug() << "Failed to process image.";
    } else {
        qDebug() << "Robert filter applied successfully!";
        qDebug() << "Output image is null: " << outputImage.isNull();
        qDebug() << "Output image size: " << outputImage.size();

        // Kiểm tra QPixmap từ QImage
        QPixmap pixmap = QPixmap::fromImage(outputImage);
        if (pixmap.isNull()) {
            qDebug() << "Failed to convert QImage to QPixmap.";
        } else {
            // Hiển thị ảnh đầu ra trong QLabel
            QLabel* label = new QLabel(this);  // Tạo QLabel mới
            label->setFixedSize(outputImage.size());  // Đảm bảo QLabel có kích thước phù hợp với ảnh
            qDebug() << "Setting QLabel size to: " << outputImage.size();
            label->setPixmap(pixmap);  // Gán ảnh vào QLabel
            label->setAlignment(Qt::AlignCenter);  // Căn giữa ảnh

            label->resize(outputImage.size());  // Đảm bảo QLabel có kích thước phù hợp
            label->show();  // Hiển thị QLabel

            // Sau khi xử lý xong, bạn có thể lưu ảnh
            if (outputImage.save("C:/Users/thanh/Desktop/DA/output_robert_image.jpg")) {
                qDebug() << "Image saved successfully.";
            } else {
                qDebug() << "Failed to save image.";
            }
        }
    }
}

// Hàm áp dụng bộ lọc Robert
QImage MainWindow::applyRobertFilter(const QImage& inputImage)
{
    QImage outputImage = inputImage.copy();  // Tạo bản sao của ảnh đầu vào

    if (inputImage.isNull()) {
        qDebug() << "Input image is null!";
        return QImage();  // Nếu ảnh đầu vào không hợp lệ, trả về ảnh null
    }

    int width = inputImage.width();
    int height = inputImage.height();

    // Duyệt qua tất cả các pixel (bỏ qua biên vì bộ lọc Robert yêu cầu xử lý 2x2 vùng)
    for (int y = 0; y < height - 1; ++y)
    {
        for (int x = 0; x < width - 1; ++x)
        {
            // Lấy các pixel trong vùng 2x2
            QRgb color1 = inputImage.pixel(x, y);        // pixel (x, y)
            QRgb color2 = inputImage.pixel(x + 1, y);    // pixel (x+1, y)
            QRgb color3 = inputImage.pixel(x, y + 1);    // pixel (x, y+1)
            QRgb color4 = inputImage.pixel(x + 1, y + 1); // pixel (x+1, y+1)

            // Tính gradient theo chiều x (Gx)
            int gx = qRed(color1) - qRed(color2) + qRed(color3) - qRed(color4);

            // Tính gradient theo chiều y (Gy)
            int gy = qRed(color1) + qRed(color2) - qRed(color3) - qRed(color4);

            // Tính độ lớn của gradient
            int gradient = qBound(0, static_cast<int>(std::sqrt(gx * gx + gy * gy)), 255);

            // Gán lại giá trị pixel
            outputImage.setPixel(x, y, qRgb(gradient, gradient, gradient));  // Dùng độ sáng cho cả ba kênh (RGB)
        }
    }

    return outputImage;
}



void MainWindow::on_actionMean_Filter_Gray_triggered()
{
    // Đảm bảo bạn đã cung cấp đường dẫn đúng tới ảnh đầu vào
    QImage inputImage("C:/Users/thanh/Downloads/hinh.jpg");  // Đảm bảo đường dẫn đúng tới ảnh của bạn
    if (inputImage.isNull()) {
        qDebug() << "Failed to load input image.";
        return;
    }
    if (inputImage.isNull()) {
        qDebug() << "Failed to load input image.";
        return;
    }

    qDebug() << "Loaded image size: " << inputImage.size();

    // Áp dụng bộ lọc trung bình (Mean filter)
    QImage outputImage = applyMeanFilter(inputImage);

    // Kiểm tra ảnh đầu ra có hợp lệ hay không
    if (outputImage.isNull()) {
        qDebug() << "Failed to process image.";
    } else {
        qDebug() << "Mean filter applied successfully!";
        qDebug() << "Output image is null: " << outputImage.isNull();
        qDebug() << "Output image size: " << outputImage.size();

        // Kiểm tra QPixmap từ QImage
        QPixmap pixmap = QPixmap::fromImage(outputImage);
        if (pixmap.isNull()) {
            qDebug() << "Failed to convert QImage to QPixmap.";
        } else {
            // Hiển thị ảnh đầu ra trong QLabel
            QLabel* label = new QLabel(this);  // Tạo QLabel mới
            label->setFixedSize(outputImage.size());  // Đảm bảo QLabel có kích thước phù hợp với ảnh
            label->setPixmap(pixmap);  // Gán ảnh vào QLabel
            label->setAlignment(Qt::AlignCenter);  // Căn giữa ảnh
            label->show();  // Hiển thị QLabel

            // Sau khi xử lý xong, bạn có thể lưu ảnh
            if (outputImage.save("C:/Users/thanh/Desktop/DA/output_mean_image.jpg")) {
                qDebug() << "Image saved successfully.";
            } else {
                qDebug() << "Failed to save image.";
            }
        }
    }
}


QImage MainWindow::applyMeanFilter(const QImage& inputImage)
{
    // Tạo ảnh đầu ra sao chép từ ảnh đầu vào
    QImage outputImage = inputImage.copy();

    // Duyệt qua từng pixel (bỏ qua biên ảnh để tránh lỗi tràn chỉ số)
    for (int y = 1; y < inputImage.height() - 1; ++y) {
        for (int x = 1; x < inputImage.width() - 1; ++x) {
            int sumRed = 0, sumGreen = 0, sumBlue = 0;
            int count = 0;

            // Lọc các pixel trong khu vực 3x3
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    QRgb color = inputImage.pixel(x + dx, y + dy);
                    sumRed += qRed(color);
                    sumGreen += qGreen(color);
                    sumBlue += qBlue(color);
                    ++count;
                }
            }

            // Tính giá trị trung bình cho mỗi kênh màu
            int avgRed = sumRed / count;
            int avgGreen = sumGreen / count;
            int avgBlue = sumBlue / count;

            // Cập nhật giá trị pixel cho ảnh đầu ra
            outputImage.setPixel(x, y, qRgb(avgRed, avgGreen, avgBlue));
        }
    }

    return outputImage;
}






void MainWindow::on_actionMean_Filter_Color_triggered()
{
    // Đảm bảo bạn đã cung cấp đường dẫn đúng tới ảnh đầu vào
    QImage inputImage("C:/Users/thanh/Downloads/hinh.jpg");  // Đảm bảo đường dẫn đúng tới ảnh của bạn
    if (inputImage.isNull()) {
        qDebug() << "Failed to load input image.";
        return;
    }

    qDebug() << "Loaded image size: " << inputImage.size();

    // Áp dụng bộ lọc trung bình màu
    QImage outputImage = applyMeanFilterColor(inputImage);

    // Kiểm tra ảnh đầu ra có hợp lệ không
    if (outputImage.isNull()) {
        qDebug() << "Failed to process image.";
    } else {
        qDebug() << "Mean filter applied successfully!";
        qDebug() << "Output image is null: " << outputImage.isNull();
        qDebug() << "Output image size: " << outputImage.size();

        // Kiểm tra QPixmap từ QImage
        QPixmap pixmap = QPixmap::fromImage(outputImage);
        if (pixmap.isNull()) {
            qDebug() << "Failed to convert QImage to QPixmap.";
        } else {
            // Hiển thị ảnh đầu ra trong QLabel
            QLabel* label = new QLabel(this);  // Tạo QLabel mới
            label->setFixedSize(outputImage.size());  // Đảm bảo QLabel có kích thước phù hợp với ảnh
            label->setPixmap(pixmap);  // Gán ảnh vào QLabel
            label->setAlignment(Qt::AlignCenter);  // Căn giữa ảnh
            label->show();  // Hiển thị QLabel

            // Sau khi xử lý xong, bạn có thể lưu ảnh
            if (outputImage.save("C:/Users/thanh/Desktop/DA/output_mean_color_image.jpg")) {
                qDebug() << "Image saved successfully.";
            } else {
                qDebug() << "Failed to save image.";
            }
        }
    }
}

QImage MainWindow::applyMeanFilterColor(const QImage& inputImage)
{
    QImage outputImage = inputImage.copy();

    // Duyệt qua từng pixel (bỏ qua biên ảnh để tránh lỗi tràn chỉ số)
    for (int y = 1; y < inputImage.height() - 1; ++y) {
        for (int x = 1; x < inputImage.width() - 1; ++x) {
            int sumRed = 0, sumGreen = 0, sumBlue = 0;
            int count = 0;

            // Lọc các pixel trong khu vực 3x3
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    QRgb color = inputImage.pixel(x + dx, y + dy);
                    sumRed += qRed(color);
                    sumGreen += qGreen(color);
                    sumBlue += qBlue(color);
                    ++count;
                }
            }

            // Tính giá trị trung bình cho mỗi kênh màu
            int avgRed = sumRed / count;
            int avgGreen = sumGreen / count;
            int avgBlue = sumBlue / count;

            // Cập nhật giá trị pixel cho ảnh đầu ra
            outputImage.setPixel(x, y, qRgb(avgRed, avgGreen, avgBlue));
        }
    }

    return outputImage;
}



void MainWindow::on_actionMedian_Filter_Gray_triggered()
{
    // Đảm bảo bạn đã cung cấp đường dẫn đúng tới ảnh đầu vào
    QImage inputImage("C:/Users/thanh/Downloads/hinh.jpg");  // Đảm bảo đường dẫn đúng tới ảnh của bạn
    if (inputImage.isNull()) {
        qDebug() << "Failed to load input image.";
        return;
    }

    qDebug() << "Loaded image size: " << inputImage.size();

    // Áp dụng bộ lọc trung vị grayscale
    QImage outputImage = applyMedianFilterGray(inputImage);

    // Kiểm tra ảnh đầu ra có hợp lệ không
    if (outputImage.isNull()) {
        qDebug() << "Failed to process image.";
    } else {
        qDebug() << "Median filter applied successfully!";
        qDebug() << "Output image is null: " << outputImage.isNull();
        qDebug() << "Output image size: " << outputImage.size();

        // Kiểm tra QPixmap từ QImage
        QPixmap pixmap = QPixmap::fromImage(outputImage);
        if (pixmap.isNull()) {
            qDebug() << "Failed to convert QImage to QPixmap.";
        } else {
            // Hiển thị ảnh đầu ra trong QLabel
            QLabel* label = new QLabel(this);  // Tạo QLabel mới
            label->setFixedSize(outputImage.size());  // Đảm bảo QLabel có kích thước phù hợp với ảnh
            label->setPixmap(pixmap);  // Gán ảnh vào QLabel
            label->setAlignment(Qt::AlignCenter);  // Căn giữa ảnh
            label->show();  // Hiển thị QLabel

            // Sau khi xử lý xong, bạn có thể lưu ảnh
            if (outputImage.save("C:/Users/thanh/Desktop/DA/output_median_gray_image.jpg")) {
                qDebug() << "Image saved successfully.";
            } else {
                qDebug() << "Failed to save image.";
            }
        }
    }
}

QImage MainWindow::applyMedianFilterGray(const QImage& inputImage)
{
    QImage outputImage = inputImage.copy();

    // Duyệt qua từng pixel (bỏ qua biên ảnh để tránh lỗi tràn chỉ số)
    for (int y = 1; y < inputImage.height() - 1; ++y) {
        for (int x = 1; x < inputImage.width() - 1; ++x) {
            QVector<int> neighborhood;

            // Lọc các pixel trong khu vực 3x3
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    QRgb color = inputImage.pixel(x + dx, y + dy);
                    int gray = qGray(color);  // Chuyển đổi màu thành grayscale
                    neighborhood.append(gray);
                }
            }

            // Sắp xếp và chọn giá trị trung vị
            std::sort(neighborhood.begin(), neighborhood.end());
            int median = neighborhood[neighborhood.size() / 2];  // Lấy giá trị trung vị

            // Cập nhật giá trị pixel cho ảnh đầu ra
            outputImage.setPixel(x, y, qRgb(median, median, median));  // Lưu lại pixel dưới dạng grayscale
        }
    }

    return outputImage;
}




void MainWindow::on_actionMedian_Filter_Color_triggered()
{
    // Đảm bảo bạn đã cung cấp đường dẫn đúng tới ảnh đầu vào
    QImage inputImage("C:/Users/thanh/Downloads/hinh.jpg");  // Đảm bảo đường dẫn đúng tới ảnh của bạn
    if (inputImage.isNull()) {
        qDebug() << "Failed to load input image.";
        return;
    }

    // Áp dụng bộ lọc Median cho ảnh màu
    QImage outputImage = applyMedianFilterColor(inputImage);

    // Kiểm tra ảnh đầu ra có hợp lệ hay không
    if (outputImage.isNull()) {
        qDebug() << "Failed to process image.";
    } else {
        qDebug() << "Median filter applied successfully!";
        qDebug() << "Output image is null: " << outputImage.isNull();
        qDebug() << "Output image size: " << outputImage.size();

        // Kiểm tra QPixmap từ QImage
        QPixmap pixmap = QPixmap::fromImage(outputImage);
        if (pixmap.isNull()) {
            qDebug() << "Failed to convert QImage to QPixmap.";
        } else {
            // Hiển thị ảnh đầu ra trong QLabel
            QLabel* label = new QLabel(this);  // Tạo QLabel mới
            label->setFixedSize(outputImage.size());  // Đảm bảo QLabel có kích thước phù hợp với ảnh
            label->setPixmap(pixmap);  // Gán ảnh vào QLabel
            label->setAlignment(Qt::AlignCenter);  // Căn giữa ảnh
            label->show();  // Hiển thị QLabel

            // Sau khi xử lý xong, bạn có thể lưu ảnh
            if (outputImage.save("C:/Users/thanh/Desktop/DA/output_median_color_image.jpg")) {
                qDebug() << "Image saved successfully.";
            } else {
                qDebug() << "Failed to save image.";
            }
        }
    }
}

// Hàm áp dụng bộ lọc trung vị cho ảnh màu
QImage MainWindow::applyMedianFilterColor(const QImage& inputImage)
{
    QImage outputImage = inputImage.copy();


    for (int y = 1; y < inputImage.height() - 1; ++y) {
        for (int x = 1; x < inputImage.width() - 1; ++x) {
            QVector<int> redValues;
            QVector<int> greenValues;
            QVector<int> blueValues;

            // Lấy các pixel xung quanh pixel (x, y) trong cửa sổ kernelSize
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    QRgb color = inputImage.pixel(x + kx, y + ky);
                    redValues.append(qRed(color));
                    greenValues.append(qGreen(color));
                    blueValues.append(qBlue(color));
                }
            }

            // Tìm giá trị trung vị cho mỗi kênh màu
            std::sort(redValues.begin(), redValues.end());
            std::sort(greenValues.begin(), greenValues.end());
            std::sort(blueValues.begin(), blueValues.end());

            int medianRed = redValues[redValues.size() / 2];
            int medianGreen = greenValues[greenValues.size() / 2];
            int medianBlue = blueValues[blueValues.size() / 2];

            // Đặt giá trị pixel mới
            outputImage.setPixel(x, y, qRgb(medianRed, medianGreen, medianBlue));
        }
    }

    return outputImage;
}


void MainWindow::on_actionDilation_triggered()
{
    // Lấy cửa sổ hiện tại trong mdiArea
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if (activeWindow == 0) // Nếu không có cửa sổ đang mở
        return;

    // Lấy widget chứa ảnh
    ImageWidget *activeWidget = qobject_cast<ImageWidget *>(activeWindow->widget());
    if (!activeWidget)
        return;

    // Lấy ảnh gốc
    QImage image_in = activeWidget->getImage();

    // Kiểm tra xem ảnh có phải là grayscale hay không
    if (!image_in.isGrayscale()) {
        QMessageBox::warning(this, "Error", "Image must be in grayscale format for Dilation.");
        return; // Thoát khỏi hàm mà không thực hiện Dilation
    }

    // Thực hiện phép Dilation
    QImage image_out = applyDilation(image_in, 3); // Kernel size mặc định là 3x3

    // Hiển thị ảnh đã xử lý
    DisplayImage(image_out, activeWindow->windowTitle() + " - Dilation");
}


QImage MainWindow::applyDilation(const QImage &image_in, int kernelSize)
{
    QImage image_out = image_in.copy();
    int width = image_in.width();
    int height = image_in.height();

    // Kiểm tra xem ảnh có phải là grayscale (ảnh chỉ có 1 kênh màu)
    if (image_in.isGrayscale() == false) {
         QMessageBox::warning(this, "Cảnh báo", "Ảnh phải ở định dạng ảnh xám để thực hiện Erosion.");
        return image_out;
    }

    // Chuẩn bị kernel
    int halfKernel = kernelSize / 2;

    // Duyệt qua từng pixel trong ảnh
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            int maxValue = 0; // Giá trị tối đa trong vùng kernel

            // Duyệt qua kernel
            for (int i = -halfKernel; i <= halfKernel; ++i) {
                for (int j = -halfKernel; j <= halfKernel; ++j) {
                    int neighborX = x + i;
                    int neighborY = y + j;

                    // Kiểm tra pixel có nằm trong ảnh không
                    if (neighborX >= 0 && neighborX < width && neighborY >= 0 && neighborY < height) {
                        int pixelValue = qGray(image_in.pixel(neighborX, neighborY));
                        maxValue = qMax(maxValue, pixelValue);
                    }
                }
            }

            // Gán giá trị lớn nhất cho pixel hiện tại
            image_out.setPixel(x, y, qRgb(maxValue, maxValue, maxValue));
        }
    }

    return image_out;
}




void MainWindow::on_actionErosion_triggered()
{
    // Lấy cửa sổ hiện tại trong mdiArea
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if (activeWindow == 0) // Nếu không có cửa sổ nào đang mở
        return;

    // Lấy widget chứa ảnh
    ImageWidget *activeWidget = qobject_cast<ImageWidget *>(activeWindow->widget());
    if (!activeWidget)
        return;

    // Lấy ảnh gốc
    QImage image_in = activeWidget->getImage();

    // Kiểm tra ảnh có phải là grayscale không
    if (!isGrayscale(image_in)) {
        QMessageBox::warning(this, "Cảnh báo", "Ảnh phải ở định dạng ảnh xám để thực hiện Erosion.");
        return;
    }

    // Áp dụng phép Erosion
    QImage image_out = applyErosion(image_in);

    // Hiển thị ảnh kết quả
    DisplayImage(image_out, activeWindow->windowTitle() + " - Erosion");
}

// Kiểm tra xem ảnh có phải là grayscale không
bool MainWindow::isGrayscale(const QImage &image)
{
    for (int x = 0; x < image.width(); ++x) {
        for (int y = 0; y < image.height(); ++y) {
            QColor color(image.pixel(x, y));
            if (color.red() != color.green() || color.green() != color.blue()) {
                return false;  // Nếu một điểm ảnh có các kênh màu khác nhau, ảnh không phải grayscale
            }
        }
    }
    return true;
}

QImage MainWindow::applyErosion(const QImage &image_in, int kernelSize)
{
    if (kernelSize < 1) kernelSize = 3; // Kích thước kernel mặc định là 3x3

    QImage image_out(image_in.size(), QImage::Format_RGB888);

    int halfKernel = kernelSize / 2;
    for (int x = 0; x < image_in.width(); ++x) {
        for (int y = 0; y < image_in.height(); ++y) {
            int minGray = 255; // Giá trị xám nhỏ nhất

            // Duyệt qua kernel
            for (int i = -halfKernel; i <= halfKernel; ++i) {
                for (int j = -halfKernel; j <= halfKernel; ++j) {
                    int nx = x + i;
                    int ny = y + j;

                    // Kiểm tra điểm ảnh có nằm trong phạm vi không
                    if (nx >= 0 && nx < image_in.width() && ny >= 0 && ny < image_in.height()) {
                        int gray = qGray(image_in.pixel(nx, ny));
                        if (gray < minGray) {
                            minGray = gray;
                        }
                    }
                }
            }

            // Gán giá trị xám nhỏ nhất cho điểm ảnh kết quả
            image_out.setPixel(x, y, qRgb(minGray, minGray, minGray));
        }
    }

    return image_out;
}







void MainWindow::on_actionNOT_triggered()
{
    // Lấy cửa sổ hiện tại trong mdiArea
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if (activeWindow == 0) // Nếu không có cửa sổ đang mở
        return;

    // Lấy widget chứa ảnh
    ImageWidget *activeWidget = qobject_cast<ImageWidget *>(activeWindow->widget());
    if (!activeWidget)
        return;

    // Lấy ảnh gốc
    QImage image_in = activeWidget->getImage();

    // Kiểm tra xem ảnh có phải là grayscale hay không
    if (image_in.isGrayscale()) {  // Kiểm tra nếu ảnh là grayscale
        // Thực hiện phép toán phủ (NOT)
        QImage image_out = applyNOT(image_in);

        // Hiển thị ảnh đã xử lý
        DisplayImage(image_out, activeWindow->windowTitle() + " - NOT Operation");
    } else {
        QMessageBox::warning(this, "Lỗi", "Ảnh phải ở định dạng ảnh xám.");
    }
}

QImage MainWindow::applyNOT(const QImage &image_in)
{
    QImage image_out = image_in.copy();  // Tạo bản sao của ảnh đầu vào

    for (int x = 0; x < image_in.width(); ++x) {
        for (int y = 0; y < image_in.height(); ++y) {
            int pixel = image_in.pixel(x, y);
            int gray = qGray(pixel);  // Lấy giá trị gray của pixel
            int inverted_gray = 255 - gray;  // Phủ (NOT)
            image_out.setPixel(x, y, qRgb(inverted_gray, inverted_gray, inverted_gray));  // Cập nhật pixel
        }
    }

    return image_out;
}


void MainWindow::on_actionAdd_triggered()
{
    // Lấy cửa sổ hiện tại trong mdiArea
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if (activeWindow == 0) // Nếu không có cửa sổ đang mở
        return;

    // Lấy widget chứa ảnh
    ImageWidget *activeWidget = qobject_cast<ImageWidget *>(activeWindow->widget());
    if (!activeWidget)
        return;

    // Lấy ảnh gốc
    QImage image_in1 = activeWidget->getImage();

    // Lấy ảnh thứ hai (ví dụ có thể từ một cửa sổ khác hoặc ảnh đã mở)
    QMdiSubWindow *anotherWindow = mdiArea->activeSubWindow(); // hoặc từ một cửa sổ khác
    if (anotherWindow == 0)
        return;

    ImageWidget *anotherWidget = qobject_cast<ImageWidget *>(anotherWindow->widget());
    if (!anotherWidget)
        return;

    QImage image_in2 = anotherWidget->getImage();

    // Kiểm tra kích thước của hai ảnh có cùng kích thước không
    if (image_in1.size() != image_in2.size()) {
        QMessageBox::warning(this, "Lỗi", "Hai ảnh phải có cùng kích thước.");
        return;
    }

    // Thực hiện phép cộng ảnh
    QImage image_out = addImages(image_in1, image_in2);

    // Hiển thị ảnh đã cộng
    DisplayImage(image_out, activeWindow->windowTitle() + " - Add Images");
}

QImage MainWindow::addImages(const QImage &image_in1, const QImage &image_in2)
{
    // Kiểm tra nếu hai ảnh có kích thước khác nhau, trả về ảnh gốc
    if (image_in1.size() != image_in2.size())
        return QImage(); // Trả về ảnh rỗng nếu kích thước không khớp

    QImage result(image_in1.size(), QImage::Format_RGB32); // Định dạng RGB32 để dễ thao tác với pixel

    // Thực hiện phép cộng pixel của 2 ảnh
    for (int x = 0; x < image_in1.width(); ++x) {
        for (int y = 0; y < image_in1.height(); ++y) {
            // Lấy giá trị màu của 2 ảnh
            QColor color1(image_in1.pixel(x, y));
            QColor color2(image_in2.pixel(x, y));

            // Cộng giá trị màu của từng kênh
            int r = qMin(color1.red() + color2.red(), 255);
            int g = qMin(color1.green() + color2.green(), 255);
            int b = qMin(color1.blue() + color2.blue(), 255);

            // Đặt giá trị mới cho pixel trong ảnh kết quả
            result.setPixel(x, y, qRgb(r, g, b));
        }
    }

    return result;
}


void MainWindow::on_actionSubtract_triggered()
{
    qDebug() << "Subtract action triggered!";  // Kiểm tra xem slot có được gọi không
    // Lấy cửa sổ hiện tại trong mdiArea
    QMdiSubWindow *activeWindow = mdiArea->currentSubWindow();
    if (activeWindow == 0) {
        return;  // Nếu không có cửa sổ nào đang mở
    }

    // Lấy widget chứa ảnh
    ImageWidget *activeWidget = qobject_cast<ImageWidget *>(activeWindow->widget());
    if (!activeWidget) {
        return;  // Nếu không phải widget ảnh
    }

    // Lấy ảnh gốc
    QImage image_in1 = activeWidget->getImage();
    QImage image_in2;  // Đây là ảnh thứ hai, có thể lấy từ cửa sổ hoặc từ một nơi khác

    // Kiểm tra nếu ảnh thứ hai không có
    if (image_in2.isNull()) {
        return;  // Nếu không có ảnh thứ hai
    }

    // Thực hiện phép trừ ảnh
    QImage image_out = subtractImages(image_in1, image_in2);

    // Hiển thị ảnh đã trừ
    DisplayImage(image_out, activeWindow->windowTitle() + " - Subtracted Image");
}

QImage MainWindow::subtractImages(const QImage &image_in1, const QImage &image_in2)
{
    if (image_in1.size() != image_in2.size()) {
        QMessageBox::warning(this, "Error", "The images must have the same size.");
        return QImage();  // Trả về ảnh rỗng nếu ảnh không cùng kích thước
    }

    QImage result(image_in1.size(), QImage::Format_RGB888);

    for (int y = 0; y < image_in1.height(); ++y) {
        for (int x = 0; x < image_in1.width(); ++x) {
            QColor color1(image_in1.pixel(x, y));
            QColor color2(image_in2.pixel(x, y));

            int r = qMax(0, color1.red() - color2.red());
            int g = qMax(0, color1.green() - color2.green());
            int b = qMax(0, color1.blue() - color2.blue());

            result.setPixel(x, y, qRgb(r, g, b));
        }
    }

    return result;
}






