#pragma once

template<typename Head, typename Tail> 
struct TypeList{
    typedef Head head;
    typedef Tail tail;
};

struct NullType {};