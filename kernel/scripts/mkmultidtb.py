#!/usr/bin/env python
# SPDX-License-Identifier: (GPL-2.0+ OR MIT)
# Copyright (c) 2018 Fuzhou Rockchip Electronics Co., Ltd
#


"""
Multiple dtb package tool

Usage: scripts/mkmultidtb.py board
The board is what you defined in DTBS dictionary like DTBS['board'],
Such as: PX30-EVB, RK3308-EVB

"""
import os
import sys
import shutil
from collections import OrderedDict

DTBS = {}

DTBS['rk3568-kickpi-k1-android'] = OrderedDict([
				('rk3568-kickpi-k1-android-hdmi', '#_saradc_ch0=500'),
				('rk3568-kickpi-k1-android-mipi0-10.1-800-1280', '#_saradc_ch5=115'),
 				('rk3568-kickpi-k1-android-mipi1-10.1-800-1280', '#_saradc_ch6=115'),
                ('rk3568-kickpi-k1-android-mipi0-10.1-800-1280-v2', '#_saradc_ch5=191'),
 				('rk3568-kickpi-k1-android-mipi1-10.1-800-1280-v2', '#_saradc_ch6=191'),
                ('rk3568-kickpi-k1-android-mipi0-8-800-1280', '#_saradc_ch5=270'),
 				('rk3568-kickpi-k1-android-mipi1-8-800-1280', '#_saradc_ch6=270'),
                ('rk3568-kickpi-k1-android-mipi0-5-720-1280', '#_saradc_ch5=360'),
 				('rk3568-kickpi-k1-android-mipi1-5-720-1280', '#_saradc_ch6=360'),
 				('rk3568-kickpi-k1-android-edp-15.6-1920-1080', '#_saradc_ch7=15'),
				('rk3568-kickpi-k1-android-lvds0-7-1024-600', '#_saradc_ch5=15'),
				('rk3568-kickpi-k1-android-lvds1-7-1024-600', '#_saradc_ch6=15')])

DTBS['rk3568-kickpi-k1-linux'] = OrderedDict([
				('rk3568-kickpi-k1-linux-hdmi', '#_saradc_ch0=500'),
				('rk3568-kickpi-k1-linux-mipi0-10.1-800-1280', '#_saradc_ch5=115'),
 				('rk3568-kickpi-k1-linux-mipi1-10.1-800-1280', '#_saradc_ch6=115'),
                ('rk3568-kickpi-k1-linux-mipi0-10.1-800-1280-v2', '#_saradc_ch5=191'),
 				('rk3568-kickpi-k1-linux-mipi1-10.1-800-1280-v2', '#_saradc_ch6=191'),
                ('rk3568-kickpi-k1-linux-mipi0-8-800-1280', '#_saradc_ch5=270'),
 				('rk3568-kickpi-k1-linux-mipi1-8-800-1280', '#_saradc_ch6=270'),
                ('rk3568-kickpi-k1-linux-mipi0-5-720-1280', '#_saradc_ch5=360'),
 				('rk3568-kickpi-k1-linux-mipi1-5-720-1280', '#_saradc_ch6=360'),
 				('rk3568-kickpi-k1-linux-edp-15.6-1920-1080', '#_saradc_ch7=15'),
				('rk3568-kickpi-k1-linux-lvds0-7-1024-600', '#_saradc_ch5=15'),
				('rk3568-kickpi-k1-linux-lvds1-7-1024-600', '#_saradc_ch6=15')])

DTBS['rk3568-kickpi-k1b-android'] = OrderedDict([
				('rk3568-kickpi-k1b-android-hdmi', '#_saradc_ch0=500'),
				('rk3568-kickpi-k1b-android-mipi0-10.1-800-1280', '#_saradc_ch5=115'),
                ('rk3568-kickpi-k1b-android-mipi0-10.1-800-1280-v2', '#_saradc_ch5=191'),
                ('rk3568-kickpi-k1b-android-mipi0-8-800-1280', '#_saradc_ch5=270'),
                ('rk3568-kickpi-k1b-android-mipi0-5-720-1280', '#_saradc_ch5=360'),
				('rk3568-kickpi-k1b-android-lvds0-7-1024-600', '#_saradc_ch5=15')])

DTBS['rk3568-kickpi-k1b-linux'] = OrderedDict([
				('rk3568-kickpi-k1b-linux-hdmi', '#_saradc_ch0=500'),
				('rk3568-kickpi-k1b-linux-mipi0-10.1-800-1280', '#_saradc_ch5=115'),
                ('rk3568-kickpi-k1b-linux-mipi0-10.1-800-1280-v2', '#_saradc_ch5=191'),
                ('rk3568-kickpi-k1b-linux-mipi0-8-800-1280', '#_saradc_ch5=270'),
                ('rk3568-kickpi-k1b-linux-mipi0-5-720-1280', '#_saradc_ch5=360'),
				('rk3568-kickpi-k1b-linux-lvds0-7-1024-600', '#_saradc_ch5=15')])

DTBS['rk3562-kickpi-k3-android'] = OrderedDict([
				('rk3562-kickpi-k3-android-duallvds-6-1920-1080', '#_saradc_ch0=500'),
				('rk3562-kickpi-k3-android-mipi-10.1-800-1280', '#_saradc_ch3=115'),
                ('rk3562-kickpi-k3-android-mipi-10.1-800-1280-v2', '#_saradc_ch3=178'),
                ('rk3562-kickpi-k3-android-mipi-8-800-1280', '#_saradc_ch3=255'),
                ('rk3562-kickpi-k3-android-mipi-5-720-1280', '#_saradc_ch3=350'),
				('rk3562-kickpi-k3-android-lvds-7-1024-600', '#_saradc_ch3=15')])

DTBS['rk3562-kickpi-k3-linux'] = OrderedDict([
				('rk3562-kickpi-k3-linux-duallvds-6-1920-1080', '#_saradc_ch0=500'),
				('rk3562-kickpi-k3-linux-mipi-10.1-800-1280', '#_saradc_ch3=100'),
                ('rk3562-kickpi-k3-linux-mipi-10.1-800-1280-v2', '#_saradc_ch3=178'),
                ('rk3562-kickpi-k3-linux-mipi-8-800-1280', '#_saradc_ch3=255'),
                ('rk3562-kickpi-k3-linux-mipi-5-720-1280', '#_saradc_ch3=350'),
				('rk3562-kickpi-k3-linux-lvds-7-1024-600', '#_saradc_ch3=15')])

DTBS['rk3588-kickpi-k8-android'] = OrderedDict([
				('rk3588-kickpi-k8-android-hdmi', '#_saradc_ch0=500'),
                ('rk3588-kickpi-k8-android-AT101DS40I-AT101DS40I', '#_saradc_ch6=373#_saradc_ch7=373'),
                ('rk3588-kickpi-k8-android-AT101DS40I-MX101BA1340', '#_saradc_ch6=373#_saradc_ch7=690'),
                ('rk3588-kickpi-k8-android-AT101DS40I-MX080B2140', '#_saradc_ch6=373#_saradc_ch7=1006'),
                ('rk3588-kickpi-k8-android-AT101DS40I-F050008M01', '#_saradc_ch6=373#_saradc_ch7=1389'),
                ('rk3588-kickpi-k8-android-MX101BA1340-AT101DS40I', '#_saradc_ch6=690#_saradc_ch7=373'),
                ('rk3588-kickpi-k8-android-MX101BA1340-MX101BA1340', '#_saradc_ch6=690#_saradc_ch7=690'),
                ('rk3588-kickpi-k8-android-MX101BA1340-MX080B2140', '#_saradc_ch6=690#_saradc_ch7=1006'),
                ('rk3588-kickpi-k8-android-MX101BA1340-F050008M01', '#_saradc_ch6=690#_saradc_ch7=1389'),
                ('rk3588-kickpi-k8-android-MX080B2140-AT101DS40I', '#_saradc_ch6=1006#_saradc_ch7=373'),
                ('rk3588-kickpi-k8-android-MX080B2140-MX101BA1340', '#_saradc_ch6=1006#_saradc_ch7=690'),
                ('rk3588-kickpi-k8-android-MX080B2140-MX080B2140', '#_saradc_ch6=1006#_saradc_ch7=1006'),
                ('rk3588-kickpi-k8-android-MX080B2140-F050008M01', '#_saradc_ch6=1006#_saradc_ch7=1389'),
                ('rk3588-kickpi-k8-android-F050008M01-AT101DS40I', '#_saradc_ch6=1389#_saradc_ch7=373'),
                ('rk3588-kickpi-k8-android-F050008M01-MX101BA1340', '#_saradc_ch6=1389#_saradc_ch7=690'),
                ('rk3588-kickpi-k8-android-F050008M01-MX080B2140', '#_saradc_ch6=1389#_saradc_ch7=1006'),
                ('rk3588-kickpi-k8-android-F050008M01-F050008M01', '#_saradc_ch6=1389#_saradc_ch7=1389'),
                ('rk3588-kickpi-k8-android-mipi0-10.1-800-1280-AT101DS40I', '#_saradc_ch6=373'),
                ('rk3588-kickpi-k8-android-mipi0-10.1-800-1280-MX101BA1340', '#_saradc_ch6=690'),
                ('rk3588-kickpi-k8-android-mipi0-8-800-1280-MX080B2140', '#_saradc_ch6=1006'),
                ('rk3588-kickpi-k8-android-mipi0-5-720-1280-F050008M01', '#_saradc_ch6=1389'),
                ('rk3588-kickpi-k8-android-mipi1-10.1-800-1280-AT101DS40I', '#_saradc_ch7=373'),
                ('rk3588-kickpi-k8-android-mipi1-10.1-800-1280-MX101BA1340', '#_saradc_ch7=690'),
                ('rk3588-kickpi-k8-android-mipi1-8-800-1280-MX080B2140', '#_saradc_ch7=1006'),
                ('rk3588-kickpi-k8-android-mipi1-5-720-1280-F050008M01', '#_saradc_ch7=1389')])

DTBS['rk3588-kickpi-k8-linux'] = OrderedDict([
				('rk3588-kickpi-k8-linux-hdmi', '#_saradc_ch0=500'),
                ('rk3588-kickpi-k8-linux-AT101DS40I-AT101DS40I', '#_saradc_ch6=373#_saradc_ch7=373'),
                ('rk3588-kickpi-k8-linux-AT101DS40I-MX101BA1340', '#_saradc_ch6=373#_saradc_ch7=690'),
                ('rk3588-kickpi-k8-linux-AT101DS40I-MX080B2140', '#_saradc_ch6=373#_saradc_ch7=1006'),
                ('rk3588-kickpi-k8-linux-AT101DS40I-F050008M01', '#_saradc_ch6=373#_saradc_ch7=1389'),
                ('rk3588-kickpi-k8-linux-MX101BA1340-AT101DS40I', '#_saradc_ch6=690#_saradc_ch7=373'),
                ('rk3588-kickpi-k8-linux-MX101BA1340-MX101BA1340', '#_saradc_ch6=690#_saradc_ch7=690'),
                ('rk3588-kickpi-k8-linux-MX101BA1340-MX080B2140', '#_saradc_ch6=690#_saradc_ch7=1006'),
                ('rk3588-kickpi-k8-linux-MX101BA1340-F050008M01', '#_saradc_ch6=690#_saradc_ch7=1389'),
                ('rk3588-kickpi-k8-linux-MX080B2140-AT101DS40I', '#_saradc_ch6=1006#_saradc_ch7=373'),
                ('rk3588-kickpi-k8-linux-MX080B2140-MX101BA1340', '#_saradc_ch6=1006#_saradc_ch7=690'),
                ('rk3588-kickpi-k8-linux-MX080B2140-MX080B2140', '#_saradc_ch6=1006#_saradc_ch7=1006'),
                ('rk3588-kickpi-k8-linux-MX080B2140-F050008M01', '#_saradc_ch6=1006#_saradc_ch7=1389'),
                ('rk3588-kickpi-k8-linux-F050008M01-AT101DS40I', '#_saradc_ch6=1389#_saradc_ch7=373'),
                ('rk3588-kickpi-k8-linux-F050008M01-MX101BA1340', '#_saradc_ch6=1389#_saradc_ch7=690'),
                ('rk3588-kickpi-k8-linux-F050008M01-MX080B2140', '#_saradc_ch6=1389#_saradc_ch7=1006'),
                ('rk3588-kickpi-k8-linux-F050008M01-F050008M01', '#_saradc_ch6=1389#_saradc_ch7=1389'),
                ('rk3588-kickpi-k8-linux-mipi0-10.1-800-1280-AT101DS40I', '#_saradc_ch6=373'),
                ('rk3588-kickpi-k8-linux-mipi0-10.1-800-1280-MX101BA1340', '#_saradc_ch6=690'),
                ('rk3588-kickpi-k8-linux-mipi0-8-800-1280-MX080B2140', '#_saradc_ch6=1006'),
                ('rk3588-kickpi-k8-linux-mipi0-5-720-1280-F050008M01', '#_saradc_ch6=1389'),
                ('rk3588-kickpi-k8-linux-mipi1-10.1-800-1280-AT101DS40I', '#_saradc_ch7=373'),
                ('rk3588-kickpi-k8-linux-mipi1-10.1-800-1280-MX101BA1340', '#_saradc_ch7=690'),
                ('rk3588-kickpi-k8-linux-mipi1-8-800-1280-MX080B2140', '#_saradc_ch7=1006'),
                ('rk3588-kickpi-k8-linux-mipi1-5-720-1280-F050008M01', '#_saradc_ch7=1389')])

def main():
    if (len(sys.argv) < 2) or (sys.argv[1] == '-h'):
        print __doc__
        sys.exit(2)

    BOARD = sys.argv[1]
    TARGET_DTBS = DTBS[BOARD]
    target_dtb_list = ''
    default_dtb = True

    for dtb, value in TARGET_DTBS.items():
        if default_dtb:
            ori_file = 'arch/arm64/boot/dts/rockchip/' + dtb + '.dtb'
            shutil.copyfile(ori_file, "rk-kernel.dtb")
            target_dtb_list += 'rk-kernel.dtb '
            default_dtb = False
        new_file = dtb + value + '.dtb'
        ori_file = 'arch/arm64/boot/dts/rockchip/' + dtb + '.dtb'
        shutil.copyfile(ori_file, new_file)
        target_dtb_list += ' ' + new_file

    print target_dtb_list
    os.system('scripts/resource_tool logo.bmp logo_kernel.bmp ' + target_dtb_list)
    os.system('rm ' + target_dtb_list)

if __name__ == '__main__':
    main()
