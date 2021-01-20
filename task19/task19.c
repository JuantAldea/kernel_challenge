#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/udp.h>
#include <linux/ip.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Juan Antonio Aldea Armenteros");
MODULE_DESCRIPTION("Task 19 - Netfilter.");

static const char EUDYP_ID[] = "0123456789AB";
static const size_t EUDYP_ID_LEN = 12;
static struct ts_config *ts_conf;

static unsigned int nf_id_logger(void *priv,
	struct sk_buff *skb,
	const struct nf_hook_state *state)
{
	unsigned int ret = skb_find_text(skb, 0, skb->len, ts_conf);

	if (ret != UINT_MAX)
		pr_alert("%s found at %d", EUDYP_ID, ret);

	return NF_ACCEPT;
}

static struct nf_hook_ops net_filter_hooks = {
	.hook = nf_id_logger,
	.hooknum = NF_INET_LOCAL_IN,
	.pf = NFPROTO_IPV4,
	.priority = NF_IP_PRI_FIRST,
};

static int __init init_m(void)
{
	ts_conf = textsearch_prepare("kmp", EUDYP_ID, EUDYP_ID_LEN, GFP_KERNEL,
		TS_AUTOLOAD);

	int ret = 0;

	if (IS_ERR(ts_conf)) {
		ret = PTR_ERR(ts_conf);
		goto error;
	}

	struct net *n;

	for_each_net(n)
		ret += nf_register_net_hook(n, &net_filter_hooks);

	pr_alert("MODULE LOADED.\n");
	return ret;

error:
	pr_alert("ERRROR WHILE LOADING MODULE.\n");
	return ret;
}

static void __exit exit_m(void)
{
	struct net *n;

	for_each_net(n)
		nf_unregister_net_hook(n, &net_filter_hooks);

	textsearch_destroy(ts_conf);
	pr_alert("MODULE UNLOADED.\n");
}

module_init(init_m);
module_exit(exit_m);
