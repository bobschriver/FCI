#include "probability_queue.h"

struct huffman_node * head = NULL;
struct huffman_node * peek()
{
	return head;
}

struct huffman_node * pop()
{
	struct huffman_node * ret = head;
	
	printf("%d\n" , head);
	
	if(head != NULL)
	{
		head = head->next;
	}

	return ret;
}

void insert(struct huffman_node * node)
{
	printf("%d %d\n" , node , head);

	if(head != NULL)
	{
		struct huffman_node * curr = head;
		struct huffman_node * prev = NULL;
		
		while(curr != NULL && curr->probability < node->probability)
		{
			prev = curr;
			curr = curr->next;
		}

		
		if(prev != NULL)
		{
			node->next = prev->next;
			prev->next = node;
		}
		else
		{
			//We're at the head
			node->next = head;
			head = node;
		}
	}
	else
	{
		head = node;
	}
}

struct huffman_node * get_head()
{
	return head;
}
