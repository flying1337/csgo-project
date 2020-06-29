//
// Created by ruppet on 04.01.2020.
//

#ifndef uc-sflying-cm-_MEMORY_H
#define uc-sflying-cm-_MEMORY_H

class c_enginememory
{
public:
    virtual ~c_enginememory();
    virtual void* alloc(size_t nSize) = 0;
    virtual void* realloc(void* pMem, size_t nSize) = 0;
    virtual void free(void* pMem) = 0;
    virtual void* unused(void* pMem, size_t nSize) = 0;
    virtual size_t get_size(void* pMem) = 0;
};

#endif //uc-sflying-cm-_MEMORY_H
