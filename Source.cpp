#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;

class Image {
public:
	Image() {
	};
	~Image() {
	};

	void menu(Image& img) {																// menu function
		ifstream inFile;
		ofstream outFile;
		string inName, outName, allName;
		bool end = true;

		cout << "Portable Pixmap (PPM) Image Stacker by Jay Ivan Gentallan" << endl << endl;
		cout << "Enter image file name: ";
		cin >> inName;														// gets input for inName
		inFile.open(inName + ".ppm");

		if (!inFile) {
			cout << "File not found! Please try again!" << endl << endl;
			menu(img);
		}

		inFile >> img;
		set(img);
		ascii(img);
		inFile.close();

		cout << "Enter output file name: ";
		cin >> outName;

		outFile.open(outName + ".text");										// creates outFile
		outFile << img;														// file data is passed through class ostream& operator and put into outFile
		outFile.close();													// closes file

		cout << endl << "Press any key to continue." << endl;
		cin.ignore();
		cin.get();

		delete[] img.colors;
		delete[] img.colorsAll;
		delete[] asciiVal;
		img.colors = nullptr;
		img.colorsAll = nullptr;	
		img.asciiVal = nullptr;
	};

	void set(Image &img) {
		int *reset, *resetAll;
		int size = img.width * img.height;

		reset = img.colors;
		resetAll = img.colorsAll;
		for (int i = 0; i < size; i++) {
			*img.colorsAll = *img.colors;
			img.colors++;
			*img.colorsAll += *img.colors;
			img.colors++;
			*img.colorsAll += *img.colors;
			img.colors++;
			img.colorsAll++;
		}
		img.colors = reset;
		img.colorsAll = resetAll;
	};

	void ascii(Image &img) {
		int *resetAll;
		char *reset;
		int size = img.width * img.height;
		char set[9] = {'_','L','C','V','U','O','G','W','M'};

		img.asciiVal = new char[size];

		reset = img.asciiVal;
		resetAll = img.colorsAll;
		for (int i = 0; i < size; i++) {
			float factor = 85;
			int count = 0;
			bool found = true;

			while (found) {
				if (*img.colorsAll <= factor) {
					found = false;
				}
				else {
					count++;
					factor += 85;
				}
			}
			*img.asciiVal = set[count];
			img.asciiVal++;
			img.colorsAll++;
		}
		img.asciiVal = reset;
		img.colorsAll = resetAll;
	}

	friend istream& operator >> (ifstream &inFile, Image &img) {							// gets called once inFile is streamed into the class img
		int *reset, *resetAll;
		string fileName, myString;
		char dump;
		static bool once = true;

		inFile >> img.type;																// extracts type from file

		inFile >> myString;																// extracts width from file and puts it into the private width member
		img.width = stoi(myString.c_str());

		inFile >> myString;																// extracts height from file and puts it into the private height member
		img.height = stoi(myString.c_str());

		inFile >> myString;																// extracts max color value from file and puts it into the private maxColor member
		img.maxColor = stoi(myString.c_str());

		int size = img.width * img.height * 3;
		img.colorsAll = new int[size / 3];
		img.colors = new int[size];
		reset = img.colors;															// set resetial pointer for colors

		inFile.read(&dump, 1);															// ignore newline
		for (int i = 0; i < size; i++) {												// loops image file to extract each pixel data
			inFile >> *img.colors;
			*++img.colors;
		}
		img.colors = reset;															// reset pointer location

		return inFile;
	};

	friend ostream& operator << (ofstream &outFile, Image &img) {						// gets called once img is streamed out into outFile
		char *reset;
		int size = img.width * img.height;											// this means height and width times 3 bytes

		reset = img.asciiVal;

		int wid = 0;

		for (int i = 0; i < size; i++) {												// loops colors and puts it into outFile
			if (wid == img.width) {
				outFile << endl;
				wid = 0;
			}
			else {
				outFile << *img.asciiVal;
				*++img.asciiVal;
				wid++;
			}
		}
		img.asciiVal = reset;

		cout << endl << "Completed!" << endl;
		return outFile;
	};

private:
	string type;	// type of picture
	int width;		// width of picture
	int height;		// height of picture
	int maxColor;   // max color value of picture
	int *colors, *colorsAll;
	char *asciiVal;
};

int main() {
	Image img;
	img.menu(img);
}