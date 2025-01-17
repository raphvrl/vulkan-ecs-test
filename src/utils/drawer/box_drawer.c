#include "box_drawer.h"

static void draw_top(u32 width) {
    printf("┌");
    for (u32 i = 0; i < width; i++) {
        printf("─");
    }
    printf("┐\n");
}

static void draw_bottom(u32 width) {
    printf("└");
    for (u32 i = 0; i < width; i++) {
        printf("─");
    }
    printf("┘\n");
}

static void draw_middle(u32 width) {
    printf("├");
    for (u32 i = 0; i < width; i++) {
        printf("─");
    }
    printf("┤\n");
}

static void draw_content(const char *str, u32 width) {
    char words[MAX_WORDS][WORD_BUFFER];
    int word_count = 0;
    int current_lin_len = 0;

    char *str_copy = strdup(str);
    char *word = strtok(str_copy, " ");

    while (word != NULL && word_count < MAX_WORDS) {
        strcpy(words[word_count++], word);
        word = strtok(NULL, " ");
    }

    printf("│");
    for (int i = 0; i < word_count; i++) {
        int word_len = strlen(words[i]);

        if (current_lin_len + word_len + 1 > (int)width) {
            for (u32 j = current_lin_len; j < width; j++) {
                printf(" ");
            }

            printf("│\n│");
            current_lin_len = 0;
        }

        if (current_lin_len > 0) {
            printf(" ");
            current_lin_len++;
        }

        printf("%s", words[i]);
        current_lin_len += word_len;
    }

    for (u32 i = current_lin_len; i < width; i++) {
        printf(" ");
    }

    printf("│\n");
    free(str_copy);
}

void draw_box(const char *str, u32 width) {
    draw_top(width);
    draw_content(str, width);
    draw_bottom(width);
}

void draw_2box(const char *top, const char *bottom, u32 width) {
    draw_top(width);
    draw_content(top, width);
    draw_middle(width);
    draw_content(bottom, width);
    draw_bottom(width);
}