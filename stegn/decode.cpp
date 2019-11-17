#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

bool isBitSet(char ch, int pos) {
	ch = ch >> pos;
	if(ch & 1)
		return true;
	return false;
}

string Decode(string inImage){
	string str;

	Mat image = imread(inImage);
	if(image.empty()) {
		cout << "Image Error\n";
		exit(-1);
	}

	char ch=0;
	int bit_count = 0;

	for(int row=0; row < image.rows; row++) {
		for(int col=0; col < image.cols; col++) {
			for(int color=0; color < 3; color++) {

				Vec3b pixel = image.at<Vec3b>(Point(row,col));

				if(isBitSet(pixel.val[color],0))
					ch |= 1;

				bit_count++;

				if(bit_count == 8) {

					if(ch == '\0')
						goto OUT;

					bit_count = 0;
					str.append(ch);
					ch = 0;
				}
				else {
					ch = ch << 1;
				}

			}
		}
	}
	OUT:;
}
