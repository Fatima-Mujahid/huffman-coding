# Huffman Coding

### Background

Data compression has revolutionized the digital multimedia. For this purpose, Compression algorithms are normally used which reduce the size of a file while ensuring that files can be fully restored to their original state if needed. There is a wide range of algorithms that can used to perform lossless data compression. One of the simplest data compression technique is known as the Run-Length Encoding (RLE) algorithm but the biggest problem with this is that there are no repeated characters, and the size of output data can be two times more than the size of input data. To eliminate this problem Huffman Coding was used. Huffman coding algorithm was introduced by David Huffman. It is a more successful method used for data compression. 

### Purpose

Every information in computing is encoded as strings of 1’s and 0’s. The main purpose of information theory is to convey information in as few bits as possible, bearing in mind that every encoding is unambiguous. Huffman's coding has the same aim and is largely successful. 
Initially, this algorithm creates a tree in the form of a leaf node and its children which has a probability of the frequent appearance of the characters. It assigns less bits to the more frequently occurring characters which decreases the overall length of the data. So, Huffman coding algorithm has more efficiency in file compression with greater compression ratio.

### Objectives

1. To Compress data without losing data
2. Use of Statistical Coding - More frequently used symbols should have shorter code words
3. Transmit information in fewest bits possible
4. Encoding that should be used must be variable-length that is use of variable number of bits for encoding

### Scope

In the modern era, the transmission of data via the Internet depends on time. More data takes more time to transmit. Huffman coding reduces the data size thus data transmission time is significantly reduced due to the Huffman coding algorithm. It is the basis of many data compression and encoding schemes and also helps in lossless data encoding. It has different real-world applications and some of them are given below: 
 - Data compression tools such as GZIP, PKZIP (WinZip) uses Huffman encoding.
 - Image format like JPEG, PNG also uses Huffman encoding. 
 - DEFLATE (a lossless data compression algorithm) also uses a combination of Huffman algorithm. 
 - Huffman based code compression techniques also used for embedded processors.
 - It is also used in advanced audio coding standard.

This algorithm is used for compressing data and reducing its size without losing any information. This algorithm consists of two major steps:

1.	Building a Huffman tree from the input characters
2.	Traversing the Huffman tree to assign codes to the characters (encoding)

### Explanation

View the explanation and demonstration video [here](https://drive.google.com/drive/folders/1jOJuS5uCMBFtSX9vM63ls392oqbv6bPP?usp=sharing)

### Analysis

For decompression(decoding of data), the Huffman tree must be somehow reconstructed. One way is to store the encoding map (containing the characters and their frequency count) along with the encoded data but the overhead in such a case could amount to several kilobytes, so this method has little practical use. For this project, we studied and used the approach where the information to reconstruct the tree is sent as a preorder traversal. The non-leaf node is indicated as 0 and leaf node as 1 and the character on the leaf is also stored with it. It takes comparatively a smaller number of bytes and hence it is one of the efficient ways to store the information of Huffman tree.

### Time Complexity
The time complexity analysis of Huffman Coding is as follows:

 - It extracts minimum frequency from the priority queue by using extractMin() which is called 2 x (n-1) times if there are n nodes.
 - As extractMin() calls minHeapify() whose complexity is O(logn).

Thus, Overall time complexity of Huffman Coding becomes O(nlogn).
Here, n is the number of unique characters in the given text.

### Conclusion
Although there are other algorithms for data compression, but Huffman Coding is used especially for lossless data compression and uses greedy approach for its implementation. It turns out to be a simple and efficient way to encode data into short representations without losing any piece of information.
