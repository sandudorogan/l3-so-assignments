
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

	if (*sentinel == NULL)
		return ERROR_CODE;

	return SUCCESS_CODE;
}

int is_empty_list(const list *sentinel)
{
	return !sentinel->first;
}

int add_in_list(list *sentinel, ENTRY_TYPE elem)
{
	struct entry *buf = calloc(1, sizeof(struct entry));

	if (buf == NULL)
		return ERROR_CODE;

	buf->elem = elem;

	if (is_empty_list(sentinel)) {
		sentinel->first = buf;
		sentinel->last = buf;
	} else {
		sentinel->last->next = buf;
		buf->prev = sentinel->last;

		sentinel->last = buf;
	}

	return SUCCESS_CODE;
}

int add_no_dup(list *sentinel, ENTRY_TYPE elem,
	       int (*compare_func)(const ENTRY_TYPE a, const ENTRY_TYPE b))
{
	struct entry *proxy;
	struct entry *buf = calloc(1, sizeof(struct entry));

	if (buf == NULL)
		return ERROR_CODE;


	buf->elem = elem;

	if (is_empty_list(sentinel)) {
		sentinel->last = buf;
		sentinel->first = buf;
	} else {
		proxy = sentinel->first;

		do {
			if (!(*compare_func)(proxy->elem, elem)) {
				free(buf);
				return FOUND;
			}
			proxy = proxy->next;
		} while (proxy);
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
	struct entry *to_be_deleted;

	to_be_deleted = find_in_list_get_pointer(sentinel, elem, compare_func);
	if (!to_be_deleted)
		return NOT_FOUND;

	if (to_be_deleted->prev)
		to_be_deleted->prev->next = to_be_deleted->next;

	if (to_be_deleted->next)
		to_be_deleted->next->prev = to_be_deleted->prev;

	if (sentinel->first == to_be_deleted) {

		sentinel->first =
			to_be_deleted->next ? to_be_deleted->next : NULL;

		if (sentinel->first)
			sentinel->first->prev = NULL;
	}
	if (sentinel->last == to_be_deleted) {

		sentinel->last =
			to_be_deleted->prev ? to_be_deleted->prev : NULL;

		if (sentinel->last)
			sentinel->last->next = NULL;
	}

	// Delete the found element.
	free_elem(to_be_deleted->elem);
	free(to_be_deleted);

	return SUCCESS_CODE;
}

struct entry *take_last(list *sentinel)
{
	struct entry *proxy;

	if (is_empty_list(sentinel))
		return NULL;

	proxy = sentinel->last;
	sentinel->last = proxy->prev ? proxy->prev : NULL;

	if (!sentinel->last)
		sentinel->first = NULL;
	else {
		sentinel->last->next = NULL;
		proxy->prev = NULL;
	}

	return proxy;
}

struct entry *take_first(list *sentinel)
{
	struct entry *proxy;

	if (is_empty_list(sentinel))
		return NULL;

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

void add_at_end(list *sentinel, struct entry *to_be_added)
{
	if (is_empty_list(sentinel))
		sentinel->first = to_be_added;

	to_be_added->next = NULL;
	to_be_added->prev = sentinel->last;

	if (sentinel->last)
		sentinel->last->next = to_be_added;


	sentinel->last = to_be_added;
}

int find_in_list(const list *sentinel, ENTRY_TYPE elem,
		 int (*compare_func)(const ENTRY_TYPE a, const ENTRY_TYPE b))
{
	struct entry *proxy;

	if (is_empty_list(sentinel))
		return NOT_FOUND;

	proxy = sentinel->first;

	do {
		if (!(*compare_func)(proxy->elem, elem))
			return FOUND;

		proxy = proxy->next;
	} while (proxy);

	return NOT_FOUND;
}

struct entry *find_in_list_get_pointer(const list *sentinel, ENTRY_TYPE elem,
				int (*compare_func)(const ENTRY_TYPE a,
						    const ENTRY_TYPE b))
{
	struct entry *proxy;

	if (is_empty_list(sentinel))
		return NULL;

	proxy = sentinel->first;

	do {
		if (!(*compare_func)(proxy->elem, elem))
			break;

		proxy = proxy->next;
	} while (proxy);

	return proxy;
}

void empty_list(list *sentinel)
{
	struct entry *proxy;

	if (is_empty_list(sentinel))
		return;

	proxy = sentinel->first;

	while (proxy->next) {
		if (proxy->prev) {
			free_elem(proxy->prev->elem);
			free(proxy->prev);
		}
		proxy = proxy->next;
	}

	free_elem(proxy->elem);
	free(proxy);

	sentinel->first = NULL;
	sentinel->last = NULL;
}

void free_list(list *sentinel)
{
	empty_list(sentinel);
	free(sentinel);
}

void print_as_string(const list *sentinel)
{
	struct entry *proxy;

	if (is_empty_list(sentinel))
		return;

	proxy = sentinel->first;

	do {
		printf("%s ", proxy->elem);

		proxy = proxy->next;
	} while (proxy);
	printf("\n");

	fflush(stdout);
}

void print_list_in_file(const list *sentinel, FILE *print_file)
{
	struct entry *proxy;

	if (is_empty_list(sentinel))
		return;

	proxy = sentinel->first;

	do {
		fprintf(print_file, "%s ", proxy->elem);

		proxy = proxy->next;
	} while (proxy);
	fprintf(print_file, "\n");

	fflush(print_file);
}

#endif
