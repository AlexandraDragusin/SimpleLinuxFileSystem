## Simple Linux File System

### Description:

	For the implementation of this homework, we were able to work with data
	structures, especially with singly linked lists, in order to recreate the
	tree-like Linux File System. Although there are quite a lot of comments in the
	code, there are a few things that require additional explanation:

		1. TOUCH / MKDIR
		These commands involve creating a list node by initializing its content
		as a file or as a folder node, then adding it to the head of the current
		directory.
	
		2. LS
		This command goes through a node and lists the content of it.

		3. CD
		This command goes through the specified path in order to change the
		current directory.

		4. PWD
		This command involves displaying the full path name of the current
		directory, starting with the root. It works by going step by step through
		the parent directory of the current node.

		5. TREE
		Recursive printing command that involves a depth-indented listing of
		files. When entering a new directory, we made sure that the number of tabs
		was increased. When exiting the directory, the tabs were decreased.

		5. RMDIR / RM / RMREC
		These commands are used to delete a file or a directory. They work
		exactly as a basic function that removes a node from a linked list. There
		are a few specific situations of usage:

		a) RMDIR is used to delete an empty directory;
		b) RM is used to delete a file node and its content;
		c) RMREC is used to delete any type of directory or file;

		6. CP
		This command copies a file into a specified path by using 'touch' and
		'rm' functions.

		7. MV
		This command moves a file or a directory into a specified path. When
		moving files, the function works the same way as the 'cp' command, except
		that this time the source file is deleted. When moving a directory, we
		rearranged the links of the node that represents the source directory.

### Other comments:
	Although we are very happy with the final product, we think that we
	could have improved the modularization of the code, but this was a real
	challenge, since we had a skel to follow. From another perspective, we are
	glad that we were able to implement a lot of bash commands.