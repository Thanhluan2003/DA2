#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMdiArea>
#include <QMainWindow>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QMdiArea *mdiArea;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QImage currentImage;
    QImage addImages(const QImage &image_in1, const QImage &image_in2); // Khai báo hàm addImages
    QImage applyErosion(const QImage &image_in, int kernelSize = 3);
    bool isGrayscale(const QImage &image); // Thêm khai báo của hàm isGrayscale
    QImage applyGradientFilter(const QImage& inputImage);
    void DisplayImage (QImage &img, QString title);
    void Histogram_GrayImage(const QImage &img, const int &his_height);
    void Histogram_ColorImage(const QImage &img, const int his_height);
private slots:
    void on_bt_Browse_clicked();

    void on_bt_HisSlide_Gray_clicked();

    void on_bt_HisSlide_Color_clicked();

    void on_bt_Contrast_Gray_clicked();

    void on_bt_Contrast_Color_clicked();

    void on_bt_Threshold_Gray_clicked();

    void on_bt_Sequential_Threshold_Gray_clicked();

    void on_bt_Otsu_Threshold_Gray_clicked();

    void on_bt_Histogram_Gray_clicked();

    void on_bt_Histogram_Color_clicked();

    void on_bt_BienDoiTuyenTinh_clicked();

    void on_bt_CanBangHistogram_clicked();

    void on_bt_Negative_Gray_clicked();

    void on_bt_Negative_Color_clicked();

    void on_bt_loctrungbinh_clicked();

    void on_bt_loctrungvi_clicked();

    void on_bt_themnhieumuoitieu_clicked();

    void on_bt_nhieumuoitieuanhmau_clicked();

    void on_bt_LocGradient_AnhXam_clicked();

    void on_bt_LocGradient_AnhMau_clicked();

    void on_pushButton_23_clicked();

    void on_bt_LocRobert_AnhMau_clicked();

    void on_pushButton_24_clicked();

    void on_bt_LocSobel_AnhMau_clicked();

    void on_pushButton_25_clicked();

    void on_bt_LocPrewitt_AnhMau_2_clicked();

    void on_pushButton_26_clicked();

    void on_bt_LocLaplace_AnhMau_3_clicked();

    void on_bt_AnMon_clicked();

    void on_bt_GianNo_clicked();

    void on_bt_MoAnh_clicked();

    void on_bt_close_clicked();





    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionExit_triggered();

    void on_actionShow_Histogram_Gray_Image_triggered();
    void on_actionShow_Histogram_Color_Image_triggered();




    void on_actionHistogram_Side_triggered();

    void on_actionHistogram_Stretch_triggered();
    void on_actionHistogram_Equalization_triggered();



     void on_actionLinear_Modification_triggered();


    void on_actionGradient_Filter_triggered();  // Thêm dòng này
     void on_actionRobert_Filter_triggered();

    void on_actionMean_Filter_Gray_triggered();

     void on_actionMean_Filter_Color_triggered();

    void on_actionMeidan_Filter_Gray_triggered();

    void on_actionMedianFilter_triggered();
    void on_actionGaussianFilter_triggered();
    void on_actionSaveImage_triggered();

    void on_actionMedian_Filter_Color_triggered();

    void on_actionDilation_triggered();

    void on_actionErosion_triggered();

    void on_actionMedian_Filter_Gray_triggered();

    void on_actionClose_triggered();

    void on_actionNOT_triggered();

    void on_actionAdd_triggered();

    void on_actionSubtract_triggered();
    void on_actionNhi_u_nh_x_m_triggered();  // Hàm xử lý nhiễu
    void on_actionOpenImage_triggered();  // Hàm mở ảnh



    void on_imageLabel_triggered();

private:
    Ui::MainWindow *ui;
    QImage draw_His_Gray(QImage &image_in);
    QImage draw_His_Color(QImage &image_in);
    QImage histogramStretch(const QImage &image_in);
    QImage histogramEqualization(const QImage &image_in);
    QImage linearModification(const QImage &image_in, double alpha, int beta);
    QImage applyRobertFilter(const QImage& inputImage);  // Khai báo hàm applyRobertFilter
    QImage applyMeanFilter(const QImage& inputImage);  // Khai báo hàm applyMeanFilter
    QImage applyMeanFilterColor(const QImage& inputImage);  // Khai báo hàm ở đây
    QImage applyMedianFilterGray(const QImage& inputImage);  // Khai báo hàm xử lý lọc trung vị
    QImage applyDilation(const QImage& inputImage, int kernelSize = 3);
    QImage applyMedianFilterColor(const QImage& inputImage);
    QAction *actionMedian_Filter_Color;
    QImage applyNOT(const QImage &image_in);  // Khai báo hàm applyNOT
    QImage subtractImages(const QImage &image_in1, const QImage &image_in2);  // Hàm trừ ảnh
    void saveImage(const QImage &image, const QString &fileName);
    QImage applyMedianFilter(const QImage& inputImage);
    QImage applyGaussianFilter(const QImage& inputImage);
    void DisplayImage(const QImage &image, const QString &title); // Hàm hiển thị ảnh
};

#endif // MAINWINDOW_H
