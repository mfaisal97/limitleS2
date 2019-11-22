#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <highgui.h>
#include "../common/DataStructures.cpp"

using namespace std;
using namespace cv;

bool isBitSet(char ch, int pos) {
	// 7 6 5 4 3 2 1 0
	ch = ch >> pos;
	if(ch & 1)
		return true;
	return false;
}

//returns the content of the resulting image
string Encode(string text, string outImageName, string inImageName = DefaultImagePath){
	string content = "";
	string outImageFullPath = "Data/" + outImageName;

	text = text + "\0";
	Mat image = imread(inImageName);
	if(image.empty()) {
		cout << "Image Error\n";
		exit(-1);
	}

	// char to work on
	int ind = 0;
	char ch;
	// reads the first char from the file
	ch = text[ind++];
	// contains information about which bit of char to work on
	int bit_count = 0;
	// to check whether file has ended
	bool last_null_char = false;
	// to check if the whole message is encoded or not
	bool encoded = false;

	for(int row=0; row < image.rows; row++) {
		for(int col=0; col < image.cols; col++) {
			for(int color=0; color < 3; color++) {

				Vec3b pixel = image.at<Vec3b>(Point(row,col));

				if(isBitSet(ch,7-bit_count))
					pixel.val[color] |= 1;
				else
					pixel.val[color] &= ~1;

				image.at<Vec3b>(Point(row,col)) = pixel;

				bit_count++;

				if(last_null_char && bit_count == 8) {
					encoded  = true;
					goto OUT;
				}

				if(bit_count == 8) {
					bit_count = 0;
					ch = text[ind++];

					if(ind == text.size()-1) {
						last_null_char = true;
					}
					ch = '\0';
				}

			}
		}
	}
	OUT:;

	//future bug project :D
	// whole message was not encoded
	if(!encoded) {
		cout << "Message too big. Try with larger image.\n";
		exit(-1);
	}

	imwrite(outImageFullPath,image);

	//fake reading the image
	ifstream rf(outImageFullPath, ios::binary);
	if (rf){
		// get length of file:
		rf.seekg (0, is.end);
		int length = is.tellg();
		rf.seekg (0, is.beg);

		char * buffer = new char [length];
		rf.read (buffer,length);
		content = FromCharArray(buffer);

		delete[] buffer;
		fr.close();
	}else {
		cout << "Cannot open Fake \"" + outImageName + "\" for reading" << endl;
	}

	return content;
}
