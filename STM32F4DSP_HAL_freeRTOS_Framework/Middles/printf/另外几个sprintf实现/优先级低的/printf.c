/**
 * @file printf.c
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief C库标准printf()函数
 *
 * 更改建议:不建议修改
 *
 * 版权所有:北京拉普兰德电子技术有限公司
 * http://www.lpld.cn
 * mail:support@lpld.cn
 *
 * @par
 * 本代码由拉普兰德[LPLD]开发并维护，并向所有使用者开放源代码。
 * 开发者可以随意修使用或改源代码。但本段及以上注释应予以保留。
 * 不得更改或删除原版权所有者姓名，二次开发者可以加注二次版权所有者。
 * 但应在遵守此协议的基础上，开放源代码、不得出售代码本身。
 * 拉普兰德不负责由于使用本代码所带来的任何事故、法律责任或相关不良影响。
 * 拉普兰德无义务解释、说明本代码的具体原理、功能、实现方法。
 * 除非拉普兰德[LPLD]授权，开发者不得将本代码用于商业产品。
 */

/*2020年我又看了github的LPLD库，没有更新*/
//#include "common.h"
#include <stdarg.h>

/********************************************************************/

typedef struct
{
    int32 dest;
    void (*func)(int8);
    int8 *loc;
} PRINTK_INFO;

int32 
printk (PRINTK_INFO *, const int8 *, va_list);

/********************************************************************/

#define DEST_CONSOLE    (1)
#define DEST_STRING     (2)

#define FLAGS_MINUS     (0x01)
#define FLAGS_PLUS      (0x02)
#define FLAGS_SPACE     (0x04)
#define FLAGS_ZERO      (0x08)
#define FLAGS_POUND     (0x10)

#define IS_FLAG_MINUS(a)    (a & FLAGS_MINUS)
#define IS_FLAG_PLUS(a)     (a & FLAGS_PLUS)
#define IS_FLAG_SPACE(a)    (a & FLAGS_SPACE)
#define IS_FLAG_ZERO(a)     (a & FLAGS_ZERO)
#define IS_FLAG_POUND(a)    (a & FLAGS_POUND)

#define LENMOD_h        (0x01)
#define LENMOD_l        (0x02)
#define LENMOD_L        (0x04)

#define IS_LENMOD_h(a)  (a & LENMOD_h)
#define IS_LENMOD_l(a)  (a & LENMOD_l)
#define IS_LENMOD_L(a)  (a & LENMOD_L)

#define FMT_d   (0x0001)
#define FMT_o   (0x0002)
#define FMT_x   (0x0004)
#define FMT_X   (0x0008)
#define FMT_u   (0x0010)
#define FMT_c   (0x0020)
#define FMT_s   (0x0040)
#define FMT_p   (0x0080)
#define FMT_n   (0x0100)

#define IS_FMT_d(a)     (a & FMT_d)
#define IS_FMT_o(a)     (a & FMT_o)
#define IS_FMT_x(a)     (a & FMT_x)
#define IS_FMT_X(a)     (a & FMT_X)
#define IS_FMT_u(a)     (a & FMT_u)
#define IS_FMT_c(a)     (a & FMT_c)
#define IS_FMT_s(a)     (a & FMT_s)
#define IS_FMT_p(a)     (a & FMT_p)
#define IS_FMT_n(a)     (a & FMT_n)

/********************************************************************/
static void printk_putc (int32 c, int32 *count, PRINTK_INFO *info)
{
    switch (info->dest)
    {
        case DEST_CONSOLE:
            info->func((int8)c);
            break;
        case DEST_STRING:
            *(info->loc) = (uint8)c;
            ++(info->loc);
            break;
        default:
            break;
    }
    *count += 1;
}

/********************************************************************/
static int32 printk_mknumstr (int8 *numstr, void *nump, int32 neg, int32 radix)
{
    int32 a,b,c;
    uint32 ua,ub,uc;

    int32 nlen;
    int8 *nstrp;

    nlen = 0;
    nstrp = numstr;
    *nstrp++ = '\0';

    if (neg)
    {
        a = *(int32 *)nump;
        if (a == 0)
        {
            *nstrp = '0';
            ++nlen;
            goto done;
        }
        while (a != 0)
        {
            b = (int32)a / (int32)radix;
            c = (int32)a - ((int32)b * (int32)radix);
            if (c < 0)
            {
                c = ~c + 1 + '0';
            }
            else
            {
                c = c + '0';
            }
            a = b;
            *nstrp++ = (int8)c;
            ++nlen;
        }
    }
    else
    {
        ua = *(uint32 *)nump;
        if (ua == 0)
        {
            *nstrp = '0';
            ++nlen;
            goto done;
        }
        while (ua != 0)
        {
            ub = (uint32)ua / (uint32)radix;
            uc = (uint32)ua - ((uint32)ub * (uint32)radix);
            if (uc < 10)
            {
                uc = uc + '0';
            }
            else
            {
                uc = uc - 10 + 'A';
            }
            ua = ub;
            *nstrp++ = (int8)uc;
            ++nlen;
        }
    }
    done:
    return nlen;
}

/********************************************************************/
static void printk_pad_zero (int32 curlen, int32 field_width, int32 *count, PRINTK_INFO *info)
{
    int32 i;

    for (i = curlen; i < field_width; i++)
    {
        printk_putc('0',count, info);
    }
}

/********************************************************************/
static void
printk_pad_space (int32 curlen, int32 field_width, int32 *count, PRINTK_INFO *info)
{
    int32 i;

    for (i = curlen; i < field_width; i++)
    {
        printk_putc(' ',count, info);
    }
}

/********************************************************************/
int32 printk (PRINTK_INFO *info, const int8 *fmt, va_list ap)
{
    /* va_list ap; */
    int8 *p;
    int32 c;

    int8 vstr[33];
    int8 *vstrp;
    int32 vlen;

    int32 done;
    int32 count = 0;

    int32 flags_used;
    int32 field_width;
#if 0
    int32 precision_used;
    int32 precision_width;
    int32 length_modifier;
#endif

    int32 ival;
    int32 schar, dschar;
    int32 *ivalp;
    int8 *sval;
    int32 cval;
    uint32 uval;

    /*
     * Start parsing apart the format string and display appropriate
     * formats and data.
     */
    for (p = (int8 *)fmt; (c = *p) != 0; p++)
    {
        /*
         * All formats begin with a '%' marker.  Special chars like
         * '\n' or '\t' are normally converted to the appropriate
         * character by the __compiler__.  Thus, no need for this
         * routine to account for the '\' character.
         */
        if (c != '%')
        {
            /*
             * This needs to be replaced with something like
             * 'out_char()' or call an OS routine.
             */
#ifndef UNIX_DEBUG
            if (c != '\n')
            {
                printk_putc(c, &count, info);
            }
            else
            {
                printk_putc(0x0D /* CR */, &count, info);
                printk_putc(0x0A /* LF */, &count, info);
            }
#else
            printk_putc(c, &count, info);
#endif

            /*
             * By using 'continue', the next iteration of the loop
             * is used, skipping the code that follows.
             */
            continue;
        }

        /*
         * First check for specification modifier flags.
         */
        flags_used = 0;
        done = FALSE;
        while (!done)
        {
            switch (/* c = */ *++p)
            {
                case '-':
                    flags_used |= FLAGS_MINUS;
                    break;
                case '+':
                    flags_used |= FLAGS_PLUS;
                    break;
                case ' ':
                    flags_used |= FLAGS_SPACE;
                    break;
                case '0':
                    flags_used |= FLAGS_ZERO;
                    break;
                case '#':
                    flags_used |= FLAGS_POUND;
                    break;
                default:
                    /* we've gone one int8 too far */
                    --p;
                    done = TRUE;
                    break;
            }
        }

        /*
         * Next check for minimum field width.
         */
        field_width = 0;
        done = FALSE;
        while (!done)
        {
            switch (c = *++p)
            {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    field_width = (field_width * 10) + (c - '0');
                    break;
                default:
                    /* we've gone one int8 too far */
                    --p;
                    done = TRUE;
                    break;
            }
        }

        /*
         * Next check for the width and precision field separator.
         */
        if (/* (c = *++p) */ *++p == '.')
        {
            /* precision_used = TRUE; */

            /*
             * Must get precision field width, if present.
             */
            /* precision_width = 0; */
            done = FALSE;
            while (!done)
            {
                switch (/* c = uncomment if used below */ *++p)
                {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
#if 0
                        precision_width = (precision_width * 10) +
                            (c - '0');
#endif
                        break;
                    default:
                        /* we've gone one int8 too far */
                        --p;
                        done = TRUE;
                        break;
                }
            }
        }
        else
        {
            /* we've gone one int8 too far */
            --p;
#if 0
            precision_used = FALSE;
            precision_width = 0;
#endif
        }

        /*
         * Check for the length modifier.
         */
        /* length_modifier = 0; */
        switch (/* c = */ *++p)
        {
            case 'h':
                /* length_modifier |= LENMOD_h; */
                break;
            case 'l':
                /* length_modifier |= LENMOD_l; */
                break;
            case 'L':
                /* length_modifier |= LENMOD_L; */
                break;
            default:
                /* we've gone one int8 too far */
                --p;
                break;
        }

        /*
         * Now we're ready to examine the format.
         */
        switch (c = *++p)
        {
            case 'd':
            case 'i':
                ival = (int32)va_arg(ap, int32);
                vlen = printk_mknumstr(vstr,&ival,TRUE,10);
                vstrp = &vstr[vlen];

                if (ival < 0)
                {
                    schar = '-';
                    ++vlen;
                }
                else
                {
                    if (IS_FLAG_PLUS(flags_used))
                    {
                        schar = '+';
                        ++vlen;
                    }
                    else
                    {
                        if (IS_FLAG_SPACE(flags_used))
                        {
                            schar = ' ';
                            ++vlen;
                        }
                        else
                        {
                            schar = 0;
                        }
                    }
                }
                dschar = FALSE;
            
                /*
                 * do the ZERO pad.
                 */
                if (IS_FLAG_ZERO(flags_used))
                {
                    if (schar)
                        printk_putc(schar, &count, info);
                    dschar = TRUE;
            
                    printk_pad_zero (vlen, field_width, &count, info);
                    vlen = field_width;
                }
                else
                {
                    if (!IS_FLAG_MINUS(flags_used))
                    {
                        printk_pad_space (vlen, field_width, &count, info);
            
                        if (schar)
                            printk_putc(schar, &count, info);
                        dschar = TRUE;
                    }
                }
            
                /* the string was built in reverse order, now display in */
                /* correct order */
                if (!dschar && schar)
                {
                    printk_putc(schar, &count, info);
                }
                goto cont_xd;

            case 'x':
            case 'X':
                uval = (uint32)va_arg(ap, uint32);
                vlen = printk_mknumstr(vstr,&uval,FALSE,16);
                vstrp = &vstr[vlen];

                dschar = FALSE;
                if (IS_FLAG_ZERO(flags_used))
                {
                    if (IS_FLAG_POUND(flags_used))
                    {
                        printk_putc('0', &count, info);
                        printk_putc('x', &count, info);
                        /*vlen += 2;*/
                        dschar = TRUE;
                    }
                    printk_pad_zero (vlen, field_width, &count, info);
                    vlen = field_width;
                }
                else
                {
                    if (!IS_FLAG_MINUS(flags_used))
                    {
                        if (IS_FLAG_POUND(flags_used))
                        {
                            vlen += 2;
                        }
                        printk_pad_space (vlen, field_width, &count, info);
                        if (IS_FLAG_POUND(flags_used))
                        {
                            printk_putc('0', &count, info);
                            printk_putc('x', &count, info);
                            dschar = TRUE;
                        }
                    }
                }

                if ((IS_FLAG_POUND(flags_used)) && !dschar)
                {
                    printk_putc('0', &count, info);
                    printk_putc('x', &count, info);
                    vlen += 2;
                }
                goto cont_xd;

            case 'o':
                uval = (uint32)va_arg(ap, uint32);
                vlen = printk_mknumstr(vstr,&uval,FALSE,8);
                goto cont_u;
            case 'b':
                uval = (uint32)va_arg(ap, uint32);
                vlen = printk_mknumstr(vstr,&uval,FALSE,2);
                goto cont_u;
            case 'p':
                uval = (uint32)va_arg(ap, void *);
                vlen = printk_mknumstr(vstr,&uval,FALSE,16);
                goto cont_u;
            case 'u':
                uval = (uint32)va_arg(ap, uint32);
                vlen = printk_mknumstr(vstr,&uval,FALSE,10);

                cont_u:
                    vstrp = &vstr[vlen];

                    if (IS_FLAG_ZERO(flags_used))
                    {
                        printk_pad_zero (vlen, field_width, &count, info);
                        vlen = field_width;
                    }
                    else
                    {
                        if (!IS_FLAG_MINUS(flags_used))
                        {
                            printk_pad_space (vlen, field_width, &count, info);
                        }
                    }

                cont_xd:
                    while (*vstrp)
                        printk_putc(*vstrp--, &count, info);

                    if (IS_FLAG_MINUS(flags_used))
                    {
                        printk_pad_space (vlen, field_width, &count, info);
                    }
                break;

            case 'c':
                cval = (int8)va_arg(ap, uint32);
                printk_putc(cval,&count, info);
                break;
            case 's':
                sval = (int8 *)va_arg(ap, int8 *);
                if (sval)
                {
                    vlen = strlen(sval);
                    if (!IS_FLAG_MINUS(flags_used))
                    {
                        printk_pad_space (vlen, field_width, &count, info);
                    }
                    while (*sval)
                        printk_putc(*sval++,&count, info);
                    if (IS_FLAG_MINUS(flags_used))
                    {
                        printk_pad_space (vlen, field_width, &count, info);
                    }
                }
                break;
            case 'n':
                ivalp = (int32 *)va_arg(ap, int32 *);
                *ivalp = count;
                break;
            default:
                printk_putc(c,&count, info);
                break;
        }
    }
    return count;
}

/********************************************************************/
int32 printf (const int8 *fmt, ...)
{
    va_list ap;
    int32 rvalue;
    PRINTK_INFO info;


    info.dest = DEST_CONSOLE;
    info.func = &out_char;
    /*
     * Initialize the pointer to the variable length argument list.
     */
    va_start(ap, fmt);
    rvalue = printk(&info, fmt, ap);
    /*
     * Cleanup the variable length argument list.
     */
    va_end(ap);
    return rvalue;
}

/********************************************************************/
int32 sprintf (int8 *s, const int8 *fmt, ...)
{
    va_list ap;
    int32 rvalue = 0;
    PRINTK_INFO info;

    /*
     * Initialize the pointer to the variable length argument list.
     */
    if (s != 0)
    {
        info.dest = DEST_STRING;
        info.loc = s;
        va_start(ap, fmt);
        rvalue = printk(&info, fmt, ap);
        *info.loc = '\0';
        va_end(ap);
    }
    return rvalue;
}

/********************************************************************/
