# MultMenu
这是用U8G2库写的一个适用于0.96寸OLED(SSD1306)屏幕的菜单，主控为STM32F103RCT6，采用I2C驱动，理论上可以无限扩展菜单项，其中菜单链表结构参考了
开源项目MorepUI，按键检测使用了multibutton库
按键数为3个，分别为上、下和确认按键，其中确认按键沿用为退出APP
## 效果如下
![image](https://github.com/JFeng-Z/MultMenu/blob/master/Image/img1.png)
![image](https://github.com/JFeng-Z/MultMenu/blob/master/Image/img2.png)
