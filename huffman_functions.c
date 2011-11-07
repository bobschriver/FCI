#include "huffman_functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

struct huffman_node * gen_priority_queue(double * probabilities , int probability_length , int * patches , int patch_size , int patches_size)
{
	struct huffman_node * first = NULL;
	
	int i = 0;
	for(i = 0; i < probability_length ; i ++)
	{
		struct huffman_node * curr = malloc(sizeof(struct huffman_node));
		memset(curr , 0 , sizeof(struct huffman_node));
		curr->probability = probabilities[i];
		curr->data_size = patch_size;
		curr->data = patches + i * patch_size;

		insert(curr);
		
		if(first == NULL)
		{	
			first = curr;
		}
	}

	first = get_head();

	printf("%d\n" , first->next);

	return first;
}

struct huffman_node * gen_huffman_tree(struct huffman_node * queue)
{
	while(queue->next != NULL)
	{
		printf("%d\n" , queue);
		fflush(stdout);
		
		struct huffman_node * comb = malloc(sizeof(struct huffman_node));

		struct huffman_node * left = pop(queue);
		struct huffman_node * right = pop(queue);

		printf("%d\n" , left);
		printf("%d\n" , right);

		comb->left = left;
		comb->right = right;
		

		if(right == NULL)
		{
			return left;
		}

		comb->probability = left->probability + right->probability;

		insert(comb);	
	}

	return queue;
}

struct huffman_node * gen_huffman_codes(struct huffman_node * tree)
{
	return gen_huffman_codes_3(tree , 0 , 1);
}

struct huffman_node * gen_huffman_codes_3(struct huffman_node * tree , unsigned long long code , int code_size)
{
	struct huffman_node * left;
	struct huffman_node * right;

	if(tree->left != NULL)
	{
		/*int temp_code = code + 1;
		code_size = 0

		while(temp_code > 0)
		{
			temp_code >>= 1;
			code_size += 1;
		}
		*/

		code = code + (1 << code_size);

		left = gen_huffman_codes_3(tree->left , code , code_size + 1);
	}

	if(tree->right != NULL)
	{
		right = gen_huffman_codes_3(tree->right , code , code_size + 1);
	}

	if(right == NULL && left == NULL)
	{
		tree->code = code;
		tree->code_size = code_size;

		return tree;
	}
	else
	{
		struct huffman_node * insert = left;

		while(insert->next != NULL)
		{
			insert = insert->next;
		}
		
		insert->next = right;
		return left;
	}
}

int main(char * args , int argc)
{
	double probabilites[] = { 0.4 , 0.2 , 0.2 , 0.1 , 0.05 , 0.05 };
	double probability_size = 6;

	int patches[] = {5 , 4 , 3 , 2 , 1 , 0};
	int patch_size = 1;
	int patches_size = 6;

	struct huffman_node * queue = gen_priority_queue(probabilites , probability_size , patches , patch_size , patches_size); 

	struct huffman_node * next = queue;

	while(next != NULL)
	{
		next = next->next;
	}

	struct huffman_node * tree = gen_huffman_tree(queue);

	printf("%f %d %d\n" , tree->probability , tree->left , tree->right);
	printf("%f %f\n" , tree->left->probability , tree->right->probability);

}
