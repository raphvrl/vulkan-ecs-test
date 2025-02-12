#include "box_drawer.h"

static void draw_top(u32 w) {
    printf("┌");
    for (u32 i = 0; i < w; i++) {
        printf("─");
    }
    printf("┐\n");
}

static void draw_bottom(u32 w) {
    printf("└");
    for (u32 i = 0; i < w; i++) {
        printf("─");
    }
    printf("┘\n");
}

static void draw_middle(u32 w) {
    printf("├");
    for (u32 i = 0; i < w; i++) {
        printf("─");
    }
    printf("┤\n");
}

static void draw_content(const char *str, u32 w) {
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

        if (current_lin_len + word_len + 1 > (int)w) {
            for (u32 j = current_lin_len; j < w; j++) {
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

    for (u32 i = current_lin_len; i < w; i++) {
        printf(" ");
    }

    printf("│\n");
    free(str_copy);
}

void draw_box(const char *str, u32 w) {
    draw_top(w);
    draw_content(str, w);
    draw_bottom(w);
}

void draw_2box(const char *top, const char *bottom, u32 w) {
    draw_top(w);
    draw_content(top, w);
    draw_middle(w);
    draw_content(bottom, w);
    draw_bottom(w);
}