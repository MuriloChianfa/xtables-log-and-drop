/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _XT_LOGD_H
#define _XT_LOGD_H

/* make sure not to change this without changing nf_log.h:NF_LOG_* (!) */
#define XT_LOGD_TCPSEQ		0x01	/* Log TCP sequence numbers */
#define XT_LOGD_TCPOPT		0x02	/* Log TCP options */
#define XT_LOGD_IPOPT		0x04	/* Log IP options */
#define XT_LOGD_UID		0x08	/* Log UID owning local socket */
#define XT_LOGD_NFLOG		0x10	/* Unsupported, don't reuse */
#define XT_LOGD_MACDECODE	0x20	/* Decode MAC header */
#define XT_LOGD_MASK		0x2f

struct xt_logd_info {
	unsigned char level;
	unsigned char logflags;
	char prefix[30];
};

#endif /* _XT_LOGD_H */