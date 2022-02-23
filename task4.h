#define BITI 8

typedef unsigned char uch;

uch rotl(uch nr, int poz)
// roteste la stanga cu un bit un intreg reprezentat pe 8 biti
{
    poz %= 8;
    return (nr << poz) | (nr >> (BITI - poz));
}

uch rotr(uch nr, int poz)
// roteste la dreapta cu un bit un intreg reprezentat pe 8 biti
{
    poz %= 8;
    return (nr >> poz) | (nr << (BITI- poz));
}

int hash(char *continut)
{
    int rez = 0;
    int i;
    for(i = 0; continut[i] != '\0'; i++)
    {
        (i % 2 == 0)?(rez ^= rotl(continut[i], i)):(rez ^= rotr(continut[i], i));
    }
    return rez;
}