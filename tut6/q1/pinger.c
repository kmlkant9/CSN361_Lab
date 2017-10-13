/*
usage: sudo ./pinger IPaddressOfWebsite
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/in_systm.h>
#include <arpa/inet.h>
#include <signal.h>
#include <strings.h>
#include <string.h>
#include <errno.h>

#define BUFSIZE 1024

void send_v4(void);
void  proc_v4(char *ptr, ssize_t len, struct timeval *tvrecv, char *host);
void sig_alrm(int signo);
void readloop(char *host);
void tv_sub(struct timeval *, struct timeval *);
uint16_t in_cksum(uint16_t * addr, int len);

int	datalen = 56;		/* data that goes with ICMP echo request */
pid_t pid;
int nsent=0;
int sockfd;
char *host;
struct sockaddr_in sasend;
char payload[50];

int main(int argc, char **argv)
{
	char *host = argv[1];
	pid = getpid();
	strncpy(payload, "hello", 5);
	payload[5] = '\0';
	signal(SIGALRM, sig_alrm);
	printf("PING %s : %d data bytes\n", argv[1], datalen);

	//struct sockaddr_in sasend;
	bzero(&sasend, sizeof(sasend));
	sasend.sin_family = AF_INET;
	sasend.sin_addr.s_addr = inet_addr(argv[1]);

	readloop(argv[1]);
	exit(0);
}
void sig_alrm(int signo)
{
	send_v4();
	alarm(1);
	return;
}
void readloop(char *host)
{
	//…
	int size;
	struct sockaddr_in sarecv;
	char recvbuf[BUFSIZE];
	struct timeval tval;
	memset(&recvbuf, 0, sizeof(recvbuf));
	memset(&sarecv, 0, sizeof(sarecv));
	sockfd = socket(AF_INET,  SOCK_RAW,  IPPROTO_ICMP);

	setuid(getuid());	/* don't need special permissions any more */
	sig_alrm(SIGALRM);		/* send first packet */
	for ( ; ; ) {
		size = sizeof(sarecv);
		int n = recvfrom(sockfd, recvbuf, sizeof(recvbuf), 0,(struct sockaddr*) &sarecv, &size);
		if (n < 0) {
			if (errno == EINTR) continue;
			else printf("recvfrom error\n");
		}
		gettimeofday(&tval, NULL);
		proc_v4(recvbuf, n, &tval, host);
	}
}
void send_v4(void)
{
	//...
	int len;
	char sendbuf[1024];
	struct icmp *icmp;

	icmp = (struct icmp *) sendbuf;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = pid;
	icmp->icmp_seq = nsent++;
	gettimeofday((struct timeval *) icmp->icmp_data, NULL);
	sprintf(icmp->icmp_data+datalen-strlen(payload)-1, "%s", payload); //adding payload
	len = 8 + datalen;		/* checksum ICMP header and data */
	icmp->icmp_cksum = 0;
	icmp->icmp_cksum = in_cksum((u_short *) icmp, len);
	sendto(sockfd, sendbuf, len, 0, (struct sockaddr *) &sasend, sizeof(sasend));
}
void  proc_v4(char *ptr, ssize_t len, struct timeval *tvrecv, char *host)
{
	//…
	struct ip *ip;
	struct icmp *icmp;
	int hlen1, icmplen;
	struct timeval *tvsend;
	double rtt;

	ip = (struct ip *) ptr;		/* start of IP header */
	hlen1 = ip->ip_hl << 2;		/* length of IP header */
	icmp = (struct icmp *) (ptr + hlen1);	/* start of ICMP header */
	if ( (icmplen = len - hlen1) < 8)
		printf("icmplen (%d) < 8", icmplen);
	if (icmp->icmp_type == ICMP_ECHOREPLY) {
		if (icmp->icmp_id != pid)
			return;		/* not a response to our ECHO_REQUEST */
		if (icmplen < 16)
			printf("icmplen (%d) < 16", icmplen);
		tvsend = (struct timeval *) icmp->icmp_data;
		tv_sub(tvrecv, tvsend);
		rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;
		printf("%d bytes from %s: seq=%u, ttl=%d, rtt=%.3f ms, data=%s\n",
				icmplen, host,icmp->icmp_seq, ip->ip_ttl, rtt, icmp->icmp_data+datalen-strlen(payload)-1);
	}
}

void tv_sub(struct timeval *out, struct timeval *in)
{
	if((out->tv_usec -= in->tv_usec) < 0) {
		--out->tv_usec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

uint16_t in_cksum(uint16_t * addr, int len) {
	int nleft = len;
	uint32_t sum = 0;
	uint16_t *w = addr;
	uint16_t answer = 0;

	while(nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	/*mop up an odd byte if needed*/
	if(nleft == 1) {
		*(unsigned char *) (&answer) = * (unsigned char *) w;
		sum += answer;
	}

	/*add back carry outs from top 16*/
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}
