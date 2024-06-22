# MultMenu
这是一个用于单色OLED屏幕的单片机多级菜单，采用双向链表结构，移植方便、在容量足够的情况下可无限扩展。
- 菜单支持类型有：
1. PARENTS：内含子菜单项
2. LOOP_FUNCTION：循环运行某项功能
3. ONCE_FUNCTION： 单次运行某项功能
4. SWITCH：开关型
5. DATA：可调参数型
6. RETURN: 用于上级返回
- 全局使用非线性动画（类PID控制器）
在菜单数据结构上参考开源项目：https://github.com/morepray/MorepUI。感谢！
# 移植准备
1. 在menuConfig.h中配置你的屏幕分辨率与菜单字体。
2. 往dispDirver.c中替换你的屏幕驱动，亦可使用example中的移植的u8g2库。
3. 在Menu_State BtnScan(void)函数中放入你的按键扫描程序。
4. 在image.c中写入你需要显示的项目logo，否则图形界面的logo为预设logo，推荐一个开源图标库：[https://github.com/hugeicons/hugeicons-react](https://github.com/hugeicons/hugeicons-react)，图片取模工具：[https://javl.github.io/image2cpp/](https://github.com/javl/image2cpp)。
5. 在新增菜单项时，应在menu.c中定义菜单所在页面(page)和菜单项(item)，然后在Craete_MenuTree中通过AddPage、AddItem加入。
6. 建立一个xMenu类型的全局结构体
7. 将Menu_Init和Menu_Loop放入你的程序中（若使用RTOS，建议给予10ms运行周期）。
# Example
基于STM32F103C8T6，使用硬件I2C2驱动0.96寸OLED(SSD1306)，按键为串口虚拟按键
其中：
- 0x01为上移按键
- 0x02为下移按键
- 0x03为确认按键
# 效果如下
![image](https://github.com/JFeng-Z/MultMenu/blob/master/Image/img1.png)
![image](https://github.com/JFeng-Z/MultMenu/blob/master/Image/img2.png)
- 视频演示地址：https://b23.tv/TOwmuKL
