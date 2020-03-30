/*!	\file		bcdecode_main.c
	\author		Jimin Park
	\date		2019-03-30
	\version	0.1

	This console application will decodes the message coded by the bcencode program.
	At the end of running bcdecoder, a messagefile is created with the message.

	 command-line interface: bcdecode.exe bookfile codedfile messagefile
	 - bookfile is the name of the ASCII text file containing the book used to encode/decode the message.
	 - codedfile is the name of the ASCII text file containing the offset numbers
	 - messagefile is the name of the ASCII text file containing the message to be encoded
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
using namespace std; //To avoid having to use the std:: qualification, type the following using directive

// forward declaration
int getBookFileC(vector<vector<unsigned char>> &vBook, string bookFile);
int getCodedFile(vector<string> &vCoded, string codedFile);
int decodeCodedFile(vector<vector<unsigned char>> vBook, vector<string> vCoded, vector<unsigned char> &vDecodedMessage);
int printInFile(vector<unsigned char> const& vDecodedMessage, string decodedFile);

int main(int argc, char* argv[]) {
	string bookFile;
	string codedFile;
	string decodedFile;

	vector<vector<unsigned char>> vBook;
	vector<string> vCoded;
	vector<unsigned char> vDecodedMessage;

	cout << "bcdecode (c) 2019, Jimin Park \n\t version 1.0.1" << endl;

	if (argc != 4) {  // If user do not enter arguments as command-line interface, then program will terminate.
		cout << "Incorrect number of arguments <" << (argc - 1) << ">" << endl;
		cout << "Usage: BookCipherDecode bookfile codedfile decodedfile" << endl;
		return EXIT_FAILURE;
	}
	else {
		// text file name assign to the variables respectively.
		bookFile = argv[1];
		codedFile = argv[2];
		decodedFile = argv[3];

		// 1. Get characters from book file
		getBookFileC(vBook, bookFile);

		// 2. Get characters from coded file
		getCodedFile(vCoded, codedFile);

		// 3. Decode message
		decodeCodedFile(vBook, vCoded, vDecodedMessage);

		// 4. Write encode message into the decoded file 
		printInFile(vDecodedMessage, decodedFile);
	}
}

/*!	 \fn int getBookFileC()
	 \return 0 - if the fuction terminate correctly.
	 \param vector<vector<unsigned char>> &vBook, string bookFile

	 Get characters from book file*/
int getBookFileC(vector<vector<unsigned char>> &vBook, string bookFile) {
	vector<unsigned char> vCols; // inner vector
	char ch;

	ifstream fin;
	fin.open(bookFile, ios::binary);

	if (!fin) {
		cout << "Could not open the file.\n";
		exit(1);  // return EXIT_FAILURE;
	}

	while (fin.get(ch)) {
		vCols.push_back((unsigned char)ch);

		if (ch == '\n') {
			vBook.push_back(vCols);
			vCols.clear();
			vector<unsigned char> vCols;
		}

		if (fin.eof()) {
			vBook.push_back(vCols);
			break;
		}
	}
	vBook.push_back(vCols);

	fin.close();

	return 0;
}

/*!	 \fn int getCodedFile()
	 \return 0 - if the fuction terminate correctly.
	 \param vector<string> &vCoded, string codedFile

	 Get characters from coded file*/
int getCodedFile(vector<string> &vCoded, string codedFile) {
	string indexChar;
	ifstream fin;
	fin.open(codedFile);

	if (!fin) {
		cout << "Could not open the file.\n";
		exit(1);  // return EXIT_FAILURE;
	}

	while (fin >> indexChar) {
		vCoded.push_back(indexChar);
	}

	fin.close();
	return 0;
}

/*!	 \fn int decodeCodedFile()
	 \return 0 - if the fuction terminate correctly.
	 \param vector<vector<unsigned char>> vBook, vector<string> vCoded, vector<unsigned char> &vDecodedMessage

	 To decode message*/
int decodeCodedFile(vector<vector<unsigned char>> vBook, vector<string> vCoded, vector<unsigned char> &vDecodedMessage) {
	size_t rowIndex = 0; // row index of book
	size_t colIndex = 0; // colum index of book
	size_t comIndex = 0; // comma index for substr
	size_t rowLength = 0; // for calculate row index(comma before)

	for (size_t i = 0; i < vCoded.size(); i++) {
		comIndex = vCoded[i].find(",");
		for (size_t j = 0; j < comIndex; j++)
			rowLength++;
		rowIndex = stoi(vCoded[i].substr(0, rowLength));
		colIndex = stoi(vCoded[i].substr(comIndex + 1));

		//puch back decoded message
		vDecodedMessage.push_back(vBook[rowIndex][colIndex]);
	}

	return 0;
}

/*!	 \fn int printInFile()
	 \return 0
	 \param vector<unsigned char> const& vDecodedMessage, string decodedFile

	 Write decoded message into the decoded File*/
int printInFile(vector<unsigned char> const& vDecodedMessage, string decodedFile) {
	ofstream fout;
	fout.open(decodedFile, ios::binary);

	if (!fout) {
		cout << "Could not open the output file.\n";
		exit(1);  //return EXIT_FAILURE;
	}

	for (size_t i = 0; i < vDecodedMessage.size(); i++) {
		fout << vDecodedMessage[i];
	}

	fout.close();
	return 0;
}