#include <stdio.h>

extern const char* currentArchName();

int main(int argc, char *argv[])
{
    printf("%s\n", currentArchName());
}
