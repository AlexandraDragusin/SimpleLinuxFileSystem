// Copyright (C) Diana Cismaru & Alexandra Dragusin (2021 - 2022 / 311CA)
#include "tree.h"

// Function that creates the root directory
FileTree createFileTree(char* rootFolderName) {
	// Alloc memory for the node
	FileTree root_dir;
	root_dir.root = malloc(sizeof(TreeNode));
	DIE(!root_dir.root, MALLOC_FAILED);

	// Initialize the node's data
	root_dir.root->parent = NULL;

	root_dir.root->name = malloc(strlen(rootFolderName) + 1);
	DIE(!root_dir.root->name, MALLOC_FAILED);
	strcpy(root_dir.root->name, rootFolderName);

	free(rootFolderName);

	root_dir.root->type = FOLDER_NODE;

	// Alloc memory for the root list
	root_dir.root->content = malloc(sizeof(FolderContent));
	DIE(!root_dir.root->content, MALLOC_FAILED);
	((FolderContent *)(root_dir.root->content))->children = malloc(sizeof(List));
	DIE(!((FolderContent *)(root_dir.root->content))->children, MALLOC_FAILED);
	((FolderContent *)(root_dir.root->content))->children->head = NULL;

	return root_dir;
}

// Function that frees the memory of all the file system
void freeTree(FileTree fileTree) {
	ListNode *node = ((FolderContent *)(fileTree.root->content))->children->head;
	if (node) {
		free_rec(node);
	}

	free(((FolderContent *)(fileTree.root->content))->children);
	free(fileTree.root->content);
	free(fileTree.root->name);
	free(fileTree.root);

	printf("\n");
}

// Function that lists the content of a node
void ls(TreeNode* currentNode, char* arg) {
	// List the content of root directory
	if (arg[0] == '\0') {
		ListNode* aux = ((FolderContent *)(currentNode->content))->children->head;
		while (aux) {
			printf("%s\n", aux->info->name);
			aux = aux->next;
		}
		return;
	}

	// Get the wanted node
	TreeNode *node = get_node(currentNode, arg);
	if (!node) {
		printf("ls: cannot access '%s': No such file or directory", arg);
		return;
	}
	// List the node's content depending its type
	if (node->type == FILE_NODE) {
		printf("%s: ", arg);
		printf("%s\n", ((FileContent *)(node->content))->text);
	} else {
		ListNode* aux = ((FolderContent *)(node->content))->children->head;
		while (aux) {
			printf("%s\n", aux->info->name);
			aux = aux->next;
		}
	}
}

// Function that prints the path to the current node
void pwd(TreeNode* treeNode) {
	// Create a string that memorizes the path
	char *path = NULL;
	char *aux = malloc(strlen(treeNode->name) + 1);
	DIE(!aux, MALLOC_FAILED);
	strcpy(aux, treeNode->name);

	// Search for the path until the root is found
	while (treeNode->parent) {
		path = realloc(path, strlen(treeNode->parent->name) + 2);
		strcpy(path, treeNode->parent->name);
		strcat(path, "/");

		path = realloc(path, strlen(path) + strlen(aux) + 1);
		strcat(path, aux);

		// Add the current node to the path
		aux = realloc(aux, strlen(path) + 1);
		DIE(!aux, MALLOC_FAILED);
		strcpy(aux, path);

		treeNode = treeNode->parent;
	}
	printf("%s\n", aux);
	free(aux);
	free(path);
}

// Function that changes directories until the wanted node is found
TreeNode* cd(TreeNode* currentNode, char* path) {
	TreeNode *initial_node = currentNode;
	char *path_copy = malloc(strlen(path) + 1);
	DIE(!path_copy, MALLOC_FAILED);
	strcpy(path_copy, path);

	// Go to the wanted directory
	char *p = strtok(path, "/");
	while (p) {
		if (!strcmp(p, PARENT_DIR)) {
			currentNode = currentNode->parent;
		} else {
			currentNode = get_node(currentNode, p);
			// Check if the node exists
			if (!currentNode) {
				printf("cd: no such file or directory: %s\n", path_copy);
				free(path_copy);
				return initial_node;
			}
		}
		p = strtok(NULL, "/");
	}
	free(path_copy);
	return currentNode;
}

// Function that recursively displays the content of a directory
void tree(TreeNode* currentNode, char* arg) {
	// Create the counters
	int directories = 0, files = 0, tabs = 0;

	char *path_copy = malloc(strlen(arg) + 1);
	DIE(!path_copy, MALLOC_FAILED);
	strcpy(path_copy, arg);

	// Go to the wanted directory
	if (arg[0] != '\0') {
		char *p = strtok(arg, "/");
		while (p) {
			currentNode = get_node(currentNode, p);
			if (!currentNode || currentNode->type == FILE_NODE) {
				printf("%s [error opening dir]\n\n0 directories, 0 files\n", path_copy);
				free(path_copy);
				return;
			}
		p = strtok(NULL, "/");
		}
	}

	// Display the content recursively
	ListNode* aux = ((FolderContent *)(currentNode->content))->children->head;
	print_rec(aux, &directories, &files, &tabs);
	printf("%d directories, %d files\n", directories, files);
	free(path_copy);
}

// Function that creates a new directory
void mkdir(TreeNode* currentNode, char* folderName) {
	// Check if the directory already exists
	if (get_node(currentNode, folderName)) {
		printf("mkdir: cannot create directory '%s': File exists\n",
			   folderName);
		free(folderName);
		return;
	}

	// Initialize a new directory node
	ListNode *new_dir = malloc(sizeof(ListNode));
	DIE(!new_dir, MALLOC_FAILED);
	new_dir->info = malloc(sizeof(TreeNode));
	DIE(!new_dir->info, MALLOC_FAILED);
	new_dir->info->parent = currentNode;
	new_dir->info->name = malloc(strlen(folderName) + 1);
	DIE(!new_dir->info->name, MALLOC_FAILED);
	strcpy(new_dir->info->name, folderName);
	free(folderName);
	new_dir->info->type = FOLDER_NODE;

	// Set the content of the new directory
	new_dir->info->content = malloc(sizeof(FolderContent));
	DIE(!new_dir->info->content, MALLOC_FAILED);
	((FolderContent *)(new_dir->info->content))->children = malloc(sizeof(List));
	DIE(!((FolderContent *)(new_dir->info->content))->children, MALLOC_FAILED);
	((FolderContent *)(new_dir->info->content))->children->head = NULL;

	// Set the links
	new_dir->next = ((FolderContent *)(currentNode->content))->children->head;
	((FolderContent *)(currentNode->content))->children->head = new_dir;
}

// Function that removes a file node from a specified directory
void rm(TreeNode* currentNode, char* fileName) {
	TreeNode *file = get_node(currentNode, fileName);

	if (file && file->type == FOLDER_NODE) {
		printf("rm: cannot remove '%s': Is a directory", fileName);
		return;
	}
	if (!file) {
		printf("rm: failed to remove '%s': No such file or directory\n", fileName);
		return;
	}

	// Rearrange the links
	ListNode *curr = ((FolderContent *)(currentNode->content))->children->head;
	ListNode *prev = NULL;

	if (!strcmp(curr->info->name, fileName)) {
		((FolderContent *)(currentNode->content))->children->head = curr->next;
		curr->next = NULL;
	} else {
		while (curr) {
			if (!strcmp(curr->info->name, fileName)) {
				prev->next = curr->next;
				curr->next = NULL;
				break;
			}
			prev = curr;
			curr = curr->next;
		}
	}

	free_rec(curr);
}

// Function that removes an empty directory
void rmdir(TreeNode* currentNode, char* folderName) {
	TreeNode *dir = get_node(currentNode, folderName);

	if (!dir) {
		printf("rmdir: failed to remove '%s': No such file or directory\n",
			   folderName);
		return;
	}

	if (dir && dir->type == FILE_NODE) {
		printf("rmdir: failed to remove '%s': Not a directory", folderName);
		return;
	}

	if (dir && ((FolderContent *)(dir->content))->children->head) {
		printf("rmdir: failed to remove '%s': Directory not empty\n",
			   folderName);
		return;
	}

	// Rearrange the links
	ListNode *curr = ((FolderContent *)(currentNode->content))->children->head;
	ListNode *prev = NULL;

	if (!strcmp(curr->info->name, folderName)) {
		((FolderContent *)(currentNode->content))->children->head = curr->next;
		curr->next = NULL;
	} else {
		while (curr) {
			if (!strcmp(curr->info->name, folderName)) {
				prev->next = curr->next;
				curr->next = NULL;
				break;
			}
			prev = curr;
			curr = curr->next;
		}
	}

	free_rec(curr);
}

// Function that recursively removes a directory
void rmrec(TreeNode* currentNode, char* resourceName) {
	if (!get_node(currentNode, resourceName)) {
		printf("rmrec: failed to remove '%s': No such file or directory\n",
			   resourceName);
		return;
	}

	TreeNode *resourceNode = get_node(currentNode, resourceName);

	if (resourceNode->type == FILE_NODE) {
		rm(currentNode, resourceName);
		return;
	}

	// Rearrange the links
	ListNode *curr = ((FolderContent *)(currentNode->content))->children->head;
	ListNode *prev = NULL;

	if (!strcmp(curr->info->name, resourceName)) {
		((FolderContent *)(currentNode->content))->children->head = curr->next;
		curr->next = NULL;
	} else {
		while (curr) {
			if (!strcmp(curr->info->name, resourceName)) {
				prev->next = curr->next;
				curr->next = NULL;
				break;
			}
			prev = curr;
			curr = curr->next;
		}
	}

	free_rec(curr);
}

// Function that creates a new file
void touch(TreeNode* currentNode, char* fileName, char* fileContent) {
	// Check if the directory already exists
	if (get_node(currentNode, fileName)) {
		free(fileName);
		free(fileContent);
		return;
	}

	// Initialize a new file node
	ListNode *new_file = malloc(sizeof(ListNode));
	DIE(!new_file, MALLOC_FAILED);
	new_file->info = malloc(sizeof(TreeNode));
	DIE(!new_file->info, MALLOC_FAILED);
	new_file->info->parent = currentNode;
	new_file->info->name = malloc(strlen(fileName) + 1);
	DIE(!new_file->info->name, MALLOC_FAILED);
	strcpy(new_file->info->name, fileName);
	free(fileName);
	new_file->info->type = FILE_NODE;

	// Set the content of the new file
	new_file->info->content = malloc(sizeof(FileContent));
	DIE(!new_file->info->content, MALLOC_FAILED);
	FileContent *new_file_content = new_file->info->content;
	new_file_content->text = NULL;

	// Initialize the content if any
	if (fileContent[0] != '\0' || fileContent) {
		new_file_content->text = malloc(strlen(fileContent) + 1);
		DIE(!new_file_content->text, MALLOC_FAILED);
		strcpy(new_file_content->text, fileContent);
	}
	free(fileContent);

	// Set the links
	new_file->next = ((FolderContent *)(currentNode->content))->children->head;
	((FolderContent *)(currentNode->content))->children->head = new_file;
}

// Function that copies the content of the source into a specified destination
void cp(TreeNode* currentNode, char* source, char* destination) {
	// Get the source node
	TreeNode *sourceNode = cd(currentNode, source);

	// Check if the node is a directory
	if (sourceNode->type == FOLDER_NODE) {
		printf("cp: -r not specified; omitting directory '%s'\n", source);
		return;
	}

	// Create a copy for the new content
	char *new_content = NULL;
	if (((FileContent *)(sourceNode->content))->text) {
		new_content = malloc(1 +
				      strlen(((FileContent *)(sourceNode->content))->text));
		DIE(!new_content, MALLOC_FAILED);
		strcpy(new_content, ((FileContent *)(sourceNode->content))->text);
	}

	char *new_name = malloc(strlen(sourceNode->name) + 1);
	DIE(!new_name, MALLOC_FAILED);
	strcpy(new_name, sourceNode->name);

	char *destination_copy = malloc(strlen(destination) + 1);
	DIE(!destination_copy, MALLOC_FAILED);
	strcpy(destination_copy, destination);

	// Go through the destination path
	char *p = strtok(destination_copy, "/");
	while (p) {
		if (!strcmp(p, PARENT_DIR)) {
			currentNode = currentNode->parent;
		} else {
			// Check if the specified path is valid
			if (!get_node(currentNode, p) && strcmp(p, sourceNode->name)) {
				printf("cp: failed to access '%s': Not a directory\n", destination);
				free(new_content);
				free(new_name);
				free(destination_copy);
				return;
			} else if ((get_node(currentNode, p))->type == FILE_NODE) {
				// If the node is a file, change the content
				rm(currentNode, p);
				strcpy(new_name, p);
			} else {
				currentNode = get_node(currentNode, p);
			}
		}
		p = strtok(NULL, "/");
	}

	// Check if there is an existing node with the source name
	if (get_node(currentNode, sourceNode->name))
		rm(currentNode, sourceNode->name);

	// Create the new file into the destination
	touch(currentNode, new_name, new_content);

	free(destination_copy);
}

// Function that moves the content of the source into a specified destination
void mv(TreeNode* currentNode, char* source, char* destination) {
	// Get the source node
	TreeNode *sourceNode = cd(currentNode, source);

	// Check if the node is a directory
	if (sourceNode->type == FOLDER_NODE) {
		ListNode *curr = ((FolderContent *)(currentNode->content))->children->head;
		ListNode *prev = NULL;

		// Point to the source node
		if (!strcmp(curr->info->name, source)) {
			((FolderContent *)(currentNode->content))->children->head = curr->next;
			curr->next = NULL;
		} else {
			while (curr) {
				if (!strcmp(curr->info->name, source)) {
					prev->next = curr->next;
					curr->next = NULL;
					break;
				}
				prev = curr;
				curr = curr->next;
			}
		}

		// Get the destination node
		TreeNode *destNode = cd(currentNode, destination);

		// Rearrange the links
		ListNode *aux = ((FolderContent *)(destNode->content))->children->head;
		curr->next = aux;
		((FolderContent *)(destNode->content))->children->head = curr;
		return;
	}

	// Create a copy for the new content
	char *new_content = NULL;
	if (((FileContent *)(sourceNode->content))->text) {
		new_content = malloc(1 +
				      strlen(((FileContent *)(sourceNode->content))->text));
		DIE(!new_content, MALLOC_FAILED);
		strcpy(new_content, ((FileContent *)(sourceNode->content))->text);
	}
	char *new_name = malloc(strlen(sourceNode->name) + 1);
	DIE(!new_name, MALLOC_FAILED);
	strcpy(new_name, sourceNode->name);

	char *destination_copy = malloc(strlen(destination) + 1);
	DIE(!destination_copy, MALLOC_FAILED);
	strcpy(destination_copy, destination);

	// Go through the destination path
	char *p = strtok(destination_copy, "/");
	while (p) {
		if (!strcmp(p, PARENT_DIR)) {
			currentNode = currentNode->parent;
		} else {
			// Check if the specified path is valid
			if (!get_node(currentNode, p) && strcmp(p, sourceNode->name)) {
				printf("mv: failed to access '%s': Not a directory\n", destination);
				free(new_content);
				free(new_name);
				free(destination_copy);
				return;
			} else if ((get_node(currentNode, p))->type == FILE_NODE) {
				// If the node is a file, change the content
				rm(currentNode, p);
				strcpy(new_name, p);
			} else {
				currentNode = get_node(currentNode, p);
			}
		}
		p = strtok(NULL, "/");
	}

	// Check if there is an existing node with the source name
	if (get_node(currentNode, sourceNode->name))
		rm(currentNode, sourceNode->name);

	// Delete the source file
	rm(sourceNode->parent, sourceNode->name);

	// Create the new file into the destination
	touch(currentNode, new_name, new_content);

	free(destination_copy);
}
