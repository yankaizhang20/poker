//
// Created by zyk on 24-4-23.
//

#ifndef POKER_STATIC_CALL_CONSTRUCTER_H
#define POKER_STATIC_CALL_CONSTRUCTER_H

class Month
{
public:
    static Month Jan()
    {
        return Month(1);
    }

    static Month Feb()
    {
        return Month(2);
    }

private:
    explicit Month(int val) : val_(val)
    {
    }

#ifdef TEST
public:
    static int Get()
    {
        return get();
    }

private:
    int get() const
    {
        return _val;
    }
#endif

private:
    int val_;
};

#endif   // POKER_STATIC_CALL_CONSTRUCTER_H
