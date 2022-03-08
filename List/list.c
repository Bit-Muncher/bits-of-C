#include "list.h"
/* See list.h for documentation */

/**
 * Notice (2022-03-08): This code hasn't been tested yet. It might compile, it 
 * might not. Most of it is a copy of prior work I've done with a hefty 
 * sprinkling of documentation, so if something is broken, you can probably fix
 * it yourself. This file WILL be updated later if it doesn't compile in its 
 * current state.
 **/

struct ListNode
{
	/* next: The node after this one
	 * prev: The node prior to this one */
	ListNode *next, *prev;

	/* The data of a node is actually the memory immediately following the structs
	 * memory, so we don't keep a pointer in here (it'd just take up more space).
	 * This is done because C doesn't have type templating, so as a work around we
	 * can just specify the size of the type we'd like the node to contain and
	 * malloc enough memory for it when the node is created. This saves and extra
	 * malloc, and greatly improves performance while allowing one list type to
	 * work for any data type.
	 * 
	 * tldr: We always malloc enough space for the ListNode and it's data, and we
	 * access it's data via *(type*)(nodePtr + 1). A macro makes quick work of 
	 * this, and saves us the space of an extra pointer variable.
	 */
};

/* A macro to get the data of a node as a specific type. This yields the actual
 * data, and not a pointer. Make sure you're using the correct type, otherwise
 * you're liable to run into errors. */
#define list_node_data( nodePtr, type ) (type*)(nodePtr+1)

struct List
{
	/* Pointers to the first, last, and cursor node, respectively.
	 * The cursor is updated after each addition to or removal from the list. */
	ListNode *top, *bot, *cursor;

	/* The [len]gth of the list - i.e., the number of nodes in it. */
	unsigned int len;

	/* The index of the cursor node: */
	unsigned int ci; /* [c]ursor [i]ndex */
};

ListNode *list_new_node( size_t data_size )
{
	ListNode *node = malloc( sizeof(ListNode) + data_size );
	if ( node ){
		/* Only work on the node of the malloc was successful. NULL is returned
		 * otherwise. */
		node->next = node->prev = NULL;
	}

	return node;
}

void list_init( List *list )
{
	list->top = list->bot = list->cursor = NULL;
	list->len = 0;
}

void list_cleanup( List *list )
{
	while ( list->bot ){
		list->bot = list->bot->prev;
		free( list->bot->next );
	}
	
	/* Reset everything. This will likely be inlined by your compiler. */
	list_init( list );
}

List *list_new()
{
	List *list = malloc( sizeof(List) );
	if ( list ){
		init_list( list );
	}

	return list;
}

void list_del( List *list )
{
	list_cleanup( list );
	free( list );
}

void list_append( List *list, ListNode *node )
{
	if ( !list->top ){ /* list is empty */
		list->top = list->bot = list->cursor = node;
	}
	else {
		node->prev = list->bot;
		node->next = NULL; /* possibly redundant, but allows for removed nodes to be
		                    * re-inserted */
		list->bot->next = node;
		list->bot = list->cursor = node;
	}
}

void list_prepend( List *list, ListNode *node )
{
	if ( !list->top ){
		list->top = list->bot = list->cursor = node;
	}
	else {
		node->next = list->top;
		node->prev = NULL; /* possibly redundant, but allows for removed nodes to be
		                    * re-inserted */

		list->top->prev = node;
		list->top = list->cursor = node;
	}
}

ListNode* llist_get_node( List* l,
                          unsigned int i )
{
	/* Check bounds and simple cases: */
  if ( i >= l->len ){
		return NULL;
	}
  else if ( i == l->ci ){
    return l->cursor;
	}
  else if ( !i ){
		return l->top;
	}
  else if ( i == l->len - 1 ){
		return l->bot;
	}

  struct ListNode* cur; /* current node */

  /* Determine distance to each anchored node */
  long long int dc = i - l->ci; /* Distance from cur node. This*/
  long long int db = l->len - i; /* Distance from bot */

	/* Using an enum here just to make the direction of iteration very clear.
	 * In my production library I use a boolean for this. I don't think there is
	 * a sufficiently measurable performance difference, but the enum will likely
	 * use more stack space, depending on your compiler and compiler settigns. */
	enum {
		FORWARD,
		BACKWARD
	} dir = dc > 0 ? FORWARD : BACKWARD;

  if ( !dir ) dc = -dc; /* Avoid negative */

  if ( dc <= i &&
       dc <= db ){ /* Search from the cursor */
    cur = l->cursor;
  }
  else if ( i <= db ){ /* Search from the top */
    cur = l->top;
    dc = i;
    dir = FORWARD;
  }
  else { /* Search from the bottom */
    cur = l->bot;
    dc = db;
    dir = BACKWARD;
  }

  if ( dir == FORWARD ) { /* Forward iteration */
    while ( dc ){
      cur = cur->next;
      --dc;
    }
  }
  else { /* Backward iteration */
    while ( dc ){
      cur = cur->prev;
      --dc;
    }
  }

  l->cursor = cur;
  l->ci = i;

  return cur;
}

bool list_insert( List *list, ListNode *node, unsigned int i )
{
	/* Bound check: */
	if ( i > list->len ){
		return false;
	}

	/* Now check to see if we can make use of an existing function: */
	if ( !i ) {
		list_prepend( list, node );
		return true;
	}
	else if ( i == list->len ){
		list_append( list, node );
		return true;
	}

	/* This function is where we first see cursor logic in action.
	 * If the provided index <i> is closer to the cursor than the top or bottom of
	 * the list, then we will begin iteration from it. Otherwise, we iterate from
	 * the next closest node (the [top] or [bot] of the list) */

	struct ListNode* cur = get_node( list, i );

	/* Insert the new node where cur used to be.
	 *
	 * Funny store: Re-writing this made me realize that I had the order of 
	 * operations wrong in my production code! I so rarely insert() into lists
	 * that it wasn't spotted until the time of this writing! */

  node->next = cur;
  node->prev = cur->prev;
  cur->prev = node;
  node->prev->next = node;

  list->cursor = node;
  list->ci = i;

  ++list->len;

  return true;
}

void *list_get( List *list, unsigned int i )
{
	ListNode *node = get_node( list, i );
	if ( !node ){
	  return NULL;
	}

	/* This returns a pointer to the memory immediately following the nodes struct
	 * heap memory, which is where we've allocted space for the nodes data. 
	 * (node + 1) performs 'pointer arithmatic'. I suggest looking it up if you're
	 * unfamilliar with it. */
	return (void*)(node + 1);
}

void llist_splice( struct List* list, struct ListNode* node )
{
	/* This might be a point for optimization, but I've yet to require additional
	 * speed for my projects. */

  if ( list->cursor == node ){
		/* This block makes sure the cursor is safely updated it it is the node
		 * being spliced */

    if ( node->prev ){ /* favour using the previous node */
      --list->ci;
      list->cursor = node->prev;
    }
    else if ( node->next ){ /* settle for the next node */
      ++list->ci;
      list->cursor = node->next;
    }
    else{ /* this means the list is empty! */
 			/* Set everything to NULL and the size to 0: */
      list_init( list );
		}
  }

	/* Adjust pointers where required: */
  if ( node->prev ){
    node->prev->next = node->next;
  }
  else{
    list->top = node->next;
  }

  if ( node->next ){
    node->next->prev = node->prev;
  }
  else{
    list->bot = node->prev;
  }

	/* And finally, decrement the length */
  --(list->len);
}