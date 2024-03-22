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
int enter_size_t_num(size_t* koeff)
{
	while ( (scanf("%zu", koeff)) != 1 )
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

char* arr_sver(char* arr, size_t size)
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

int bit_dump(char* src, size_t amount, size_t marked)
{
    printf(GREEN("#######################################DUMP BEGINNING#####################################\n\n"));

    for(size_t co = 1; co <= amount; co++)
    {
        if((co-1) == marked)
        {
            printf(RED("%3zu"), co);
        }
        else
        {
            printf("%3zu", co);
        }
    }

    putchar('\n');

    for(size_t co = 0; co < amount; co++)
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

size_t cont_bits_counter(size_t wr_size)
{
    size_t co = 0;

    for(; pow(2, co) < (double)wr_size + co + 1; co++);

    printf(GREEN("In word will be %zu extraletters in real word\n"), co);

    return co;
}

int count_bits(size_t pow_of_2, char* src, size_t cnt_in_wrd, size_t size)
{
    size_t rez     = 0;
    size_t counter = 0;

    for(size_t co = cnt_in_wrd; co < size;)
    {
        counter = 0;

        if(co == cnt_in_wrd)
        {
            counter++;
        }
        
        //printf("co = %zu\n", co);

        for(; ((double)counter < pow_of_2) && (counter + co < size); counter++)
        {
            //printf("counter = %zu\n", counter);
            //printf(GREEN("Now we are in bit:\n"));
            //bit_dump(src, size, co + counter);

            rez = rez ^ src[co + counter];
            
            //printf(RED("rez = %zu\n"), rez);
        }

        co += 2 * (size_t)pow_of_2;
    }

    return rez;
}

int full_word(char* dst, size_t big_wr_size, size_t h_m_cntrl_bits)
{
    double pow_of2 = 0.0;

    for(size_t cnt_in_wrd = 0, cnt_of_cntrl_bits = 0; cnt_in_wrd < big_wr_size; cnt_in_wrd++)
        {
            pow_of2 = pow(2, cnt_of_cntrl_bits);

            if(is_equal_with_zero((double)cnt_in_wrd - (pow_of2 - 1.0)) && (cnt_of_cntrl_bits < h_m_cntrl_bits))  //Minus 1 because of counting since 0
            {                                                                              //strict inequality because we dont care about least non-controle leters
                dst[cnt_in_wrd] = count_bits(pow_of2, dst, cnt_in_wrd, big_wr_size);
                printf("$$$$$$$$$$$$$$dst[%zu] = %d\n", cnt_in_wrd, dst[cnt_in_wrd]);
                cnt_of_cntrl_bits++;
            }
        }

    return 0;
}

int show_nums(size_t wr_size)
{
    for(size_t co = 1; co <= wr_size; co++)
    {
        printf(GREEN("%3zu"), co);
    }

    putchar('\n');

    return 0;
}

int show(char* meg, size_t size, size_t wr_size)
{
    for(size_t co = 0; co < size; co++)
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

size_t mess_proc(char* mes, size_t counter, size_t wr_size, size_t h_m_words)
{
    if(counter % wr_size)
    {
        h_m_words++;

        CAP = h_m_words*wr_size;

        mes = (char*)realloc((void*)mes, CAP);

        for(size_t cnt = counter; cnt < CAP; cnt++)
        {
            mes[cnt] = 0;
        }
    }

    return h_m_words;
}

int enter_mes(char* mes)
{
    size_t co = 0;
    char buf = 0;

    for(; 1; co++)
    {
        scanf("%c", &buf);

        if((buf == '0') || (buf == '1'))
        {
            mes[co] = buf - 48;
        }
        else
        {
            //printf(RED("Entered nonbinar value! Wait mistake in result.\n"));
            break;
        }

        if(co == (CAP - 1))
        {
            CAP *= 2;
            mes = (char*)realloc((void*)mes, CAP);
        }
    }
    return co;
}

int codim(char* msg, char* coded, size_t wr_size, size_t h_m_words, size_t h_m_cntrl_bits)
{
    size_t shift = 0;
    size_t big_wr_size = wr_size + h_m_cntrl_bits;

    for(size_t co = 0; co < h_m_words; co++)
    {
        shift = co*(big_wr_size);

        for(size_t cnt_in_wrd = 0, cnt_of_cntrl_bits = 0; cnt_in_wrd < big_wr_size; cnt_in_wrd++)
        {
            //printf("cnt_in_wrd = %zu\n", cnt_in_wrd);

            if((is_equal_with_zero((double)cnt_in_wrd - (pow(2, cnt_of_cntrl_bits) - 1.0))) && (cnt_of_cntrl_bits < h_m_cntrl_bits))//if place is owened by controle bit 
                                                                                          //Minus 1 because of counting since 0
            {                                                                             //strict inequality because we care about least informative leters
                coded[shift + cnt_in_wrd] = 0;
                cnt_of_cntrl_bits++;
                /*printf("wow!\n");
                printf("cnt_in_wrd = %zu\n", cnt_in_wrd);
                printf("wow!\n");*/
            }
            else
            {
                coded[shift + cnt_in_wrd] = msg[co*wr_size + cnt_in_wrd - cnt_of_cntrl_bits];
                //printf("coded[shift + cnt_in_wrd] = msg[co*wr_size + cnt_in_wrd - cnt_of_cntrl_bits]\n %d = %d\n", coded[shift + cnt_in_wrd], msg[co*wr_size + cnt_in_wrd - cnt_of_cntrl_bits]);
            }
        }

        full_word(coded + shift, big_wr_size, h_m_cntrl_bits);

    }

    return 0;
}

int kvazi_random(size_t bottom, size_t ceiling)
{
    return bottom + (rand() + time(0)) % (ceiling - bottom + 1);
}

int rand_miss(char* mes, size_t size)
{
    char num = kvazi_random(0, size - 1);
    
    mes[num] = !mes[num];

    printf("mes[%d] = %d\n", num + 1, mes[num]);

    return 0;
}

int fullness_check(char* coded, size_t h_m_cntrl_bits, size_t wr_size)
{
    double pow_of2  = 0.0;
    int    syndrome  = -1;
    size_t big_wr_size = wr_size + h_m_cntrl_bits;

    for(size_t cnt_in_wrd = 0, cnt_of_cntrl_bits = 0; cnt_in_wrd <= big_wr_size; cnt_in_wrd++)
    {
        pow_of2 = pow(2, cnt_of_cntrl_bits);

        if((is_equal_with_zero((double)cnt_in_wrd - (pow_of2 - 1.0))) && (cnt_of_cntrl_bits < h_m_cntrl_bits)) 
        {
            if (count_bits(pow_of2, coded, cnt_in_wrd, big_wr_size) != coded[cnt_in_wrd])
            {
                printf("count_bits = %d, coded[%zu] = %d\n", count_bits(pow_of2, coded, cnt_in_wrd, big_wr_size), cnt_in_wrd, coded[cnt_in_wrd]);
                printf("syndrome = %d\n", syndrome);
                printf("WE FOUND MISTAKE IN BIT #%zu\n", cnt_in_wrd + 1);
                bit_dump(coded, big_wr_size, cnt_in_wrd);
                if(syndrome == -1)
                {
                    syndrome = cnt_in_wrd + 1;  //1 because we need to find a number of incorrect controle bit
                }
                else
                {
                    syndrome += cnt_in_wrd + 1;  //1 because we need to add a number of incorrect controle bit
                }
                printf("syndrome = %d\n", syndrome);
            }

            cnt_of_cntrl_bits++;
        }
    }

    syndrome--;              //because it's in array, where numbers starts by 0

    return syndrome;
}

int decodim(char* mes, char* coded, size_t wr_size, size_t h_m_words, size_t h_m_cntrl_bits)
{
    size_t shift        = 0;
    size_t big_wr_size  = wr_size + h_m_cntrl_bits;
    int syndrome     = -2;                               // Syndrome will show missed bit in word// negative value help to catch miss in first letter of word
    int old_syndrome = syndrome;

    for(size_t co = 0; co < h_m_words; co++)
    {
        shift = co*big_wr_size;
        printf("We are in %zu word:\n", co + 1);

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
    size_t wr_size = 0;
    printf("Enter size of coding word:\n");
    enter_size_t_num(&wr_size);
    printf("%zu size\n", wr_size);

    size_t h_m_cntrl_bits = cont_bits_counter(wr_size);
    size_t big_wr_size = wr_size + h_m_cntrl_bits;

    printf("Enter your message:\n");
    char* mes = (char*)calloc(CAP, sizeof(char));

    size_t counter = 0;
    counter = enter_mes(mes);                       //In mes counter numbers
    printf("There are %zu letters\n", counter);

    char* origin_mes = arr_sver(mes, counter);
    size_t origin_co  = counter;

    size_t h_m_words = counter/wr_size;

    h_m_words =  mess_proc(mes, counter, wr_size, h_m_words);
    printf("There are %zu words\n", h_m_words);

    size_t len_of_coded = h_m_words * big_wr_size;
    printf("There are %zu len_of_coded\n", len_of_coded);

    char* coded_msg = (char*)calloc((size_t)len_of_coded, sizeof(char));
    codim(mes, coded_msg, wr_size, h_m_words, h_m_cntrl_bits);

    printf(RED("\nEncoded message:\n\n"));
    show(coded_msg, len_of_coded, big_wr_size);

    rand_miss(coded_msg, len_of_coded);

    char* new_mes = (char*)calloc(CAP, sizeof(char));
    decodim(new_mes, coded_msg, wr_size, h_m_words, h_m_cntrl_bits);

    printf(RED("\nDecoded message:\n\n"));
    show(new_mes, origin_co, wr_size);

    printf(RED("\nOriginal message:\n\n"));
    show(origin_mes, origin_co, wr_size);

    free(mes);    
    free(new_mes);    
    free(coded_msg);
    free(origin_mes);

    return 0;
}