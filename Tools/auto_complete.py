from threading import Thread
import tkinter
import keyboard
import sys
import time
import tkinter as tk
from tkinter import Button, Label


class AutoCompleter(object):
    def __init__(self):
        super().__init__()
        self.reg = "_.qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789"
        self.listen_flag = True
        self.close_flag = False
        self.listen_thread = ""

    def set_flag(self):
        self.listen_flag = True

    def remove_flag(self):
        self.listen_flag = False

    def press_ctrl(self, x):
        print(x)
        self.remove_flag()

    def release_ctrl(self, x):
        print(x)
        self.set_flag()
        
    """
        我们写代码的时候，一般只按下"ctrl + s"或者"ctrl + z"或者"ctrl + c"或者"ctrl + v"或者"ctrl + x"组合键，
        因此，我们只需检测第一个按下"ctrl"就行。
        按下"ctrl"后，脚本马上进入等待第二个键按下的状态，1s后没有等到第二个键，状态退出；
        按下第二个键后，如果是"s"或者"z"，表明是组合键，不用进行代码补全；
        如果不是，那说明"ctrl"是无意中按到的，可以对该输入进行代码补全
    """
    def auto_complete(self, x: keyboard.KeyboardEvent):
        if self.reg.find(x.name) != -1 and self.listen_flag and x.event_type == "down":
            print("detect input " + x.name)
            keyboard.send('alt+/')
            sys.stdout.flush()

        # 100ms检查一次
        time.sleep(0.1)
    
    
    def auto_complete_init(self):
        keyboard.hook(self.auto_complete)
        keyboard.on_press_key("ctrl", self.press_ctrl)
        keyboard.on_release_key("ctrl", self.release_ctrl)
        while True:
            time.sleep(1)
            if self.close_flag:
                break

    def close_auto_complete(self):
        self.close_flag = True
        keyboard.unhook_all()
        self.listen_thread.join()

    def boot_auto_complete(self):
        self.close_flag = False
        self.listen_thread = Thread(target=self.auto_complete_init)
        self.listen_thread.start()

class SimpleGUI(object):
    def __init__(self):
        super().__init__()
        root = tk.Tk()
        autoCompleter = AutoCompleter()
        root.wm_attributes('-topmost',True)
        root.title("SylixOS IDE智能补全脚本")
        
        hints = Label(root, text="在编辑代码时，开启此脚本；切换至其他应用时，切记关闭此脚本")
        hints.pack(fill=tk.X, side=tk.TOP)

        btn1 = Button(root, text="点击开启Intellisense", command=autoCompleter.boot_auto_complete)
        btn1.pack()

        btn2 = Button(root, text="点击关闭Intellisense", command=autoCompleter.close_auto_complete)
        btn2.pack()
        
        root.mainloop()


SimpleGUI()
    