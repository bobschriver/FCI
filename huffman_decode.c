#include "huffman_decode.h"
#include "mex.h"

struct huffman_node * decode_huffman_tree(unsigned long long * huffman_tree_binary , int binary_size ,  int patch_size , int data_size)
{
	int i = 0;

	struct huffman_node * curr_branch = malloc(sizeof(struct huffman_node));
	//The way we encode, the left node is first	
	int left_right_branch = 0;

	for(i = 0; i < binary_size;)
	{
		unsigned long long curr = huffman_tree_binary[i];
	
		int curr_offset = 0;

		for(curr_offset = sizeof(unsigned long long) - 1; curr_offset > 0 ; curr_offset--)
		{
			int curr_value = curr >> curr_offset & 1;

			struct huffman_node * next_branch = malloc(sizeof(struct huffman_node));

			if(curr_branch->data == NULL)
			{
				curr_branch->left = next_branch;
				next_branch->parent = curr_branch;
				curr_branch = next_branch;
			}
			else
			{
				while(curr_branch->right != NULL)
				{
					if(curr_branch->parent != NULL)
					{
						curr_branch = curr_branch->parent;
					}
					else
					{
						//Something bad has happened
					}
				}

				curr_branch->right = next_branch;
				next_branch->parent = curr_branch;
				curr_branch = next_branch;
			}

			//Mask off the but we just read
			curr = curr & ((1 << curr_offset) - 1);

			//We are currently at a leaf
			if(curr_value == 1)
			{	
				int bytes_to_read = patch_size * data_size;
				
				int * data = malloc(bytes_to_read);
				int b = 0;
				for(b = 0; b < patch_size; b++)
				{
					if(curr_offset < bytes_to_read)
					{
						data[b] += curr << (sizeof(int) * data_size) - curr_offset;					  
						curr_offset = (sizeof(unsigned long long) - 1) - (bytes_to_read - curr_offset);
						curr = huffman_tree_binary[++i];
					}
					
					curr_offset = curr_offset - bytes_to_read;
					data[b] += curr >> (curr_offset);
					curr = curr >> curr_offset;
				}
			}
		}
	}
}



int * decode_data(struct huffman_node * huffman_tree , unsigned long long * data , int data_size , int image_size)
{
	int i = 0;

	int curr_decode_data = 0;
	int * decoded_data = malloc(image_size);


	struct huffman_node * curr = huffman_tree;
	for(i = 0; i < data_size; i++)
	{
		int curr_offset = 0;
		
		
		for(curr_offset = sizeof(unsigned long long) * 8; curr_offset > 0; curr_offset--)
		{
			if(curr->data != NULL)
			{
				decoded_data[curr_decode_data] = curr->data;
				curr = huffman_tree; 
			}
		
			int curr_value = !!(data[i] & (1 << curr_offset));
			
			if(curr_value)
			{
				//Value is one, go left
				curr = curr->left;
			}
			else
			{
				curr = curr->right;
			}
		}
	}

	return decoded_data;
}
