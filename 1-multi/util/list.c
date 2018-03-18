
#ifndef LIST_C_
#define LIST_C_

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "list.h"


void free_elem(ENTRY_TYPE elem)
{
	free(elem);
}

int init_list(list **sentinel)
{
	*sentinel = calloc(1, sizeof(list));
	/*sentinel->first and last are already null*/
	if (*sentinel == NULL) {
		return ERROR_CODE;
	}
	return SUCCESS_CODE;
}

int is_empty_list(list *sentinel) 
{
	return !sentinel->first;
}

int add_in_list(list *sentinel, ENTRY_TYPE elem)
{
	entry *buf = calloc(1, sizeof(entry));
	/*prev and next are null.*/
	if (buf == NULL) {
		return ERROR_CODE;
	}

	buf->elem = elem;

	if (sentinel->first == NULL) {
		sentinel->first = buf;
		sentinel->last = buf;
	} else {
		/*
		 * Because we know the last element, adding is just a matter
		 * of linking.
		 */

		sentinel->last->next = buf;
		buf->prev = sentinel->last;

		sentinel->last = buf;
	}

	return SUCCESS_CODE;
}

int add_no_dup(list *sentinel, ENTRY_TYPE elem,
	       int (*compare_func)(const ENTRY_TYPE a, const ENTRY_TYPE b))
{
	entry *proxy;
	entry *buf = calloc(1, sizeof(entry));
	/*prev and next are null.*/
	if (buf == NULL) {
		return ERROR_CODE;
	}

	buf->elem = elem;

	if (sentinel->first == NULL) {
		sentinel->first = buf;
		sentinel->last = buf;
	} else {
		proxy = sentinel->first;

		do {
			if (!(*compare_func)(proxy->elem, elem)) {
				return SUCCESS_CODE;
			}
			proxy = proxy->next;
		} while (proxy);
		/*
		 * Because we know the last element, adding is just a matter
		 * of linking.
		 */

		sentinel->last->next = buf;
		buf->prev = sentinel->last;

		sentinel->last = buf;
	}

	return SUCCESS_CODE;
	
}

int remove_from_list(list *sentinel, ENTRY_TYPE elem,
		     int (*compare_func)(const ENTRY_TYPE a,
					 const ENTRY_TYPE b))
{
	entry *to_be_deleted;

	to_be_deleted = find_in_list_get_pointer(sentinel, elem, compare_func);
	if (!to_be_deleted) {
		return NOT_FOUND;
	}

	if (to_be_deleted->prev) {
		to_be_deleted->prev->next = to_be_deleted->next;
	}
	if (to_be_deleted->next) {
		to_be_deleted->next->prev = to_be_deleted->prev;
	}
	if (sentinel->first == to_be_deleted) {
		// If its the last item, assign null to the first.
		sentinel->first =
			to_be_deleted->next ? to_be_deleted->next : NULL;
		// If its not the last, current's first prev should be null.
		if (sentinel->first)
			sentinel->first->prev = NULL;
	}
	if (sentinel->last == to_be_deleted) {
		sentinel->last =
			to_be_deleted->prev ? to_be_deleted->prev : NULL;
		if (sentinel->last) {
			sentinel->last->next = NULL;
		}
	}
	free(to_be_deleted);

	return SUCCESS_CODE;
}

entry *take_last(list *sentinel)
{
	entry *proxy;

	if (!sentinel->last) {
		return NULL;
	}

	// Update the last pointer.
	// The first elem has no prev.
	proxy = sentinel->last;
	sentinel->last = proxy->prev ? proxy->prev : NULL;

	if (!sentinel->last) {
		// It was the first element, update the first pointer.
		sentinel->first = NULL;
	} else {
		// It wasn't the only elemnt in the list.
		sentinel->last->next = NULL;
		proxy->prev = NULL;
	}

	return proxy;
}

entry *take_first(list *sentinel)
{
	entry *proxy;

	if (!sentinel->first) {
		return NULL;
	}

	proxy = sentinel->first;
	if (sentinel->first == sentinel->last) {
		sentinel->last = NULL;
		sentinel->first = NULL;
	} else {
		sentinel->first = sentinel->first->next;
		sentinel->first->prev = NULL;
		proxy->next = NULL;
	}

	return proxy;
}

void add_at_end(list *sentinel, entry *to_be_added)
{
	if (!sentinel->first) {
		sentinel->first = to_be_added;
	}

	to_be_added->next = NULL;
	to_be_added->prev = sentinel->last;

	if (sentinel->last) {
		sentinel->last->next = to_be_added;
	}

	sentinel->last = to_be_added;
}

int find_in_list(const list *sentinel, ENTRY_TYPE elem,
		 int (*compare_func)(const ENTRY_TYPE a, const ENTRY_TYPE b))
{
	entry *proxy = sentinel->first;

	do {
		if (!(*compare_func)(proxy->elem, elem)) {
			return FOUND;
		}

		proxy = proxy->next;
	} while (proxy);

	return NOT_FOUND;
}

entry *find_in_list_get_pointer(const list *sentinel, ENTRY_TYPE elem,
				int (*compare_func)(const ENTRY_TYPE a,
						    const ENTRY_TYPE b))
{
	entry *proxy = sentinel->first;

	do {
		if (!(*compare_func)(proxy->elem, elem)) {
			return proxy;
		}

		proxy = proxy->next;
	} while (proxy);

	return NULL;
}

void empty_list(list *sentinel)
{
	if (is_empty_list(sentinel)) {
		return;
	}

	entry *proxy = sentinel->first;

	// Go to the last.
	do {
		// Free the previous.
		if (proxy->prev) {
			free_elem(proxy->prev->elem);
			free(proxy->prev);
		}
		// Go to the next.
		proxy = proxy->next;
	} while (proxy);
	// Free the last one.
	free(proxy);

	sentinel->first = NULL;
	sentinel->last = NULL;

	return;
}

void free_list(list *sentinel)
{
	empty_list(sentinel);
	free(sentinel);
}

void print_as_string(const list *sentinel)
{
	// See if there are things to print.
	if (!sentinel->first) {
		return;
	}

	entry *proxy = sentinel->first;

	do {
		printf("%s ", proxy->elem);

		proxy = proxy->next;
	} while (proxy);
	printf("\n");

	fflush(stdout);
	return;
}

void print_list_in_file(const list *sentinel, FILE *print_file)
{
	// See if there are things to print.
	if (!sentinel->first) {
		return;
	}

	entry *proxy = sentinel->first;

	do {
		fprintf(print_file, "%s ", proxy->elem);

		proxy = proxy->next;
	} while (proxy);
	fprintf(print_file, "\n");

	fflush(print_file);
	return;
}

#endif
