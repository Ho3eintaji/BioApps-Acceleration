// Copyright EPFL contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    uint32_t *ptr = (uint32_t *)0x00000000;

    // Write ram blocks
    ptr = (uint32_t *)0x00010000;
    for (uint32_t word=0; word<128; word++)
    {
        *ptr = (uint32_t)0xabababab;
        ptr++;
    }

    ptr = (uint32_t *)0x00018000;
    for (uint32_t word=0; word<128; word++)
    {
        *ptr = (uint32_t)0xabababab;
        ptr++;
    }

    ptr = (uint32_t *)0x00020000;
    for (uint32_t word=0; word<128; word++)
    {
        *ptr = (uint32_t)0xabababab;
        ptr++;
    }

    ptr = (uint32_t *)0x00028000;
    for (uint32_t word=0; word<128; word++)
    {
        *ptr = (uint32_t)0xabababab;
        ptr++;
    }

    ptr = (uint32_t *)0x00030000;
    for (uint32_t word=0; word<128; word++)
    {
        *ptr = (uint32_t)0xabababab;
        ptr++;
    }

    ptr = (uint32_t *)0x00038000;
    for (uint32_t word=0; word<128; word++)
    {
        *ptr = (uint32_t)0xabababab;
        ptr++;
    }

    // Verify ram blocks
    ptr = (uint32_t *)0x00010000;
    for (uint32_t word=0; word<128; word++)
    {
        if (*ptr != (uint32_t)0xabababab)
        {
            printf("Fail.\n");
            return EXIT_FAILURE;
        }
        ptr++;
    }

    ptr = (uint32_t *)0x00018000;
    for (uint32_t word=0; word<128; word++)
    {
        if (*ptr != (uint32_t)0xabababab)
        {
            printf("Fail.\n");
            return EXIT_FAILURE;
        }
        ptr++;
    }

    ptr = (uint32_t *)0x00020000;
    for (uint32_t word=0; word<128; word++)
    {
        if (*ptr != (uint32_t)0xabababab)
        {
            printf("Fail.\n");
            return EXIT_FAILURE;
        }
        ptr++;
    }

    ptr = (uint32_t *)0x00028000;
    for (uint32_t word=0; word<128; word++)
    {
        if (*ptr != (uint32_t)0xabababab)
        {
            printf("Fail.\n");
            return EXIT_FAILURE;
        }
        ptr++;
    }

    ptr = (uint32_t *)0x00030000;
    for (uint32_t word=0; word<128; word++)
    {
        if (*ptr != (uint32_t)0xabababab)
        {
            printf("Fail.\n");
            return EXIT_FAILURE;
        }
        ptr++;
    }

    ptr = (uint32_t *)0x00038000;
    for (uint32_t word=0; word<128; word++)
    {
        if (*ptr != (uint32_t)0xabababab)
        {
            printf("Fail.\n");
            return EXIT_FAILURE;
        }
        ptr++;
    }

    printf("Success.\n");
    return EXIT_SUCCESS;
}
