#ifndef TYPE_H
#define TYPE_H
#include <iostream>
#include <assert.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <omp.h>

#include <thread>
#include "src/filesystem.hpp"
#define PAGESIZE 4096
#define IOBUFFER 25165824

typedef int VertexID;
typedef int VertexNum;
typedef long EdgeNum;
typedef float Weight;

#endif
