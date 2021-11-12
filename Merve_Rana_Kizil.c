// CSE2025 Data Structures
// Project #1
// Merve Rana Kýzýl - 150119825

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <limits.h>

typedef struct Node {	
	struct Node* left;
	struct Node* right;
	char* word;
	int freq;
} Node;

Node* newNode(char* word, int freq);
Node* insertIntoBST(Node *node, char* word, int freq, unsigned int isWordKey);
Node* insertIntoBSTUtil(Node* root, char* word, int freq, unsigned int isWordKey);
void printInorder(Node* root, unsigned int isWordKey);
int getLevel(Node *node, char* word);
int getLevelUtil(Node *node, char* word, int level);
Node* constructBT(int arr[], char arr2[][100], Node* root, int i, int n);
int addToArray(Node *node, int arr[], char arr2[][100], int i);
int getNumberOfLines(char* filename);

int main() {
	
	char* filename = "input.txt";
	
	FILE* fp = fopen(filename, "r");
	
	if (fp == NULL) {
		printf("Could not open file %s", filename);
		return 0;
	}

	int size = getNumberOfLines(filename);
    char* token;
    char* word;
    int freq;
    int word_level;
    int freq_level;
    int total_access_time = 0;
    char word_arr[size][100];
    int freq_arr[size];
    int total_access_time_word = 0;
    int total_access_time_freq = 0;
       
    Node* word_root = NULL;
    Node* freq_root_bst = NULL;
    Node* freq_root_bt = NULL;
    
    // Read from the given input file
    if(fp != NULL) {
    	
        char line[1000];
        
        while(fgets(line, sizeof line, fp) != NULL) {
        	
        	// Split the string using strtok
        	token = strtok(line, ",");
        	
        	for(int i = 0; i < 2; i++) {
        				
        	token = strtok(NULL, ",");
        			
        		if(i == 0){
        			word = token;	
				}		
				else{					
					freq = atoi(token);		
				}		
			}
			
			word_root = insertIntoBSTUtil(word_root, word, freq, 1);
			freq_root_bst = insertIntoBSTUtil(freq_root_bst, word, freq, 0);
			
			word_level = getLevel(word_root, word);
			
			// Find the total access time in the BST
			total_access_time_word += word_level*freq;
        }
        
        fclose(fp);
    }
    
   	addToArray(freq_root_bst, freq_arr, word_arr, 0);
    freq_root_bt = constructBT(freq_arr, word_arr, freq_root_bst, 0, size);
    
    // Find the total access time in the BT
    for(int i = 0; i < size; i++) {
    	
    	word = word_arr[i];
    	freq = freq_arr[i];
    	
    	freq_level = getLevel(freq_root_bt, word);
	
    	total_access_time_freq += freq_level*freq;
	}


	printf(" Inorder traversal of BST that is built with the key \"Word\": \n");
	printInorder(word_root, 1);
	printf("\n Total Access Time in the BST that is built with the key \"Word\" : %d \n", total_access_time_word);
	printf("\n Inorder traversal of the BT: \n");
	printInorder(freq_root_bt, 0);  
    printf("\n Total Access Time in the BT: %d ", total_access_time_freq);
    
   	return 0;	
}

// Insert a new node
Node* newNode(char* word, int freq) {
	Node* temp = malloc(sizeof(Node));
	temp->word = strdup(word);
	temp->freq = freq;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

// Insert node in BST
Node* insertIntoBST(Node *node, char* word, int freq, unsigned int isWordKey) {

	if (node == NULL) {
		return newNode(word, freq);
	}
			
	if((stricmp(word, node->word) < 0 && isWordKey)  || ((freq < node->freq) && !isWordKey)) {
		// Insert left child
		node->left = insertIntoBST(node->left, word, freq, isWordKey);
	}
	else if((stricmp(word, node->word) >= 0 && isWordKey)  || ((freq >= node->freq) && !isWordKey)) { 
		// Insert right child 	
	   	node->right = insertIntoBST(node->right, word, freq, isWordKey);
	}		
    return node;
}

// A utility function to insert into a BST
Node* insertIntoBSTUtil(Node* root, char* word, int freq, unsigned int isWordKey){
	
	if(root != NULL)		
		insertIntoBST(root, word, freq, isWordKey);     	
    else
	    root = insertIntoBST(root, word, freq, isWordKey);
	    
	return root;
}

// Add the words and frequencies to arrays
int addToArray(Node *node, int arr[], char arr2[][100], int i) {
	
    if(node == NULL)
        return i;

    if(node->right != NULL)
        i = addToArray(node->right, arr, arr2, i);
          
    arr[i] = node->freq;
    strcpy(arr2[i], node->word);
    i++;
 	
 	if(node->left != NULL)
        i = addToArray(node->left, arr, arr2, i);
    
    return i;
}

// Print inorder traversal
void printInorder(Node* node, unsigned int isWordKey) {
	if (node == NULL)
        return;
    
    printInorder(node->left, isWordKey);
    if(isWordKey)
    	printf(" %s\n", node->word);  	
	else
		printf(" %d\n", node->freq);
		  
    printInorder(node->right, isWordKey);		
}

// Find the level of the node
int getLevelUtil(Node *node, char* word, int level) {
	
	if (node == NULL)
        return 0;
    
	if(stricmp(word, node->word) == 0)
        return level;
      	
    int downlevel = getLevelUtil(node->left, word, level + 1);
	if (downlevel != 0)
        return downlevel;
        
    downlevel = getLevelUtil(node->right, word, level + 1);
    return downlevel;
}

// A utility function to get the level of the node
int getLevel(Node *node, char* word) { 
    return getLevelUtil(node, word, 1);
}

// Insert node in BT using the arrays
Node* constructBT(int arr[], char arr2[][100], Node* root, int i, int n) {

    if (i < n) {
		
        Node* temp = newNode(arr2[i], arr[i]);
        root = temp;
  
        // Insert left child
        root->left = constructBT(arr, arr2, root->left, 2 * i + 1, n);
  
        // Insert right child
        root->right = constructBT(arr, arr2, root->right, 2 * i + 2, n);
    }
    
    return root;
}

// Count the number of lines in the given input file
int getNumberOfLines(char* filename) {
	int count = 0;
	FILE* fp = fopen(filename, "r");	
	for (char c = getc(fp); c != EOF; c = getc(fp)) 
        if (c == '\n')// Increment count if this character is newline
            count += 1;
	fclose(fp);
	return count;
}
