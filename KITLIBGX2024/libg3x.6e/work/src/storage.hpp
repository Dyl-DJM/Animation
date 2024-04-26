#pragma once

#include <assert.h>
#include <stdlib.h>

template <class T, std::size_t MaxSize>
struct Storage
{

    Storage()
        : tab{(T *)malloc(sizeof(T) * MaxSize)}
    {
    }

    const T &operator[](std::size_t i) const
    {
        assert(i < size && "Index out of range.\n");
        return tab[i];
    }

    T &operator[](std::size_t i)
    {
        assert(i < size && "Index out of range.\n");
        return tab[i];
        stackLastIdx = i > stackLastIdx ? i : stackLastIdx;
    }

    void pushElement(T elem)
    {
        size++;
        assert(size <= MaxSize && "Couldn't add more elements.\n");
        assert(stackLastIdx < size && "Index out of range.\n");
        tab[stackLastIdx] = elem;
        stackLastIdx++;
    }

    unsigned int stackLastIdx = 0;
    unsigned int size = 0;
    T *tab;
};