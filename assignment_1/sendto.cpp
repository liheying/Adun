#include <libnet.h>
#include <map>
#include <stdlib.h>
#include <time.h>

static std::map<const char *, const char *> dest_mac_map;
static const char *src_ip_str[] = {"116.147.65.191", "153.35.112.229", "223.117.113.4", "218.75.200.253", "115.202.250.21"};
static u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd};

int libnet_do_send_udp(libnet_t *handle, )
{
}

int main(int argc, char **argv)
{
	dest_mac_map["192.168.2.170"] = "\x00\x0c\x29\x6d\x4d\x5c";

	const char *device = "ens33";
	const char *dst_ip_str = "192.168.2.170";
	int des_port = 30000;
	int src_port = 30330;
	int opt;
	const char *optstring = "d:i:p:";
	while ((opt = getopt(argc, argv, optstring)) != -1)
	{
		switch (opt)
		{
		case 'd':
			device = optarg;
			break;
		case 'i':
			dst_ip_str = optarg;
			break;
		case 'p':
			des_port = atoi(optarg);
			break;
		default:
			break;
		}
	}

	srand((unsigned)time(NULL));

	libnet_t *handle = NULL;
	u_char dst_mac[6] = {0x08, 0x00, 0x27, 0x71, 0x44, 0x19};

	char ebuff[LIBNET_ERRBUF_SIZE];					/* 出错信息 */
	libnet_ptag_t eth_tag, ip_tag, udp_tag; /* 各层build函数返回值 */

	u_char payload[1400] = {0};
	u_long payload_s = 0;

	if ((handle = libnet_init(LIBNET_LINK, device, ebuff)) == NULL)
	{
		printf("libnet_init failure : %s\n", ebuff);
		return (-1);
	};

	int src_idx = rand() % (sizeof(src_ip_str) / sizeof(src_ip_str[0]));
	u_long dst_ip = libnet_name2addr4(handle, (char *)dst_ip_str, LIBNET_RESOLVE);
	u_long src_ip = libnet_name2addr4(handle, (char *)src_ip_str[src_idx], LIBNET_RESOLVE);
	printf("udp: %s:%d -> %s:%d\n", src_ip_str[src_idx], src_port, dst_ip_str, des_port);

	payload_s = sizeof(payload);

	udp_tag = libnet_build_udp(
			src_port,
			des_port,
			LIBNET_UDP_H + payload_s, /* 长度 */
			0,
			payload,
			payload_s,
			handle,
			0);
	if (udp_tag == -1)
	{
		printf("libnet_build_tcp failure\n");
		return (-3);
	};

	ip_tag = libnet_build_ipv4(
			LIBNET_IPV4_H + LIBNET_UDP_H + payload_s,
			0,																			 /* tos */
			(u_short)libnet_get_prand(LIBNET_PRu16), /* id,随机产生0~65535 */
			0,																			 /* frag 片偏移 */
			(u_int8_t)libnet_get_prand(LIBNET_PR8),
			IPPROTO_UDP,
			0, /* 校验和，此时为0，表示由Libnet自动计算 */
			src_ip,
			dst_ip,
			NULL,
			0,
			handle,
			0);
	if (ip_tag == -1)
	{
		printf("libnet_build_ipv4 failure\n");
		return (-4);
	};

	/* 构造一个以太网协议块,只能用于LIBNET_LINK */
	eth_tag = libnet_build_ethernet(
			dst_mac,			/* 以太网目的地址 */
			src_mac,			/* 以太网源地址 */
			ETHERTYPE_IP, /* 以太网上层协议类型，此时为IP类型 */
			NULL,					/* 负载，这里为空 */
			0,						/* 负载大小 */
			handle,				/* Libnet句柄 */
			0							/* 协议块标记，0表示构造一个新的 */
	);

	if (eth_tag == -1)
	{
		printf("libnet_build_ethernet failure\n");
		return (-5);
	};

	libnet_write(handle);
	libnet_destroy(handle);

	return (0);
}
