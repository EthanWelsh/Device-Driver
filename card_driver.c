#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>
#include <asm/uaccess.h>
static char cardsDealt[52];
static char index;


void start_array(void){
	int i = 0;
	for(i = 0; i < 52; i++){
		cardsDealt[i] = i+1;
	}
}
char get_random_byte(int max) {
    char c;
    get_random_bytes(&c, 1);
    c = (c < 0)? 0-c:c;
    return c%max;
}
void shuffleDeck(int times){
while(times > 0){
    char rand_index;
    int i;
    rand_index = get_random_byte(52);
    for(i = 0; i < 52; i++){
        int temp;
        char holder;
        temp = (i+rand_index)%52;
        holder = cardsDealt[i];
        cardsDealt[i] = cardsDealt[temp];
        cardsDealt[temp] = holder;
    }
    times--;
    }
}
 char send_random_card(void){
    if(index >=51){
        shuffleDeck(20);
        index = 0;
    }
	return cardsDealt[index++];
}
static ssize_t card_read(struct file * file, char * buf, size_t count, loff_t *ppos)
{
	char temp;
	int i;

    for(i=0; i < count; i++){
        temp = send_random_card();
        copy_to_user(buf, &temp, 1);

		buf++;
		*ppos = *ppos + 1;
    }
	return count;
}



static const struct file_operations card_fops = {
	.owner		= THIS_MODULE,
	.read		= card_read,
};

static struct miscdevice card_dev = {
	MISC_DYNAMIC_MINOR,
	"cards",
	&card_fops
};

static int __init card_init(void)
{
	int ret;
	index = 0;
    start_array();
    shuffleDeck(40);
	ret = misc_register(&card_dev);
	if (ret)
		printk(KERN_ERR "Unable to register \"Card_Driver\" misc device\n");

	return ret;
}

module_init(card_init);

static void __exit
card_exit(void)
{
	misc_deregister(&card_dev);
}

module_exit(card_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("dev");
