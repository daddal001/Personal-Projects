import tkinter as tk
from tkinter import font as tkfont
from tkinter import messagebox,PhotoImage
names = set()


class MainUI(tk.Tk):

    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        """global names
        with open("nameslist.txt", "r") as f:
            x = f.read()
            z = x.rstrip().split(" ")
            for i in z:
                names.add(i)"""
        self.title_font = tkfont.Font(family='Helvetica', size=16, weight="bold")
        self.sub_font = tkfont.Font(family='Helvetica', size=12, weight="bold")
        self.title("Face Recognizer")
        self.resizable(False, False)
        self.geometry("500x250")
        self.protocol("WM_DELETE_WINDOW", self.on_closing)
        self.active_name = None
        container = tk.Frame(self)
        container.grid(sticky="nsew")
        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)
        self.frames = {}
        #for F in (StartPage, PageOne, PageTwo, PageThree, PageFour):
        F = StartPage
        page_name = F.__name__
        frame = F(parent=container, controller=self)
        self.frames[page_name] = frame
        frame.grid(row=0, column=0, sticky="nsew")
        self.show_frame("StartPage")

    def show_frame(self, page_name):
            frame = self.frames[page_name]
            frame.tkraise()

    def on_closing(self):

        if messagebox.askokcancel("Quit", "Are you sure?"):
            global names
            f =  open("nameslist.txt", "a+")
            for i in names:
                    f.write(i+" ")
            self.destroy()


class StartPage(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        label = tk.Label(self, text="        Home Page        ", font=self.controller.title_font, fg="#ffffff", anchor="e")
        label2 = tk.Label(self, text="How would you like to login?", fg="#ffffff", font=self.controller.sub_font, anchor="e")
        label3 = tk.Label(self, text="", fg="#ffffff", font=self.controller.sub_font, anchor="e")
        label.grid(row=1, column=1, sticky="ne", padx=40)
        label2.grid(row=2, column=1, sticky="ne", padx=40)
        label3.grid(row=0, column=0, sticky="nsew", padx=60)
        button1 = tk.Button(self, text="   Sign up  ", fg="#263942", bg="#ffffff", command=lambda: self.controller.show_frame("PageOne"), anchor="center")
        button2 = tk.Button(self, text="   Check a User  ", fg="#263942", bg="#ffffff", command=lambda: self.controller.show_frame("PageTwo"), anchor="center")
        button3 = tk.Button(self, text="Quit", fg="#263942", bg="#ffffff", command=self.on_closing, anchor="center")
        button1.grid(row=3, column=1, sticky="nsew", ipady=3, ipadx=7)
        button2.grid(row=4, column=1, sticky="nsew", ipady=3, ipadx=2)
        button3.grid(row=5, column=1, sticky="nsew", ipady=3, ipadx=32)

    def on_closing(self):
        if messagebox.askokcancel("Quit", "Are you sure?"):
            global names
            with open("nameslist.txt", "w") as f:
                for i in names:
                    f.write(i + " ")
            self.controller.destroy()
app = MainUI()
#app.iconphoto(True, tk.PhotoImage(file='icon.ico'))
app.mainloop()