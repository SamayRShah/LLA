#include <stdlib.h>
#include <string.h>

#include <kv.h>

size_t Hash(const char*val, size_t capacity)
{
    //size_t hash = 0x0123456789abcdef;
    size_t hash = 0x13371337deadbeef;

    while (*val) {
        hash ^= *val;
        hash = hash << 8;
        hash += *val;
        val++;
    }
    return hash % capacity;
}

kv_t *kv_init(size_t capacity)
{
    if(capacity == 0) return NULL;

    kv_t *table = malloc(sizeof(kv_t));
    if(table == NULL) return NULL;

    table->capacity = capacity;
    table->count = 0;

    table->entries = calloc(sizeof(kv_entry_t), capacity);
    if(table->entries == NULL) return NULL;

    return table;
}

int kv_put(kv_t *db, const char*key, const char*value) 
{
    if(!db || !key || !value) return -1;

    size_t idx = Hash(key, db->capacity);

    for(size_t i = 0; i < db->capacity; i++)
    {
        size_t real_idx = (idx + i) % db->capacity;
        kv_entry_t *entry = &db->entries[real_idx];

        // key is set -> updating
        if(entry->key && entry->key != TOMBSTONE 
            && !strcmp(entry->key, key))
        {
            // allocate new val in heap
            char *newVal = strdup(value);
            if(!newVal) return -1;
            free(entry->value);
            entry->value = newVal;
            return 0;
        }

        // land in empty slot
        if(!entry->key || entry->key == TOMBSTONE)
        {
            // allocate new val in heap
            char *newVal = strdup(value);
            char *newKey = strdup(key);
            if(!newVal || !newKey) {
                free(newKey); free(newVal);
                return -1;
            }
            entry->key = newKey;
            entry->value = newVal;
            db->count++;
            return 0;
        }
    }

    // db is full
    return -2;
}

char *kv_get(kv_t *db, const char*key) {
    if(!db || !key) return NULL;

    size_t idx = Hash(key, db->capacity);

    for(size_t i = 0; i < db->capacity; i++)
    {
        size_t real_idx = (idx + i) % db->capacity;
        kv_entry_t *entry = &db->entries[real_idx];

        if(entry->key == NULL) return NULL;

        // correct key
        if(entry->key && entry->key != TOMBSTONE
             && !strcmp(entry->key, key))
        {
            return entry ->value;
        }
    }

    // key not found
    return NULL;
}

int kv_delete(kv_t *db, const char*key)
{
    if(!db || !key) return -1;

    size_t idx = Hash(key, db->capacity);

    for(size_t i = 0; i < db->capacity; i++)
    {
        size_t real_idx = (idx + i) % db->capacity;
        kv_entry_t *entry = &db->entries[real_idx];

        if(entry->key == NULL) return -1;

        // correct key
        if(entry->key && entry->key != TOMBSTONE
             && !strcmp(entry->key, key))
        {
            free(entry->value);
            free(entry->key);
            db->count--;
            entry->key = TOMBSTONE;
            entry->value = NULL;
            return 0;
        }
    }

    // key not found
    return -1;
}

void kv_free(kv_t *db)
{
    if(!db) return;
    for(size_t i = 0; i < db->capacity; i++)
    {
        kv_entry_t *e = &db->entries[i];
        if(e->key && e->key != TOMBSTONE){
            free(e->key); free(e->value);
            e->key = NULL; e->value = NULL;
        }
    }

    free(db->entries);
    free(db);
}