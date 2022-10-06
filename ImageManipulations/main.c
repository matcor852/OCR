#include "main.h"

int main(int argc, char ** argv)
{
    if (argc < 3)
        errx(EXIT_FAILURE, "%s", "Not enough arguments");

    if (str_cmp(argv[2],"--rotate"))
        return rotate(argc,argv);

    if (str_cmp(argv[2],"--resize"))
        return resize(argc, argv);

    errx(EXIT_FAILURE, "%s", "Unknown first argument, you\
 should try : filename --rotate, or filename --resize");
}

//compare two strings (used for args)
int str_cmp(char * str1, char * str2)
{
    size_t i =0;
    while (str1[i]!=0)
    {
        if (str2[i]==0 || str1[i]!=str2[i])
            return 0;
        i++;
    }
    return 1;

}