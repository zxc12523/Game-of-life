#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define swap(type, a, b) \
    {                    \
        type tmp = a;    \
        a = b;           \
        b = tmp;         \
    }

#define check(i, j)                                                            \
    {                                                                          \
        int n = 0, row = i, col = j;                                           \
                                                                               \
        if (0 <= row && row < r && 0 <= col && col < c && grid[row][col] == 1) \
            n++;                                                               \
        row--, col--;                                                          \
        if (0 <= row && row < r && 0 <= col && col < c && grid[row][col] == 1) \
            n++;                                                               \
        row++;                                                                 \
        if (0 <= row && row < r && 0 <= col && col < c && grid[row][col] == 1) \
            n++;                                                               \
        row++;                                                                 \
        if (0 <= row && row < r && 0 <= col && col < c && grid[row][col] == 1) \
            n++;                                                               \
        col++;                                                                 \
        if (0 <= row && row < r && 0 <= col && col < c && grid[row][col] == 1) \
            n++;                                                               \
        col++;                                                                 \
        if (0 <= row && row < r && 0 <= col && col < c && grid[row][col] == 1) \
            n++;                                                               \
        row--;                                                                 \
        if (0 <= row && row < r && 0 <= col && col < c && grid[row][col] == 1) \
            n++;                                                               \
        row--;                                                                 \
        if (0 <= row && row < r && 0 <= col && col < c && grid[row][col] == 1) \
            n++;                                                               \
        col--;                                                                 \
        if (0 <= row && row < r && 0 <= col && col < c && grid[row][col] == 1) \
            n++;                                                               \
                                                                               \
        if (grid[i][j])                                                        \
            tmp_grid[i][j] = (3 == n || n == 4);                               \
        else                                                                   \
            tmp_grid[i][j] = (n == 3);                                         \
    }

int r, c, epoch, flag, num, SWAP;
int **grid, **tmp_grid;

void *process_col(void *id)
{
    int mod = (int)id;
    for (int j = mod; j < c; j += num) // each thread deal with the col | for all (col % number of thread) = tid
        for (int i = 0; i < r; i++)
        {
            check(i, j); // update status for one cell
        }

    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    num = atoi(argv[2]);
    FILE *infile = fopen(argv[3], "r");
    FILE *outfile = fopen(argv[4], "w");

    // printf("%d\n", num);

    char *s;
    fscanf(infile, "%d %d %d\n", &r, &c, &epoch);

    s = malloc(sizeof(char) * c);

    if (r > c)
    {
        swap(int, r, c);
        SWAP = 1;
    }

    grid = malloc(sizeof(int *) * r);
    tmp_grid = malloc(sizeof(int *) * r);

    for (int i = 0; i < r; i++)
    {
        grid[i] = malloc(sizeof(int) * c);
        tmp_grid[i] = malloc(sizeof(int) * c);
    }

    if (SWAP == 0)
    {
        for (int i = 0; i < r; i++)
        {
            fscanf(infile, "%s", s);
            for (int j = 0; j < strlen(s); j++)
                grid[i][j] = s[j] == 'O' ? 1 : 0;
        }
    }
    else
    {
        for (int i = 0; i < c; i++)
        {
            fscanf(infile, "%s", s);
            for (int j = 0; j < strlen(s); j++)
                grid[j][i] = s[j] == 'O' ? 1 : 0;
        }
    }

    if (strcmp(argv[1], "-t") == 0)
    {
        pthread_t *thread_arr = malloc(sizeof(pthread_t) * num);

        for (int t = 0; t < epoch; t++)
        {
            for (int i = 0; i < num; i++)
                pthread_create(&thread_arr[i], NULL, process_col, (void *)i);

            for (int i = 0; i < num; i++)
                pthread_join(thread_arr[i], NULL);

            swap(int **, grid, tmp_grid);
        }
    }
    else
    {
        for (int t = 0; t < epoch; t++)
        {
            if (vfork() == 0) // fork share the same memory with its parent
            {
                for (int i = 0; i < r; i++)
                    for (int j = 0; j <= c / 2; j++) // child do the half
                        check(i, j);

                _exit(0);
            }
            else
            {
                for (int i = 0; i < r; i++)
                    for (int j = c / 2 + 1; j < c; j++) // parent do for another
                        check(i, j);

                wait();

                swap(int **, grid, tmp_grid);
            }
        }
    }

    if (SWAP == 0)
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
                fprintf(outfile, "%c", grid[i][j] ? 'O' : '.');
            fprintf(outfile, "\n");
        }
    else
        for (int i = 0; i < c; i++)
        {
            for (int j = 0; j < r; j++)
                fprintf(outfile, "%c", grid[j][i] ? 'O' : '.');
            fprintf(outfile, "\n");
        }
}
