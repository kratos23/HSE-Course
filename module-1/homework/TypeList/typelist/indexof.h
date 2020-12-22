#pragma once

#include "typelist.h"

template<typename TList, typename TargetType>
struct IndexOf;

template<typename TargetType> struct IndexOf<NullType, TargetType> {
    static const int pos = -1;
};

template<typename Head, typename Tail> struct IndexOf<TypeList<Head, Tail>, Head> {
    static const int pos = 0;
};

template<typename Head, typename Tail, typename TargetType> struct IndexOf<TypeList<Head, Tail>, TargetType> {
    static constexpr int indexShift(int index) {
        if (index == -1) {
            return -1;
        } else {
            return index + 1;
        }
    }

    static const int pos = indexShift(IndexOf<Tail, TargetType>::pos);
};