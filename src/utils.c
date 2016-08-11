#include "screen.h"
#include "string.h"
#include "utils.h"

void itoa(char* buf, long num, int base){
	unsigned long i = num, i2 = num;
	int sig = 0;
	if(num < 0 && base == 10){
		sig = 1;
	}
	int j = 0;
	int k = 0;
	if(sig > 0){
		i2 = -i2;
		if(i2 == 0){
			buf[j] = '0';
			j++;
			goto num_is_zero;
		}
		while(i2 > 0){
			k = i2 % base;
			buf[j] = to_char(k);
			j++;
			i2 /= base;
		}
	}
	else{
		if(i == 0){
			buf[j] = '0';
			j++;
			goto num_is_zero;
		}
		while(i > 0){
			k = i % base;
			buf[j] = to_char(k);
			j++;
			i /= base;
		}
	}
	if(sig > 0){
		buf[j] = '-';
		j++;
	}
num_is_zero:
	if(base == 16){
		buf[j] = 'x';
		j++;
		buf[j] = '0';
		j++;
	}
	buf[j] = 0;
	strfilp(buf);
}
