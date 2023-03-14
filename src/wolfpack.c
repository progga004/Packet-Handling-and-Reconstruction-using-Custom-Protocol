#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include "wolfpack.h"

void print_packet_sf(const unsigned char *packet)
{

    unsigned long long address = 0;
    for (int i = 0; i < 5; i++)
    {
        address = (address << 8) | packet[i];
    }

    printf("%010llx\n", address);
    address = 0;
    for (int i = 5; i < 10; i++)
    {
        address = (address << 8) | packet[i];
    }
    unsigned int source_address = 0;
    printf("%010llx\n", address);
    printf("%02x\n", packet[10]);
    printf("%02x\n", packet[11]);
    for (int i = 12; i < 15; i++)
    {
        source_address = (source_address << 8) | packet[i];
    }
    printf("%06x\n", source_address);
    source_address = 0;
    for (int i = 15; i < 17; i++)
    {
        source_address = (source_address << 8) | packet[i];
    }

    printf("%04x\n", source_address);
    source_address = 0;
    for (int i = 17; i < 20; i++)
    {
        source_address = (source_address << 8) | packet[i];
    }
    int length = source_address;
    ;

    printf("%06x\n", source_address);
    source_address = 0;
    for (int i = 20; i < 24; i++)
    {
        source_address = (source_address << 8) | packet[i];
    }
    printf("%08x\n", source_address);
    int length1 = length - 24;
    ;
    for (int i = 24; i < (length1 + 24); i++)
    {
        char c = packet[i];
        printf("%c", c);
    }
    printf("\n");
}
unsigned int checksum_sf(const unsigned char *packet);
unsigned int packetize_sf(const char *message, unsigned char *packets[], unsigned int packets_len, unsigned int max_payload,
                          unsigned long src_addr, unsigned long dest_addr, unsigned short flags)
{

   unsigned int count=(strlen(message)/max_payload);
	    unsigned int payloads_length=max_payload;
	    
	    unsigned int new_count=0;
	    //printf("the message length is: %d\n",strlen(message));
	    if(strlen(message)%max_payload!=0)
	    {
	        count=count+1;
	    }
	    unsigned int i=0;
	    for( i=0;i<packets_len;i++)
	    {
	        //extract the payloads length
	        
	        if(i==count-1 && strlen(message)%max_payload!=0)
	        {
	            payloads_length=strlen(message)%max_payload;
	           
	        }
	       
	        unsigned int fragment_offset=i*max_payload;
	        unsigned int total_length=payloads_length+24;// confusion check this one
	        packets[i]=malloc(24+payloads_length);
	        packets[i][0]=(src_addr>>32) & 0xFF;
	        packets[i][1]=(src_addr>>24) & 0xFF;
	        packets[i][2]=(src_addr>>16) & 0xFF;
	        packets[i][3]=(src_addr>>8) & 0xFF;
	        packets[i][4]=(src_addr>>0) & 0xFF;
	        packets[i][5]=(dest_addr>>32) & 0xFF;
	        packets[i][6]=(dest_addr>>24) & 0xFF;
	        packets[i][7]=(dest_addr>>16) & 0xFF;
	        packets[i][8]=(dest_addr>>8) & 0xFF;
	        packets[i][9]=(dest_addr>>0) & 0xFF;
	        packets[i][10]=32;
	        packets[i][11]=64;
	        //calculate the fragment offset
	        packets[i][12]=(fragment_offset>>16) & 0xFF;
	        packets[i][13]=(fragment_offset>>8) & 0xFF;
	        packets[i][14]=(fragment_offset>>0) & 0xFF;
	        packets[i][15]=(flags>>8) & 0xFF;
	        packets[i][16]=(flags>>0) & 0xFF;
	        packets[i][17]=(total_length>>16) & 0xFF;
	        packets[i][18]=(total_length>>8) & 0xFF;
	        packets[i][19]=(total_length>>0) & 0xFF;
	        //calculate the checksum
	        unsigned int checksum=checksum_sf(packets[i]);
	        packets[i][20]=(checksum>>24) & 0xFF;
	        packets[i][21]=(checksum>>16) &0xFF;
	        packets[i][22]=(checksum>>8) & 0xFF;
	        packets[i][23]=(checksum>>0) & 0xFF;
	        //calculating the payload
	        unsigned int p=fragment_offset;
	        for(unsigned int k=24;k<payloads_length+24;k++)
	        {
	            packets[i][k]=message[p];
	           //printf("the packets contents are: %02x\n",packets[i][k]);
	            p++;
	        }
	        new_count++;
	        //printf("Go there\n");
	       
	        
	    }
	    
	   packets_len=new_count; 
      return packets_len; 
}

unsigned int checksum_sf(const unsigned char *packet)
{
    unsigned long long address = 0;
    unsigned long sum = 0;
    for (int i = 0; i < 5; i++)
    {
        address = (address << 8) | packet[i];
    }
    sum = sum + address;
    address = 0;

    for (int i = 5; i < 10; i++)
    {
        address = (address << 8) | packet[i];
    }
    sum = sum + address;
    address = 0;
    address = (address << 8) | packet[10];
    sum = sum + address;
    address = 0;
    address = (address << 8) | packet[11];
    sum = sum + address;
    unsigned int source_address = 0;

    for (int i = 12; i < 15; i++)
    {
        source_address = (source_address << 8) | packet[i];
    }
    sum = sum + source_address;
    source_address = 0;
    for (int i = 15; i < 17; i++)
    {
        source_address = (source_address << 8) | packet[i];
    }

    sum = sum + source_address;
    source_address = 0;
    for (int i = 17; i < 20; i++)
    {
        source_address = (source_address << 8) | packet[i];
    }
    sum = sum + source_address;
    unsigned int checksum = sum % (unsigned long)(pow(2, 32) - 1);
    return checksum;
}





int total_length_read(unsigned char *packets) // with the payload
{
    
    int address=0;
    int sum=0;
    for(int i=17;i<20;i++)
    {
        address=(address<<8)|packets[i];
    }
    sum=sum+address;
    int length=sum;
    return length;
}
unsigned int reconstruct_sf(unsigned char *packets[], unsigned int packets_len, char *message, unsigned int message_len)
{
   
   unsigned long index=0;
   unsigned int count=0;
   unsigned long sum=0;
   for(unsigned int i=0;i<packets_len;i++) // individual packets
   {
    unsigned int checksum= checksum_sf(packets[i]);// passing the checksum of the individual packets
    unsigned int source_address=0;
     
     for(int j=20;j<24;j++) //extract the checksum
    {
       
        source_address=(source_address<<8) | packets[i][j];
    }
   
      
      if(source_address==checksum) // valid packet
      {
        source_address=0;
        sum=0;
        
        for(int p=12;p<15;p++) // extract the fragment offset
        {
            source_address=(source_address<<8)| packets[i][p];
            
        }
        
        sum=sum+source_address; //from where the message will start// fragment part
        int length= total_length_read(packets[i]);
        int payload_length=length-24; // payload length has got, now go throgh the packets[j]
        
        for(int k=24;k<24+payload_length;k++) //extraxting the payload from the packet and write it into the message
        {
         char c = packets[i][k];
        
          if(sum<=message_len-1) // store upto message_len -1 bytes
        {
            
            message[sum]=c;
            
             if(index<sum)
            {
                index=sum;
            }
            
           unsigned int p=sum+1;
            if(p<=message_len-1)
            {
                sum=p;
            }
         
        }
       
        
        else
        {
            message[sum-1]='\0';
            break;
        }
        }
        
        
        if(sum!=message_len && sum<=message_len-1)
        { 
            count=count+1;
           
      }
   
       
      
   }}
   message[index+1]='\0';
   //printf("The message is:%s\n",message);
    //printf("The count is:%d\n",count);
   return count;  
}
     
        
       
          
        
        
       
      
   


// int main()
// {
//      unsigned char packet[] = "\x00\x00\x00\x30\x39\x00\x00\x01\x09\x3b\x20\x40\x00\x00\x00\x10\x00\x00\x00\x1d\x00\xa5\x25\x17\x41\x42\x43\x44\x45RANDOM GARBAGE YOU SHOULD NOT SEE THIS";
//      print_packet_sf(packet);

//     return 0;
// }
