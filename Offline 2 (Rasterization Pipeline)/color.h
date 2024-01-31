#include <bits/stdc++.h>

static unsigned long int g_seed = 1;

inline int rand(){
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}
class Color{
public:
    int r,g,b;

    Color(){
        r = g = b = 0;
    }

    void set(){
        r = rand();
        g = rand();
        b = rand();
    }
};