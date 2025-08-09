#include <iostream>
#include "libppm.h"
#include <cstdint>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

int runningWindowSize = 5;
int t = (runningWindowSize-1)/2;
/// 
struct image_t* S1_smoothen(struct image_t *input_image)
{
	// TODO
	// remember to allocate space for smoothened_image. See read_ppm_file() in libppm.c for some help.
	
	int height = input_image->height;
	int width = input_image->width;
	//cout<<input_height<<" "<<input_width;
	struct image_t* image = new struct image_t;
	image->height = height;
	image->width = width;
	image->image_pixels = new uint8_t**[image->height];
		for(int i = 0; i < image->height; i++)
		{
			image->image_pixels[i] = new uint8_t*[image->width];
			for(int j = 0; j < image->width; j++)
				image->image_pixels[i][j] = new uint8_t[3];
		}
		
		// maxval	
		// get pixel values
		for(int i = 0; i < image->height; i++)
		{
			for(int j = 0; j < image->width; j++)
			{
				for(int k = 0; k < 3; k++)
				{
					int temp = 0;
					uint8_t uint_temp = temp;
					image->image_pixels[i][j][k] = uint_temp;
				}
			}
		}



	for(int i =0; i< input_image->height; i++){
		for(int j = 0; j< input_image->width; j++){
			int r = 0;
			int g = 0;																																																
			int b = 0;
			for(int ii = i-t; ii <= i+t; ii++){
				for(int jj = j-t; jj<=j+t; jj++){
					if(ii<0 || ii>= input_image->height|| jj<0 || jj>=input_image->width) continue;
					int t_r = input_image->image_pixels[ii][jj][0];
					int t_b = input_image->image_pixels[ii][jj][1];
					int t_g = input_image->image_pixels[ii][jj][2];
					r += t_r;
					g += t_g;
					b += t_b;
				}
			}
			image->image_pixels[i][j][0] = (uint8_t)(r/(runningWindowSize*runningWindowSize));
			image->image_pixels[i][j][1] = (uint8_t)(g/(runningWindowSize*runningWindowSize));
			image->image_pixels[i][j][2] = (uint8_t)(b/(runningWindowSize*runningWindowSize));
		}
	}
	return image;
}

struct image_t* S2_find_details(struct image_t *input_image, struct image_t *smoothened_image)
{
	struct image_t* detailed_image = new struct image_t;
	int height = input_image->height;
	int width = input_image->width;
	detailed_image->height = height;
	detailed_image->width = width;
	detailed_image->image_pixels = new uint8_t**[detailed_image->height];
		for(int i = 0; i < detailed_image->height; i++)
		{
			detailed_image->image_pixels[i] = new uint8_t*[detailed_image->width];
			for(int j = 0; j < detailed_image->width; j++)
				detailed_image->image_pixels[i][j] = new uint8_t[3];
		}
		
		for(int i = 0; i < detailed_image->height; i++)
		{
			for(int j = 0; j < detailed_image->width; j++)
			{
				for(int k = 0; k < 3; k++)
				{
					int r_s = smoothened_image->image_pixels[i][j][0];
					int g_s = smoothened_image->image_pixels[i][j][1];
					int b_s = smoothened_image->image_pixels[i][j][2];

					int r_i = input_image->image_pixels[i][j][0];
					int g_i = input_image->image_pixels[i][j][1];
					int b_i = input_image->image_pixels[i][j][2];

					int r_d = r_i - r_s;
					int g_d = g_i - g_s;
					int b_d = b_i - b_s;
					if(r_d<0) r_d = 0;
					if(g_d<0) g_d = 0;
					if(b_d<0) b_d = 0;

					if(r_d>255) r_d = 255;
					if(g_d>255) g_d = 255;
					if(b_d>255) b_d = 255;

					detailed_image->image_pixels[i][j][0] = (uint8_t)r_d;
					detailed_image->image_pixels[i][j][1] = (uint8_t)g_d;
					detailed_image->image_pixels[i][j][2] = (uint8_t)b_d;
				}
			}
		}
	return detailed_image;
}

struct image_t* S3_sharpen(struct image_t *input_image, struct image_t *details_image)
{
	int alpha = 2;
	struct image_t* sharpened_image = new struct image_t;
	int height = input_image->height;
	int width = input_image->width;
	sharpened_image->height = height;
	sharpened_image->width = width;
	sharpened_image->image_pixels = new uint8_t**[sharpened_image->height];
		for(int i = 0; i < sharpened_image->height; i++)
		{
			sharpened_image->image_pixels[i] = new uint8_t*[sharpened_image->width];
			for(int j = 0; j < sharpened_image->width; j++)
				sharpened_image->image_pixels[i][j] = new uint8_t[3];
		}
		
		// maxval	
		// get pixel values
		for(int i = 0; i < sharpened_image->height; i++)
		{
			for(int j = 0; j < sharpened_image->width; j++)
			{
				for(int k = 0; k < 3; k++)
				{
					int r_s = details_image->image_pixels[i][j][0];
					int g_s = details_image->image_pixels[i][j][1];
					int b_s = details_image->image_pixels[i][j][2];

					int r_i = input_image->image_pixels[i][j][0];
					int g_i = input_image->image_pixels[i][j][1];
					int b_i = input_image->image_pixels[i][j][2];

					int r_d = r_i + (alpha)*r_s;
					int g_d = g_i + (alpha)*g_s;
					int b_d = b_i + (alpha)*b_s;

					if(r_d>255) r_d = 255;
					if(g_d>255) g_d = 255;
					if(b_d>255) b_d = 255;


					sharpened_image->image_pixels[i][j][0] = (uint8_t)r_d;
					sharpened_image->image_pixels[i][j][1] =(uint8_t)g_d;
					sharpened_image->image_pixels[i][j][2] = (uint8_t)b_d;
				}
			}
		}
	return sharpened_image;
}

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		cout << "usage: ./a.out <path-to-original-image> <path-to-transformed-image>\n\n";
		//exit(0);
	}
	
	struct image_t *input_image = read_ppm_file(argv[1]);
	cout<<"Smoothening image..."<<endl;
	struct image_t *smoothened_image = S1_smoothen(input_image);
	cout<<"Smooth image created..."<<endl;
	cout<<"Finding details..."<<endl;
	struct image_t *details_image = S2_find_details(input_image, smoothened_image);
	cout<<"Details found..."<<endl;
	cout<<"Adding details..."<<endl;
	struct image_t *sharpened_image = S3_sharpen(input_image, details_image);
	//cout<<"Completed!"<<endl;
	write_ppm_file(argv[2], sharpened_image);
	return 0;
}
