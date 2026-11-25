/*
 * Password Generator
 * Skills used : Strings, Loops, Random Functions
 * Description : Generates random passwords based on user-selected
 *               length and character options.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_LEN 4
#define MAX_LEN 64

const char UPPER[]   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char LOWER[]   = "abcdefghijklmnopqrstuvwxyz";
const char DIGITS[]  = "0123456789";
const char SYMBOLS[] = "!@#$%^&*()-_=+[]{};:,.<>?";

/* Reads an integer safely (avoids infinite loops on bad input) */
int readInt(const char *prompt, int min, int max)
{
    int value;
    char buffer[100];

    while (1)
    {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL &&
            sscanf(buffer, "%d", &value) == 1 &&
            value >= min && value <= max)
        {
            return value;
        }
        printf("  Invalid input! Enter a number between %d and %d.\n", min, max);
    }
}

/* Asks a yes/no question, returns 1 for yes, 0 for no */
int askYesNo(const char *prompt)
{
    char buffer[100];

    while (1)
    {
        printf("%s (y/n): ", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            if (buffer[0] == 'y' || buffer[0] == 'Y') return 1;
            if (buffer[0] == 'n' || buffer[0] == 'N') return 0;
        }
        printf("  Please enter 'y' or 'n'.\n");
    }
}

/* Returns a random character from the given string */
char randomChar(const char *set)
{
    return set[rand() % strlen(set)];
}

/* Shuffles a string (Fisher-Yates) so guaranteed characters aren't always first */
void shuffle(char *str)
{
    int i, j;
    char temp;
    int n = strlen(str);

    for (i = n - 1; i > 0; i--)
    {
        j = rand() % (i + 1);
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

/* Builds the password into 'password' */
void generatePassword(char *password, int length,
                      int useUpper, int useLower, int useDigits, int useSymbols)
{
    char pool[200] = "";
    int count = 0, i;

    /* Build pool of allowed characters and guarantee one from each chosen set */
    if (useUpper)   { strcat(pool, UPPER);   password[count++] = randomChar(UPPER); }
    if (useLower)   { strcat(pool, LOWER);   password[count++] = randomChar(LOWER); }
    if (useDigits)  { strcat(pool, DIGITS);  password[count++] = randomChar(DIGITS); }
    if (useSymbols) { strcat(pool, SYMBOLS); password[count++] = randomChar(SYMBOLS); }

    /* Fill the remaining length randomly from the pool */
    for (i = count; i < length; i++)
        password[i] = randomChar(pool);

    password[length] = '\0';
    shuffle(password);
}

/* Rates the strength of a password */
void showStrength(const char *password, int types)
{
    int len = strlen(password);

    printf("  Strength : ");
    if (len >= 12 && types >= 3)      printf("Strong\n");
    else if (len >= 8 && types >= 2)  printf("Medium\n");
    else                              printf("Weak\n");
}

int main(void)
{
    char password[MAX_LEN + 1];
    int length, count, i;
    int useUpper, useLower, useDigits, useSymbols, types;
    char again = 'y';

    srand((unsigned int)time(NULL));   /* seed random generator */

    printf("=====================================\n");
    printf("        PASSWORD GENERATOR\n");
    printf("=====================================\n");

    do
    {
        length = readInt("\nEnter password length (4-64): ", MIN_LEN, MAX_LEN);

        printf("\nSelect character options:\n");
        do
        {
            useUpper   = askYesNo("  Include UPPERCASE letters (A-Z)");
            useLower   = askYesNo("  Include lowercase letters (a-z)");
            useDigits  = askYesNo("  Include digits (0-9)");
            useSymbols = askYesNo("  Include symbols (!@#$...)");

            types = useUpper + useLower + useDigits + useSymbols;
            if (types == 0)
                printf("\n  Select at least one option!\n\n");
            else if (types > length)
                printf("\n  Length too short for chosen options!\n\n");
        } while (types == 0 || types > length);

        count = readInt("\nHow many passwords to generate (1-10): ", 1, 10);

        printf("\nGenerated Password(s):\n");
        printf("-------------------------------------\n");
        for (i = 1; i <= count; i++)
        {
            generatePassword(password, length, useUpper, useLower, useDigits, useSymbols);
            printf("%2d. %s\n", i, password);
            showStrength(password, types);
        }
        printf("-------------------------------------\n");

        again = askYesNo("\nGenerate more passwords") ? 'y' : 'n';

    } while (again == 'y');

    printf("\nThank you for using Password Generator!\n");
    return 0;
}