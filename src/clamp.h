#ifndef CLAMP_H
#define CLAMP_H


template <typename T> T min(T x, T lo)
{
    return x < lo ? x : lo;
}

template <typename T> T max(T x, T hi)
{
    return x > hi ? x : hi;
}

template <typename T> T clamp(T x, T lo, T hi)
{
    return min(max(x, lo), hi);
}

#endif /* CLAMP_H */
