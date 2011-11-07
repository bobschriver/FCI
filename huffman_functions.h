#include "probability_queue.h"

struct huffman_node * gen_priority_queue(double * probabilities , int probability_length , int * patches , int patch_size , int patches_size);

struct huffman_node * gen_huffman_tree(struct huffman_node * queue);

struct huffman_node * gen_huffman_codes(struct huffman_node * tree);

struct huffman_node * gen_huffman_codes_3(struct huffman_node * tree , unsigned long long code , int code_size);
