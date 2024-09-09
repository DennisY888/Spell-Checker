#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"


// a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;


// hash table starting size
unsigned int N = 1024;
int word_count = 0;

// Hash table
node **hashtable = NULL;  // initialize dynamically

// max load factor
const float MAX_LOAD_FACTOR = 0.75;

// Function declarations
bool resize();
unsigned int hash_index(const char *word);



// return true if word is in dictionary
bool check(const char *word)
{
    // create a copy of lower-cased word
    int n = strlen(word);
    char word_copy[LENGTH + 1];
    for (int i = 0; i < n; i++)
    {
        word_copy[i] = tolower(word[i]);
    }
    word_copy[n] = '\0';

    // get index
    int h = hash_index(word_copy);

    // traverse linked list (possibly) at index
    node *cursor = hashtable[h];
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word_copy) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}



// hashes word
unsigned int hash_index(const char *word)
{
    unsigned int hash = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash = (hash << 2) ^ word[i];
    }
    return hash % N;
}



// Loads dictionary into memory, returning true if successful
bool load(const char *dictionary)
{
    // allocate initial hash table
    hashtable = calloc(N, sizeof(node *));
    if (hashtable == NULL)
    {
        return false;
    }

    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // Scans dictionary word by word (populating hash table)
    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        // calculate load factor
        float load_factor = (float)word_count / N;
        if (load_factor > MAX_LOAD_FACTOR)
        {
            // resize hash table if load factor > 0.75
            if (!resize())
            {
                fclose(file);
                unload();
                return false;
            }
        }

        // malloc a new node for word
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        strcpy(new_node->word, word);

        int h = hash_index(new_node->word);

        // insert node into hash table
        new_node->next = hashtable[h];
        hashtable[h] = new_node;
        word_count++;
    }
    fclose(file);
    return true;
}



// dynamic resizing
bool resize()
{
    // doubles hash table size
    unsigned int new_size = N * 2;
    node **new_table = calloc(new_size, sizeof(node *));
    if (new_table == NULL)
    {
        return false;
    }

    // rehash all current words in hash table
    for (unsigned int i = 0; i < N; i++)
    {
        node *cursor = hashtable[i];
        while (cursor != NULL)
        {
            node *next = cursor->next; 

            // Rehash the word into the new table
            unsigned int new_hash = hash_index(cursor->word) % new_size;
            cursor->next = new_table[new_hash];
            new_table[new_hash] = cursor;

            cursor = next;
        }
    }

    // free the old table and point to new table
    free(hashtable);
    hashtable = new_table;
    N = new_size;
    return true;
}



// current number of words in dictionary
unsigned int size(void)
{
    return word_count;
}



// unload dictionary from memory
bool unload(void)
{
    // Free all nodes in the hash table
    for (unsigned int i = 0; i < N; i++)
    {
        node *cursor = hashtable[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }

    free(hashtable);
    return true;
}
