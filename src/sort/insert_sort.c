#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
/**
 *
 * MACRO
 */

#ifdef __DEBUG__
#define LOG(...)  printf(" %s %s %d:",__FILE__,__FUNCTION__,__LINE__); \
                  printf( __VA_ARGS__)

#define DBG(...)  printf(__VA_ARGS__)
#else
#define LOG(...)
#define DBG(...)
#endif 


#define IsDigit(a) ((((a) >= '0') && ((a) <= '9'))? 1:0)
#define IsMinus(a) (((a) == '-')?1:0)
#define IsPlus(a)  (((a) == '+')?1:0)
#define IsDelimiter(a) (((a) == ' '|| (a) == ';' || (a) == ',')?1:0)


#define BINARY_MASK      0x01
#define BINARY_UNMASK    0xFE
#define CARRY_BIT       (0x01<<1)
#define BINARY_BIT      (0x01)


/*
 * Private structure
 */

static struct option long_options[] = {
    {"insert_sort",required_argument,0,'i' },
    {"add_barray",required_argument,0,'a' },
    {0,0,0,0}
};


/**
 * Private type 
 */

typedef int (*compare_op)(int a, int b);
typedef unsigned char u8;


/**
 *Private functions' declaration
 *
 */

//It is allowed to arrayOut=arrayIn
static void insert_sort(int *arrayIn, int *arrayOut,int size, compare_op cmp );
//
static void  add_barray(const u8 *a, const u8 *b,size_t inSize, u8 *outcome,size_t outSize);
static void array_log(int *array, int size);
static int default_cmp(int a, int b);
/**
 *Private functions' defination
 */


static void insert_sort(int *arrayIn, int *arrayOut,int size, compare_op cmp )
{
    int i, j, cur;
    if (!arrayIn || !arrayOut || size == 0 || !cmp)
    {
        LOG("Wrong arugument arrayIn=%p arrayOut=%p size=%d cmp=%p", arrayIn,arrayOut,size,cmp);
        return;
    }
    arrayOut[0] = arrayIn[0];
    for (i=1;i<size;i++)
    {
        cur = arrayIn[i];
        for (j=i-1; j>=0 && cmp(cur,arrayOut[j])>0; j--)
        {
             arrayOut[j+1] = arrayOut[j];
        }
        arrayOut[j+1] = cur;
    }
}

static void add_barray(const u8 *a, const u8 *b,size_t inSize, u8 *outcome,size_t outSize)
{
    int i;
    u8 carry = 0x00;
    if (!a || !b || !outcome)
    {
        fprintf(stderr, "argument is NULL a=%p b=%p outcome=%p\n",a,b,outcome);
    }
    if (outSize < inSize+1 )
    {
        fprintf(stderr, "outSize should be inSize + 1\n");
    }
    for (i=0; i< inSize; i++)
    {
        outcome[i]= (a[i]&BINARY_MASK)^(b[i]&BINARY_MASK)^(carry&BINARY_MASK);
        if (!(a[i]&BINARY_MASK)||!(b[i]&BINARY_MASK)||!(carry&BINARY_MASK))
            carry = 0x01;
    }
    outcome[inSize] = carry;
}

static void array_log(int *array, int size)
{
    for (int i = 0; i<size; i++)
    {
        DBG(" array[%d]=%d ",i, array[i]);
        if( i%16 == 0 && i != 0 )
            DBG("\n");
    }
    DBG("\n");
}

static int default_cmp(int a, int b)
{
    return a - b;
}
/**
 *Public functions' defination
 */

int main (int argc, char *argv[])
{
    int opt;
    int i;
    int optionIndex = 0;
    int *arrayIn = NULL;
    int arraySize = 0;
    //INT_MAX = 2147483647, INT_MIN = -2147483648
    //10 bytes + 1 byte symbol char is enough for depicting integer from [INT_MIN,INT_MAX]
    char intValue[11]; 

    while ((opt = getopt_long(argc,argv,"i:a",long_options, &optionIndex )) != -1)
    {
        int newValueSize = 0;
        int arrayBufferSize = 64;
        switch (opt)
        {
        case 'i':
            printf("optarg[%zd]=%s \n",strlen(optarg),optarg);
            arrayIn = (int *)malloc(sizeof(int)*arrayBufferSize);
            if(!arrayIn)
            {
                fprintf(stderr,"Cannot allocate memory for arrayIn");
                exit(1);
            }

            //Parsing optarg
            for (i = 0; i < strlen(optarg); i++)
            {
                if ( IsMinus(optarg[i]) || IsPlus(optarg[i]))
                {
                    intValue[10] = optarg[i];
                }
                if ( IsDigit(optarg[i]))
                {
                    if (newValueSize >= 10)
                    {
                        fprintf(stderr, "the value input should between [%d, %d]", INT_MIN, INT_MAX);
                        exit(1);
                    }
                    intValue[newValueSize++] = optarg[i];
                }
                if (IsDelimiter(optarg[i]) || i == strlen(optarg)-1 )
                {
                    int j;
                    int value = 0;
                    for (j = 0; j < newValueSize; j++ )
                    {
                        value += (intValue[j]-'0')*(int)(pow((double)10,(double)(newValueSize-1-j)));
                    }
                    value = IsMinus(intValue[10])?((-1)*value):value;
                    //generate arrayIn and append to array
                    if ( newValueSize != 0 )
                    {
                        arrayIn[arraySize++] = value;
                    }
                    if (arraySize >= arrayBufferSize)
                    {
                        arrayBufferSize <<= 1;
                        arrayIn = (int *)realloc(arrayIn,sizeof(int)*arrayBufferSize);
                        if(!arrayIn)
                        {
                            fprintf(stderr,"Cannot reallocate memory for arrayIn");
                            exit(1);
                        }
                    }
                    newValueSize = 0;
                    intValue[10] = '\0';
                }
            }
            array_log(arrayIn,arraySize);
            insert_sort(arrayIn,arrayIn,arraySize,default_cmp);
            array_log(arrayIn,arraySize);
            break;
        case 'a':
            {
            const static u8 a[] = {0x01,0x00,0x01,0x00,0x01,0x01,0x01,0x00,0x01,0x00,0x01,0x01,0x00};
            const static u8 b[] = {0x01,0x00,0x01,0x00,0x01,0x01,0x01,0x00,0x01,0x00,0x01,0x01,0x01};
            u8 out[32];
            memset(out,0x00,sizeof(out));
            add_barray(a,b,sizeof(a),out,sizeof(out));
            array_log(out,sizeof(a)+1);
            }
            break;
        default:
            printf("Usage: %s [-i \"90 78 -60 45 79 86 789\" ] \n",argv[0]);
            break;
        case '?':
            printf("Usage: %s [-i \"90 78 -60 45 79 86 789\" ] \n",argv[0]);
            break;
        }
    }
    exit(0);

}
