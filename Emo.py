import time
import datetime
import serial.tools.list_ports
import keyboard

if __name__ == '__main__':
    # 读取串口列表
    ports_list = list(serial.tools.list_ports.comports())
    if len(ports_list) <= 0:
        print("无串口设备")
    else:
        print("可用的串口设备如下: ")
        print("%-10s %-30s %-10s" % ("num", "name", "number"))
        for i in range(len(ports_list)):
            comport = list(ports_list[i])
            comport_number, comport_name = comport[0], comport[1]
            print("%-10s %-30s %-10s" % (i, comport_name, comport_number))

        # 打开串口
        port_num = ports_list[0][0]
        print("默认选择串口: %s" % port_num)
        # 串口号: port_num, 波特率: 115200, 数据位: 8, 停止位: 1, 超时时间: 0.5秒
        ser = serial.Serial(port=port_num, baudrate=115200, bytesize=serial.EIGHTBITS, stopbits=serial.STOPBITS_ONE,
                            timeout=0.5)
        if not ser.isOpen():
            print("打开串口失败")
        else:
            print("打开串口成功, 串口号: %s" % ser.name)

            # 串口发送数据
            f = open('Emo.h', 'r', encoding='utf-8')  # 打开hex图片文本文档，图片文本文档由imageLCD生成，由join together.py合并
    j = -1
    t0 = time.time()  # 统计总播放耗时，开始的时间
    # 获取当前时间
    current_time = datetime.datetime.now()

    ser.write(bytes.fromhex("01"))  # 发送数据包包头 01
    time_str = current_time.strftime("%Y-%m-%d-%H-%M-%S-")
    ser.write(bytes.fromhex("02"))  # 发送数据包包头 02
    while True:
        # 将时间字符串发送至串口
        ser.write(time_str.encode())
        j += 1
        
        ta = time.time()  # 统计每帧播放时长，开始计时
        dd = ""  # 中间变量
        ff = ""  # 存储每一帧并发给串口
        i = 0

        # 找到每帧数据开始的地方
        while ff != "{":  # 读取hex图片文本文档，见到”{“才继续
            ff = f.read(1)  # 读下一个字

        # 读取每帧数据，把每帧数据传给 dd
        while i < 1030:  # 每帧数据有 1030 个字节
            while ff != "X":  # 将图片文本中的 x 作为每个字节数据开始的标志，读到 x 才继续
                ff = f.read(1)
            ff = ""  # 清空 ff
            dd = dd + f.read(2) + " "  # dd 存储每个字节数据
            i += 1
        print(dd)
        # 通过串口发给单片机
        # 将16进制数转换为字节
        ser.write(bytes.fromhex("FF"))  # 发送数据包包头 FF
        ser.write(bytes.fromhex(dd))  # 发送数据
        ser.write(bytes.fromhex("FE"))  # 发送包尾

        while time.time() - ta < 1/30:  # 等待，直到帧播放时长为0.03333
            ""

        if j >= 255:
            j = 0
            f.seek(0)  # 将文件指针移回起始位置
        if  keyboard.is_pressed('esc'):
            quit()