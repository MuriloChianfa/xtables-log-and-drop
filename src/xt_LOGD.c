/*
 * This is a module which is used for logging packets and drop after this.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/skbuff.h>
#include <linux/if_arp.h>
#include <linux/ip.h>
#include <net/ipv6.h>
#include <net/icmp.h>
#include <net/udp.h>
#include <net/tcp.h>
#include <net/route.h>

#include <linux/netfilter.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv6/ip6_tables.h>
#include <net/netfilter/nf_log.h>
#include "xt_LOGD.h"

MODULE_ALIAS("ipt_LOGD");
MODULE_ALIAS("ip6t_LOGD");

static unsigned int
logd_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_logd_info *loginfo = par->targinfo;
	struct net *net = xt_net(par);
	struct nf_loginfo li;

	li.type = NF_LOG_TYPE_LOG;
	li.u.log.level = loginfo->level;
	li.u.log.logflags = loginfo->logflags;

	nf_log_packet(net, xt_family(par), xt_hooknum(par), skb, xt_in(par),
		      xt_out(par), &li, "%s", loginfo->prefix);
	return NF_DROP;
}

static int logd_tg_check(const struct xt_tgchk_param *par)
{
	const struct xt_logd_info *loginfo = par->targinfo;
	int ret;

	if (par->family != NFPROTO_IPV4 && par->family != NFPROTO_IPV6)
		return -EINVAL;

	if (loginfo->level >= 8) {
		pr_debug("level %u >= 8\n", loginfo->level);
		return -EINVAL;
	}

	if (loginfo->prefix[sizeof(loginfo->prefix)-1] != '\0') {
		pr_debug("prefix is not null-terminated\n");
		return -EINVAL;
	}

	ret = nf_logger_find_get(par->family, NF_LOG_TYPE_LOG);
	if (ret != 0 && !par->nft_compat) {
		request_module("%s", "nf_log_syslog");

		ret = nf_logger_find_get(par->family, NF_LOG_TYPE_LOG);
	}

	return ret;
}

static void logd_tg_destroy(const struct xt_tgdtor_param *par)
{
	nf_logger_put(par->family, NF_LOG_TYPE_LOG);
}

static struct xt_target logd_tg_regs[] __read_mostly = {
	{
		.name		= "LOGD",
		.family		= NFPROTO_IPV4,
		.target		= logd_tg,
		.targetsize	= sizeof(struct xt_logd_info),
		.checkentry	= logd_tg_check,
		.destroy	= logd_tg_destroy,
		.me		= THIS_MODULE,
	},
#if IS_ENABLED(CONFIG_IP6_NF_IPTABLES)
	{
		.name		= "LOGD",
		.family		= NFPROTO_IPV6,
		.target		= logd_tg,
		.targetsize	= sizeof(struct xt_logd_info),
		.checkentry	= logd_tg_check,
		.destroy	= logd_tg_destroy,
		.me		= THIS_MODULE,
	},
#endif
};

static int __init logd_tg_init(void)
{
	return xt_register_targets(logd_tg_regs, ARRAY_SIZE(logd_tg_regs));
}

static void __exit logd_tg_exit(void)
{
	xt_unregister_targets(logd_tg_regs, ARRAY_SIZE(logd_tg_regs));
}

module_init(logd_tg_init);
module_exit(logd_tg_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MuriloChianfa <murilo.chianfa@outlook.com>");
MODULE_DESCRIPTION("Xtables: IPv4/IPv6 packet logging and drop");
MODULE_SOFTDEP("pre: nf_log_syslog");