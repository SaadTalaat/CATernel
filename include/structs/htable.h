/**
 * @file htable.h
 * @author Saad Talaat
 * @brief Chained Hash Table
 */

#include <types.h>

typedef struct {
	void **table;
	uint32_t buckets;
	uint32_t (*hash)(uint32_t);
	uint32_t (*destroy)(uint32_t);
	uint32_t size;
} htable_t;


extern void htable_init(htable_t *, uint32_t, uint32_t (*hash)(uint32_t), uint32_t (*destroy)(uint32_t));
extern void htable_destroy(htable_t *);
extern void htable_insert(htable_t *);
extern uint32_t htable_remove(htable_t *);
extern void table_lookup(htable_t *, value);

