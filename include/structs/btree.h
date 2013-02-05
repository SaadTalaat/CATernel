/**
 * @file btree.h
 * @author Saad Talaat
 * @brief The B-Trees data structure
 */
#include <types.h>
#include <arch/x86/page.h>

typedef struct btree_node{
	btree_node *parent;
	btree_node *left;
	btree_node *right;
	uint32_t key;
	uint32_t val;
} btree_node_t;

typedef struct {
	btree_node_t*		root;
	uint32_t 		size;
	
	LIST_ENTRY(btree_t)	link;
} btree_t;

LIST_HEAD(Btree_List, btree_t);
extern void btree_init(btree_t *);
extern void btree_insert(btree_t, uint32_t key, uint32_t value);
extern void btree_remove(btree_t, uint32_t key);
extern void btree_get(btree_t, uint32_t key);
