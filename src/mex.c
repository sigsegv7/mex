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
