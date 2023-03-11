#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include "wolfpack.h"

void print_packet_sf(const unsigned char *packet) {
     
    unsigned long long address=0;
    for(int i=0;i<5;i++)
    {
        address=(address<<8) | packet[i];
    }
    address=0;
    printf("%010llx\n",address);
    for(int i=5;i<10;i++)
    {
        address=(address<<8) | packet[i];
    }
    unsigned int source_address=0;
    printf("%010llx\n",address);
    printf("%02x\n",packet[10]);
    printf("%02x\n",packet[11]);
    for(int i=12;i<15;i++)
    {
        source_address=(source_address<<8) | packet[i];
    }
    printf("%06x\n",source_address);
    source_address=0;
     for(int i=15;i<17;i++)
    {
        source_address=(source_address<<8) | packet[i];
    }
     
     printf("%04x\n",source_address);
    source_address=0;
     for(int i=17;i<20;i++)
    {
        source_address=(source_address<<8) | packet[i];
    }
     int length= source_address;;
    
    printf("%06x\n",source_address);
    source_address=0;
     for(int i=20;i<24;i++)
    {
        source_address=(source_address<<8) | packet[i];
    }
    printf("%08x\n",source_address);
    int length1=length-24;;
    for(int i=24;i<(length1+24);i++)
    {
       char c=packet[i];
       printf("%c",c);
    }
    printf("\n");
    

}

unsigned int packetize_sf(const char *message, unsigned char *packets[], unsigned int packets_len, unsigned int max_payload,
    unsigned long src_addr, unsigned long dest_addr, unsigned short flags) {
    return 0;
}

unsigned int checksum_sf(const unsigned char *packet) {
   unsigned long long address=0;
    unsigned int sum=0;
    for(int i=0;i<5;i++)
    {
        address=(address<<8) | packet[i];
        
    }
    sum=sum+address;
    address=0;
    
    for(int i=5;i<10;i++)
    {
        address=(address<<8) | packet[i];
    }
    sum=sum+address;
    address=0;
    address=(address<<8) |packet[10];
    sum=sum+address;
    address=0;
    address=(address<<8) |packet[11];
    sum=sum+address;
    unsigned int source_address=0;
    
    for(int i=12;i<15;i++)
    {
        source_address=(source_address<<8) | packet[i];
    }
    sum=sum+source_address;
    source_address=0;
     for(int i=15;i<17;i++)
    {
        source_address=(source_address<<8) | packet[i];
    }
     
     sum=sum+source_address;
    source_address=0;
     for(int i=17;i<20;i++)
    {
        source_address=(source_address<<8) | packet[i];
    }
    sum=sum+source_address;
    unsigned int checksum= sum % (int)(pow(2,32)-1);
    return checksum;

}

unsigned int reconstruct_sf(unsigned char *packets[], unsigned int packets_len, char *message, unsigned int message_len) {
    return 0;
}

