#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>


typedef unsigned uns;
size_t CAP = 16;
const double EPSILON = 1.0e-09;

enum Errors
{
    unknown_error           = -10,
    EOF_error               = EOF,
    arg_function_is_nullptr = 0,
    no_error                = 1
};

#define check(condition, type_error)      				        														                   		     \
    if (!(condition))                                                                                                                                \
    {                                                                                                                                                \
		printf("\033[91mОшибка в условии %s в строке %d в файле %s в функции %s \n\033[39m", #condition, __LINE__, __FILE__, __PRETTY_FUNCTION__ ); \
        return type_error;                                                                                                                           \
    }

#define hard_check(condition)           				        														                   		     \
    if (!(condition))                                                                                                                                \
    {                                                                                                                                                \
		printf("\033[91mОшибка в условии %s в строке %d в файле %s в функции %s \n\033[39m", #condition, __LINE__, __FILE__, __PRETTY_FUNCTION__ ); \
        abort();                                                                                                                                     \
    }

#define RED(b) "\033[91m" b "\033[39m"
#define GREEN(b) "\033[32m" b "\033[39m"

//Для очистки буффера
int clear_buf()
{
	while (getchar() != '\n');

    return no_error;
}

//Для корректного ввода целых числовых значений
int enter_int_num(int* koeff)
{
	while ( (scanf("%d", koeff)) != 1 )
	{
		printf(RED("Введено нецелое число. А должно быть введено целое число. Попробуйте ввести его ещё раз. \n"));
		clear_buf();
	}
	clear_buf();

    return no_error;
}

//Для корректного ввода нецелых числовых значений
int enter_double_num(double* koeff)
{
	while ( (scanf("%lf", koeff)) != 1 )
	{
		printf(RED("Введено не число. А должно быть введено число. Попробуйте ввести его ещё раз. \n"));
		clear_buf();
	}
	clear_buf();

    return no_error;
}

char* arr_sver(char* arr, int size)
{
    char* sv = (char*)calloc((size_t)size, sizeof(char));

    for(unsigned co = 0; co < size; co++)
    {
        sv[co] = arr[co];
    }

    return sv;
}

int is_equal_with_zero(double comp_num)
{
	return ( fabs(comp_num) < EPSILON ) ? 1 : 0;
}

int bit_dump(char* src, int amount, int marked)
{
    printf(GREEN("#######################################DUMP BEGINNING#####################################\n\n"));

    for(int co = 1; co <= amount; co++)
    {
        if(co-1 == marked)
        {
            printf(RED("%3d"), co);
        }
        else
        {
            printf("%3d", co);
        }
    }

    putchar('\n');

    for(int co = 0; co < amount; co++)
    {
        if(co == marked)
        {
            printf(RED("%3d"), src[co]);
        }
        else
        {
            printf("%3d", src[co]);
        }
    }
    putchar('\n');

    printf(GREEN("\n#######################################DUMP END###########################################\n"));
    getchar();

    return 0;
}

int cont_bits_counter(int wr_size)
{
    int co = 0;

    for(; pow(2, co) <= wr_size; co++);

    printf(RED("co = %d\n"), co);

    return co;
}

int count_bits(int pow_of_2, char* src, int cnt_in_wrd, int size)
{
    int rez     = 0;
    int flag    = 1;
    int counter = 0;

    for(int co = cnt_in_wrd; co < size;)
    {
        counter = 0;

        if(co == cnt_in_wrd)
        {
            co++;
            counter++;
        }

        for(; (counter < pow_of_2) && (co < size); counter++, co++)
        {
            rez = rez ^ src[co];
        }

        co += pow_of_2;
    }

    return rez;
}

int full_word(char* dst, int big_wr_size)
{
    double pow_of2 = 0.0;

    for(size_t cnt_in_wrd = 0, cnt_of_cntrl_bits = 0; cnt_in_wrd < big_wr_size; cnt_in_wrd++)
        {
            pow_of2 = pow(2, cnt_of_cntrl_bits);

            if(is_equal_with_zero((double)cnt_in_wrd - (pow_of2 - 1.0)))  //Minus 1 because of counting since 0
            {
                dst[cnt_in_wrd] = count_bits(pow_of2, dst, cnt_in_wrd, big_wr_size);
                cnt_of_cntrl_bits++;
            }
        }

    return 0;
}

int show_nums(int wr_size)
{
    for(int co = 1; co <= wr_size; co++)
    {
        printf(GREEN("%3d"), co);
    }

    putchar('\n');

    return 0;
}

int show(char* meg, int size, int wr_size)
{
    for(int co = 0; co < size; co++)
    {
        if(!(co % wr_size))
        {
            show_nums(wr_size);
        }

        printf("%3d", meg[co]);

        if(!((co + 1) % wr_size))
        {
            putchar('\n');
        }
    }

    putchar('\n');

    return 0;
}

int codim(char* msg, char* coded, int wr_size, int h_m_words, int h_m_cntrl_bits)
{
    int shift = 0;
    int big_wr_size = wr_size + h_m_cntrl_bits;

    for(size_t co = 0; co < h_m_words; co++)
    {
        shift = co*(big_wr_size);

        for(size_t cnt_in_wrd = 0, cnt_of_cntrl_bits = 0; cnt_in_wrd < big_wr_size; cnt_in_wrd++)
        {

            if(is_equal_with_zero((double)cnt_in_wrd - (pow(2, cnt_of_cntrl_bits) - 1.0)))//if place is owened by controle bit 
                                                                                          //Minus 1 because of counting since 0
            {
                coded[shift + cnt_in_wrd] = 0;
                cnt_of_cntrl_bits++;
            }
            else
            {
                coded[shift + cnt_in_wrd] = msg[co*wr_size + cnt_in_wrd - cnt_of_cntrl_bits];
            }
        }

        full_word(coded + shift, big_wr_size);

    }

    return 0;
}

int kvazi_random(int bottom, int ceiling)
{
    return bottom + (rand() + time(0)) % (ceiling - bottom + 1);
}

int rand_miss(char* mes, int size)
{
    char num = kvazi_random(0, size - 1);
    
    mes[num] = !mes[num];

    return 0;
}

int fullness_check(char* coded, int h_m_cntrl_bits, int wr_size)
{
    double pow_of2  = 0;
    int    syndrom  = -1;
    int big_wr_size = wr_size + h_m_cntrl_bits;

    for(int cnt_in_wrd = 0, cnt_of_cntrl_bits = 0; cnt_in_wrd <= big_wr_size; cnt_in_wrd++)
    {
        pow_of2 = pow(2, cnt_of_cntrl_bits);

        if(is_equal_with_zero((double)cnt_in_wrd - (pow_of2 - 1))) 
        {
            if (count_bits(pow_of2, coded, cnt_in_wrd, big_wr_size) != coded[cnt_in_wrd])
            {
                if(syndrom == -1)
                {
                    syndrom = cnt_in_wrd + 1;  //1 because of syndrome things
                }
                else
                {
                    syndrom += cnt_in_wrd + 1;  //1 because of syndrome things
                }
            }

            cnt_of_cntrl_bits++;
        }
    }

    syndrom--;              //because it's in array, where numbers starts by 0

    return syndrom;
}

int decodim(char* mes, char* coded, int wr_size, int h_m_words, int h_m_cntrl_bits)
{
    int shift        = 0;
    int syndrome     = -2;
    int old_syndrome = syndrome;
    int big_wr_size  = wr_size + h_m_cntrl_bits;

    for(size_t co = 0; co < h_m_words; co++)
    {
        shift = co*(big_wr_size);

        for(size_t cnt_in_wrd = 0, cnt_of_cntrl_bits = 0; cnt_in_wrd < big_wr_size; cnt_in_wrd++)
        {
            syndrome = fullness_check(coded + shift, h_m_cntrl_bits, wr_size);
            
            if((syndrome != old_syndrome) && (syndrome != -2))
            {
                printf("Missed bit in %zu word:\n", co + 1);

                bit_dump(coded + shift, big_wr_size, syndrome);
                coded[shift + syndrome] = !coded[shift + syndrome];

                bit_dump(coded + shift, big_wr_size, syndrome);

                old_syndrome = syndrome;
            }

            if(is_equal_with_zero((double)cnt_in_wrd - (pow(2, cnt_of_cntrl_bits) - 1.0)))//if place is owened by controle bit
            {
                cnt_of_cntrl_bits++;
            }
            else
            {
                mes[co*wr_size + cnt_in_wrd - cnt_of_cntrl_bits] = coded[shift + cnt_in_wrd];
            }
        }
    }

    return 0;
}

int main()
{    
    int wr_size = 0;
    printf("Enter size of coding word:\n");
    enter_int_num(&wr_size);

    int h_m_cntrl_bits = cont_bits_counter(wr_size);
    int big_wr_size = wr_size + h_m_cntrl_bits;

    char buf = 0;
    printf("Enter your message:\n");
    char* mes = (char*)calloc(CAP, sizeof(char));

    int co = 0;

    for(; 1; co++)
    {
        scanf("%c", &buf);

        if((buf >= '0') && (buf <= '1'))
        {
            mes[co] = buf - 48;
        }
        else
        {
            break;
        }

        if(co == (CAP - 1))
        {
            CAP *= 2;
            mes = (char*)realloc((void*)mes, CAP);
        }
    }
                        //In mes co numbers

    char* origin_mes = arr_sver(mes, co);
    int   origin_co  = co;

    int h_m_words = co/wr_size;
    
    if(co % wr_size)
    {
        h_m_words++;
    }

    if(int flag = (co % wr_size))
        {
            co = h_m_cntrl_bits*wr_size;

            if(co >= CAP)
            {
                CAP = co + 10;
                mes = (char*)realloc((void*)mes, CAP);
            }

            for(int cnt = co; cnt < co; cnt++)
            {
                mes[cnt] = 0;
            }
        }

    int len_of_coded = h_m_words * big_wr_size;

    char* coded_msg = (char*)calloc((size_t)len_of_coded, sizeof(char));

    codim(mes, coded_msg, wr_size, h_m_words, h_m_cntrl_bits);

    printf(RED("\nEncoded message:\n\n"));
    show(coded_msg, len_of_coded, big_wr_size);

    rand_miss(coded_msg, len_of_coded);

    decodim(mes, coded_msg, wr_size, h_m_words, h_m_cntrl_bits);

    printf(RED("\nDecoded message:\n\n"));
    show(mes, origin_co, wr_size);

    printf(RED("\nOriginal message:\n\n"));
    show(origin_mes, origin_co, wr_size);

    free(mes);    
    free(coded_msg);
    free(origin_mes);

    return 0;
}