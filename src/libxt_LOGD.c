#include <stdio.h>
#include <string.h>
#include <xtables.h>
#include <linux/version.h>
#include <linux/netfilter/nf_conntrack_common.h>
#include <linux/netfilter/xt_LOG.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

static void logd_help(void)
{
	printf(
"LOGD target options:\n"
" none\n"
	);
}


static void logd_save(const void *ip, const struct xt_entry_target *target)
{
}

static struct xtables_target ct_target_reg[] = {
	{
		.family        = NFPROTO_UNSPEC,
		.name          = "LOGD",
		.revision      = 0,
		.version       = XTABLES_VERSION,
		.size          = XT_ALIGN(sizeof(struct xt_log_info)),
     	.save		   = logd_save,
		.userspacesize = 0,
		.help          = logd_help
	},
};

void _init(void)
{
	xtables_register_targets(ct_target_reg, ARRAY_SIZE(ct_target_reg));
}