#pragma once

#include <cstdlib>

// int
inline int randomIntCloseAOpenB(int a, int b) { return rand() % (b - a) + a; }

inline int randomIntABClose(int a, int b) { return rand() % (b - a + 1) + a; }

inline int randomIntOpenACloseB(int a, int b) { return rand() % (b - a) + a + 1; }

// float
inline double randomFloat01Close() { return rand() / double(RAND_MAX); }
inline double randomFloatABClose(double a, double b) { return a + randomFloat01Close() * (b -a); }