#include "probability_queue.h"

struct huffman_node * gen_priority_queue(double * probabilities , int probability_length , int * patches , int patch_size , int patches_size);

struct huffman_node * gen_huffman_tree(struct huffman_node * queue);

struct huffman_node * gen_huffman_codes(struct huffman_node * tree);

struct huffman_node * gen_huffman_codes_3(struct huffman_node * tree , unsigned long long code , int code_size);

unsigned long long * encode_huffman_tree(struct huffman_node * tree , int patch_size);

unsigned long long * encode_huffman_tree_r(struct huffman_node * tree , int patch_size , int code_size , int * curr_offset , unsigned long long * curr_data , int* data_size);

unsigned long long * encode_data(struct huffman_node * codes , int * patches , int patch_size , int patches_size , int * encode_data_size);
