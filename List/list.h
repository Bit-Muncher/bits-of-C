/*
 * Description: A doubly-linked list with a cursor to allow O(1) sequential
 *              operations. Not thread safe. All functions that use pointers as
 *              arguments assume that the pointers are not NULL.
 * 
 *              Sequential operations are operations that mutate adjacent 
 *              indices in a list. E.g., in a new list that's just been 
 *              populated, list_get(100) might take 100 iterations to get, but
 *              calling get(101) afterwards will only require 1 iteration. This
 *              greatly speeds up iterating over a list, and avoids the 
 *              requirement of an iterator object at the expense of a few more
 *              bytes.
 * 
 * Author: Bit-Muncher (fickleuser@gmail.com)
 * Date of creation: 2022-03-08
 *
 * License: The Unlicense (https://unlicense.org)
 */

/* Header-guard to avoid multiple inclusions */
#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdbool.h> /* So we can use boolean logic in a more familiar way */
#include <stdlib.h> /* for size_t */

/* This header contains functions and declarations that are strictly required
 * for use of the List and ListNode structs. Everything else is hidden in the
 * C file. */

/* Forward declaration of the List structure. This library "hides" the internals
 * from people using the header file, and requires developers to use the
 * provided APIs. This keeps the list safe from accidental tampering. */
struct List;

/* This typedef means that instead of always typing 'struct List', we can now
 * type 'List' to the same effect. */
typedef struct List List;

/* Forward declaration of the ListNode structure. */
struct ListNode;
typedef struct ListNode ListNode;

/**
 * @brief Heap-allocate a new ListNode with <data_size> bytes of space. ListNode
 * objects should be immediately placed into a List for safe keeping. A ListNode
 * can be deleted using free(), which will also delete the corresponding node
 * data.
 * 
 * This function actually returns a pointer to a hunk of memory with the 
 * following memory layout:
 * [memory for ListNode struct | memory for node data (<data_size> bytes)]
 * 
 * @param data_size The number of bytes of space to allocate for node data.
 * @return ListNode* A pointer to the malloc'd ListNode.
 */
ListNode *list_new_node( size_t data_size );

/**
 * @brief Initialize the provided List. This allows easy initialization of both
 * heap and stack allocated List obejcts.
 * @param list A pointer to the list to initialize
 */
void list_init( List *list );

/**
 * @brief Cleanup memory used by the provided List. This deletes node data, but
 * not that which node data may point to. This allows easy cleanup of both heap
 * and stack allocated List objects.
 * @param list The list to cleanup.
 */
void list_cleanup( List *list );

/**
 * @brief Create a new List and return a pointer to it.
 * 
 * @return List* A pointer to a new list. Must be deallocated with list_del().
 */
List *list_new();

/**
 * @brief Deallocate the provided list. This function assumes <list> is a valid
 * pointer.
 */
void list_del( List *list );

/**
 * @brief Append <node> to the given <list>. Adds to the top of the list.
 * 
 * @param list The list to append to
 * @param node The node to append to the list
 */
void list_append( List *list, ListNode *node );

/**
 * @brief Prepend <node> to the given <list>. Adds to the bottom of the list.
 * 
 * @param list The list to prepend to
 * @param node The node to prepend to the list
 */
void list_prepend( List *list, ListNode *node );

/**
 * @brief Insert <node> into the given <list>. Inserts at the given index.
 * 
 * @param list The list to insert into
 * @param node The node to insert to the list
 * @param i The index to insert at
 * 
 * @return int index of insertion, of < 0 if the index is out of bounds.
 */
bool list_insert( List *list, ListNode *node, unsigned int i );

/**
 * @brief Remove a node from <list> at the given index.
 * 
 * @param list The list to remove from
 * @param i The index of the node to remove
 * 
 * @return ListNode* the removed node, or NULL if <i> is out of bounds.
 */
ListNode *list_remove( List *list, unsigned int i );

/**
 * @brief Get a pointer to a nodes data.
 * 
 * @param list The list to get from
 * @param i The index to get from
 * @return void* a pointer to the nodes data, or NULL if <i> is out of bounds.
 */
void *list_get( List *list, unsigned int i );

/**
 * @brief Get a pointer to a node. This is useful when you want to splice a node
 * then insert into another list.
 * 
 * @param list The list to get from
 * @param i The index to get from
 * @return ListNode* the node from <list> at index <i>, or NULL if <i> is out of
 * bounds.
 */
ListNode *list_get_node( List *list, unsigned int i );

/**
 * @brief Splice the given node from the given list.
 * If the node doesn't belong to the given list, things will probably break.
 * 
 */
void llist_splice( struct LList* list, struct LList_Node* node );

#endif /* LIST_H_INCLUDED */