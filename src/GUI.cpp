
#include "include/GUI.h"

Mat create_img_with_text(_image img){
    Mat img_with_text;
    cvtColor(img.original_img, img_with_text, CV_GRAY2BGR);
    putText(img_with_text, "ID: " + to_string(img.id) + " " + img.texture_class,                         //" (" + img.file_name + ")"
            cvPoint(45,img.original_img.size().height - 5),
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
    waitKey(0);
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

void result_mode_add_text(Mat &dst, bool success){
    if(success){
        putText(dst, "SUCCESS", cvPoint(dst.size().width / 2 - 20, 10),
                CV_FONT_NORMAL, 0.3, cvScalar(0,255,0), 1, CV_AA);
    }else{
        putText(dst, "MISTAKE", cvPoint(dst.size().width / 2 - 20, 10),
                CV_FONT_NORMAL, 0.3, cvScalar(0,0,255), 1, CV_AA);
    }
}

void result_mode_show_two_images(_image im_a, _image im_b, bool success){
    Mat a = create_img_with_text(im_a);
    Mat b = create_img_with_text(im_b);
    Mat dst; hconcat(a, b, dst);
    result_mode_add_text(dst, success);

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






