#ifndef MIX_H
#define MIX_H

template <typename T, typename R> T mix(const T& left, const T& right, R x)
{
    return left + x * (right - left);
}

#endif /* MIX_H */
