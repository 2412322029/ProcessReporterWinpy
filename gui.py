import tkinter as tk
import subprocess
import threading
import queue
import os

class Application:
    def __init__(self, root):
        self.root = root
        self.root.title("Process Reporter")
        self.output_text = tk.Text(self.root, wrap=tk.WORD, state=tk.DISABLED)
        self.output_text.pack(fill=tk.BOTH, expand=True)
        self.canvas = tk.Canvas(self.root, width=20, height=20)
        self.canvas.pack()
        self.status_color = "red"  # 初始状态为红色
        self.status_indicator = self.canvas.create_oval(
            5, 5, 15, 15, fill=self.status_color)
        self.start_button = tk.Button(
            self.root, text="Start", command=self.start_process)
        self.start_button.pack(pady=10)
        self.sprocess=None
        self.output_queue = queue.Queue()
        self.process = threading.Thread(target=self.run_process, daemon=True)
        self.update_output()

    def start_process(self):
        if self.start_button.cget("text") == "Start":
            self.process.start()
            self.start_button.config(text="Stop")
            self.status_color = "green"
        else:
            self.process = threading.Thread(target=self.run_process, daemon=True)  
            os.system('taskkill /f /pid %s' % self.sprocess.pid)
            self.output_queue.put("kill [%s]\n" % self.sprocess.pid)
            self.start_button.config(text="Start")
            self.status_color = "red"
        self.canvas.itemconfig(self.status_indicator, fill=self.status_color)

    def run_process(self):
        if not os.path.exists("core\\ProcessReporterWincpp.exe"):
            self.output_queue.put("缺少 ProcessReporterWincpp.exe")
        self.sprocess = subprocess.Popen(["core\\ProcessReporterWincpp.exe"],
                                    stdout=subprocess.PIPE,
                                    stderr=subprocess.PIPE,
                                    encoding='utf-8',
                                    text=True, bufsize=1)
        self.output_queue.put(f"starting pid[{self.sprocess.pid}]\n")
        while not self.sprocess.stdout.closed:
            try:
                output = self.sprocess.stdout.readline().strip()
                if output:
                    self.output_queue.put(output)
                if self.sprocess.poll() is not None:
                    break
            except UnicodeDecodeError as e:
                self.output_queue.put(str(e))       

    def update_output(self):
        while not self.output_queue.empty():
            output = self.output_queue.get()
            self.output_text.config(state=tk.NORMAL)
            self.output_text.insert(tk.END, output+"\n")
            self.output_text.see(tk.END)
            self.output_text.config(state=tk.DISABLED)

        self.root.after(100, self.update_output)

    def on_close(self):
        if self.sprocess:
            os.system('taskkill /f /pid %s' % self.sprocess.pid)
        self.root.destroy()  # 关闭窗口
        self.root.quit()  # 退出应用程序


if __name__ == "__main__":
    root = tk.Tk()
    app = Application(root)
    root.mainloop()
