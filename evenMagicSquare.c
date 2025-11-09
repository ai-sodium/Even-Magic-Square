#include <stdio.h>
#define N 100

int main()
{
    int n, num, i, j;
	int retry = 1;
	

while (retry == 1)
{
    printf("\nEnter an even number: ");
    scanf("%d", &n);
	int magicSquare[n][n];
	
	if (n % 2 != 0)
    {
        printf("\nPlease enter an even number.\n");
        do {
		    printf("\nDo you want to try again? 1 - yes, 0 - no: ");
		    scanf("%d", &retry);
		
		    if (retry != 0 && retry != 1) {
		        printf("\nInvalid input! Please enter 1 or 0 only.\n");
		    }
		} while (retry != 0 && retry != 1);
        continue;
    }
    
	//----------------------------Case for N in the form 4n (doubly even numbers)-----------------------------
    else if (n % 4 == 0)
    {
        // Fill the square from 1 to n²
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
                magicSquare[i][j] = (n * i) + j + 1;
        }

        // Modify certain cells using the rule: (n*n + 1) - value
        // Top-left corner
        for (i = 0; i < n / 4; i++)
        {
            for (j = 0; j < n / 4; j++)
                magicSquare[i][j] = (n * n + 1) - magicSquare[i][j];
        }

        // Top-right corner
        for (i = 0; i < n / 4; i++)
        {
            for (j = 3 * (n / 4); j < n; j++)
                magicSquare[i][j] = (n * n + 1) - magicSquare[i][j];
        }

        // Bottom-left corner
        for (i = 3 * n / 4; i < n; i++)
        {
            for (j = 0; j < n / 4; j++)
                magicSquare[i][j] = (n * n + 1) - magicSquare[i][j];
        }

        // Bottom-right corner
        for (i = 3 * n / 4; i < n; i++)
        {
            for (j = 3 * n / 4; j < n; j++)
                magicSquare[i][j] = (n * n + 1) - magicSquare[i][j];
        }

        // Invert the values in the central 2x2 square
        for (i = n / 4; i < 3 * n / 4; i++)
        {
            for (j = n / 4; j < 3 * n / 4; j++)
                magicSquare[i][j] = (n * n + 1) - magicSquare[i][j];
        }
    }

    //----------------------------Case for N in the form 4n + 2 (singly even numbers)-----------------------------
    else if (n % 4 == 2)
    {
        int miniSquare[n / 2][n / 2];

        // Initialize the smaller matrix with zeros
        for (i = 0; i < n / 2; i++)
        {
            for (j = 0; j < n; j++)
            {
                miniSquare[i][j] = 0;
            }
        }

        // Starting position for number 1 in the smaller square
        i = n / 4;
        j = (n / 2) - 1;

        // Fill the smaller square (odd-number logic)
        for (num = 1; num <= ((n / 2) * (n / 2));)
        {
            i = i % (n / 2);
            j = j % (n / 2);
            if (i < 0)
                i = i + (n / 2);
            if (j < 0)
                j = j + (n / 2);

            if (miniSquare[i][j] == 0)
            {
                miniSquare[i][j] = num;
                i--;
                j++;
                num++;
            }
            else
            {
                j -= 2;
                i++;
            }
        }

        // Rotate the smaller square 90° clockwise
        for (i = 0; i < (n / 4); i++)
        {
            for (j = i; j < (n / 2) - i - 1; j++)
            {
                int temp = miniSquare[i][j];
                miniSquare[i][j] = miniSquare[j][(n / 2) - 1 - i];
                miniSquare[j][(n / 2) - 1 - i] = miniSquare[(n / 2) - 1 - i][(n / 2) - 1 - j];
                miniSquare[(n / 2) - 1 - i][(n / 2) - 1 - j] = miniSquare[(n / 2) - 1 - j][i];
                miniSquare[(n / 2) - 1 - j][i] = temp;
            }
        }

        // Mirror the smaller square vertically
        for (i = 0; i < (n / 2); i++)
        {
            for (j = 0; j < (n / 4); j++)
            {
                int temp = miniSquare[i][j];
                miniSquare[i][j] = miniSquare[i][(n / 2) - j - 1];
                miniSquare[i][(n / 2) - j - 1] = temp;
            }
        }

        // Fill the large square with variations of the smaller one
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                if (i < n / 2 && j < n / 2)
                    magicSquare[i][j] = miniSquare[i][j];
                if (i >= n / 2 && j < n / 2)
                    magicSquare[i][j] = miniSquare[i - (n / 2)][j] + (3 * n * n / 4);
                if (i < n / 2 && j >= n / 2)
                    magicSquare[i][j] = miniSquare[i][j - (n / 2)] + (n * n / 2);
                if (i >= n / 2 && j >= n / 2)
                    magicSquare[i][j] = miniSquare[i - (n / 2)][j - (n / 2)] + (n * n / 4);
            }
        }

        // Swap quadrants 2 and 3 for the final correction
        int quarter = n / 4;

        for (i = 0; i < n / 2; i++)
        {
            for (j = 0; j < n / 4; j++)
            {
                if (i == quarter && j == 0)
                {
                    int temp = magicSquare[i][j + quarter];
                    magicSquare[i][j + quarter] = magicSquare[i + (n / 2)][j + quarter];
                    magicSquare[i + (n / 2)][j + quarter] = temp;
                }
                else
                {
                    int temp = magicSquare[i][j];
                    magicSquare[i][j] = magicSquare[i + (n / 2)][j];
                    magicSquare[i + (n / 2)][j] = temp;
                }
            }
        }

        // If n > 6, make extra swaps for correction
        if (n > 6)
        {
            for (i = 0; i < n / 2; i++)
            {
                for (j = 0; j < quarter - 1; j++)
                {
                    int temp = magicSquare[i][n - j - 1];
                    magicSquare[i][n - j - 1] = magicSquare[i + (n / 2)][n - j - 1];
                    magicSquare[i + (n / 2)][n - j - 1] = temp;
                }
            }
        }
    }
    // Display and save the result	
        printf("\n\nn = %d, sum = %d\n\n", n, (((n * n) + 1) * (n / 2)));
        FILE *file = fopen("magicSquare.txt", "w");

        if (file == NULL)
        {
            printf("Error opening file!\n");
            return 1;
        }

        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                printf("%d  ", magicSquare[i][j]);
                fprintf(file, "%d  ", magicSquare[i][j]);
            }
            fprintf(file, "\n");
            printf("\n");
        }

        fclose(file);

        do {
		    printf("\nDo you want to try again? 1 - yes, 0 - no: ");
		    scanf("%d", &retry);
		
		    if (retry != 0 && retry != 1) {
		        printf("\nInvalid input! Please enter 1 or 0 only.\n");
		    }
		} while (retry != 0 && retry != 1);
    }

    printf("\nProgram ended.\n");
    return 0;
}
