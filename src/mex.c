/*
 * Copyright (c) 2023-2024 Ian Marco Moffett and the Osmora Team.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Hyra nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#define LINE_LEN 16

static void
dump_line(const char *line, size_t len)
{
    /* The amount of bytes we write */
    const uint8_t BYTE_COUNT = 2;

    for (size_t i = 0; i < LINE_LEN; ++i) {
        if (i < len) {
            printf("%02X", line[i] & 0xFF);
        } else {
            printf("  ");
        }

        /* Put spacing between bytes */
        if (((i + 1) % BYTE_COUNT) == 0) {
            printf(" ");
        }
    }

    printf(" ");
    for (size_t i = 0; i < len; ++i) {
        if (line[i] > 31 && line[i] < 127) {
            printf("%c", line[i]);
        } else {
            printf(".");
        }
    }

    printf("\n");
}

static void
dump_file(FILE *fp)
{
    char *buf = malloc(LINE_LEN * sizeof(char));
    ssize_t count;
    size_t offset = 0;

    for (;;) {
        count = fread(buf, sizeof(char), LINE_LEN, fp);
        if (count <= 0) {
            break;
        }

        printf("%08X: ", offset);
        offset += LINE_LEN;
        dump_line(buf, count);
    }

    free(buf);
}

int
main(int argc, char **argv)
{
    FILE *fp;

    if (argc < 2 || argc > 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    fp = fopen(argv[1], "rb");

    if (fp == NULL) {
        perror("fopen");
        return -1;
    }

    dump_file(fp);
    fclose(fp);

    return 0;
}
