#include <unistd.h>
#include <iostream>
#include <cstring>

class N {
    public:
        N(int x) : number(x) {
        }
        void setAnnotation(char *str) {
            memcpy(annotation, str, strlen(str));
        }
        virtual int operator+(N &nb) {
            return number + nb.number;
        }
        virtual int operator-(N &nb) {
            return number - nb.number;
        }
    private:
        char annotation[100];
        int number;
};

int main(int argc, char **argv)
{
    if (argc < 2)
        _exit(1);

    N *nb5 = new N(5);
    N *nb6 = new N(6);
    N &five = *nb5, &six = *nb6;

    five.setAnnotation(argv[1]);

    return six + five;
}