# Bootloader Example4

发布版本：1.0

作者邮箱：703632667@qq.com

日期：2019.04

文件密级：公开资料

------

**前言**

**概述**

bootloader简单例子。

**读者对象**

本文档（本指南）主要适用于以下工程师：

技术支持工程师

软件开发工程师

**产品版本**

**修订记录**

| **日期**   | **版本** | **作者**  | **修改说明** |
| ---------- | -------- | --------- | ------------ |
| 2019-04-10 | V1.0     | Jason Zhu | 初始版本     |

------

[TOC]

------

## 1 介绍

本文介绍如何引导linux。

为了引导ARM Linux，需要在运行内核之前跑一段小代码，也就是BootLoader。BootLoader应该可以初始化设备，向内核传递信息并跳转到内核程序。

BootLoader至少提供的功能有：

- 初始化RAM
- 初始化串口
- 检测处理器类型
- 建立内核标签列表
- 调用内核镜像

### 1.1 初始化SDRAM

linux内核为大程序，需要运行在SDRAM内，故BootLoader需要初始化SDRAM并把linux镜像搬移到SDRAM内。

### 1.2 初始化串口

可选的，用于调试。

BootLoader初始化一个串口后，通过传递“console=”给内核，告诉内核可以使用哪个串口。参考文档《Documentation/kernel-parameters.txt》。

### 1.3 检测内核类型

传递MACH_TYPE_xxx值，参考《linux/arch/arm/tools/mach-types》

### 1.4 建立Boot数据

BootLoader必须提供一个标签或者一个dtb(设备树)映象来向内核传递配置数据。Boot数据的物理地址通过存放在寄存器R2中以向内核传递。

BootLoader至少传递下面几个参数：系统内存的起始地址和大小、根文件系统的位置。dtb在内存中的位置不能被内核解压过程覆盖。推荐存放位置是RAM的最初16KB位置：不能将其存放在物理地址0处，因为r2=0表示既不是tag数据，也不是dtb数据。

### 1.5 调用内核镜像

## 2 参考

《Documentation/arm/Booting》

