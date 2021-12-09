#include <iostream>
#include <fstream>
#include <cstdint>
#include <filesystem>
#include <sstream>
#include <bitset>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
using namespace std;

const char CHAR_EOF = -1;

//Class HUFFMAN TREE
class HuffmanTree {
public:
	//Struct HUFFMAN NODE
	struct HuffmanNode {
		char ch;
		int freq;
		struct HuffmanNode* left;
		struct HuffmanNode* right;

		//Default constructor
		HuffmanNode()
			: ch('\0'), freq(0), left(nullptr), right(nullptr)
		{
		}

		//Paramerterized constructor
		HuffmanNode(char ch, int freq, HuffmanNode* left, HuffmanNode* right)
			: ch(ch), freq(freq), left(left), right(right)
		{
		}
	}*root;

	//Struct COMPARE FREQUENCY
	//Used for making min priority queue 
	struct CompareFrequency {
		bool operator()(HuffmanNode* const& n1, HuffmanNode* const& n2)
		{
			return n1->freq > n2->freq;
		}
	};

	//Default constructor
	HuffmanTree() {
		root = nullptr;
	}

	//Function Headers
	void preorderTraversal(HuffmanNode* root);
	void preorderHuffmanTree(HuffmanNode* root, string& traversal);
	void createEncodedPreorderTraversal(HuffmanNode* root, string& traversal);
	void createTreeFromPreorder(HuffmanNode* root, int& index, string& traversal);
};

//Class HUFFMAN ENCODING
class HuffmanEncoding {
public:

	//Function Headers
	unordered_map<int, int> countFrequencies(ifstream& infile);
	HuffmanTree::HuffmanNode* buildEncodingTree(unordered_map<int, int>& freq);
	void buildEncodingMap(HuffmanTree::HuffmanNode* root, string code, unordered_map<int, string>& codes);
	void compressFile(ifstream& infile, HuffmanTree::HuffmanNode* root, unordered_map<int, string>& encodingMap, ofstream& outfile);
	void decompressFile(ifstream& infile, ofstream& outfile);
};

//Functions

/*----------------
  HUFFMAN TREE
----------------*/

//PREORDER TRAVERSAL

void HuffmanTree::preorderTraversal(HuffmanNode* root) {
	if (root != nullptr) {
		cout << root->ch << " ";
		preorderTraversal(root->left);
		preorderTraversal(root->right);
	}
}

//PREORDER HUFFMAN TREE

void HuffmanTree::preorderHuffmanTree(HuffmanNode* root, string& traversal) {
	if (root != nullptr) {

		//If leaf node, append 1 and the charcater stored on the leaf
		if (!(root->left) && !(root->right)) {
			traversal += '1';
			traversal += root->ch;
		}

		//If non-leaf node, append 0
		else if ((root->left) && (root->right)) {
			traversal += '0';
		}
		preorderHuffmanTree(root->left, traversal);
		preorderHuffmanTree(root->right, traversal);
	}
}

//CERATE ENCODED PREORDER TRAVERSAL

void HuffmanTree::createEncodedPreorderTraversal(HuffmanNode* root, string& traversal) {
	if (root != nullptr) {

		//If leaf node, append 1
		if (!(root->left) && !(root->right)) {
			traversal += '1';

			//Append encoding for CHAR_EOF
			if ((int)root->ch == -1) {
				traversal += "11111111";
			}

			//Append the charcater stored on the leaf
			else {
				traversal += (bitset<8>(root->ch).to_string());
			}
		}

		//If non-leaf node, append 0
		else if ((root->left) && (root->right)) {
			traversal += '0';
		}
		createEncodedPreorderTraversal(root->left, traversal);
		createEncodedPreorderTraversal(root->right, traversal);
	}
}

//CREATE TREE FROM PREORDER

void HuffmanTree::createTreeFromPreorder(HuffmanNode* root, int& index, string& traversal) {
	if (root != nullptr) {

		//If 1, make a leaf node and store the charcater
		if (traversal[index] == '1') {
			index++;
			root->ch = traversal[index];
			return;
		}
		root->left = new HuffmanNode;
		createTreeFromPreorder(root->left, ++index, traversal);
		root->right = new HuffmanNode;
		createTreeFromPreorder(root->right, ++index, traversal);
	}
}

/*--------------------
  HUFFMAN ENCODING
--------------------*/

//COUNT FREQUENCIES

unordered_map<int, int> HuffmanEncoding::countFrequencies(ifstream& infile){
	unordered_map<int, int> freq;
	string fileText;
	stringstream buffer;

	//Read data from file
	buffer << infile.rdbuf();
	fileText = buffer.str();

	//Reset the input stream to the start of file
	infile.clear();
	infile.seekg(0);

	//Count frequencies for every characters
	for (char ch : fileText) {
		freq[ch]++;
	}

	//CHAR_EOF occurs only once
	freq[CHAR_EOF]++;
	return freq;
}

//BUILD ENCODING TREE

HuffmanTree::HuffmanNode* HuffmanEncoding::buildEncodingTree(unordered_map<int, int> &freq) {
	HuffmanTree t;
	priority_queue<HuffmanTree::HuffmanNode*, vector<HuffmanTree::HuffmanNode*>, HuffmanTree::CompareFrequency> pq;

	//Make a min priority queue of Huffman nodes (containing character with frequency)
	for (auto& x : freq) {
		pq.push(new HuffmanTree::HuffmanNode(x.first, x.second, nullptr, nullptr));
	}

	//Repeatedly remove two nodes from the the queue and join them into a new node whose frequency is their sum
	//The two nodes are positioned as children of the new node and the new node is re-inserted into the queue in sorted order
	while (pq.size() != 1) {
		HuffmanTree::HuffmanNode* n1 = pq.top(); 
		pq.pop();
		HuffmanTree::HuffmanNode* n2 = pq.top();
		pq.pop();
		pq.push(new HuffmanTree::HuffmanNode('\0', n1->freq + n2->freq, n1, n2));
	}
	HuffmanTree::HuffmanNode* root = pq.top();
	cout << "\n\nPreorder Traversal of Huffman Tree:\n" << endl;
	t.preorderTraversal(root);
	cout << endl;
	return root;
}

//BUILD ENCODING MAP

void HuffmanEncoding::buildEncodingMap(HuffmanTree::HuffmanNode* root, string code, unordered_map<int, string> &codes) {
	if (root == nullptr) {
		return;
	}

	//Stop on the leaf node and assign the code to the character stored on the leaf
	if(!(root->left) && !(root->right)) {
		codes[root->ch] = code;
 	}

	//For Huffman codes, each left branch is considered 0 and each right branch 1
	buildEncodingMap(root->left, code + "0", codes);
	buildEncodingMap(root->right, code + "1", codes);
}

//COMPRESS FILE

void HuffmanEncoding::compressFile(ifstream& infile, HuffmanTree::HuffmanNode* root, unordered_map<int, string> &encodingMap, ofstream &outfile) {
	HuffmanTree t;
	string encoding = "", traversal = "";
	int surplusBits = 0;
	char ch;
	t.preorderHuffmanTree(root, traversal);
	cout << "\n\nTree Information To Be Used During Decompression:\n" << endl;
	cout << traversal << endl;

	//Create encoding for preorder traversal of Huffman tree to store it in file header
	traversal = "";
	t.createEncodedPreorderTraversal(root, traversal);
	traversal += '1';
	traversal += "11111110";   //Header separation from the encoded data
	encoding += traversal;

	//Check if streams are open
	if (infile.is_open() && outfile.is_open()) {

		//Encode each character of the input file
		while (infile.get(ch)) {
			encoding += (encodingMap.at((int)ch));
		}

		//Append encoding for CHAR_EOF
		encoding += (encodingMap.at(-1));

		//Check if encoding length is exact multiple of 8 (Data can be written to a file only as bytes)
		surplusBits = encoding.length() % 8;
		if (surplusBits != 0) {
			for (int x = 0; x < (8 - surplusBits); x ++) {
				encoding += "0";
			}
		}

		//Append zeroes to the end of encoding if required
		int pos = 0;
		for (int y = 0; y < (encoding.length() / 8); y++) {
			bitset<8> b(encoding.substr(pos, 8));
			outfile << static_cast<uint_fast8_t>(b.to_ulong());
			pos += 8;
		}
	}
	if (!infile.eof() && infile.fail()) {
		cout << "Error reading the file." << endl;
	}
}

//DECOMPRESS FILE

void HuffmanEncoding::decompressFile(ifstream& infile, ofstream& outfile) {
	HuffmanTree t;
	string decoding = "", headerInfo = "";
	int j = 0;
	bool headerEnded = false;

	//Read the encoded file
	stringstream buffer;
	buffer << infile.rdbuf();
	string encodedData = "";

	//Convert to binary form
	for (std::size_t i = 0; i < (buffer.str()).size(); i++)
	{
		encodedData += (bitset<8>((buffer.str()).c_str()[i])).to_string();
	}

	//Extract tree information (preorder traversal) from file header
	while (headerEnded == false && j < encodedData.size()) {
		headerInfo;

		//If 1, append 1 and extract the character (1 is for leaf node)
		if (encodedData[j] == '1') {

			//If character is CHAR_EOF
			if (encodedData.substr(j + 1, 8) == "11111111") {
				headerInfo += '1';
				headerInfo += CHAR_EOF;
			}

			//If character is header separation character, loop ends
			else if (encodedData.substr(j + 1, 8) == "11111110") {
				headerEnded = true;
			}

			//Extract character
			else {
				headerInfo += '1';
				headerInfo += (bitset<8>(encodedData.substr(j + 1, 8)).to_ulong());
			}
			j += 8;
		}
		//If 0, append 0 (0 is for non-leaf node)
		else if (encodedData[j] == '0') {
			headerInfo += '0';
		}
		j++;
	}
	cout << "\n\nTree Information Extracted from File Header:\n" << endl;
	cout << headerInfo << endl;
	HuffmanTree::HuffmanNode* root = new HuffmanTree::HuffmanNode;
	int index = 0;

	//Create tree from the extracted information of preorder
	t.createTreeFromPreorder(root, index, headerInfo);
	cout << "\n\nTree Created From Preorder Traversal:\n" << endl;
	t.preorderTraversal(root);

	bool fileEnded = false;
	HuffmanTree::HuffmanNode* temp = root;

	//Decode the encoded data
	while (fileEnded == false && j < encodedData.size()) {

		//If 1, move right in the tree
		if (encodedData[j] == '1') {
			temp = temp->right;
		}

		//If 0, move left in th tree
		else {
			temp = temp->left;
		}

		//If leaf node is reached
		if (!(temp->left) && !(temp->right)) {

			//If CHAR_EOF is reached, loop ends
			if ((int)(temp->ch) == -1) {
				fileEnded = true;
			}

			//Write the character to the output file
			else {
				decoding += temp->ch;
				outfile << temp->ch;
				temp = root;
			}
		}
		j ++;
	}
}

/*--------------------
  MAIN FUNCTION
--------------------*/

int main() {
	HuffmanEncoding h;
	HuffmanTree::HuffmanNode* r;
	ifstream infile1, infile2;
	ofstream outfile1, outfile2;
	unordered_map<int, int> frequencies;
	unordered_map<int, string> codes;
	string fileName, fileName_;
	std::uintmax_t size1, size2;
	size_t lastIndex;
	int userChoice, i, count1, j, count2;

	//Display menu
	cout << "\n--------------------" << endl;
	cout << "Huffman Coding";
	cout << "\n--------------------\n" << endl;
	cout << "1. Compress a File" << endl;
	cout << "2. Decompress a File" << endl;
	cout << "\nEnter your choice : ";
	cin >> userChoice;
	cin.ignore();

	switch (userChoice) {

	//COMPRESS FILE
	case 1:

		//Take user input 
		cout << "Enter name/location of file to encode: ";
		getline(cin, fileName);
		lastIndex = fileName.find_last_of(".");
		fileName_ = fileName.substr(0, lastIndex);

		//Open file streams
		infile1.open(fileName);
		outfile1.open(fileName_ + "_encoded.huf", ios::out | ios::binary);

		//Count frequencies
		frequencies = h.countFrequencies(infile1);

		//Display frequency map
		cout << "\n\nFrequency Map:\n" << endl;
		i = 1;
		count1 = frequencies.size();
		cout << "{";
		for (auto& x : frequencies) {
			if (i != count1) {
				cout << (char)x.first << ":" << x.second << ", ";
			}
			else {
				cout << (char)x.first << ":" << x.second;
			}
			i++;
		}
		cout << "}" << endl;

		//Build Huffman tree
		r = h.buildEncodingTree(frequencies);

		//Build encoding map
		h.buildEncodingMap(r, "", codes);

		//Display encoding map
		cout << "\n\nEncoding Map:\n" << endl;
		j = 1;
		count2 = codes.size();
		cout << "{";
		for (auto& y : codes) {
			if (j != count2) {
				cout << (char)y.first << ":" << y.second << ", ";
			}
			else {
				cout << (char)y.first << ":" << y.second;
			}
			j++;
		}
		cout << "}" << endl;

		//Compress file
		h.compressFile(infile1, r, codes, outfile1);

		//Close streams
		infile1.close();
		outfile1.close();

		//Display sizes and compression ratio
		size1 = std::filesystem::file_size(fileName);
		size2 = std::filesystem::file_size(fileName_ + "_encoded.huf");
		cout << "\n\nSize of the original file is " << size1 << " bytes" << endl;
		cout << "Size of the encoded file is " << size2 << " bytes" << endl;
		cout << "Compression ratio is " << (float)size1/size2 << ":1" << endl;
		cout << "Compression percentage is " << 100 - ((float)size2 / size1) * 100 << "%" << endl;
		break;

	//DECOMPRESS FILE
	case 2:
		
		//Take user input 
		cout << "Enter name/location of file to decode: ";
		getline(cin, fileName);
		lastIndex = fileName.find("_encoded.huf");
		fileName_ = fileName.substr(0, lastIndex);

		//Open file streams
		infile2.open(fileName, ios::binary | ios::in);
		outfile2.open(fileName_ + "_decoded.txt");

		//Decompress file
		h.decompressFile(infile2, outfile2);

		//Close streams
		infile2.close();
		outfile2.close();
		cout << endl;
		break;

	default:
		cout << "Wrong choice" << endl;
	}
	return 0;
}
