#include "huffman_node.h"

struct huffman_node * decode_huffman_tree(unsigned long long * huffman_tree_binary , int binary_size , int patch_size , int data_size);

int * decode_data(struct huffman_node * tree , unsigned long long * huffman_data_binary , int data_size , int image_size);
