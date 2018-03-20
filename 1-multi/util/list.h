
#ifndef LIST_H_
#define LIST_H_

#define FOUND 1
#define NOT_FOUND 0

#define ENTRY_TYPE char *

typedef struct entry {
	ENTRY_TYPE elem;
	struct entry *prev;
	struct entry *next;
} entry;


typedef struct list {
	entry *first;
	entry *last;
} list;

/*
 *
 */
int init_list(list **sentinel);

/*
 * IMPORTANT: Modify this function depending on your @ENTRY_TYPE.
 */
void free_elem(ENTRY_TYPE elem);

/*
 * Return 1 if the list is empty, 0 otherwise.
 */
int is_empty_list(const list *sentinel);

/*
 *
 */
int add_in_list(list *sentinel, ENTRY_TYPE elem);

/*
 *
 */
int add_no_dup(list *sentinel, ENTRY_TYPE elem,
	       int (*compare_func)(const ENTRY_TYPE a, const ENTRY_TYPE b));

/*
 * @sentinel     - pointer to the sentinel.
 * @elem         - elem to be found.
 * @compare_func - a function to compare the @elem and the entries in the list.
 *      It's return value is treated as strcmp's.
 * @return       - the values for @FOUND and @NOT_FOUND
 */
int find_in_list(const list *sentinel, ENTRY_TYPE elem,
		 int (*compare_func)(const ENTRY_TYPE a, const ENTRY_TYPE b));

/*
 * @sentinel     - pointer to the sentinel.
 * @elem         - elem to be found.
 * @compare_func - a function to compare the @elem and the entries in the list.
 *      It's return value is treated as strcmp's.
 * @return       - a pointer to the entry in case it found the @elem,
 *      or a null pointer
 */
entry *find_in_list_get_pointer(const list *sentinel, ENTRY_TYPE elem,
				int (*compare_func)(const ENTRY_TYPE a,
						    const ENTRY_TYPE b));
/*
 * Removes an @elem containing box from the list.
 *
 * Returns @NOT_FOUND in case there is no such box.
 * @SUCCESS_CODE in case the operation was successful.
 */
int remove_from_list(list *sentinel, ENTRY_TYPE elem,
		     int (*compare_func)(const ENTRY_TYPE a,
					 const ENTRY_TYPE b));

/*
 * Deletes the links to last element from the list and returns it.
 *
 * Returns the last element of the given list.
 */
entry *take_last(list *sentinel);

/*
 * As @take_last.
 *
 * Returns the first element of the given list.
 */
entry *take_first(list *sentinel);

/*
 * Adds the @entry to the rightmost of the list, as the last element.
 * Takes care of the case when the list is empty.
 */
void add_at_end(list *sentinel, entry *to_be_added);

/*
 * Calls free for all the entries in the list.
 */
void empty_list(list *sentinel);

/*
 * As @empty_list, but also frees the sentinel.
 */
void free_list(list *sentinel);

/*
 * Prints to stdout all the elements in @sentinel.
 */
void print_as_string(const list *sentinel);


/*
 * Prints in @print_file all the elements in @sentinel.
 */
void print_list_in_file(const list *sentinel, FILE *print_file);

#endif
