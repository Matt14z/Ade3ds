#include "compiler.h"
#include "emulator.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <3ds.h>

/*
    lexer_t *lexer = lexer_new();
    symbol_t *head = symbol_new(NULL, "", -1, NULL);
    error_t result;

    lexer_retrieve_tokens("ldv 0x10 st c fine: jmp fine c: 0x00", lexer, &result);

    printf("%d %s\n", result.code, result.message);

    first_pass(lexer, head);

    symbol_t *current = head;
    while(current != NULL) {
        //printf("%s\n", current->label);
        current = current->next;
    }

    unsigned char *bytes = second_pass(lexer, &result);

    ade8_t *ade8 = ade8_new();
    memcpy(ade8->memory, bytes, 256); 

    for(int i = 0; i < 20; i++) {
        if(result.code == -1) {
            ade8_execute_next(ade8);
            
            for(int i = 0; i < 256; i++) {
                if(i % 16 == 0) {
                   // printf("\n");
                }

                //printf("%02x  ", ade8->memory[i]);
            }

            //printf("Acc: %d", ade8->acc);
            //printf("\n\n\n");
        }
    }

    //printf("%d %s", result.code, result.message);
*/

#define MAX_ROWS 256
#define MAX_COLUMNS 47
#define MEM_SIZE 256
#define FLATTENED_SIZE ((MAX_COLUMNS + 1) * MAX_ROWS)

PrintConsole topConsole, bottomConsole;
char text[MAX_ROWS][MAX_COLUMNS];
int current_row = 0;
int current_page = 0;
u32 kDown;

char* flatten_text(char text[MAX_ROWS][MAX_COLUMNS]) {
    static char output_flat[FLATTENED_SIZE];
    output_flat[0] = '\0';

    for (int i = 0; i < MAX_ROWS; i++) {
        strncat(output_flat, text[i], MAX_COLUMNS - 1);
        strcat(output_flat, "\n");
    }

    return output_flat;
}

void redraw_if_changed(uint8_t *current_memory, uint8_t *previous_memory) {
    if (memcmp(current_memory, previous_memory, MEM_SIZE) != 0) {
        printf("\x1b[2J\x1b[H");
        
        for(int i = 0; i < 256; i++) { 
            printf("%02x  ", current_memory[i]); 
        }

        memcpy(previous_memory, current_memory, MEM_SIZE);
    }
}

void run_program() {
    consoleSelect(&bottomConsole);
    printf("\x1b[2J\x1b[H");

    lexer_t *lexer = lexer_new();
    symbol_t *head = symbol_new(NULL, "", -1, NULL);
    error_t result;

    lexer_retrieve_tokens(flatten_text(text), lexer, &result);

    if(result.code != -1) {
        printf("%d %s\n", result.code, result.message);
    }

    first_pass(lexer, head);
    unsigned char *bytes = second_pass(lexer, &result);
    
    ade8_t *ade8 = NULL;

    if(result.code != -1) {
        printf("%d %s\n", result.code, result.message);
    } else {
        ade8 = ade8_new();
        memset(ade8->memory, 0, 256);
        memcpy(ade8->memory, bytes, 256); 
    
        for(int i = 0; i < 256; i++) {
            printf("%02x  ", ade8->memory[i]);
        }

        uint8_t previous_memory[MEM_SIZE];
        memset(previous_memory, 0xFF, MEM_SIZE);

        while(!(kDown & KEY_SELECT)){
            hidScanInput();
            kDown = hidKeysDown();

            ade8_execute_next(ade8);

            redraw_if_changed(ade8->memory, previous_memory);

            gfxFlushBuffers();
            gfxSwapBuffers();
            gspWaitForVBlank();
            svcSleepThread(10000000);
        }
    }

    if(lexer != NULL) {
        if(lexer->tokens != NULL) {
            free(lexer->tokens);
        }
        free(lexer);
    }

    symbol_t *current = head;

    while(current != NULL) {
        symbol_t *temp = current->next;
        free(current);
        current = temp;
    }

    free(bytes);

    if(ade8 != NULL) {
        free(ade8);
    }
}

void show_text_editor() { 
    SwkbdState swkbd;
    SwkbdButton button;
	int writing = 0;
    int needs_redraw = 1;

    while (aptMainLoop())
	{
		hidScanInput();

		kDown = hidKeysDown();

        if(!writing) {
            if (kDown & KEY_A) {
                swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 1, -1);
                swkbdSetFeatures(&swkbd, SWKBD_FIXED_WIDTH);
                writing = 1;

                button = swkbdInputText(&swkbd, text[current_row], MAX_COLUMNS);
            }

            if(kDown & KEY_DUP) {
                if(current_row != 0) {
                    current_row -= 1;
                }

                needs_redraw = true;
            }

            if(kDown & KEY_DDOWN) {
                if(current_row < MAX_ROWS -1) {
                    current_row += 1;
                }

                needs_redraw = true;
            }

            if(kDown & KEY_DLEFT) {
                if(current_page > 0) {
                    current_page -= 1;
                }

                needs_redraw = true;
            }

            if(kDown & KEY_RIGHT) {
                if(current_page < 7) {
                    current_page += 1;
                }

                needs_redraw = true;
            }
        }else {
            if(button == SWKBD_BUTTON_CONFIRM) {
                writing = 0;
                needs_redraw = true;
            }
        }

        if (kDown & KEY_START) {
			run_program();
            consoleSelect(&topConsole);
        }


        if (needs_redraw) {
            printf("\x1b[2J\x1b[H");
            for (int i = 0; i < 29; i++) {
                int index = i + current_page * 29;
                if (index == current_row) {
                    printf("\x1b[47;30m%2x\x1b[0m %s\n", index, text[index]);
                } else {
                    printf("%2x %s\n", index, text[index]);
                }
            }

            needs_redraw = 0;
        }

        
        gfxFlushBuffers();
		gspWaitForVBlank();
		gfxSwapBuffers();
    }
}

int main() {
	gfxInitDefault();
    consoleInit(GFX_BOTTOM, &bottomConsole);
    consoleInit(GFX_TOP, &topConsole);

    for(int i = 0; i < MAX_ROWS; i++) {
        memset(text[i], 0, MAX_COLUMNS);
    }

    show_text_editor();

	gfxExit();
	return 0;
}