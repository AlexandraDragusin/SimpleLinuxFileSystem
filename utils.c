// Copyright (C) Diana Cismaru & Alexandra Dragusin (2021 - 2022 / 311CA)
#include "tree.h"

// Function that gets a node from a list and return its content
TreeNode *get_node(TreeNode* currentNode, char *name)
{
	// Point to the head of the list
	ListNode* aux = ((FolderContent *)(currentNode->content))->children->head;
	if (!aux) {
		return NULL;
	}

	// Search for the node by its name
	while (aux) {
		if (!strcmp(aux->info->name, name)) {
			return aux->info;
		}
		aux = aux->next;
	}
	return NULL;
}

// Recursive function that frees the content of a ListNode
void free_rec(ListNode *aux)
{
	// Check if a file node has to be freed
	if (aux->info->type == FILE_NODE) {
		ListNode *node = aux->next;

		aux->info->parent = NULL;
		free(aux->info->name);
		if (((FileContent *)(aux->info->content))->text)
			free(((FileContent *)(aux->info->content))->text);
		free(aux->info->content);
		free(aux->info);
		free(aux);

		if (!node)
			return;

		aux = node;
		free_rec(aux);
		return;
	}

	// Free the content of a folder node
	ListNode *next_node = ((FolderContent *)(aux->info->content))->children->head;

	// Check if the folder node has content
	if (next_node) {
		free_rec(next_node);
	}

	ListNode *node = aux->next;

	aux->info->parent = NULL;
	free(aux->info->name);
	free(((FolderContent *)(aux->info->content))->children);
	free(aux->info->content);
	free(aux->info);
	free(aux);

	if (!node)
		return;
	aux = node;
	free_rec(aux);
}

// Recursive function that prints the content of a directory
void print_rec(ListNode *aux, int *directories, int *files, int *tabs) {
	// Print the necessary tabs
	int i;
	for (i = 0; i < (*tabs); i++)
			printf("\t");

	// Print the name of the current file/directory
	printf("%s\n", aux->info->name);

	if (aux->info->type == FILE_NODE) {
			(*files)++;
	} else {
		(*directories)++;
		// Get the content of the directory
		ListNode *next_node = ((FolderContent *)(aux->info->content))->children->head;
		if (!next_node) {
			// Decrease the tabs number when exiting a directory
			(*tabs)--;
			return;
		}
		// Increase the tabs number when entering a directory
		(*tabs)++;

		// Print the content of the directory
		print_rec(next_node, directories, files, tabs);
	}

	if (!aux->next) {
		(*tabs)--;
		return;
	}

	// Print the content of the next node
	aux = aux->next;
	print_rec(aux, directories, files, tabs);
}
