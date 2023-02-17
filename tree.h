// Copyright (C) Diana Cismaru & Alexandra Dragusin (2021 - 2022 / 311CA)
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."
#define MALLOC_FAILED "Malloc failed!\n"
#define DIE(assertion, call_description)  \
	do									  \
	{									  \
		if (assertion)					  \
		{								  \
			fprintf(stderr, "(%s, %d): ", \
					__FILE__, __LINE__);  \
			perror(call_description);	  \
			exit(errno);				  \
		}								  \
	} while (0)

typedef struct FileContent FileContent;
typedef struct FolderContent FolderContent;
typedef struct TreeNode TreeNode;
typedef struct FileTree FileTree;
typedef struct ListNode ListNode;
typedef struct List List;

enum TreeNodeType {
	FILE_NODE,
	FOLDER_NODE
};

struct FileContent {
	char* text;
};

struct FolderContent {
	List* children;
};

struct TreeNode {
	TreeNode* parent;
	char* name;
	enum TreeNodeType type;
	void* content;
};

struct FileTree {
	TreeNode* root;
};

struct ListNode {
	TreeNode* info;
	ListNode* next;
};

struct List {
	ListNode* head;
};

// Function that gets a node from a list and return its content
TreeNode *get_node(TreeNode* currentNode, char *name);

// Recursive function that frees the content of a ListNode
void free_rec(ListNode *aux);

// Recursive function that prints the content of a directory
void print_rec(ListNode *aux, int *directories, int *files, int *tabs);

// Function that lists the content of a node
void ls(TreeNode* currentNode, char* arg);

// Function that prints the path to the current node
void pwd(TreeNode* treeNode);

// Function that changes directories until the wanted node is found
TreeNode* cd(TreeNode* currentNode, char* path);

// Function that recursively displays the content of a directory
void tree(TreeNode* currentNode, char* arg);

// Function that creates a new directory
void mkdir(TreeNode* currentNode, char* folderName);

// Function that removes a file node from a specified directory
void rm(TreeNode* currentNode, char* fileName);

// Function that removes an empty directory
void rmdir(TreeNode* currentNode, char* folderName);

// Function that recursively removes a directory
void rmrec(TreeNode* currentNode, char* resourceName);

// Function that creates a new file
void touch(TreeNode* currentNode, char* fileName, char* fileContent);

// Function that copies the content of the source into a specified destination
void cp(TreeNode* currentNode, char* source, char* destination);

// Function that moves the content of the source into a specified destination
void mv(TreeNode* currentNode, char* source, char* destination);

// Function that creates the root directory
FileTree createFileTree();

// Function that frees the memory of all the file system
void freeTree(FileTree fileTree);
