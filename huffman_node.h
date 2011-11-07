#include <stdlib.h>

struct huffman_node
{
	double probability;

	//Since this is both in a tree and a list
	//We have to cover all the bases
	struct huffman_node * left;
	struct huffman_node * right;

	struct huffman_node * next;

	int code_size;
	unsigned long long code;

	int data_size;
	int * data;
};

