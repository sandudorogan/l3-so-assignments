#ifndef HASHTAB_H_
#define HASHTAB_H_

/**
 *
 */
int init_hashtable(list ***hashtable, const int size);

/**
 *
 */
void free_hashtable(list **hashtable, const int size);

int add_in_hash(list **hashtable, int hash_size, ENTRY_TYPE entry);

/**
 *
 */
void append_in_hash(list **hashtable, int hash_size, entry *to_be_added);

/**
 *
 */
int remove_from_hash(list **hashtable, int hash_size, ENTRY_TYPE entry,
		     const int (*compare_func)(const ENTRY_TYPE a,
					       const ENTRY_TYPE b));

/**
 *
 */
int find_in_hash(list **hashtable, int hash_size, ENTRY_TYPE entry,
		 const int (*compare_func)(const ENTRY_TYPE a,
					   const ENTRY_TYPE b));

/**
 *
 */
void empty_hashtable(list **hashtable, int hash_size);

/**
 *
 */
void print_hash_in_file(list **hashtable, int hash_size, FILE *file);

#endif