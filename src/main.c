#include <stdio.h>
#include <assert.h>
#include <kv.h>

int main() {
    kv_t *db = kv_init(16);
    assert(db != NULL);
    assert(db->capacity == 16);
    assert(db->count == 0);
    free(db);
}