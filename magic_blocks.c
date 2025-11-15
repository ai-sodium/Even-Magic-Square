/* magic_blocks.c
   Compile on Windows (Visual Studio): cl magic_blocks.c
   or using gcc for Windows (MinGW): gcc magic_blocks.c -o magic_blocks.exe
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define MIN_N 6
#define MAX_N 100

/* --- Helper: set console color --- */
void set_color(WORD attr) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr);
}

/* --- Count digits --- */
int digits_of(int x) {
    if (x == 0) return 1;
    int d = 0;
    while (x) { d++; x /= 10; }
    return d;
}

/* --- Siamese method for odd m: generate m x m magic square (values 1..m*m) --- */
int* generate_siamese(int m) {
    int *A = (int*)calloc(m * m, sizeof(int));
    if (!A) return NULL;

    int num = 1;
    int row = 0;
    int col = m / 2;          /* "center box on top row" */
    while (num <= m * m) {
        if (A[row * m + col] == 0) {
            A[row * m + col] = num;
            /* next tentative position: up one, right one */
            int newr = row - 1;
            int newc = col + 1;
            if (newr < 0) newr = newr + m;          /* wrap top -> bottom */
            if (newc >= m) newc = newc - m;        /* wrap right -> left */
            if (A[newr * m + newc] != 0) {
                /* occupied -> go directly below current */
                row = row + 1;
                if (row >= m) row = row - m;       /* wrap if needed */
                /* col remains same */
            } else {
                row = newr;
                col = newc;
            }
            num++;
        } else {
            /* should not happen because logic places only into empty cell,
               but safety: move down one */
            row = (row + 1) % m;
        }
    }
    return A;
}

/* --- Expand small m x m into big n x n by repeating each cell blockSize x blockSize --- */
/* small: m*m array, blockSize >= 1, n = m * blockSize */
int *expand_blocks(const int *small, int m, int blockSize) {
    int n = m * blockSize;
    int *big = (int *)malloc(n * n * sizeof(int));
    if (!big) return NULL;
    int i, j, bi, bj;

    for (i = 0; i < m; ++i) {
        for (j = 0; j < m; ++j) {
            int val = small[i * m + j];
            for (bi = 0; bi < blockSize; ++bi) {
                for (bj = 0; bj < blockSize; ++bj) {
                    int rr = i * blockSize + bi;
                    int cc = j * blockSize + bj;
                    big[rr * n + cc] = val;
                }
            }
        }
    }
    return big;
}

/* --- color palette: use several distinct attributes (foreground only) --- */
WORD palette[] = {
    0x0E, /* Yellow */
    0x0C, /* Red */
    0x09, /* Blue */
    0x0A, /* Green */
    0x0B, /* Aqua */
    0x0D, /* Magenta */
    0x0F, /* Bright White */
    0x08, /* Grey */
    0x06, /* Brown/Olive */
    0x03, /* Cyan-ish */
    0x02, /* Green dim */
    0x05, /* Purple */
};
int palette_len = (sizeof(palette)/sizeof(palette[0]) - 1); /* ignore last 0 */

/* --- Print big matrix with coloring and alignment --- */
void print_colored_matrix(int *mat, int n, int maxPrintedValue) {
    /* width for zero-padded field (minimum 2 to match examples) */
    int width = digits_of(maxPrintedValue);
    if (width < 2) width = 2;

    char fmt[32];
    /* build format like "%0*d " where * is width */
    /* We'll use printf("%0*d ", width, val) but we want to print with color per val. */
    int numDistinct = maxPrintedValue; /* distinct printed values are 1..maxPrintedValue */

    /* map value -> palette index */
    int *map = (int*)malloc((numDistinct + 1) * sizeof(int));
    int v;
    for (v = 1; v <= numDistinct; ++v) {
        map[v] = (v - 1) % palette_len;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    WORD defaultAttr = csbi.wAttributes;

	int r, c;
    for (r = 0; r < n; ++r) {
        for (c = 0; c < n; ++c) {
            int val = mat[r * n + c];
            WORD attr = palette[ map[val] ];
            if (attr == 0) attr = defaultAttr;
            set_color(attr);
            /* zero padded with width */
            printf("%0*d", width, val);
            set_color(defaultAttr); /* reset after printing number */
            if (c < n - 1) putchar(' ');
        }
        putchar('\n');
    }

    free(map);
}

/* --- find smallest blockSize (power of two, starting 4) such that (n / blockSize) is odd --- */
/* returns blockSize if found, else 0 */
int find_blocksize_for_odd_quotient(int n) {
    int bs = 4;
    while (bs <= n) {
        if (n % bs == 0) {
            int q = n / bs;
            if ((q % 2) == 1) return bs; /* found */
            /* else try larger power of two */
        } else {
            /* n not divisible by this bs -> cannot use this block size; break */
            break;
        }
        bs *= 2;
    }
    return 0; /* not found */
}

int isPowerOfTwo(int n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

/* --- main program --- */
int main(void) {
    int retry = 1;

    while (retry == 1) {
        int n;
        do {
            printf("\nEnter an even number (minimum %d, maximum %d): ", MIN_N, MAX_N);
            scanf("%d", &n);
            if (isPowerOfTwo(n)) {
				printf("\nPower of 2's like 2, 4, 8, 16, 32, 64, ... are hopeless case\n");
				do {
            		printf("\nDo you want to try again? 1 - yes, 0 - no: ");
            		if (scanf("%d", &retry) != 1) { while (getchar() != '\n'); retry = -1; }
            		if (retry != 0 && retry != 1) printf("\nPlease enter 1 or 0 only.\n");
        		} while (retry != 0 && retry != 1);
        		if (retry == 1) continue;   // restart program loop
    			else break;                 // exit program
			}

            if (n < MIN_N || n > MAX_N || (n % 2) != 0) {
                printf("\nInvalid input. Please enter an even integer >= %d.\n", MIN_N);
                int inner;
                do {
                    printf("\nDo you want to try again? 1 - yes, 0 - no: ");
                    if (scanf("%d", &inner) != 1) { while (getchar() != '\n'); inner = -1; }
                    if (inner != 0 && inner != 1) printf("\nPlease enter 1 or 0 only.\n");
                } while (inner != 0 && inner != 1);
                if (inner == 0) {
                    printf("Program ended.\n");
                    return 0;
                } else {
                    /* repeat outer input loop */
                    continue;
                }
            } else {
                break; /* valid n */
            }
        } while (1);

        /* At this point n is even and >= MIN_N */
        if (n % 4 == 2) {
            /* singly-even: n = 2 * m, where m is odd */
            int m = n / 2;
            int *mini = generate_siamese(m);
            if (!mini) { printf("Memory allocation failed.\n"); return 1; }
            int *big = expand_blocks(mini, m, 2);
            if (!big) { printf("Memory allocation failed.\n"); free(mini); return 1; }

            /* max printed value equals m*m */
            int maxVal = m * m;
            printf("\nGenerated %dx%d (singly-even expanded 2x2 blocks) pattern:\n\n", n, n);
            print_colored_matrix(big, n, maxVal);

            free(mini);
            free(big);
        } else if (n % 4 == 0) {
            /* doubly-even: find suitable blockSize (4,8,16,...) so that (n/blockSize) is odd */
            int blockSize = find_blocksize_for_odd_quotient(n);
            if (blockSize == 0) {
                printf("\nOut of bounds: could not find block size to produce odd quotient (n/bs odd).\n");
            } else {
                int m = n / blockSize; /* m is odd */
                int *mini = generate_siamese(m);
                if (!mini) { printf("Memory allocation failed.\n"); return 1; }
                int *big = expand_blocks(mini, m, blockSize);
                if (!big) { printf("Memory allocation failed.\n"); free(mini); return 1; }

                int maxVal = m * m;
                printf("\nResult (%d x %d) — doubly-even with blockSize = %d, expanded %d x %d blocks:\n\n", n, n, blockSize, blockSize, blockSize);
                print_colored_matrix(big, n, maxVal);

                free(mini);
                free(big);
            }
        } else {
            /* Should never reach here because we validated evenness, but keep safe message */
            printf("Unhandled case for n = %d\n", n);
        }

        /* Ask user to retry */
        do {
            printf("\nDo you want to try again? 1 - yes, 0 - no: ");
            if (scanf("%d", &retry) != 1) { while (getchar() != '\n'); retry = -1; }
            if (retry != 0 && retry != 1) printf("\nPlease enter 1 or 0 only.\n");
        } while (retry != 0 && retry != 1);
    }

    printf("Program ended.\n");
    return 0;
}

