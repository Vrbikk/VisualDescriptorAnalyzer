//
// Created by vrbik on 21.9.16.
//
#include "include/GUI.h"

Mat create_img_with_text(_image img){
    Mat img_with_text;
    cvtColor(img.original_img, img_with_text, CV_GRAY2BGR);
    putText(img_with_text, "ID: " + to_string(img.id) + " (" + img.file_name + ")",
            cvPoint(15,img.original_img.size().height - 5),
            CV_FONT_NORMAL, 0.37, cvScalar(255,10,0), 1, CV_AA);

    return img_with_text;
}

void show_image(Mat a){
    Mat view_a;
    convert_CV32SC1_to_CV8U(a, view_a);
    namedWindow("Display image", WINDOW_NORMAL);
    resizeWindow("Display image", 3 * a.size().width, 3 * a.size().height);
    imshow("Display image", view_a);
    waitKey(0);
}

void show_image(_image im_a){
    Mat a = create_img_with_text(im_a);
    namedWindow("Display _image", WINDOW_NORMAL);
    resizeWindow("Display _image", 2 * a.size().width, 2 * a.size().height);
    imshow("Display _image", a);
}

void show_two_images(Mat a, Mat b){

    Mat view_a;
    Mat view_b;
    convert_CV32SC1_to_CV8U(a, view_a);
    convert_CV32SC1_to_CV8U(b, view_b);

    Mat dst;
    hconcat(view_a, view_b, dst);

    namedWindow("Two Images", WINDOW_NORMAL);
    resizeWindow("Two Images", 2 * dst.size().width, 2 * dst.size().height);
    imshow("Two Images", dst);

    waitKey(0);
}

void show_two_images(_image im_a, _image im_b){

    Mat a = create_img_with_text(im_a);
    Mat b = create_img_with_text(im_b);
    Mat dst;
    hconcat(a, b, dst);

    namedWindow("Two Images", WINDOW_NORMAL);
    resizeWindow("Two Images", 2 * dst.size().width, 2 * dst.size().height);
    imshow("Two Images", dst);
}

void draw_progress_to_console(string name, int actual, int size) {
    if(actual == size){
        cout << ">>> " + name + " status: " << actual << "/" << size << endl;
    }else{
        cout << ">>> " + name + " status: " << actual << "/" << size << "\r" << flush;
    }
}

void csi_add_text(Mat &dst, bool success){
    if(success){
        putText(dst, "SUCCESS", cvPoint(dst.size().width / 2 - 20, 10),
                CV_FONT_NORMAL, 0.3, cvScalar(0,255,0), 1, CV_AA);
    }else{
        putText(dst, "MISTAKE", cvPoint(dst.size().width / 2 - 20, 10),
                CV_FONT_NORMAL, 0.3, cvScalar(0,0,255), 1, CV_AA);
    }
}

void csi_show_two_images(_image im_a, _image im_b, bool success){
    Mat a = create_img_with_text(im_a);
    Mat b = create_img_with_text(im_b);
    Mat dst; hconcat(a, b, dst);
    csi_add_text(dst, success);

    namedWindow("Result mode", WINDOW_NORMAL);
    resizeWindow("Result mode", 2 * dst.size().width, 2 * dst.size().height);
    imshow("Result mode", dst);
}

void convert_CV32F_to_CV8U(Mat &src, Mat &dst) {
    src.convertTo(dst, CV_8U, 255, 0 );
}

void convert_CV32SC1_to_CV8U(Mat &src, Mat &dst) {
    double Max, Min;
    minMaxLoc(src, &Min, &Max);
    Mat view_a;
    src.convertTo(dst,CV_8U,255.0/(Max-Min),-255.0*Min/(Max-Min));
}

string gabor_editor = "Gabor filter editor";
int kernel_size=10;
int pos_sigma= 5;
int pos_lambda = 50;
int pos_theta = 20;
int pos_gamma= 0;
int pos_psi = 90;
int min_black_value = 10;

Mat gabor_src;
Mat dest;

void Process(int , void *)
{
    if(kernel_size == 0){
        kernel_size = 1;
    }

    Size KernalSize(kernel_size,kernel_size);
    double Sigma = pos_sigma;
    double Lambda = 0.5 + pos_lambda / 100.0;
    double Theta = pos_theta * CV_PI / 180;
    double Psi = pos_psi * CV_PI / 180;;
    double Gamma = pos_gamma;

    Mat kernel = getGaborKernel(KernalSize, Sigma, Theta, Lambda, Gamma, Psi);
    filter2D(gabor_src, dest, CV_8U, kernel);

    Mat black(gabor_src.size(), CV_8U);

    for(int x = 0; x < black.cols; x++){
        for(int y = 0; y < black.rows; y++){

            unsigned char val = dest.at<unsigned char>(x, y);

            if(val < min_black_value){
                black.at<unsigned char>(x, y) = 0;
            }else{
                black.at<unsigned char>(x, y) = 255;
            }
        }
    }

    vector<Mat> matrices = {
            gabor_src,
            dest,
            black,
    };

    Mat target;
    hconcat(matrices, target);
    imshow(gabor_editor, target);

    /*Mat Lkernel(kernel_size*10, kernel_size*10, CV_32F);
    resize(kernel, Lkernel, Lkernel.size());
    Lkernel /= 2.;
    Lkernel += 0.5;
    imshow("Kernel", Lkernel);*/
}

void Gabor_editor(Mat &src) {
    gabor_src = src;
    cv::namedWindow(gabor_editor, 1);

    cv::createTrackbar("Kernel size (def. 10)", gabor_editor, &kernel_size, 50, Process);
    cv::createTrackbar("Sigma", gabor_editor, &pos_sigma, 50, Process);
    cv::createTrackbar("Lambda", gabor_editor, &pos_lambda, 100, Process);
    cv::createTrackbar("Theta", gabor_editor, &pos_theta, 180, Process);
    cv::createTrackbar("Psi", gabor_editor, &pos_psi, 360, Process);
    cv::createTrackbar("Gamma", gabor_editor, &pos_gamma, 100, Process);
    cv::createTrackbar("Min Black Value", gabor_editor, &min_black_value, 255, Process);

    Process(0,0);
    waitKey(0);
}






