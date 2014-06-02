// File: defines.h
// Author: Jarielle Catbagan
// Created: 30, May 2014

#include <stdio.h>

#ifndef DEFINES_H
#define DEFINES_H

#define STDERR(str, ...) fprintf(stderr, "error: " str, ##__VA_ARGS__)
#define STDOUT(str, ...) fprintf(stdout, str, ##__VA_ARGS__)
#define DEBUG(str, ...) fprintf(stdout, "debug: " str, ##__VA_ARGS__)
#define EFAILURE exit(EXIT_FAILURE);
#define ESUCCESS exit(EXIT_SUCCESS);

typedef struct binval_parameters_t {
        uint16_t address;
        uint8_t data;
} binval_parameters_t;

typedef enum validity_status_t {UNKNOWN = 0, INVALID, VALID} validity_status_t;

#endif
