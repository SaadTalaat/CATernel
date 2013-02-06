/**
 * @file htable.h
 * @author Saad Talaat
 * @brief Chained Hash Table
 */

#include <types.h>
#include <arch/x86/mm/page.h>

/**
 * @name Hash Table
 * @{
 * @struct htable_t
 * @brief a hash table structure
 * @details
 * Hash table holds a pointer-to-pointer which is considered the actual table,
 * It also holds the bucket_size array which tracks the size of buckets
 * the buckets which represents the number of buckets, a hash function
 * passed on inialization time, same as destroy function.
 * the overall size which is used to determine the overall inserted values count.
 */

typedef struct {
	uint32_t key;
	uint32_t value;
} htable_node_t;

typedef struct {
	void **table;
	uint32_t buckets;
	uint32_t *bucket_size;
	uint32_t (*hash)(uint32_t);
	uint32_t (*destroy)(uint32_t);
	uint32_t size;
	uint32_t bucket_length;
} htable_t;



/**
 *@}
 */
extern void htable_init(htable_t *, uint32_t, uint32_t (*hash)(uint32_t), uint32_t (*destroy)(uint32_t));
extern void htable_destroy(htable_t *);
extern uint32_t htable_insert(htable_t *, uint32_t key, uint32_t value);
extern uint32_t htable_remove(htable_t *, uint32_t key);
extern uint32_t table_lookup(htable_t *, uint32_t value);

