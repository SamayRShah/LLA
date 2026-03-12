#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <kv.h>

int tmain()
{
    kv_t *db = kv_init(16);

    kv_put(db, "name", "alice");
    kv_put(db, "city", "berlin");

    assert(kv_delete(db, "name") == 0);
    assert(kv_get(db, "name") == NULL);
    assert(db->count == 1);

    assert(kv_delete(db, "missing") == -1);

    for(size_t i = 0; i < db->capacity; i++)
    {
        if(db->entries[i].key && db->entries[i].key != TOMBSTONE)
        {
            printf("[%lu] %s: %s\n", 
                i,
                db->entries[i].key,
                db->entries[i].value
            );
        }
    }

    kv_free(db);
}