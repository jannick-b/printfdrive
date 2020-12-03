#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <mach_debug/ipc_info.h>
#include <unistd.h>
#include "libft/libft.h"
#include <limits.h>

typedef struct flaglist
{
    int width;
    int width_pos;
    int zero;
    int zero_pos;
    int minus;
    int minus_pos;
    int dot;
    int dot_pos;
    int dot_prec;
    int dot_prec_pos;
    int is_negative;
    char spec;
    int save_number;

} flaglist;

flaglist *init_flags(flaglist *flags)
{
    flags->width = 0;
    flags->width_pos = 0;
    flags->zero = 0;
    flags->zero_pos = 0;
    flags->minus = 0;
    flags->minus_pos = 0;
    flags->dot = 0;
    flags->dot_pos = 0;
    flags->dot_prec = 0;
    flags->dot_prec_pos = 0;
    flags->is_negative = 0;
    flags->spec = 0;
    flags->save_number = 0;
    return (flags);
}

int size_int(long value, flaglist flags, int base)
{
    int size;

    size = 0;
    while (value)
    {
        value = value / base;
        size++;
    }
    if (((flags.dot == 0 && flags.is_negative) || (flags.dot == 1 && (flags.minus == 1 || flags.width_pos < flags.dot_pos) && flags.is_negative)) && base < 16)
        size++;
    if ((size == 0 && !flags.dot) || (size == 0 && flags.dot && flags.dot_prec))
        size = 1;
    return (size);
}

int print_hex(unsigned int value, int caps)
{
    int size;

    size = 0;
    if (value / 16)
        print_hex(value / 16, caps);
    if (value % 16 > 9)
        if (!caps)
            ft_putchar_fd((value % 16) + 'W', 1);
        else
            ft_putchar_fd((value % 16) + '7', 1);
    else
        ft_putchar_fd((value % 16) + '0', 1);
    size++;
   // printf("size = |%d|", size);
    return (size);
}

int print_pointer(unsigned long value, int caps)
{
    if (value / 16)
        print_pointer(value / 16, caps);
    if (value % 16 > 9)
        if (!caps)
            ft_putchar_fd((value % 16) + 'W', 1);
        else
            ft_putchar_fd((value % 16) + '7', 1);
    else
        ft_putchar_fd((value % 16) + '0', 1);
    return (0);
}

int put_prec(flaglist flags, int size)
{
    int char_count;
    int prec_size;

    char_count = 0;
    prec_size = size - flags.is_negative;                      // kan korter door mee te geven als functie param
    if (flags.width && flags.dot_prec && flags.is_negative)
        ft_putchar_fd('-', 1);
    while (flags.dot_prec - prec_size > 0)
    {
        /*
        if ((flags.zero || (flags.dot_prec && flags.spec != 's')) && !flags.minus)
            ft_putchar_fd('0', 1);
        else
            ft_putchar_fd(' ', 1);*/
        ft_putchar_fd('0', 1);
        char_count++;
        flags.dot_prec--;
    }
    return (char_count);
}

int put_width(flaglist flags, int size)
{
    int char_count;
    int prec_size;

    prec_size = size - flags.is_negative;
    char_count = 0;
    if (flags.dot_prec > prec_size && (flags.spec == 'd' || flags.spec == 'u' || flags.spec == 'i' || flags.spec == 'x' || flags.spec == 'X' || flags.spec == 'p'))
        size = size + (flags.dot_prec - prec_size);
    while (flags.width - size > 0)
    {
        if (flags.zero && !flags.dot_prec && !flags.minus)        //(flags.zero || (flags.dot && flags.spec != 's' && (flags.dot_prec && !flags.width))) && !flags.minus)
            ft_putchar_fd('0', 1);
        else
            ft_putchar_fd(' ', 1);
        char_count++;
        flags.width--;
    }
    return (char_count);
}

int size_uint(unsigned long value, flaglist flags, int base)
{
    int size;

    size = 0;
    while (value)
    {
        value = value / base;
        size++;
    }
    if (((flags.dot == 0 && flags.is_negative) || (flags.dot == 1 && (flags.minus == 1 || flags.width_pos < flags.d\
ot_pos) && flags.is_negative)) && base < 16)
    size++;
    if ((size == 0 && !flags.dot) || (size == 0 && flags.dot && flags.dot_prec))
        size = 1;
    return (size);
}

int do_int_valuecheck(long *value, flaglist *flags)
{
    int char_count;

    char_count = 0;
    if (*value == INT_MIN)
    {
        flags->save_number = *value % 10;
        if (flags->save_number < 0)
            flags->save_number = flags->save_number * -1;
        *value = *value / 10;
        char_count++;
    //    ft_putchar_fd('-', 1);
//        return (1);
    }
    if (*value < 0) {
        *value = *value * -1;
        flags->is_negative = 1;
        if (!flags->width || (flags->zero && !flags->dot_prec))//(flags->dot_prec && !flags->width) || flags->zero) // || (flags->dot && flags->width && flags->dot_prec))
        {
            ft_putchar_fd('-', 1);
            //char_count++;
        }
    }
    return (char_count);
}

int do_int(long value, flaglist *flags)
{
    int char_count;

    char_count = 0;
    char_count = char_count + do_int_valuecheck(&value, flags);
    if (!flags->minus)
    {
        char_count = char_count + put_width(*flags, size_int(value, *flags, 10));
        char_count = char_count + put_prec(*flags, size_int(value, *flags, 10));
    }
    if (flags->width && !flags->dot_prec && flags->is_negative && !flags->zero)//(!flags->dot_prec && flags->is_negative == 1 && !flags->zero)    // dot pos en width pos matters !!!
        ft_putchar_fd('-', 1);
    if (flags->minus)
        char_count = char_count + put_prec(*flags, size_int(value, *flags, 10));
    if (!(value == 0 && flags->dot && !flags->dot_prec))
       ft_putnbr_fd(value, 1);
    else
        char_count = char_count - size_int(value, *flags, 10);
    if (flags->save_number)
        ft_putnbr_fd(flags->save_number, 1);
    if (flags->minus)
        char_count = char_count + put_width(*flags, size_int(value, *flags, 10));
    return (char_count + (size_int(value, *flags,10)));
}

int do_uint(unsigned long value, flaglist *flags)
{
    int char_count;

    char_count = 0;
    if (!flags->minus)
    {
        char_count = char_count + put_width(*flags, size_uint(value, *flags, 10));
        char_count = char_count + put_prec(*flags, size_uint(value, *flags, 10));
    }
    if (flags->minus)
        char_count = char_count + put_prec(*flags, size_uint(value, *flags, 10));
    if (!(value == 0 && flags->dot && !flags->dot_prec))
        ft_putnbr_fd(value, 1);
    else
        char_count = char_count - size_uint(value, *flags, 10);
    if (flags->minus)
        char_count = char_count + put_width(*flags, size_uint(value, *flags, 10));
    return (char_count + (size_uint(value, *flags,10)));
    /*
    if (flags->minus)
        ft_putnbr_fd(value, 1);
    char_count = char_count + put_width(*flags, size_int(value, *flags, 10));
    if (!flags->minus)
        ft_putnbr_fd(value, 1);
    return (char_count);*/
}


int do_string(char *value, flaglist *flags)
{
    int char_count;
    int value_len;

    if (value == NULL)
        value = "(null)";
    if (flags->dot_prec && flags->dot_prec < (int)ft_strlen(value))
        value_len = flags->dot_prec;
    else
        value_len = ft_strlen(value);
    if (flags->dot && !flags->dot_prec)
        value_len = 0;
//    if (flags->dot && flags->width)
//        flags->width = value_len;
    if (flags->minus)
        write(1, value, value_len);
    char_count = put_width(*flags, value_len);
    if (!flags->minus)
        write(1, value, value_len);
    return (char_count + value_len);
}

int do_char(int value, flaglist *flags) // different sequence might be better, if .. putwidth  --YOUR CODE--  if! putwidth
{
    int char_count;

    char_count = 0;
    if (flags->dot && flags->width && flags->dot_pos < flags->width_pos)
        flags->width = 1;
    if (flags->minus)
        ft_putchar_fd(value, 1);
    char_count = char_count + put_width(*flags, 1);
    if (!flags->minus)   // else
        ft_putchar_fd(value, 1);
    return (char_count + 1);
}

int do_pointer(void *value, flaglist *flags)
{
    int char_count;

    char_count = 0;
    if (!flags->minus)
    {
        char_count = char_count + put_width(*flags, size_uint((uintmax_t)value, *flags, 16) + 2);
        ft_putstr_fd("0x",1);
        char_count = char_count + put_prec(*flags, size_uint((uintmax_t)value, *flags, 16));
    }
    if (flags->minus)
    {
        ft_putstr_fd("0x",1);
        char_count = char_count + put_prec(*flags, size_uint((uintmax_t)value, *flags, 16) + 2);
    }
    if (!(value == 0 && flags->dot && !flags->dot_prec))
        print_pointer((uintmax_t)value, 0);
    else
        char_count = char_count - (size_uint((uintmax_t)value, *flags, 16));
    if (flags->minus)
        char_count = char_count + put_width(*flags, size_uint((uintmax_t)value, *flags, 16) + 2);
    return (char_count + (size_uint((uintmax_t)value, *flags,16) + 2));
}

int do_hex(unsigned long value, flaglist *flags, int caps)
{
    int char_count;

    char_count = 0;
    if (!flags->minus)
    {
        char_count = char_count + put_width(*flags, size_int(value, *flags, 16));
        char_count = char_count + put_prec(*flags, size_int(value, *flags, 16));
    }
    if (flags->minus)
        char_count = char_count + put_prec(*flags, size_int(value, *flags, 16));
    if (!(value == 0 && flags->dot && !flags->dot_prec))
        print_hex(value, caps);
    else
        char_count = char_count - size_int(value, *flags, 16);
    if (flags->minus)
        char_count = char_count + put_width(*flags, size_int(value, *flags, 16));
    return (char_count + (size_int(value, *flags,16)));
}

int control_datatypes(char c, va_list args, flaglist *flags) {
    int count = 0;

    if (c == 'd' || c == 'i')
        count = do_int(va_arg(args, int), flags);
    if (c == 's')
        count = do_string(va_arg(args, char*), flags);
    if (c == 'c')
        count = do_char(va_arg(args, int), flags);
    if (c == 'p')
        count = do_pointer(va_arg(args, void*), flags);
    if (c == 'u')
        count = do_uint(va_arg(args, unsigned int), flags);
    if (c == 'x')
        count = do_hex(va_arg(args, unsigned int), flags, 0);
    if (c == 'X')
        count = do_hex(va_arg(args, unsigned int), flags, 1);
    if (c == '%')
        count = do_char('%', flags);
    return count;
}

int		ft_isspecifier(int c)
{
    char *specifiers = "diucsxXp%";

    while (*specifiers)
    {
        if (c == *specifiers)
            return (1);
        specifiers++;
    }
    return (0);
}

int ft_set_flags(const char *format, flaglist *flags, va_list args, int spec)  // kan zonder i, met pointer++
{
    int i;
    int copy_i;

    i = 0;
    init_flags(flags);
    flags->spec = spec;
    while (!ft_isspecifier(format[i]) && format[i] && spec != '\0')
    {
        copy_i = i;
        if (format[i] == '0')
        {
            flags->zero = 1;
            i++;
            flags->zero_pos = i;
        }
        if (format[i] == '-')
        {
            if (!(flags->dot && flags-> width))   // check if format is 20.- or 20-., als dot achter width staat en - voorkomt, dan moet min uitstaan
                flags->minus = 1;
            i++;
            flags->minus_pos = i;
        }
        if (format[i] == '.')
        {
            flags->dot = 1;
            if (flags->minus && !flags->width)
                flags->minus = 0;
            i++;
            flags->dot_pos = i;
            while (ft_isdigit(format[i]) && (spec == 's' || spec == 'd' || spec == 'u' || spec == 'i' || spec == 'x' || spec == 'X' || spec == 'p'))
            {
                flags->dot_prec = flags->dot_prec * 10;
                flags->dot_prec = flags->dot_prec + ft_atoi((char[2]){(char)format[i], '\0'});        // might not be norm, pas atoi aan
                i++;
                flags->dot_prec_pos = i;
            }

        }
        if (format[i] == '*')
        {
            if (flags->dot_pos && flags->dot_pos <= i)
            {
                flags->dot_prec = va_arg(args, int);
                if (flags->dot_prec < 0)
                {
                    flags->dot_prec = 0;
                    flags->dot = 0;
                }
                i++;
                flags->dot_prec_pos = i;
            }
            else
            {
                flags->width = va_arg(args, int);
                i++;
                flags->width_pos = i;
            }
        }
        if (ft_isdigit(format[i]))
        {
            if (flags->width)
                flags->width = 0;
            while (ft_isdigit(format[i]))
            {
                flags->width = flags->width * 10;
                flags->width = flags->width + ft_atoi((char[2]){(char)format[i], '\0'});        // might not be norm , pas atoi aan
                i++;
                flags->width_pos = i;
            }
        }
        if (copy_i == i)
            i++;
    }
    if (flags->width < 0)
    {
        flags->width = flags->width * -1;
        flags->minus = 1;
    }
    if (flags->dot_prec < 0)
    {
       //flags->dot_prec = 0;
        flags->dot_prec = flags->dot_prec * -1;
        flags->minus = 1;
    }
    if (flags->width_pos < flags->dot_pos && flags->width_pos < flags->minus_pos && flags->dot_pos < flags-> minus_pos)
    {
        printf("WAAAAAA!!!!!!!!!!!!!!!");
    }
    return (i);
}

char get_next_specifier(const char *format)
{
    while (!ft_isspecifier(*format) && *format)             // kan problemen geven
        format++;
    return (*format);
}

int ft_printf(const char *format, ...) {  // kan korter als flags in setflags defined is

    va_list args;
    va_start(args, format);
    flaglist flags;
    int ret = 0;
    char spec;

    while (*format)
    {
        if (*format == '%')
        {
 //           if (*(format + 1) == '%')// format need another ++, also check if next char is \0
 //           {
  //              ft_putchar_fd('%', 1);
  //              format = format + 2;
 //               ret++;
  //          }
  //          else
            {
                format++;  // skip the '%'
                spec = get_next_specifier(format);
  //            ret = ret + ft_set_flags(format, &flags, args);            // betere oplossing?
                format = format + ft_set_flags(format, &flags, args, spec);
                ret = ret + control_datatypes(*format, args, &flags);
                if (ft_isspecifier(*format))
                    format++;  // skip the specifier (d, i , u, x, X) etc
            }
        }
        else {
            ft_putchar_fd(*format, 1);
            format++;
            ret++;
        }
    }
    return (ret);
}

int main()
{
 //   int a = 1243;
 //   unsigned int b = 4294967291;
 //   double d = 42.5678;
 //   double dou = 19.9876;
 //   long l = 9223372036854775801;
 //   char c = 'W';
    int d = 0;


//    printf("|%.8d|\n", a);
//    ft_printf("|%.8d|\n", a);
/*
    int r = printf("|%08.5d|", 34);
    printf("\n");
    int s = ft_printf("|%08.5d|", 34);
    printf("\n");
    printf("   printf return value : |%d|\n", r);
    ft_printf("ft printf return value : |%d|\n", s);
*/
 //   int mx_i = 0;
 //   char *mx_s = "Hello, World!";

  //  uintmax_t *value = 0;
 //   intmax_t *value2 = 0;

   // int ivalue = value / 10;
  //  int ivalue2 = value2 / 10;

    int j = printf("-->|%0*.0d|<--\n", 1, d);;
    printf("\n");
    int k = ft_printf("-->|%0*.0d|<--\n", 1, d);;
    printf("\n");
    printf("   printf return value : |%d|\n", j);
    ft_printf("ft printf return value : |%d|\n\n\n\n", k);
/*
    int v = printf("%u", INT_MAX + 1);
    printf("\n");
    int w = ft_printf("%u", INT_MAX + 1);
    printf("\n");
    printf("   printf return value : |%d|\n", v);
    ft_printf("ft printf return value : |%d|\n", w);

    int g = printf("Il fait au moins %u", -8000);
    printf("\n");
    int h = ft_printf("Il fait au moins %u", -8000);
    printf("\n");
    printf("   aaaaaaprintf return value : |%d|\n", g);
    ft_printf("ft printf return value : |%d|\n", h);
*/

//    printf("|%.3x|", 938862);
 //   printf("\n");
 //   ft_printf("|%.3x|", 938862);
  //  printf("   val |%-.8d|, char |%c|\n", a, c);
  //  ft_printf("ft val |%-.8d|, char |%c|\n", a, c);
  //  printf("   val |%.-8d|\n", a);
  //  ft_printf("ft val |%.-8d|\n", a);
 //   printf("   val |%-8d|\n", a);
 //   ft_printf("ft val |%-8d|\n\n", a);

 //   printf("   val |%-.18x|\n", a);
 //  ft_printf("ft val |%-.18x|\n", a);

  //  printf("   val |%-.8d|\n", a);
  //  ft_printf("ft val |%-.8d|\n\n", a);

 //   printf("val |%p|\n", dou);
  //  ft_printf("ft val |%p|\n", dou);

  //  printf("val2 |%p|\n", &dou);
  //  ft_printf("ft val2 |%p|\n\n\n", &dou);
  //  printf("val |%-5X|\n", &a);
 //   printf("val |%-5p|\n", &a);
 //  ft_printf("val |%-5p|\n", &a);

    //int j = 750;
    //printf("variable D is at address: %p / 0x%08x with value: %u\n", j, &j, j);


 //   printf("val1 |%20.-x|\n", &a);
  //  printf("val2 |%.20-p|\n", &a);
 //   printf("val3 |%d|\n", &a);
 //   printf("val4 |%u|\n", &a);
 //   printf("val5 |%X|\n", &a);
  //  printf("val6 |%c|\n", &a);
 //   printf("val7 |%s|\n\n\n", &a);

  //  ft_printf("val1 |%20.-x|\n", &a);
//   ft_printf("val2 |%.20-p|\n", &a);
 //   ft_printf("val3 |%d|\n", &a);
 //   ft_printf("val4 |%u|\n", &a);
 //   ft_printf("val5 |%X|\n", &a);
 //   ft_printf("val6 |%c|\n", &a);
  //  ft_printf("val7 |%s|\n\n", &a);

  //  printf("val |%%|\n");
    //ft_printf("|%d|\n", &a);
    //ft_printf("im |%0*d| y o", 5, 26);

 //   printf("   val |%0-8.d|\n", a);
 //   ft_printf("ft val |%0-8.d|\n", a);
    return (0);
}

/// PROBLEMS NOT HANDLED:             TODO :    maak functie ft_isvalidspecifier(char) -> put in getnextspec en set_flags                  ------- testen INT_MAX, UINT_MAX, HEX_MAX ETC

// problemen gevonden met int, 8.d werkt nu behalve return value maar 8.6 werkt nog niet omdat dot_prec niet met int werkt, .8d werkt wel al maar zal ook gebruik kunnen maken van
//                                                                                                                           dot_prec

// functies maken voor veel herhaalde lines of codes zoals place_width

// hoe gaat het met de volgorde van flags samen met 0?  -> |%0.-8d| is anders dan |%0-.8d|

// protect van write?

/// PROBLEMS PARTLY HANDLED:

/// volgorde van de flags -. en width matters alot -- (  DE GEVALLEN DIE AFWIJKEN EN BEKEND ZIJN WERKEN TOT NU TOE!! )   --->aparte functie die een controle op volgorde flags uitvoert in het stukje van minus, of positie van flag opslaan (in setflags bv dot * 10 + i)
// ^ gevallen die verschillen : |%8.-d|    (controlleer dit door alle combos af te werken, abc, bac, cba etc)
 //   combos: width dot minus!     dot width minus$     minus dot width$     width minus dot$     dot minus width$     minus width dot$
 //   abc! bac$ cba$ acb$ bca$ cab$


/// Wat als je width en * hebt, bv "|8*d|", int a = 8, int b = 5???

/// alle %specifiers werken , zelf pointers, maar wanneer ik een double meestuur ipv het juiste type geeft printf iets anders dan ft_printf











/*
int do_int(int value, flaglist *flags) // flags hoeft geen pntr te zijn kan ook alleen data
{
    int char_count;

    char_count = 0;
    if (value < 0) {
        char_count++;
        value = value * -1;
        flags->is_negative = 1;
        if (((flags->dot && flags->width_pos > flags->dot_pos)) || (flags->dot && flags->width && flags->width_pos > flags->dot_pos))
            ft_putchar_fd('-', 1);
    }
    if (!flags->minus)
        char_count = char_count + put_width(*flags, size_int(value, *flags, 10));
    if ((!flags->dot || flags->width_pos < flags->dot_pos) && flags->is_negative)    // dot pos en width pos matters !!!
        ft_putchar_fd('-', 1);
    ft_putnbr_fd(value, 1);
    if (flags->minus)
        char_count = put_width(*flags, size_int(value, *flags, 10));
    return (char_count);
}
*/


