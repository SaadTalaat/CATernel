/**
 * @addtogroup Data-Structures
 * @{
 * @file htable.c
 * @author Saad Talaat
 * @brief Chained hash table
 */

#include <structs/htable.h>
/**
 * @fn void htable_init(htable_t*, uint32_t, uint32_t, uint32_t(*)(uint32_t), uint32t(*)(uint32_t));
 * @param htable_t* a non NULL hash-table pointer
 * @param uint32_t the number of buckets to be initalized
 * @param uint32_t the length of every bucket.
 * @param uint32_t(*)(uint32_t) the hash function
 * @param uint32_t(*)(uint32_t)
 * @brief Early Hash Table initalization function
 * @details
 * Early Hash Table initalization function is used to initalize structures that 
 * take place in memory before MM inialization.
 */
void
early_htable_init(htable_t *table, uint32_t buckets_count, uint32_t bucket_length,
			uint32_t (*hash)(uint32_t),
			uint32_t (*destroy)(uint32_t))
{
	int count,table_size,bucket_size;

	if(table == NULL)
		return;

	table_size 	= buckets_count*sizeof(uint32_t);
	bucket_size 	= bucket_length*sizeof(htable_node_t);
	table->buckets 	= buckets_count;
	table->bucket_length = bucket_length;
	table->size 	= 0;	// actual inserted values
	table->hash 	= hash;
	table->destroy 	= destroy;
	table->table 	= allocate(table_size, table_size);
	table->bucket_size = allocate(table_size, table_size);

	for(count = 0; count < buckets_count; count ++)
		table->table[count] = allocate(bucket_size,bucket_size);

	for(count = 0; count < bucket_length*2; count++)
		table->bucket_size[count] = 0;
	return;
}

/**
 * @fn void htable_init(htable_t *, uint32_t, uint32_t (*)(uint32_t), uint32_t (*)(uint32_t));
 * @param htable_t* a non NULL hash-table pointer
 * @param uint32_t the number of buckets to be initalized
 * @param uint32_t(*)(uint32_t) hashing function
 * @param uint32_t(*)(uint32_t) table destroy function
 * @brief Initalizes new hash table
 */
void
htable_init(htable_t *table, uint32_t buckets_count,
		uint32_t (*hash)(uint32_t),
		uint32_t (*destroy)(uint32_t))
{
	int count;
	if(table == NULL)
		return;
	/** FIXME
	 * 
	 * allocate space for table inside the htable once kmalloc is made.
	 */
	table->buckets 	= buckets_count;
	table->size 	= 0;
	table->hash 	= hash;
	table->destroy 	= destroy;
	for(count = 0; count < buckets_count; count ++)
		table->table[count] = NULL;
}


/**
 * @fn htable_destroy(htable_t*)
 * @param htable_t* the reference to hash table
 * @brief deletes hash table
 * @details
 * destroys hash tables buckets using the internal destroy function
 * if not it just frees the memory the HTable is using
 * Note: Early hash table SHOULD NOT be destroyed.
 */
void
htable_destroy(htable_t* table)
{
	int count;
	if(table == NULL)
		return;
	for(count =0; count < table->buckets; count++)
	{
		if(table->destroy == NULL)
			/* TODO
			 * Free the bucket by MM function
			 */
			continue;
		else
			table->destroy(count);
	}
	table = NULL;
	return;
}

/**
 * @fn htable_insert(htable_t*, uint32_t)
 * @param htable_t* reference to hash table
 * @param uint32_t the value to be stored.
 * @brief Inserts value into the hash table
 */
uint32_t
htable_insert(htable_t* table, uint32_t key, uint32_t value)
{

	uint32_t bucket_index, slot_index;

	if(table == NULL)
		return -1;
	
	bucket_index = table->hash(key) % table->buckets;
	slot_index = table->bucket_size[bucket_index];
	if (slot_index >= table->bucket_length)
		return -1;
	(&((htable_node_t *) table->table[bucket_index])[slot_index])->key = key;
	(&((htable_node_t *) table->table[bucket_index])[slot_index])->value = value;
	table->bucket_size[bucket_index]++;
	return 0;
}

/**
 * @fn htable_remove(htable_t*, uint32_t)
 * @param htable_t* reference to hash table
 * @param uint32_t the value to be stored.
 * @brief Inserts value into the hash table
 */
uint32_t
htable_remove(htable_t* table, uint32_t key)
{
	uint32_t bucket_index, slot_index,value;
	if(table == NULL)
		return -1;
	bucket_index = table->hash(key) % table->buckets;
	slot_index  = table->bucket_size[bucket_index];

	(&((htable_node_t *)table->table[bucket_index])[slot_index])->key = 0;
	value = (&((htable_node_t *)table->table[bucket_index])[slot_index])->value;
	(&((htable_node_t *)table->table[bucket_index])[slot_index])->value = 0;
	table->bucket_size[bucket_index]--;
	return value;
}

/**
 * @fn htable_get(htable_t*, uint32_t)
 * @param htable_t* reference to Hash table
 * @param uint32_t key to return its value or -1 if not found
 * @brief hash table look up function with O(n)
 */
uint32_t
htable_get(htable_t* table, uint32_t key)
{
	uint32_t bucket_index,slot_index,count;
	htable_node_t *node;
	if(table == NULL)
		return -1;
	bucket_index = table->hash(key) % table->buckets;
	slot_index = table->bucket_size[bucket_index];

	if(slot_index == 0)
		return -1;
	for(node = (&((htable_node_t *) table->table[bucket_index])[slot_index]);
		count >= 0; node = (&((htable_node_t *) table->table[bucket_index])[--slot_index]))
	{
		if(node->key == key)
			return node->value;
	}
	return -1;
}

/**
 * @}
 */
