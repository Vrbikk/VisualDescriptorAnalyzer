//
// Created by vrbik on 21.9.16.
//

#ifndef PLAYGROUND_GUI_H
#define PLAYGROUND_GUI_H

#include "stdim.h"

// adding information text to image
Mat create_img_with_text(_image img);

// converting functions
void convert_CV32F_to_CV8U(Mat &src, Mat &dst);
void convert_CV32SC1_to_CV8U(Mat &src, Mat &dst);

// GABOR EDITOR
void Gabor_editor(Mat &src);

// functions for debugging
void show_image(Mat a);
void show_image(_image im_a);
void show_two_images(_image im_a, _image im_b);
void show_two_images(Mat a, Mat b);

// function for progress in console
void draw_progress_to_console(string name, int actual, int size);

// RESULT MODE
void result_mode_show_two_images(_image im_a, _image im_b, bool success);
void result_mode_add_text(Mat &dst, bool success);

#endif
