#include "huffman_encode.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mex.h>

//Generates the priority queue containing all of the data values and their probabilities
struct huffman_node * gen_priority_queue(double * probabilities , int probability_length , int * patches , int patch_size , int patches_size)
{
	
	int i = 0;
	for(i = 0; i < probability_length ; i ++)
	{
		//Create our struct and clear it
		struct huffman_node * curr = malloc(sizeof(struct huffman_node));
		memset(curr , 0 , sizeof(struct huffman_node));
		
		//Set the probability and data to point to the right places
		curr->probability = probabilities[i];
		curr->data_size = patch_size;
		curr->data = patches + i * patch_size;

		//Insert it into our queue
		//Will automatically deal with ordering
		insert(curr);
	}

	//get_head returns the head of our queue
	return get_head();
}

struct huffman_node * gen_huffman_tree(struct huffman_node * queue)
{

	struct huffman_node * left = pop(queue);
	struct huffman_node * right = pop(queue);


	//While we have more than one item in the queue
	while(right != NULL)
	{
		struct huffman_node * comb = malloc(sizeof(struct huffman_node));
		memset(comb , 0 , sizeof(struct huffman_node));

		//Combine the two together
		comb->left = left;
		comb->right = right;	

		comb->probability = left->probability + right->probability;

		//Reinsert the combination
		insert(comb);

		left = pop(queue);
		right = pop(queue);
	}

	//Return the last value
	return left;
}

struct huffman_node * gen_huffman_codes(struct huffman_node * tree)
{
	return gen_huffman_codes_3(tree , 0 , 0);
}

struct huffman_node * gen_huffman_codes_3(struct huffman_node * tree , unsigned long long code , int code_size)
{
	struct huffman_node * left = NULL;
	struct huffman_node * right = NULL;

	printf("Tree left %d tree right %d\n" , tree->left , tree->right);
	printf("Code Size %d Code %d\n\n" , code_size , code);

	if(tree->left != NULL)
	{
		
		unsigned long long new_code = code << 1;
		left = gen_huffman_codes_3(tree->left , new_code , code_size + 1);
	}

	if(tree->right != NULL)
	{
		
		unsigned long long new_code = (code << 1) + 1;
		right = gen_huffman_codes_3(tree->right , new_code , code_size + 1);
	}

	if(right == NULL && left == NULL)
	{
		tree->code = code;
		tree->code_size = code_size;

		printf("\nEND OF TREE\n");
		printf("Prob %f\n" , tree->probability);
		printf("Code size %d\n" , tree->code_size);
		printf("Code %d\n\n" , tree->code);	

		return tree;
	}
	else
	{
		struct huffman_node * insert = left;

		printf("Insert probability %f\n" , insert->probability);

		while(insert->next != NULL)
		{
			printf("%d " , insert);
			insert = insert->next;

		}
		
		insert->next = right;
		return insert;
	}
}

unsigned long long * encode_huffman_tree(struct huffman_node * tree , int patch_size)
{

	int code_size = sizeof(unsigned long long) * 8;
	int curr_offset = code_size;
	unsigned long long data = 0;
	int data_size = 0;

	unsigned long long * ret_data = encode_huffman_tree_r(tree , patch_size , code_size , &curr_offset , &data , &data_size);

	return ret_data;
}

//This is probably the worst thing I've ever written
unsigned long long * encode_huffman_tree_r(struct huffman_node * tree , int patch_size , int code_size , int * curr_offset , unsigned long long * curr_data , int * data_size)
{

	int node_or_leaf = tree->left == NULL & tree->right == NULL;

	if(curr_offset > 0)
	{
		*data_size++;
		curr_data = realloc(curr_data , code_size * (*data_size + 1));
					
	}
	
	curr_data[*data_size] += node_or_leaf << *curr_offset;
	*curr_offset--;	

	//If we are at a leaf
	if(tree->left == NULL && tree->right == NULL)
	{		

		int i = 0;
		for(i = 0; i < patch_size; i ++)
		{	
			int this_node_offset = code_size;

			if(*curr_offset < this_node_offset)
			{
				curr_data[*data_size] += tree->data[i] >> (this_node_offset - *curr_offset);
				this_node_offset -= *curr_offset;
				curr_offset = code_size;
				*data_size++;
				curr_data = realloc(curr_data , code_size * (*data_size + 1));
			}

			curr_data[*data_size] += tree->data[i] << (*curr_offset - this_node_offset);
			*curr_offset--;
		}
	}
	else
	{
		curr_data = encode_huffman_tree_r(tree->left , patch_size , code_size , curr_offset , curr_data , data_size);
		curr_data = encode_huffman_tree_r(tree->right , patch_size , code_size , curr_offset , curr_data , data_size);
	}
	

	return curr_data;
} 

unsigned long long * encode_data(struct huffman_node * codes , int * patches , int patch_size , int patches_size , int * stored_data_size_ret)
{
	int i = 0;

	int curr_shift = sizeof(unsigned long long) * 8 - 1;


	unsigned long long * stored_data = malloc(sizeof(unsigned long long));
	stored_data[0] = 0;
	int stored_data_size = 0;



	for(i = 0; i < patches_size; i += patch_size)
	{

		int * curr_data = &patches[i];

		struct huffman_node * curr = codes;
		struct huffman_node * match = NULL;
		
		//Search for the node that has the same data as us
		while(curr != NULL)
		{
			int j = 0;
			int is_match = 1;

			for(j = 0; j < patch_size; j ++)
			{
				if(curr->data[j] != curr_data[j])
				{
					is_match = 0;
					break;
				}				
			}

			if(is_match)
			{
				match = curr;
				break;
			}
			
			curr = curr->next;
		}

		//If we haven't found a match, something bad happened
		if(match == NULL)
		{
			continue;
		}

		//We've found a match, now we need to stuff the code in.
		unsigned long long code = match->code;
		int code_size = match->code_size;

	
		curr_shift -= code_size;

		if(curr_shift < 0)
		{
			//Put the first bit of the current code in
			stored_data[stored_data_size] += code >> (curr_shift * -1);

			stored_data_size++;

			//We initialize stored_data_size to 0 for indexing, so we have to add one here
			stored_data = realloc(stored_data , sizeof(unsigned long long) * (stored_data_size + 1));
			stored_data[stored_data_size] = 0;

			//The number of bits we need to store is the code size + the current shift , since the curr shift now
			//Store the number of overflow bits, and is negative
			curr_shift = (sizeof(unsigned long long) * 8 - 1) - (code_size - curr_shift * -1);
		}

		stored_data[stored_data_size] += code << curr_shift;

		printf("Code shift %d Insert Code %llu Current code %llu\n" , curr_shift , code ,  stored_data[stored_data_size]);
	}	

	//Kinda janky, should fix
	*stored_data_size_ret = stored_data_size + 1;

	return stored_data;
}

void mexFunction(int nlhs, mxArray *plhs[ ], int nrhs, const mxArray *prhs[ ]) 
{
	const mxArray * mx_probabilities = prhs[0];
	const mxArray * mx_patches = prhs[1];
	const mxArray * mx_patch_size = prhs[2];
	const mxArray * mx_data = prhs[3];

	mexPrintf("%d\n" , nrhs);
	mexPrintf("%p %p %p\n" , mx_probabilities , mx_patches , mx_patch_size);
	mexPrintf("%d %d\n" , mxGetNumberOfElements(mx_probabilities) , mxGetNumberOfElements(mx_patches));
	
	double * probabilites = (double *)mxGetPr(mx_probabilities);
	int probability_size = mxGetNumberOfElements(mx_probabilities);

	unsigned int * patches = (unsigned int *)mxGetData(mx_patches);
	int patches_size = mxGetNumberOfElements(mx_patches);	

	unsigned int patch_size = * (unsigned int *)mxGetData(mx_patch_size);


	struct huffman_node * queue = gen_priority_queue(probabilites , probability_size , patches , patch_size , patches_size);

	struct huffman_node * tree = gen_huffman_tree(queue);

	struct huffman_node * codes = gen_huffman_codes(tree);
	
	unsigned int * data = (unsigned int *)mxGetData(mx_data);
	int data_size = mxGetNumberOfElements(mx_data);	

	//Encoded data length will store our returned data size	
	int encoded_data_length = 0;
	unsigned long long * encoded_data = encode_data(codes , data , patch_size , data_size , &encoded_data_length);
	

	
	int size[] = {1 , encoded_data_length};
	mxArray * mx_ret = mxCreateNumericArray(2 , size , mxUINT64_CLASS , mxREAL);

	mwIndex i;
	for(i = 0; i < encoded_data_length; i ++)
	{
		unsigned long long * pr = (unsigned long long *)mxGetData(mx_ret);
		pr[i] = encoded_data[i];
	}
	free(encoded_data);
	
	plhs[0] = mx_ret;

		
}

int main(char * args , int argc)
{
	double probabilites[] = { 0.4 , 0.35 , 0.2 , 0.05};
	double probability_size = 4;

	int patches[] = {5 , 4 , 3 , 2};
	int patch_size = 1;
	int patches_size = 4;

	struct huffman_node * queue = gen_priority_queue(probabilites , probability_size , patches , patch_size , patches_size); 

	struct huffman_node * next = queue;

	while(next != NULL)
	{
		next = next->next;
	}

	struct huffman_node * tree = gen_huffman_tree(queue);

	printf("%f %d %d\n" , tree->probability , tree->left , tree->right);
	printf("%f %f\n" , tree->left->probability , tree->right->probability);


	struct huffman_node * codes = gen_huffman_codes(tree);

	struct huffman_node * curr = codes;

	while(curr != NULL)
	{
		printf("Code length %d Prob %f code %d\n" , curr->code_size , curr->probability , curr->code);

		curr = curr->next;
	}

	int data[] = {5 , 5 , 4 , 5 , 4 , 3 , 3 , 4 , 5 , 5};
	int data_size = 10;

	int encoded_data_size = 0;	
	unsigned long long * encoded_data = encode_data(codes , data , patch_size , data_size, &encoded_data_size);

	printf("%llu\n" , encoded_data[0]);
	printf("%llu\n" , encoded_data[1]);
}
