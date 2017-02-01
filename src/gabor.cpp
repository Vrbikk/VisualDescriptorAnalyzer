//
// Created by vrbik on 23.1.17.
//

#include "include/gabor.h"

void gabor(vector<_image> &train, vector<_image> &test) {

    if(!CONFIG->getJobMode()){
        LOGGER->Info("Applying gabor filters");
    }

    int i = 0;
    for(auto &a : train){
        assign_points(a, CONFIG->getGaborPoints(), CONFIG->getGaborBorderSize());
        draw_progress_to_console("Training data gabor filters", i++ + 1, (int)train.size());
    }
    i = 0;
    for(auto &a : test){
        assign_points(a, CONFIG->getGaborPoints(), CONFIG->getGaborBorderSize());
        draw_progress_to_console("Testing data gabor filters", i++ + 1, (int)test.size());
    }
}

void assign_points(_image &img, int count, int border) {

    img.points.clear();

    // initial filter configuration
    int TMPkernel_size=13;
    int TMPpos_sigma= 5;
    int TMPpos_lambda = 44;
    int TMPpos_theta = 15;
    int TMPpos_gamma= 3;
    int TMPpos_psi = 104;

    Size KernalSize(TMPkernel_size,TMPkernel_size);
    double Sigma = TMPpos_sigma;
    double Lambda = 0.5 + TMPpos_lambda / 100.0;
    double Theta = TMPpos_theta * CV_PI / 180;
    double Psi = TMPpos_psi * CV_PI / 180;;
    double Gamma = TMPpos_gamma;

    //Mat equalized;
    //equalizeHist(img.original_img , equalized);

    Mat kernel = getGaborKernel(KernalSize, Sigma, Theta, Lambda, Gamma, Psi);
    filter2D(img.original_img, img.gabor_filter, CV_8U, kernel);
    img.points_from_gabor = Mat(img.gabor_filter.size(), CV_8U);
    img.points_from_gabor = Scalar(0);

    std::multimap<unsigned int, Point> points;

    for(int x = border; x < img.gabor_filter.cols - (border + 3); x+=3){
        for(int y = border; y < img.gabor_filter.rows - (border + 3); y+=3) {

            unsigned int sum = 0;
            for (int i = x; i < x + 3; i++) {
                for (int j = y; j < y + 3; j++) {
                    sum += img.gabor_filter.at <unsigned char> (i, j);
                }
            }

            if(sum > 5){
                points.insert(make_pair(sum, Point(x+1, y+1)));
            }
        }
    }

    std::multimap<unsigned int, Point>::reverse_iterator rit;
    int i = 0;
    for (rit=points.rbegin(); rit!=points.rend() && i++ < count; ++rit){
        img.points_from_gabor.at < unsigned char > (rit->second.x,rit->second.y) = 255;
        img.points.push_back(rit->second);
    }

    //show_image(img.points_from_gabor);

}
