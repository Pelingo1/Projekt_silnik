#ifndef CLAMP_H
#define CLAMP_H

template <typename T>
T clamp(T value, T min, T max) {
    if (value < min)
        return min;
    else
        return max;
}

#endif //CLAMP_H
