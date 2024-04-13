#include <string.h>
#include <iostream>

#include "memorypool.h"

MemoryPool::MemoryPool(BaseBlockFactory* factory)
    : factory_(factory)
{
    int blocksize = MIN_BLOCK_SIZE;
    for (int i=0;i<INIT_LISTS;++i)
    {
        bllist_.push_back(new BlockList(blocksize, factory));
        blocksize <<= 1;
    }
}

MemoryPool::~MemoryPool()
{
    for (auto i:bllist_)
    {
        delete i;
    }
}

BaseBlock* MemoryPool::blockalloc(int size)
{
  // 确保请求的大小至少为MIN_BLOCK_SIZE
  if (size < MIN_BLOCK_SIZE) {
      size = MIN_BLOCK_SIZE;
  }
  int blockSize = MIN_BLOCK_SIZE;
  int count = 0;
  // 线性搜索合适大小的块
  for (count = 0; count < bllist_.size(); ++count) {
      if (blockSize >= size) {
          // 找到了合适大小的块
          break;
      }
      blockSize *= 2; // 考虑下一个大小等级
  }
  // 如果现有BlockList不足以满足大小要求，则扩展
  if (count == bllist_.size()) {
      std::lock_guard<std::mutex> lock(list_lock_);
      bllist_.push_back(new BlockList(blockSize, factory_));
  }
  return bllist_[count]->malloc();
}

void* MemoryPool::malloc(int size)
{
    BaseBlock* target = blockalloc(size);
    memcpy(target->blockaddr(), &target, sizeof(void*));

    return target->dataaddr();
}

BaseBlock* MemoryPool::getblock(void* dataaddr)
{
    BaseBlock* target;
    memcpy(&target, dataaddr-sizeof(void*), sizeof(void*));

    return target;
}

void MemoryPool::free(void* dataaddr)
{
    BaseBlock* target = getblock(dataaddr);
    target->free();
}

void MemoryPool::travel()
{
    std::cout << "============\n";
    for (auto i:bllist_)
    {
        std::cout << "Size: " << i->blocksize() << " " << i->inuse << "/" << i->total() << "\n";
    }
    std::cout << "============\n";
}