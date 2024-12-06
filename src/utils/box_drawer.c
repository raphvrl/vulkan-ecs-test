#include "box_drawer.h"

static const box_style_t single_line = {
    .top_left = '+',
    .top_right = '+',
    .bottom_left = '+',
    .bottom_right = '+',
    .horizontal = '-',
    .vertical = '|',
    .left_T = '+',
    .right_T = '+'
};

static void draw_top(usize width)
{
    putchar(single_line.top_left);
    for (usize i = 0; i < width; i++) {
        putchar(single_line.horizontal);
    }
    putchar(single_line.top_right);
    putchar('\n');
}

static void draw_bottom(usize width)
{
    putchar(single_line.bottom_left);
    for (usize i = 0; i < width; i++) {
        putchar(single_line.horizontal);
    }
    putchar(single_line.bottom_right);
    putchar('\n');
}

static void draw_content(const char *content, usize width)
{
    usize content_len = strlen(content);
    usize pos = 0;
    usize line_pos = 0;
    
    while (pos < content_len) {
        putchar(single_line.vertical);
        putchar(' ');
        line_pos = 0;
        
        while (pos < content_len && line_pos < width) {
            usize word_len = 0;
            while (
                pos + word_len < content_len && 
                content[pos + word_len] != ' ') {
                word_len++;
            }
            
            if (line_pos > 0 && line_pos + word_len >= width) {
                break;
            }
            
            while (word_len > 0 && line_pos < width) {
                putchar(content[pos]);
                pos++;
                line_pos++;
                word_len--;
            }
            
            if (
                line_pos < width - 1 && 
                pos < content_len && 
                content[pos] == ' ') {
                putchar(' ');
                pos++;
                line_pos++;
            }
        }   
      
        while (line_pos < width - 1) {
            putchar(' ');
            line_pos++;
        }    
  
        putchar(single_line.vertical);
        putchar('\n');
    }
}

static void draw_middle(usize width)
{
    putchar(single_line.left_T);
    for (usize i = 0; i < width; i++) {
        putchar(single_line.horizontal);
    }
    putchar(single_line.right_T);
    putchar('\n');
}

void draw_box(const char *content, usize width)
{
    draw_top(width);
    draw_content(content, width);
    draw_bottom(width);
}

void draw_box_2(const char *top, const char *bottom, usize width)
{
    draw_top(width);
    draw_content(top, width);
    draw_middle(width);
    draw_content(bottom, width);
    draw_bottom(width);
}