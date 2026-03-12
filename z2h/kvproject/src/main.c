#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <kv.h>

int tmain()
{
    kv_t *db = kv_init(16);

    kv_put(db, "name", "alice");
    assert(strcmp(kv_get(db, "name"), "alice") == 0);
    assert(kv_get(db, "missing") == NULL);

    for(size_t i = 0; i < db->capacity; i++)
    {
        if(db->entries[i].key)
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