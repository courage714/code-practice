
/*
*Implementation of binary search in the way of recursive method and iterative method
*Assumption: the input array has been already sorted
*/


#include <stdlib.h>
#include <stdio.h>


/*
* recursive binary search
*/
static int binary_search_r(int array[], int begin,int end, int target)
{
   int pos;
   if (!array )
   {
       printf("Error: the input array is wrong\n");
	   return -2;
   }
   if (end >= begin)
   {
       pos = begin + ((end - begin)>>1);
	   if (array[pos] == target)
	       return pos;
	   else if (array[pos] < target)
	   {
	       return binary_search_r(array,pos+1,end,target);
	   }
	   else
	   {
	       return binary_search_r(array,begin,pos-1,target);
	   }
   }
   else
   {
       return -1;
   }
}

/*
*iterative binary search
*/
static int binary_search_i(int array[],size_t size, int target)
{
    int begin, end, mid;
    if (!array || size == 0)
	{
	    printf("Error: input array is wrong!!!\n");
	    return -2;
	}
    begin = 0;
	end  = size - 1;
	while(end >= begin)
	{
	    mid =  begin + ((end-begin)>>1);
		if (array[mid] == target)
		    return mid;
		else if (array[mid] < target)
		{
		    begin = mid + 1;
		}
		else 
		{
		    end  = mid - 1;
		}
	}
	return -1;
}

static int binary_search_rightmost(int array[], size_t n, int x)
{
    int l,h,m;
	if ( !array || n==0 )
	{
	    printf("ERROR:Invalid array!\n");
		return -1;
	}
	l = 0;
	h = n-1;
	while( l<=h )
	{
	    m = l + ((h-l)>>1);
		if ( array[mid] == x )
		{
		    if ( mid == 0 )//arr[0] == x
			{ 
			    return mid;
			}
			else if ( array[mid-1] != x ) //arr[mid] is already rightmost element such as value is x
			{
			    return mid;
			}
		}

		if ( array[mid] >= x)
		{
		    h = mid - 1;    
		}
		else
		{
		    l = mid + 1;
		}

	}
}
static int binary_search_leftmost(int array[], size_t n, int x)
{
    int l,h,m;
	if ( !array || n==0 )
	{
	    printf("ERROR:Invalid array!\n");
		return -1;
	}
	l = 0;
	h = n-1;
	while( l<=h )
	{
	    m = l + ((h-l)>>1);
		if ( array[mid] == x )
		{
		    if ( mid == n-1 )//arr[n-1] == x
			{ 
			    return mid;
			}
			else if ( array[mid+1] != x ) //arr[mid] is already rightmost element such as value is x
			{
			    return mid;
			}
		}

		if ( array[mid] <= x)
		{
		    l = mid + 1;    
		}
		else
		{
		    h = mid - 1;
		}

	}
}

int main(int argc, char *argv[])
{
    int array[] = {2,4,6,7,8,10,12,34,67,68,68,100};
    int pos;
	int target = 12;
	pos =  binary_search_r(array,0,sizeof(array)/sizeof(int) - 1, target);
	printf("The position of %d in the array is %d \n",target, pos);
    target = 68;
	pos =  binary_search_r(array,0,sizeof(array)/sizeof(int) - 1, target);
	printf("The position of %d in the array is %d \n",target, pos);
    target = 3;
	pos =  binary_search_r(array,0,sizeof(array)/sizeof(int) - 1, target);
	printf("The position of %d in the array is %d \n",target, pos);

    target = 12;
	pos =  binary_search_i(array,sizeof(array)/sizeof(int), target);
	printf("The position of %d in the array is %d \n",target, pos);
    target = 68;
	pos =  binary_search_i(array,sizeof(array)/sizeof(int), target);
	printf("The position of %d in the array is %d \n",target, pos);
    target = 3;
	pos =  binary_search_i(array,sizeof(array)/sizeof(int), target);
	printf("The position of %d in the array is %d \n",target, pos);
}
