#ifndef _MYLIST_H_
#define _MYLIST_H_

/*
 * A ListNode in a linked list.
 */
struct ListNode {
    void *data;
    struct ListNode *next;
};

/*
 * A linked list.  
 * 'head' points to the first ListNode in the list.
 */
struct List {
    struct ListNode *head;
};

/*
 * Initialize an empty list.  
 */
static inline void initList(struct List *list)
{
    list->head = 0;
}

/*
 * In all functions below, the 'list' parameter is assumed to point to
 * a valid List structure.
 */

/*
 * Create a ListNode that holds the given data pointer,
 * and add the ListNode to the front of the list.
 *
 * Note that this function does not manage the lifetime of the object
 * pointed to by 'data'.
 * 
 * It returns the newly created ListNode on success and NULL on failure.
 */
struct ListNode *addFront(struct List *list, void *data);

/*
 * Traverse the list, calling f() with each data item.
 */
void traverseList(struct List *list, void (*f)(void *));

/*
 * Traverse the list, comparing each data item with 'dataSought' using
 * 'compar' function.  ('compar' returns 0 if the data pointed to by
 * the two parameters are equal, non-zero value otherwise.)
 *
 * Returns the first ListNode containing the matching data, 
 * NULL if not found.
 */
struct ListNode *findListNode(struct List *list, const void *dataSought,
	int (*compar)(const void *, const void *));

/*
 * Flip the sign of the double value pointed to by 'data' by
 * multiplying -1 to it and putting the result back into the memory
 * location.
 */
void flipSignDouble(void *data);

/*
 * Compare two double values pointed to by the two pointers.
 * Return 0 if they are the same value, 1 otherwise.
 */
int compareDouble(const void *data1, const void *data2);

/*
 * Returns 1 if the list is empty, 0 otherwise.
 */
static inline int isEmptyList(struct List *list)
{
    return (list->head == 0);
}

/*
 * Remove the first ListNode from the list, deallocate the memory for the
 * ndoe, and return the 'data' pointer that was stored in the ListNode.
 * Returns NULL is the list is empty.
 */
void *popFront(struct List *list);

/*
 * Remove all ListNodes from the list, deallocating the memory for the
 * ListNodes.  You can implement this function using popFront().
 */
void removeAllListNodes(struct List *list);

/*
 * Create a ListNode that holds the given data pointer,
 * and add the ListNode right after the ListNode passed in as the 'prevListNode'
 * parameter.  If 'prevListNode' is NULL, this function is equivalent to
 * addFront().
 *
 * Note that prevListNode, if not NULL, is assumed to be one of the ListNodes
 * in the given list.  The behavior of this function is undefined if
 * prevListNode does not belong in the given list.
 *
 * Note that this function does not manage the lifetime of the object
 * pointed to by 'data'.
 * 
 * It returns the newly created ListNode on success and NULL on failure.
 */
struct ListNode *addAfter(struct List *list, 
	struct ListNode *prevListNode, void *data);

/* 
 * Reverse the list.
 *
 * Note that this function reverses the list purely by manipulating
 * pointers.  It does NOT call malloc directly or indirectly (which
 * means that it does not call addFront() or addAfter()).
 *
 * Implementation hint: keep track of 3 consecutive ListNodes (previous,
 * current, next) and move them along in a while loop.  Your function
 * should start like this:


      struct ListNode *prv = NULL;
      struct ListNode *cur = list->head;
      struct ListNode *nxt;

      while (cur) {
          ......


 * And at the end, prv will end up pointing to the first element of
 * the reversed list.  Don't forget to assign it to list->head.
 */
void reverseList(struct List *list);

#endif /* #ifndef _MYLIST_H_ */
