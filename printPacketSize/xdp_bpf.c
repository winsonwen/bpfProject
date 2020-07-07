#include <stdbool.h>
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/pkt_cls.h>
#include <stdio.h>
#include "bpf_endian.h"
#include "bpf_helpers.h"

#ifndef __inline
# define __inline                         \
   inline __attribute__((always_inline))
#endif

#define bpfprint(fmt, ...)                        \
    ({                                             \
        char ____fmt[] = fmt;                      \
        bpf_trace_printk(____fmt, sizeof(____fmt), \
                         ##__VA_ARGS__);           \
    })


SEC("xdp")
int xdp_print_size(struct xdp_md *ctx)
{
	
	void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
	
	// transfer data to struct ethhdr
	struct ethhdr *eth = data;    
	// transfer struct ethhdr to struct iphdr
	struct iphdr *iph = (struct iphdr *)(eth + 1);

	// Check the Ethernet frame has IP packet or not
	if ((void *)(iph + 1) > data_end)
     	 return XDP_PASS;

	// obtain realative information
	unsigned int ipSzie = iph->tot_len;
	//unsigned int headerLength = iph->ihl;
	unsigned int ip_src = iph->saddr;
	
	// output
	bpfprint("src ip addr1: %d.%d.%d\n",(ip_src) & 0xFF,(ip_src >> 8) & 0xFF,(ip_src >> 16) & 0xFF);
    	bpfprint("src ip addr1: .%d\n",(ip_src >> 24) & 0xFF);
	//bpfprint("Header Length : %d byte\n",headerLength);
	bpfprint("The total size of datagram: %d byte\n",ipSzie);
  	return XDP_PASS;
}
char _license[] SEC("license") = "GPL";









