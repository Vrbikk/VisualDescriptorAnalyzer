//
// Created by vrbik on 21.9.16.
//

#ifndef PLAYGROUND_GUI_H
#define PLAYGROUND_GUI_H

#include "stdim.h"

Mat create_img_with_text(_image img);
void convert_CV32F_to_CV8U(Mat &src, Mat &dst);
void convert_CV32SC1_to_CV8U(Mat &src, Mat &dst);
void Gabor_editor(Mat &src);
void show_image(Mat a);
void show_image(_image im_a);
void show_two_images(_image im_a, _image im_b);
void show_two_images(Mat a, Mat b);
void draw_progress_to_console(string name, int actual, int size);
void csi_show_two_images(_image im_a, _image im_b, bool success);
void csi_add_text(Mat &dst, bool success);

#endif //PLAYGROUND_GUI_H
