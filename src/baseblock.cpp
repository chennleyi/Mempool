#include "blocklist.h"

#include <stdlib.h>
#include <iostream>

BaseBlock::BaseBlock(BlockList* blocklist, int size)
    : blocklist_(blocklist), size_(size),
    prev_(NULL), next_(NULL), status_(READY)
{
    if (size > 0) blockaddr_ = malloc(size+sizeof(void*));
    dataaddr_ = blockaddr_ + sizeof(void*);
}

void BaseBlock::free()
{
    blocklist_->free(this);
}
