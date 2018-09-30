/*
 * link_list.c
 *
 * Jun 30, 2012 file created by Mark Lu.
 *
 * Sample code for kernel link list usage.
 */

#include <linux/init.h>
#include <linux/module.h>	/* For kernel module	*/
#include <linux/kernel.h>
#include <linux/list.h>		/* For kernel Link list	*/
#include <linux/slab.h>		/* For kmalloc 		*/

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello");
MODULE_AUTHOR("Mark Lu");

/*
 * Personal structure
 */
struct Person {
	char			name[30];	/* Person name			*/
	unsigned int		weight;		/* Person weight		*/
	unsigned char		gender;		/* Person gender		*/
	struct list_head	list;		/* Kernel's list structure	*/
};

struct Person personList;

/*
 * Initialized module.
 */
static int linklist_init(void)
{
	struct Person *aNewPerson, *aPerson;
	int i;

	printk(KERN_INFO "initialized kernel module\n");

	/* Create a list head -> could also INIT_HEAD(personList); */
	INIT_LIST_HEAD(&personList.list);

	/* Adding element to mylist */
	for (i = 0; i < 5; i++)
	{
		aNewPerson = kmalloc(sizeof(struct Person), GFP_KERNEL);
		strcpy(aNewPerson->name, "tlclu1209");
		aNewPerson->weight = 130 + i;
		aNewPerson->gender = 1;
		INIT_LIST_HEAD(&aNewPerson->list);

		/* Add the new node in mylist */
		list_add_tail(&(aNewPerson->list), &(personList.list));
	}

	printk(KERN_INFO "traversing the list using list_for_each_entry()\n");
	list_for_each_entry(aPerson, &personList.list, list)
	{
		printk(KERN_INFO "Person: %s, weight: %d, gender: %s\n", aPerson->name, aPerson->weight, aPerson->gender == 0 ? "Female" : "Male");
	}
	printk(KERN_INFO "\n");

	return 0;

} /* end init_module */

/*
 * Release module.
 */
static void linklist_clean(void)
{
	struct Person *aPerson, *tmp;

	printk(KERN_INFO "kernel clean module.\n");
	printk(KERN_INFO "deleting the list using list_for_each_entry_safe().\n");
	
	list_for_each_entry_safe(aPerson, tmp, &personList.list, list)
	{
		printk(KERN_INFO "freeing node %s\n", aPerson->name);
		list_del(&aPerson->list);
		kfree(aPerson);
	}

} /* end clean_module */

module_init(linklist_init);
module_exit(linklist_clean);
