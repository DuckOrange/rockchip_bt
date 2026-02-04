/*
 *
 * FD650 Driver
 *
 * Copyright (C) 2015 Fdhisi, Inc.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/errno.h>
#include <linux/string.h>

#include <linux/ioctl.h>
#include <linux/device.h>

#include <linux/errno.h>
#include <linux/mutex.h>


#include <linux/miscdevice.h>
#include <linux/fs.h>

#include <linux/fcntl.h>
#include <linux/poll.h>

#include <linux/sched.h>




#include "fd650.h"

static struct fd650_dev *pdata = NULL;



static void FD650_start( struct fd650_dev *dev )
{
//	FD650_SDA_SET;  
	gpio_direction_output(dev->dat_pin, 1);
//	FD650_SDA_D_OUT;
//	FD650_SCL_SET;
    gpio_direction_output(dev->clk_pin, 1);
//	FD650_SCL_D_OUT;
	DELAY;
//	FD650_SDA_CLR;
	gpio_direction_output(dev->dat_pin, 0);
	DELAY;      
//	FD650_SCL_CLR;
	gpio_direction_output(dev->clk_pin, 0);
}

/** 
 * @brief   ֹͣFD650
 * @param   ��
 * @return  ��
 * @note   ��SCL�ߵ�ƽ�ڼ䲶��SDA�������أ�ʹFD650ֹͣ����     
 */ 
static void FD650_stop( struct fd650_dev *dev )
{
//	FD650_SDA_CLR;
	gpio_direction_output(dev->dat_pin, 0);
//	FD650_SDA_D_OUT;
	DELAY;
//	FD650_SCL_SET;
	gpio_direction_output(dev->clk_pin, 1);
	DELAY;
//	FD650_SDA_SET;
	gpio_direction_output(dev->dat_pin, 1);
	DELAY;
}

/** 
 * @brief   ����һ���ֽڣ�8λ�����ݸ�FD650
 * @param   dat �޷���8λ����
 * @return  ��
 * @note   ��SCL������д��FD650 ��������9���ֽڶ���Ӧ���ź�	    
 */ 
static void FD650_writebyte( u_int8 dat, struct fd650_dev *dev)
{
	u_int8 i;
//	FD650_SDA_D_OUT;
	for( i = 0; i < 8; i++ )
	{
		if( dat & 0x80 ) 
		{
	//	    FD650_SDA_SET;
		  gpio_direction_output(dev->dat_pin, 1);
		}
		else 
		{
	//	    FD650_SDA_CLR;
			gpio_direction_output(dev->dat_pin, 0);
		}
		DELAY;
	//	FD650_SCL_SET;
		gpio_direction_output(dev->clk_pin, 1);
		dat <<= 1;
		DELAY;  // ��ѡ��ʱ
	//	FD650_SCL_CLR;
		gpio_direction_output(dev->clk_pin, 0);
	}
//	FD650_SDA_SET;
	gpio_direction_output(dev->dat_pin, 1);
//	FD650_SDA_D_IN;
	//gpio_direction_input(dev->dat_pin);
	DELAY;
//	FD650_SCL_SET;
	gpio_direction_output(dev->clk_pin, 1);
	DELAY;
//	FD650_SCL_CLR;
	gpio_direction_output(dev->clk_pin, 0);
}


#if 0
/** 
 * @brief   ��ȡFD650�İ�����ֵ
 * @param   ��
 * @return  �����޷���8λ����
 * @note   ��SCL�½��ض�ȡFD650 ��������9���ֽڷ�����ЧӦ��    
 */ 
static u_int8  FD650_readbyte(struct fd650_dev *dev )
{
	u_int8 dat,i;
//	FD650_SDA_SET;
	gpio_direction_output(dev->dat_pin, 1);
//	FD650_SDA_D_IN;
	//gpio_direction_input(dev->dat_pin);
	dat = 0;
	for( i = 0; i != 8; i++ )
	{
		DELAY;  // ��ѡ��ʱ
	//	FD650_SCL_SET;
		gpio_direction_output(dev->clk_pin, 1);
		DELAY;  // ��ѡ��ʱ
		dat <<= 1;
	//	if( FD650_SDA_IN ) dat++;
		if( gpio_get_value(dev->dat_pin) ) dat++;
	//	FD650_SCL_CLR;
		gpio_direction_output(dev->clk_pin, 0);
		
	}
	//FD650_SDA_SET;
	gpio_direction_output(dev->dat_pin, 1);
	DELAY;
	//FD650_SCL_SET;
	gpio_direction_output(dev->clk_pin, 1);
	DELAY;
	//FD650_SCL_CLR;
	gpio_direction_output(dev->clk_pin, 0);
	return dat;
}
#endif

void FD650_command(u_int8 cmd, struct fd650_dev *dev) 
{
	FD650_start(dev);
	FD650_writebyte(SET, dev);
	FD650_writebyte(cmd, dev);
	FD650_stop(dev);
}


void FD650_disp(u_int8 address ,u_int8 dat, struct fd650_dev *dev)
{
	FD650_start(dev);
	FD650_writebyte(address,dev);
	FD650_writebyte(dat,dev);
	FD650_stop(dev);
}

/** 
 * @brief   ת���ַ�Ϊ����ܵ���ʾ��
 * @param   cTemp ��ת��Ϊ��ʾ����ַ�
 * @return  ��ʾ��ֵ,8λ�޷���
 * @note    ��ֵ��BCD_decode_tab[LEDMAPNUM]���������޷�ת�����ַ�����0  
 */ 
u_int8 Led_Get_Code(char cTemp)
{
	u_int8 i, bitmap=0x00;

	for(i=0; i<LEDMAPNUM; i++)
	{
		if(BCD_decode_tab[i].character == cTemp)
		{
			bitmap = BCD_decode_tab[i].bitmap;
			break;
		}
	}

	return bitmap;
}

/** 
 * @brief   �򵥵��������ʾӦ��ʾ��
 * @param   acFPStr  ��ʾ���ַ���
 * @param   sec_flag	  ����С�����־λ��
 * @param   Lock		  ����С�����־λ��	
 * @return  ��
 * @note    ���ַ�����ʽд��,��ת��Ϊ�������ʾ������,��ʾ��Ϊ4λ�����
   @code 
	   Led_Show_650��"ABCD",1,1��;
   @endcode
 
static void Led_Show(  u_int8  *acFPStr, struct fd650_dev *dev)
{
	u_int8 i, iLenth;
	u_int8	data[4]={0};	
	if( strcmp(acFPStr, "") == 0 )
	{
		return;
	}
	iLenth = strlen(acFPStr);
	if(iLenth>4)
		iLenth = 4;
	
	for(i=0; i<iLenth; i++)
	{
		data[i] = Led_Get_Code(acFPStr[i]);
	}
	//Send display data
	
	FD650_disp(DIG1,data[0],dev);
	FD650_disp(DIG2,data[1],dev);
	FD650_disp(DIG3,data[2],dev);
	FD650_disp(DIG4,data[3],dev);
}
 */


static int fd650_dev_open(struct inode *inode, struct file *file)
{
   
    file->private_data = pdata;
	FD650_command(FD650_SYSON,pdata);

	pr_dbg("fd650_dev_open now.............................\r\n");
    return 0;
}

static int fd650_dev_release(struct inode *inode, struct file *file)
{
	file->private_data = NULL;
	//modify for display longer

	FD650_command(~FD650_SYSON,pdata);
	
	 FD650_disp(DIG1,0x00,pdata);  //-
	 FD650_disp(DIG2,0x00,pdata);  //-
	 FD650_disp(DIG3,0x00,pdata);  //-
	 FD650_disp(DIG4,0x00,pdata);  //-

	pr_error("succes to close  fd650_dev in release.............\n");

    return 0;
}

static ssize_t  fd650_dev_write(struct file *filp, const char __user *buf,		size_t count, loff_t *f_pos)
{

	struct fd650_dev *dev;
	unsigned long	missing;
	size_t			status = 0, i = 0;
	char data[5] = {0};
	char tmp[4]= {0};
	dev = filp->private_data;
	if (count > 5)		
		count = 5;
	missing = copy_from_user(data, buf, count);
	pr_dbg("write buf: %s \r\n",data);
	if (missing == 0)
	{	
		for(i=0; i<count; i++)
		{
			tmp[i] = Led_Get_Code(data[i]);
			pr_dbg("Led_Get_Code buf: 0x%x \r\n",tmp[i]);
		}

		if (data[4] == 1)
			tmp[1] |= 0x80;

		FD650_disp(DIG1,tmp[0],dev);
		FD650_disp(DIG2,tmp[1],dev);
		FD650_disp(DIG3,tmp[2],dev);
		FD650_disp(DIG4,tmp[3],dev);
			
		status = count;
	} 
	pr_dbg("fd650_dev_write count : %zu\n",count );
	return status;
}


static struct file_operations fd650_fops = {
	.owner		=	THIS_MODULE,
	.open		=	fd650_dev_open,
	.release	=	fd650_dev_release,
	.write      =   fd650_dev_write,

	
};
static struct miscdevice fd650_device = {
	.minor	=	MISC_DYNAMIC_MINOR,
	.name	=	DEV_NAME,
	.fops	=	&fd650_fops,
};

static int register_fd650_driver(void)
{
    int ret = 0;
    ret = misc_register(&fd650_device);
	if(ret)
		pr_dbg("%s: failed to add fd650 module\n", __func__);
	else
		pr_dbg("%s: Successed to add fd650  module \n", __func__);
    return ret;
}

static void deregister_fd650_driver(void) 
{
	misc_deregister(&fd650_device);
}

/*
static ssize_t fd650_panel_show(struct class *dev,
                 struct class_attribute *attr, char *buf){
	return sprintf(buf, "%s\n", pdata->wbuf);
}

static ssize_t fd650_panel_store(struct class *dev,
                 struct class_attribute *attr,
                 const char *buf, size_t count){

        FD650_command(FD650_SYSON,pdata);
	sscanf(buf, "%s", pdata->wbuf);
	Led_Show(pdata->wbuf,pdata);
	return count;
}



static struct class_attribute fd650_class_attrs[] = {
        __ATTR(panel, S_IRUGO|S_IWUSR|S_IWGRP, fd650_panel_show, fd650_panel_store),
        __ATTR_NULL
};
*/
/* static int fb_notifier_callback(struct notifier_block *p,
				unsigned long action, void *data)
{
	struct fb_event *event = data;
	int blank_mode = *((int *)event->data);
	if (action == FB_EARLY_EVENT_BLANK) {
		switch (blank_mode) {
			case FB_BLANK_UNBLANK:
				FD650_command(FD650_SYSON,pdata);
				pr_error("FB_EARLY_EVENT_BLANK->FB_BLANK_UNBLANK Command to open fd650 from earlysuspend as resume");
        break;
      default:	
				FD650_command(~FD650_SYSON,pdata);
				pr_error("Command to close fd650 in earlysuspend.......\r\n");
            break;
      } 
  } else if (action == FB_EVENT_BLANK) {
		switch (blank_mode) {
			case FB_BLANK_UNBLANK:
				FD650_command(FD650_SYSON,pdata);
				pr_error("FB_EVENT_BLANK->FB_BLANK_UNBLANK Command to open fd650 from earlysuspend as resume");
				break;
      default:
				FD650_command(~FD650_SYSON,pdata);
				pr_error("Command to close fd650 in earlysuspend.......\r\n");
				break;
      }
  }
  return NOTIFY_OK;
}
*/

static int fd650_driver_remove(struct platform_device *pdev)
{   
    deregister_fd650_driver();
	class_unregister(&pdata->sysfs);
    // fb_unregister_client(&pdata->notifier);
#ifdef CONFIG_OF
	gpio_free(pdata->clk_pin);
   	gpio_free(pdata->dat_pin);
    pr_error("======= gpio free in remove========");	
	kfree(pdata);
#endif
    return 0;
}

static int fd650_driver_suspend(struct platform_device *dev, pm_message_t state)
{
    /* disable gpio_free to fix kernel warning 
    gpio_free(pdata->clk_pin);
   	gpio_free(pdata->dat_pin);*/

	FD650_command(~FD650_SYSON,pdata);
  pr_dbg("==========gpio free in suspend might cause kernel warning, so disable it !!!");
	return 0;
}

static int fd650_driver_resume(struct platform_device *dev)
{
	/*int ret = -1;
    pr_error("fd650_driver_resume in resume");
	ret = gpio_request(pdata->clk_pin, DEV_NAME);
	if(ret){
		pr_error("---%s----can not request pin %d\n",__func__,pdata->clk_pin);
	}	
	ret = gpio_request(pdata->dat_pin, DEV_NAME);
	if(ret){
		pr_error("---%s----can not request pin %d\n",__func__,pdata->dat_pin);
	}	*/

	FD650_command(FD650_SYSON,pdata);
  pr_dbg("fd650_driver_resume in resume");
  return 0;
}



static int fd650_driver_probe(struct platform_device *pdev)
{
	int state=-EINVAL;

	char buf[32];
	int ret;
	const char *str;

	unsigned int desc;

	pr_dbg("lsd==%s==\n", __func__);

    if (!pdev->dev.of_node) {
				pr_error("fd650_driver: pdev->dev.of_node == NULL!\n");
				state = -EINVAL;
				goto get_fd650_node_fail;
	}

	pdata = kzalloc(sizeof(*pdata), GFP_KERNEL);
    if (!pdata) {
        pr_error("platform data is required!\n");
        state = -EINVAL;
        goto get_fd650_mem_fail;
    }
	snprintf(buf, sizeof(buf), "clk_pin");
	ret = of_property_read_string(pdev->dev.of_node, buf, &str);
	if(!ret){
		desc = of_get_named_gpio_flags(pdev->dev.of_node, buf, 0, NULL);		
		pdata->clk_pin = desc;
		pr_dbg("%s: %s\n", buf, str);
	}else{
		pr_error("cannot find resource \"%s\"\n", buf);
	}
	snprintf(buf, sizeof(buf), "dat_pin");
	ret = of_property_read_string(pdev->dev.of_node, buf, &str);
	if(!ret){
		desc = of_get_named_gpio_flags(pdev->dev.of_node, buf, 0, NULL);		
		pdata->dat_pin = desc;
		pr_dbg("%s: %s\n", buf, str);
	}else{
		pr_error("cannot find resource \"%s\"\n", buf);
	}

    ret = gpio_request(pdata->clk_pin, DEV_NAME);
	if(ret){
		pr_error("lsd---%s----can not request clk_pin %d\n",__func__,pdata->clk_pin);
		goto get_fd650_mem_fail ;
	}	
	ret = gpio_request(pdata->dat_pin, DEV_NAME);
	if(ret){
		pr_error("lsd2---%s----can not request dat_pin %d\n",__func__,pdata->dat_pin);
		goto get_fd650_mem_fail ;
	}	
    //DTS 2018-04-12 add EARLYSUSPEND by terry
    // pdata->notifier.notifier_call = fb_notifier_callback;

	// ret = fb_register_client(&pdata->notifier);
	// if (ret)
	// {
//	pr_error(&pdev->dev, "unable to register qled trigger\n");
		// goto get_fd650_mem_fail ;
	// }
    //DTS
	pdata->sysfs.name = "fd650";
//	pdata->sysfs.class_attrs = fd650_class_attrs;
	pdata->vpause_flag = 0;
        pdata->vplay_flag = 0;
	ret= class_register(&pdata->sysfs);
	if(ret < 0)
		goto error_sysfs; 
		
	platform_set_drvdata(pdev, pdata);
	
    register_fd650_driver();
	
    FD650_command(FD650_SYSON,pdata);
    pr_dbg( "====================lsd Command opne in probe=============\n");    
	/*
    FD650_disp(DIG1,0x38,pdata);  //L
	FD650_disp(DIG2,0x3f,pdata);  //o
	FD650_disp(DIG3,0x77,pdata);  //a
	FD650_disp(DIG4,0x5e,pdata);  //d
	*/
	FD650_disp(DIG1,0x7F,pdata);  //8
	FD650_disp(DIG2,0x7F,pdata);  //8
	FD650_disp(DIG3,0x7F,pdata);  //8
	FD650_disp(DIG4,0x7F,pdata);  //8
    pr_dbg( "====================lsd Display boot in probe=============\n"); 
    //DTS
	
    
	
    return 0;


    get_fd650_mem_fail:
			kfree(pdata);
    get_fd650_node_fail:
    error_sysfs:	
    return state;
}


#ifdef CONFIG_OF
static const struct of_device_id fd650_dt_match[]={
	{	.compatible = "oranth,fd650",
	},
	{},
};
#else
#define fd650_dt_match NULL
#endif

static struct platform_driver fd650_driver = {
    .probe      = fd650_driver_probe,
    .remove     = fd650_driver_remove,
    .suspend    = fd650_driver_suspend,
    .resume     = fd650_driver_resume,
    .driver     = {
        .name   = DEV_NAME,
				.owner	= THIS_MODULE,
        .of_match_table = fd650_dt_match,
    },
};

static int __init fd650_driver_init(void)
{
    pr_dbg( "Fd650 Driver init.\n");
    return platform_driver_register(&fd650_driver);
}

static void __exit fd650_driver_exit(void)
{
    pr_dbg("Fd650 Driver exit.\n");
    platform_driver_unregister(&fd650_driver);
}

module_init(fd650_driver_init);
module_exit(fd650_driver_exit);

MODULE_AUTHOR("fdhisi");
MODULE_DESCRIPTION("fd650 Driver");
MODULE_LICENSE("GPL");

