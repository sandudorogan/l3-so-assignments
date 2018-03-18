#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "list.h"
#include "utils.h"

int init_hashtable(list ***hashtable, const int size)
{
	int i;
	int er;

	*hashtable = malloc(size * sizeof(list));
	if (!*hashtable) {
		return ERROR_CODE;
	}

	for (i = 0; i < size; ++i) {
		list *sentinel;
		er = init_list(&sentinel);
		if (er == ERROR_CODE) {
			return ERROR_CODE;
		}

		(*hashtable)[i] = sentinel;
	}

	return SUCCESS_CODE;
}

void free_hashtable(list **hashtable, const int size)
{
	int i;

	for (i = 0; i < size; ++i) {
		free_list(hashtable[i]);
	}

	free(hashtable);
}

int add_in_hash(list **hashtable, int hash_size, ENTRY_TYPE entry)
{
	int word_pos;

	word_pos = hash(entry, hash_size);
	return add_no_dup(hashtable[word_pos], entry, &strcmp);
}

void append_in_hash(list **hashtable, int hash_size, entry *to_be_added)
{
	int word_pos;

	word_pos = hash(to_be_added->elem, hash_size);
	add_at_end(hashtable[word_pos], to_be_added);
}

int remove_from_hash(list **hashtable, int hash_size, ENTRY_TYPE entry,
		     int (*compare_func)(const ENTRY_TYPE a,
					 const ENTRY_TYPE b))
{
	int word_pos;

	word_pos = hash(entry, hash_size);
	return remove_from_list(hashtable[word_pos], entry, compare_func);
}

int find_in_hash(list **hashtable, int hash_size, ENTRY_TYPE entry,
		 int (*compare_func)(const ENTRY_TYPE a, const ENTRY_TYPE b))
{
	int word_pos;

	word_pos = hash(entry, hash_size);
	return find_in_list(hashtable[word_pos], entry, &strcmp);
}

void empty_hashtable(list **hashtable, int hash_size)
{
	int idx;

	for (idx = 0; idx < hash_size; idx++) {
		empty_list(hashtable[idx]);
	}
}

void print_hash_in_file(list **hashtable, int hash_size, FILE *file)
{
	int idx;

	for (idx = 0; idx < hash_size; idx++) {
		print_list_in_file(hashtable[idx], file);
	}
}
