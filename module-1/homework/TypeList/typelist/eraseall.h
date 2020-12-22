#pragma once

#include "typelist.h"

template<typename TList, typename TargetType>
struct EraseAll;

template<typename TargetType>
struct EraseAll<NullType, TargetType> {
    typedef NullType NewTypeList;
};

template<typename Head, typename Tail>
struct EraseAll<TypeList<Head, Tail>, Head> {
    typedef typename EraseAll<Tail, Head>::NewTypeList NewTypeList;
};

template<typename Head, typename Tail, typename TargetType>
struct EraseAll<TypeList<Head, Tail>, TargetType> {
    typedef TypeList<Head, typename EraseAll<Tail, TargetType>::NewTypeList> NewTypeList;
};