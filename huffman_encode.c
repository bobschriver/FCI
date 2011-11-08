#include "huffman_encode.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mex.h>

struct huffman_node * gen_priority_queue(double * probabilities , int probability_length , int * patches , int patch_size , int patches_size)
{
	mexPrintf("%p %d %p %d %d\n" , probabilities , probability_length , patches , patch_size , patches_size);

	struct huffman_node * first = NULL;
	
	int i = 0;
	for(i = 0; i < probability_length ; i ++)
	{
		struct huffman_node * curr = malloc(sizeof(struct huffman_node));
		memset(curr , 0 , sizeof(struct huffman_node));
		curr->probability = probabilities[i];
		curr->data_size = patch_size;
		curr->data = patches + i * patch_size;

		mexPrintf("%f %d %p\n" , curr->probability , curr->data_size , curr->data);

		insert(curr);
		
		if(first == NULL)
		{	
			first = curr;
		}
	}

	first = get_head();

	mexPrintf("%p\n" , first);

	return first;
}

struct huffman_node * gen_huffman_tree(struct huffman_node * queue)
{
	while(1)
	{
		//printf("%d\n" , queue);
		//fflush(stdout);
		
		struct huffman_node * comb = malloc(sizeof(struct huffman_node));
		memset(comb , 0 , sizeof(struct huffman_node));
		struct huffman_node * left = pop(queue);
		struct huffman_node * right = pop(queue);

		//printf("%d\n" , left);
		//printf("%d\n" , right);

		comb->left = left;
		comb->right = right;
		

		if(right == NULL)
		{
			return left;
		}

		printf("Left probability %f Right probability %f\n\n" , left->probability , right->probability);

		comb->probability = left->probability + right->probability;

		insert(comb);	
	}

	return queue;
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

unsigned long long * encode_data(struct huffman_node * codes , int * patches , int patch_size , int patches_size , int * stored_data_size_ret)
{
	int i = 0;

	int curr_shift = sizeof(unsigned long long) * 8;


	unsigned long long * stored_data = malloc(sizeof(unsigned long long));
	stored_data[0] = 0;
	int stored_data_size = 0;



	for(i = 0; i < patches_size; i += patch_size)
	{

		int * curr_data = &patches[i];

		//printf("%d\n" , curr_data[i]);

		struct huffman_node * curr = codes;
		struct huffman_node * match = NULL;
		
		//Search for the node that has the same data as us
		while(curr != NULL)
		{
			int j = 0;
			int is_match = 1;

			for(j = 0; j < patch_size; j ++)
			{
				//printf("%d\n" , j);
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

		if(match == NULL)
		{
			continue;
		}

		//We've found a match, now we need to stuff the code in.
		unsigned long long code = match->code;
		int code_size = match->code_size;

		//printf("%d %d %d\n" , match->data[0] , code_size , code);
	
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
			curr_shift = sizeof(unsigned long long) * 8 - (code_size - curr_shift * -1);
		}
	
		printf("%d\n" , code);
		printf("%d\n" , curr_shift);
		stored_data[stored_data_size] += code << curr_shift;
		printf("%llu\n" , stored_data[stored_data_size]);
	}	

	printf("Stored data ret %p %d\n" , stored_data_size_ret , *stored_data_size_ret);
	*stored_data_size_ret = stored_data_size + 1;

	printf("Stored data ret %d %d\n" , *stored_data_size_ret , stored_data_size + 1);
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
	

	double * probabilities = malloc(mxGetNumberOfElements(mx_probabilities));

	mexPrintf("In mexFunction\n");
	
	mwIndex i;
	for(i = 0; i < mxGetNumberOfElements(mx_probabilities); i ++)
	{
		double * pr = (double *) mxGetPr(mx_probabilities);
		printf("%f\n" , pr[i]);
		probabilities[i] = pr[i];
	}

	unsigned int * patches = malloc(mxGetNumberOfElements(mx_patches));


	mexPrintf("Finding Patches\n");
	for(i = 0; i < mxGetNumberOfElements(mx_patches); i ++)
	{
		unsigned int * pr = (unsigned int *)mxGetData(mx_patches);
		//mexPrintf("%p\n" , pr);
		mexPrintf("%d\n" , pr[i]);

		patches[i] = pr[i];
	}

		unsigned int patch_size = * (unsigned int *)mxGetData(mx_patch_size);

	mexPrintf("%d\n" , patch_size);

	struct huffman_node * queue = gen_priority_queue(probabilities , mxGetNumberOfElements(mx_probabilities) , patches , patch_size , mxGetNumberOfElements(mx_patches));

	//free(probabilities);
	//free(patches);	

	mexPrintf("Gen Tree\n");
	struct huffman_node * tree = gen_huffman_tree(queue);

	mexPrintf("Gen Codes\n");
	struct huffman_node * codes = gen_huffman_codes(tree);
	
	unsigned int * data = malloc(mxGetNumberOfElements(mx_data));
	mexPrintf("Getting Data\n");
	for(i = 0; i < mxGetNumberOfElements(mx_data); i ++)
	{
		unsigned int * pr = (unsigned int *)mxGetData(mx_data);
		
		data[i] = pr[i];
	}

	mexPrintf("Encode data\n");
	int encoded_data_length = 0;
	unsigned long long * encoded_data = encode_data(codes , data , patch_size , mxGetNumberOfElements(mx_data) , &encoded_data_length);
	
	//free(data);

	mexPrintf("Encoded Data Length %d\n" , encoded_data_length);
	
	mexPrintf("%d\n" , nlhs);
	//mexPrintf("%p\n" , plhs[0]);
	int size[] = {1 , encoded_data_length};
	mxArray * mx_ret = mxCreateNumericArray(2 , size , mxUINT64_CLASS , mxREAL);
	plhs[0] = mx_ret;
	//mexPrintf("%p\n" , plhs[0]);
	//mxArray * mx_ret = plhs[0];
	mexPrintf("%p\n" , mx_ret);

	for(i = 0; i < encoded_data_length; i ++)
	{
		unsigned long long * pr = (unsigned long long *)mxGetData(mx_ret);
		mexPrintf("%p\n" , pr);	
		pr[i] = encoded_data[i];
	}
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
