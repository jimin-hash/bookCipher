/*!	\file		bcencode_main.c
	\author		Jimin Park
	\date		2019-03-30
	\version	0.1

	This console application will encodes a text message using a book cipher.
	 command-line interface: bcencode.exe bookfile messagefile codedfile
	 - bookfile is the name of the ASCII text file containing the book used to encode/decode the message.
	 - messagefile is the name of the ASCII text file containing the message to be encoded
	 - codedfile is the name of the ASCII text file containing the offset numbers

	 Encoding rules
	 Encode each character of the source file by replacing it with two numbers, the first representing the line in the book (zero-based),
	 and the second the offset to that character in that line (also zero-based).
	 Any method of choosing which offset to use is acceptable so long as the same offset isn’t used repeatedly.
	 At the end of running bcencoder, a codedfile is created with offsets.
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <ctime>			
#include <random>			
using namespace std; //To avoid having to use the std:: qualification, type the following using directive

// forward declaration
int getBookFileC(vector<vector<unsigned char>> &vBook, string bookFile);
int getMessageFileC(vector<vector<unsigned char>> &vMessage, string messageFile);
int encodingMessage(vector<string> &vEncodedMessage, vector<vector<unsigned char>> vBook, vector<vector<unsigned char>> vMessage, string bookFile, string messageFile);
int randomOffset(vector<string> temp, vector<string> vEncodedMessage, size_t countChar, size_t numPush);
int countcharInMessage(vector<vector<unsigned char>> vMessage, unsigned char ch);
int printInFile(vector<string> const& vEncodedMessage, string codedFile);
bool findChar(vector<vector<unsigned char>> vBook, unsigned char ch);

int main(int argc, char* argv[]) {
	string bookFile;
	string messageFile;
	string codedFile;

	vector<string> vEncodedMessage;
	vector<vector<unsigned char>> vBook;
	vector<vector<unsigned char>> vMessage;

	cout << "bcencode (c) 2019, Jimin Park \n\t version 1.0.1" << endl;

	if (argc != 4) { // If user do not enter arguments as command-line interface, then program will terminate.
		cout << "Incorrect number of arguments <" << (argc - 1) << ">" << endl;
		cout << "Usage: BookCipherEncode bookfile originalfile codedfile" << endl;
		return EXIT_FAILURE;
	}
	else {
		srand(unsigned(time(NULL)));

		// text file name assign to the variables respectively.
		bookFile = argv[1];
		messageFile = argv[2];
		codedFile = argv[3];

		// 1. Get characters from book file
		getBookFileC(vBook, bookFile);

		// 2. Get characters from message file
		getMessageFileC(vMessage, messageFile);

		// 3. Encode message 
		encodingMessage(vEncodedMessage, vBook, vMessage, bookFile, messageFile);

		// 4. Write encode message into the coded file 
		printInFile(vEncodedMessage, codedFile);
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

/*!	 \fn int getMessageFileC()
	 \return 0 - if the fuction terminate correctly.
	 \param vector<vector<unsigned char>> &vMessage, string messageFile

	 Get characters from message file*/
int getMessageFileC(vector<vector<unsigned char>> &vMessage, string messageFile) {
	vector<unsigned char> vCols; // inner vector
	char ch;

	ifstream fin;//(messageFile);
	fin.open(messageFile, ios::binary);

	if (!fin) {
		cout << "Could not open the file.\n";
		exit(1); //return EXIT_FAILURE;
	}

	while (fin.get(ch)) {
		vCols.push_back((unsigned char)ch);

		if (ch == '\n') {
			vMessage.push_back(vCols);
			vCols.clear();
			vector<unsigned char> vCols;
		}

		if (fin.eof()) {
			vMessage.push_back(vCols);
			break;
		}
	}
	vMessage.push_back(vCols);

	fin.close();
	return 0;
}

/*!	 \fn bool findChar()
	 \return if ch is in book file, it will return true, it not return false.
	 \param vector<vector<unsigned char>> vBook, unsigned char ch

	 For check, if one char is not in book file, this program will terminate.*/
bool findChar(vector<vector<unsigned char>> vBook, unsigned char ch) {
	for (size_t row = 0; row < vBook.size(); row++) {
		vector<unsigned char>::iterator it = find(vBook[row].begin(), vBook[row].end(), ch);
		if (it != vBook[row].end())
			return true;
	}
	return false;
}

/*!	 \fn int countcharInMessage()
	 \return count
	 \param vector<vector<unsigned char>> vMessage, unsigned char ch

	 the count will be used for created random offset
	 - same number of chars between book and message file
	 - the characters in book file are more than characters in meesage file*/
int countcharInMessage(vector<vector<unsigned char>> vMessage, unsigned char ch) {
	size_t count = 0;
	for (size_t i = 0; i < vMessage.size(); i++) {
		for (size_t j = 0; j < vMessage[i].size(); j++) {
			if (vMessage[i][j] == ch)
				count++;
		}
	}
	return count;
}

/*!	 \fn int randomOffset()
	 \return ranIndex - index
	 \param vector<string> temp, vector<string> vEncodedMessage, int countChar

	 create random index for encoding
	  \note temp vector - it contains the indexes of book of certain char*/
int randomOffset(vector<string> temp, vector<string> vEncodedMessage, size_t countChar, size_t numPush) {
	int ranIndex = rand() % temp.size();

	for (;;) {
		// if the encoded message is already in vEncodedMessage, create rIndex again.
		vector<string>::iterator it = find(vEncodedMessage.begin(), vEncodedMessage.end(), temp[ranIndex]);
		if (it != vEncodedMessage.end()) { //founded
			ranIndex = rand() % temp.size();

			// -same number of chars between book and message file
			// - the characters in book file are more than characters in meesage file
			if (temp.size() <= countChar && temp.size() <= numPush)
				break;
		}
		else
			break;
	}
	return ranIndex;
}

/*!	 \fn int encodingMessage()
	 \return 0
	 \param vector<string> &vEncodedMessage, vector<vector<unsigned char>> vBook, vector<vector<unsigned char>> vMessage, string bookFile, string messageFile

	 This function will push_back the encoded message into the vEncodedMessage
	  \note temp vector - it contains the indexes of book of certain char*/
int encodingMessage(vector<string> &vEncodedMessage, vector<vector<unsigned char>> vBook, vector<vector<unsigned char>> vMessage, string bookFile, string messageFile) {
	vector<string> temp;
	vector<int> vCols;

	size_t rowIndex = 0, colIndex = 0; // for vMessage
	size_t maxRow = vMessage.size();
	size_t countChar = 0;
	size_t numPush = 0;
	unsigned char ch;

	while (rowIndex < maxRow) {
		for (size_t a = 0; a < vBook.size(); a++) {
			for (size_t b = 0; b < vBook[a].size(); b++) {
				ch = vMessage[rowIndex][colIndex];
				// if one char is not in book file, this program will terminate 
				bool charYN = findChar(vBook, ch);
				if (charYN == false) {
					cout << "Error <" << messageFile << "> is not encodable by <" << bookFile << ">. Missing character " << ch << " : " << (int)ch << endl;
					exit(1); //return EXIT_FAILURE;
				}

				if (vBook[a][b] == vMessage[rowIndex][colIndex]) {
					// puch back to temp
					temp.push_back(to_string(a) + "," + to_string(b));
				}
			}
		}

		// the count will be used for created random offset
		countChar = countcharInMessage(vMessage, ch);
		if (countChar == 1)
			vEncodedMessage.push_back(temp[0]);
		else {
			// create random index
			int rIndex = randomOffset(temp, vEncodedMessage, countChar, numPush);
			vEncodedMessage.push_back(temp[rIndex]);
			numPush++;
		}
		colIndex++;
		countChar = 0;
		temp.clear();

		if (colIndex == vMessage[rowIndex].size()) { // row + 1
			rowIndex++;
			colIndex = 0;
		}
	}
	return 0;
}

/*!	 \fn int printInFile()
	 \return 0
	 \param vector<string> const& vEncodedMessage, string codedFile

	 Write encode message into the coded file */
int printInFile(vector<string> const& vEncodedMessage, string codedFile) {
	ofstream fout;
	fout.open(codedFile, ios::binary);

	if (!fout) {
		cout << "Could not open the output file.\n";
		exit(1);  // return EXIT_FAILURE;
	}

	for (size_t i = 0; i < vEncodedMessage.size(); i++) {
		fout << vEncodedMessage[i] << " ";
	}

	fout.close();
	return 0;
}