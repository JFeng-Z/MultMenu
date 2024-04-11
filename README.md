# MultMenu
这是一个用于单色OLED屏幕的单片机多级菜单，采用双向链表结构，移植方便、在容量足够的情况下可无限扩展。
- 菜单支持类型有：
1. PARENTS  内含子菜单项
2. LOOP_FUNCTION： 循环运行某项功能
3. ONCE_FUNCTION：  单次运行某项功能
4. SWITCH：  开关型
5. DATA：  可调参数型  
- 光标移动使用非线性动画，菜单指示器使用线性动画。
在菜单数据结构上参考开源项目MorepUI
# 移植准备
1.往dispDirver.c中替换你的屏幕驱动，亦可使用example中的移植好的u8g2库。
2.需在Menu_State BtnScan(void)函数中放入按键扫描程序。
3.在新增菜单项时，应在menu.c中定义菜单所在页面(page)和菜单项(item)，然后在Menu_Team中通过AddPage、AddItem加入。
4.最后将Menu_Init()和Menu_Task()放入你的程序中。
# Example
基于STM32F103C8T6，使用硬件I2C2驱动0.96寸OLED(SSD1306)，按键为串口虚拟按键
其中：
- 0x00为上移按键
- 0x01为下移按键
- 0x02为确认按键
# 效果如下
![image](https://github.com/JFeng-Z/MultMenu/blob/master/Image/img1.png)
![image](https://github.com/JFeng-Z/MultMenu/blob/master/Image/img2.png)
