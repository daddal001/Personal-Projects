import time
import tkinter as tk
from tkinter import font as tkfont
from tkinter import messagebox,PhotoImage
import cv2
import numpy as np
import os
import random
from PIL import Image
import shutil

names = set()


class MainUI(tk.Tk):

    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        self.title_font = tkfont.Font(family='Helvetica', size=16, weight="bold")
        self.sub_font = tkfont.Font(family='Helvetica', size=12, weight="bold")
        self.title("Burglar Alarm System")
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
        for F in (StartPage, PageOne, Root, manage_modes, manage_users, add_user, remove_user, manage_modes,
                  manage_devices, buzzer, leds, faces, change_password, activate, activate2, change_password2, general):
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
            self.destroy()

    def verify_password(self, password):
        if password == "1234":
            return True
        else:
            return False
        #send password to arduino
        #verify password
        #get answer


class StartPage(tk.Frame):

        def __init__(self, parent, controller):
            tk.Frame.__init__(self, parent)
            self.controller = controller
            label = tk.Label(self, text="        Home Page        ", font=self.controller.title_font,fg="#ffffff", anchor="center")
            label2 = tk.Label(self, text="How would you like to login?", fg="#ffffff",  font=self.controller.sub_font, anchor="center")
            label3 = tk.Label(self, text="", fg="#ffffff", font=self.controller.sub_font, anchor="center")
            label.grid(row=1, column=1, sticky="nsew", padx=40)
            label2.grid(row=2, column=1, sticky="nsew", padx=40)
            label3.grid(row=0, column=0, sticky="nsew", padx=60)
            button1 = tk.Button(self, text="   Pin  ", fg="#263942", bg="#ffffff",command=lambda: self.controller.show_frame("PageOne"), anchor="center")
            button2 = tk.Button(self, text="    Face ID  ", fg="#263942", bg="#ffffff",command=lambda: self.facercognition(), anchor="center")
            button3 = tk.Button(self, text="Quit", fg="#263942", bg="#ffffff", command=self.on_closing, anchor="center")
            button1.grid(row=3, column=1, sticky="nsew", ipady=3, ipadx=7)
            button2.grid(row=4, column=1, sticky="nsew", ipady=3, ipadx=2)
            button3.grid(row=5, column=1, sticky="nsew",ipady=3, ipadx=32)


        def on_closing(self):
            if messagebox.askokcancel("Quit", "Are you sure?"):
                self.controller.destroy()

        def facercognition(self):
            messagebox.showerror("Error", "Look at the camera")
            recognizer = cv2.face.LBPHFaceRecognizer_create()
            recognizers = ["trainer/" + f for f in os.listdir('trainer') if not f.startswith(".")]
            cascadePath = "haarcascade_frontalface_default.xml"
            faceCascade = cv2.CascadeClassifier(cascadePath);

            font = cv2.FONT_HERSHEY_SIMPLEX

            # iniciate id counter
            id = 0

            # Initialize and start realtime video capture
            cam = cv2.VideoCapture(0)
            cam.set(3, 640)  # set video widht
            cam.set(4, 480)  # set video height

            # Define min window size to be recognized as a face
            minW = 0.1 * cam.get(3)
            minH = 0.1 * cam.get(4)
            found = False

            correct = 0
            wrong = 0
            interupt = False

            while True:

                ret, img = cam.read()
                # img = cv2.flip(img, -1) # Flip vertically

                gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

                faces = faceCascade.detectMultiScale(
                    gray,
                    scaleFactor=1.2,
                    minNeighbors=5,
                    minSize=(int(minW), int(minH)),
                )
                for (x, y, w, h) in faces:

                    cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)

                    for i in range(len(recognizers)):
                        recognizer.read(recognizers[i])
                        id, confidence = recognizer.predict(gray[y:y + h, x:x + w])

                        # Check if confidence is less them 100 ==> "0" is perfect match
                        if (confidence < 45):
                            correct += 1
                            id = "YAAAY"
                            confidence = "  {0}%".format(round(confidence))
                            if correct > 10:
                                found = True
                        else:
                            wrong += 1
                            id = "unknown"
                            confidence = "  {0}%".format(round(confidence))
                            if wrong > 10:
                                found = False
                                interupt = True

                        cv2.putText(img, str(id), (x + 5, y - 5), font, 1, (255, 255, 255), 2)
                        cv2.putText(img, str(confidence), (x + 5, y + h - 5), font, 1, (255, 255, 0), 1)

                cv2.imshow('camera', img)

                k = cv2.waitKey(10) & 0xff  # Press 'ESC' for exiting video
                if k == 27 or found == True or interupt == True:
                    break

            # Do a bit of cleanup
            print("\n [INFO] Exiting Program and cleanup stuff")
            cam.release()
            cv2.destroyAllWindows()
            if found != True:
                messagebox.showerror("Error", "Invalid face ID!")
                self.controller.show_frame("StartPage")
            else:
                self.controller.show_frame("general")


class PageOne(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        tk.Label(self, text="Enter the PIN", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=1, column=0, pady=10, padx=5)
        self.user_name = tk.Entry(self, borderwidth=3, fg="#000000", bg="lightgrey", font='Helvetica 11', show="*")
        tk.Label(self, text="").grid(row=0)
        self.user_name.grid(row=1, column=1, pady=10, padx=10)
        self.buttoncanc = tk.Button(self, text="Cancel", bg="#263942", fg="#000000", command=lambda: controller.show_frame("StartPage"), anchor="center")
        self.buttonext = tk.Button(self, text="Next", fg="#000000", bg="#263942", command=self.start_training, anchor="center")
        self.buttonclear = tk.Button(self, text="Clear", command=self.clear, bg="#263942", fg="#000000", anchor="center")
        self.buttoncanc.grid(row=2, column=0, pady=10, ipadx=5, ipady=4)
        self.buttonext.grid(row=2, column=1, pady=10, ipadx=5, ipady=4)
        self.buttonclear.grid(row=2,padx=5, ipady=4, column=2, pady=10)

    def start_training(self):
        global names
        if not self.user_name.get().isdigit():
            self.clear()
            messagebox.showerror("Error", "Password has to be 4 digits")
            return
        elif len(self.user_name.get()) > 4:
            self.clear()
            messagebox.showerror("Error", "Password is too long, has to be 4 digits")
            return
        elif len(self.user_name.get()) < 4:
            self.clear()
            messagebox.showerror("Error", "Password is too short, has to be 4 digits")
            return
        elif len(self.user_name.get()) == 0:
            self.clear()
            messagebox.showerror("Error", "Password cannot be empty!")
            return
        elif self.user_name.get() == "root":
            self.controller.show_frame("Root")
            return
        if self.verify_password(self.user_name.get()) == 3:
            self.clear()
            messagebox.showerror("Error", "Password is incorrect")
            return
        if self.verify_password(self.user_name.get()) == 1:
            self.clear()
            self.controller.show_frame("Root")

        if self.verify_password(self.user_name.get()) == 2:
            self.clear()
            self.controller.show_frame("general")

    def clear(self):
        self.user_name.delete(0, 'end')

    def verify_password(self, password):
        if password == "1234":
            return 1
        elif password == "0000":
            return 2
        else:
            return 3
        # send password to arduino
        # verify password
        # get answer

class Root(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        tk.Label(self, text="Root User Options", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=1, column=0, padx=40, sticky="nsew")
        tk.Label(self, text="", anchor="center").grid(row=0, column=0, padx=60, sticky="nsew")
        # change password, add user, remove user,
        self.buttonmanu = tk.Button(self, text="Manage Users", fg="#000000", bg="#263942", command=lambda: controller.show_frame("manage_users"), anchor="center")
        self.buttonmanm = tk.Button(self, text="Manage Modes", fg="#000000", bg="#263942", command=lambda: controller.show_frame("manage_modes"), anchor="center")
        self.buttonmand = tk.Button(self, text="Manage Devices", fg="#000000", bg="#263942", command=lambda: controller.show_frame("manage_devices"), anchor="center")
        self.buttonmanf = tk.Button(self, text="Manage Facial Recognition", fg="#000000", bg="#263942", command=lambda: controller.show_frame("faces"), anchor="center")
        self.buttonchanp = tk.Button(self, text="Change Password", fg="#000000", bg="#263942", command=lambda: controller.show_frame("change_password"), anchor="center")
        self.buttoncanc = tk.Button(self, text="Log Out", bg="#263942", fg="#000000",
                                    command=lambda: controller.show_frame("StartPage"), anchor="center")
        self.buttonact = tk.Button(self, text="Activate System", fg="#000000", bg="#263942", command=lambda: controller.show_frame("activate"))
        self.buttondeact = tk.Button(self, text="Deactivate System", fg="#000000", bg="#263942", command=self.deactivate)
        self.buttonclear = tk.Button(self, text="Clear", command=self.clear, bg="#263942", fg="#000000")
        self.buttonmanu.grid(row=2, column=0, ipadx=5, ipady=4, sticky="nsew")
        self.buttonmanm.grid(row=2, column=1, ipadx=5, ipady=4, sticky="nsew")
        self.buttonmand.grid(row=3, column=0, ipadx=5, ipady=4, sticky="nsew")
        self.buttonmanf.grid(row=3, column=1, ipadx=5, ipady=4, sticky="nsew")
        self.buttonchanp.grid(row=4, column=0, ipadx=5, ipady=4, sticky="nsew")
        self.buttonact.grid(row=4, column=1, ipadx=5, ipady=4, sticky="nsew")
        self.buttondeact.grid(row=5, column=0, ipadx=5, ipady=4, sticky="nsew")
        self.buttoncanc.grid(row=5, column=1, ipadx=5, ipady=4, sticky="nsew")


    def clear(self):
        self.user_name.delete(0, 'end')

    def deactivate(self):
        messagebox.showerror("Error", "The alarm has been deactivated")
        self.controller.show_frame("StartPage")


class manage_users(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        tk.Label(self, text="User Management Options", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=1, column=1, padx=40, sticky="nsew")
        tk.Label(self, text="", anchor="center").grid(row=0, column=0, padx=60, sticky="nsew")
        button1 = tk.Button(self, text="Add User", fg="#263942", bg="#ffffff",
                            command=lambda: self.controller.show_frame("add_user"), anchor="center")
        button2 = tk.Button(self, text="Delete User", fg="#263942", bg="#ffffff",
                            command=lambda: self.controller.show_frame("remove_user"), anchor="center")
        button3 = tk.Button(self, text="Cancel", fg="#263942", bg="#ffffff", command=lambda: self.controller.show_frame("Root"), anchor="center")
        button1.grid(row=3, column=1, sticky="nsew", ipady=3, ipadx=7)
        button2.grid(row=4, column=1, sticky="nsew", ipady=3, ipadx=2)
        button3.grid(row=5, column=1, sticky="nsew", ipady=3, ipadx=32)

    def clear(self):
        self.user_name.delete(0, 'end')

class add_user(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        tk.Label(self, text="Enter user PIN", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=1, column=0, pady=10, padx=5)
        self.user_name = tk.Entry(self, borderwidth=3, fg="#000000", bg="lightgrey", font='Helvetica 11', show="*")
        tk.Label(self, text="").grid(row=0)
        self.user_name.grid(row=1, column=1, pady=10, padx=10)
        button1 = tk.Button(self, text="Save", fg="#263942", bg="#ffffff",
                            command=self.save_user, anchor="center")
        button2 = tk.Button(self, text="Cancel", fg="#263942", bg="#ffffff",
                            command=lambda: self.controller.show_frame("Root"), anchor="center")
        button1.grid(row=2, column=0, sticky="nsew", ipady=3, ipadx=7)
        button2.grid(row=2, column=1, sticky="nsew", ipady=3, ipadx=2)

    def save_user(self):
        if not self.user_name.get().isdigit():
            messagebox.showerror("Error", "Password has to be 4 digits")
            self.clear()
            return
        elif len(self.user_name.get()) > 4:
            messagebox.showerror("Error", "Password is too long, has to be 4 digits")
            self.clear()
            return
        elif len(self.user_name.get()) < 4:
            messagebox.showerror("Error", "Password is too short, has to be 4 digits")
            self.clear()
            return
        elif len(self.user_name.get()) == 0:
            messagebox.showerror("Error", "Password cannot be empty")
            self.clear()
            return
        if self.verify_password(self.user_name.get()) == False:
            messagebox.showerror("Error", "User has been added")
            self.clear()
            self.controller.show_frame("Root")
            return
        else:
            self.clear()
            messagebox.showerror("Error", "Password already exits")
            return
    def verify_password(self, password):
        if password in ["1234", "0000"]:
            return True
        else:
            return False

    def clear(self):
        self.user_name.delete(0, 'end')

class remove_user(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        tk.Label(self, text="Enter user PIN", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=1, column=0, pady=10, padx=5)
        self.user_name = tk.Entry(self, borderwidth=3, fg="#000000", bg="lightgrey", font='Helvetica 11', show="*")
        tk.Label(self, text="").grid(row=0)
        self.user_name.grid(row=1, column=1, pady=10, padx=10)
        button1 = tk.Button(self, text="Save", fg="#263942", bg="#ffffff",
                            command=self.rm_user, anchor="center")
        button2 = tk.Button(self, text="Cancel", fg="#263942", bg="#ffffff",
                            command=lambda: self.controller.show_frame("Root"), anchor="center")
        button1.grid(row=2, column=0, sticky="nsew", ipady=3, ipadx=7)
        button2.grid(row=2, column=1, sticky="nsew", ipady=3, ipadx=2)

    def rm_user(self):
        if not self.user_name.get().isdigit():
            messagebox.showerror("Error", "Password has to be 4 digits")
            self.clear()
            return
        elif self.user_name.get() == "1234":
            messagebox.showerror("Error", "Root user cannot be deleted")
            self.clear()
            return
        elif len(self.user_name.get()) > 4:
            messagebox.showerror("Error", "Password is too long, has to be 4 digits")
            self.clear()
            return
        elif len(self.user_name.get()) < 4:
            messagebox.showerror("Error", "Password is too short, has to be 4 digits")
            self.clear()
            return
        elif len(self.user_name.get()) == 0:
            messagebox.showerror("Error", "Password cannot be empty")
            self.clear()
            return
        if self.verify_password(self.user_name.get()) == True:
            messagebox.showerror("Error", "User has been deleted")
            self.clear()
            self.controller.show_frame("Root")
            return
        else:
            self.clear()
            messagebox.showerror("Error", "User does not exist")
            return
    def verify_password(self, password):
        if password in ["1234", "0000"]:
            return True
        else:
            return False
    def clear(self):
        self.user_name.delete(0, 'end')

class manage_modes(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        tk.Label(self, text="Mode Management Options", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=1, column=1, padx=40, sticky="nsew")
        tk.Label(self, text="", anchor="center").grid(row=0, column=0, padx=60, sticky="nsew")
        button1 = tk.Button(self, text="Manage Mode 1", fg="#263942", bg="#ffffff",
                            command=lambda: self.controller.show_frame("add_user"), anchor="center")
        button2 = tk.Button(self, text="Manage Mode 2", fg="#263942", bg="#ffffff",
                            command=lambda: self.controller.show_frame("remove_user"), anchor="center")
        button3 = tk.Button(self, text="Cancel", fg="#263942", bg="#ffffff", command=lambda: self.controller.show_frame("Root"), anchor="center")
        button1.grid(row=3, column=1, sticky="nsew", ipady=3, ipadx=7)
        button2.grid(row=4, column=1, sticky="nsew", ipady=3, ipadx=2)
        button3.grid(row=5, column=1, sticky="nsew", ipady=3, ipadx=32)

    def clear(self):
        self.user_name.delete(0, 'end')

class manage_devices(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        tk.Label(self, text="Select Device", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=1, column=1, padx=40, sticky="nsew")
        tk.Label(self, text="", anchor="center").grid(row=0, column=0, padx=60, sticky="nsew")
        button1 = tk.Button(self, text="Buzzer", fg="#263942", bg="#ffffff",
                            command=lambda: self.controller.show_frame("buzzer"), anchor="center")
        button2 = tk.Button(self, text="LEDs", fg="#263942", bg="#ffffff",
                            command=lambda: self.controller.show_frame("leds"), anchor="center")
        button3 = tk.Button(self, text="Cancel", fg="#263942", bg="#ffffff", command=lambda: self.controller.show_frame("Root"), anchor="center")
        button1.grid(row=3, column=1, sticky="nsew", ipady=3, ipadx=7)
        button2.grid(row=4, column=1, sticky="nsew", ipady=3, ipadx=2)
        button3.grid(row=5, column=1, sticky="nsew", ipady=3, ipadx=32)

    def clear(self):
        self.user_name.delete(0, 'end')

class buzzer(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        tk.Label(self, text="Select Volume Level", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=1, column=1, padx=40, sticky="nsew")
        tk.Label(self, text="", anchor="center").grid(row=0, column=0, padx=60, sticky="nsew")
        button1 = tk.Button(self, text="High", fg="#263942", bg="#ffffff",
                            command=self.high, anchor="center")
        button2 = tk.Button(self, text="Mid", fg="#263942", bg="#ffffff",
                            command= self.mid, anchor="center")
        button3 = tk.Button(self, text="Low", fg="#263942", bg="#ffffff", command=self.low, anchor="center")
        button4 = tk.Button(self, text="Cancel", fg="#263942", bg="#ffffff",
                            command=lambda: self.controller.show_frame("Root"), anchor="center")
        button1.grid(row=3, column=1, sticky="nsew", ipady=3, ipadx=7)
        button2.grid(row=4, column=1, sticky="nsew", ipady=3, ipadx=2)
        button3.grid(row=5, column=1, sticky="nsew", ipady=3, ipadx=32)
        button4.grid(row=6, column=1, sticky="nsew", ipady=3, ipadx=32)

    def clear(self):
        self.user_name.delete(0, 'end')
    def high(self):
        #set buzzer volume high and play buzzer
        messagebox.showerror("Error", "Volume has been sent to high")
        self.controller.show_frame("Root")
        return
    def mid(self):
        #set buzzer volume mid and play buzzer
        messagebox.showerror("Error", "Volume has been sent to medium")
        self.controller.show_frame("Root")
        return
    def low(self):
        #set buzzer volume low and play buzzer
        messagebox.showerror("Error", "Volume has been sent to low")
        self.controller.show_frame("Root")
        return

class leds(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        tk.Label(self, text="Select LED Setting", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=1, column=1, padx=40, sticky="nsew")
        tk.Label(self, text="", anchor="center").grid(row=0, column=0, padx=60, sticky="nsew")
        button1 = tk.Button(self, text="ON", fg="#263942", bg="#ffffff",
                            command=self.on, anchor="center")
        button2 = tk.Button(self, text="OFF", fg="#263942", bg="#ffffff",
                            command=self.off, anchor="center")
        button3 = tk.Button(self, text="Cancel", fg="#263942", bg="#ffffff",
                            command=lambda: self.controller.show_frame("Root"), anchor="center")
        button1.grid(row=3, column=1, sticky="nsew", ipady=3, ipadx=7)
        button2.grid(row=4, column=1, sticky="nsew", ipady=3, ipadx=2)
        button3.grid(row=5, column=1, sticky="nsew", ipady=3, ipadx=32)

    def clear(self):
        self.user_name.delete(0, 'end')
    def on(self):
        #turn leds on
        messagebox.showerror("Error", "LEDs have been turned on")
        self.controller.show_frame("Root")
        return
    def off(self):
        #turn leds off
        messagebox.showerror("Error", "LEDs have been turned off")
        self.controller.show_frame("Root")
        return

class faces(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        tk.Label(self, text="Facial Recognition Management", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=1, column=1, padx=40, sticky="nsew")
        tk.Label(self, text="", anchor="center").grid(row=0, column=0, padx=60, sticky="nsew")
        button1 = tk.Button(self, text="Add face", fg="#263942", bg="#ffffff",
                            command=self.add_face, anchor="center")
        button2 = tk.Button(self, text="Remove face", fg="#263942", bg="#ffffff",
                            command=self.remove_face, anchor="center")
        button3 = tk.Button(self, text="Cancel", fg="#263942", bg="#ffffff", command=lambda: self.controller.show_frame("Root"), anchor="center")
        button1.grid(row=3, column=1, sticky="nsew", ipady=3, ipadx=7)
        button2.grid(row=4, column=1, sticky="nsew", ipady=3, ipadx=2)
        button3.grid(row=5, column=1, sticky="nsew", ipady=3, ipadx=32)

    def clear(self):
        self.user_name.delete(0, 'end')
    def add_face(self):
        messagebox.showerror("Error", "Look At The Camera, while we capture your facial data")
        def remove_empty_folders(path_abs):
            walk = list(os.walk(path_abs))
            for path, _, _ in walk[::-1]:
                if len(os.listdir(path)) == 0:
                    os.rmdir(path)

        cam = cv2.VideoCapture(0)
        cam.set(3, 640)  # set video width
        cam.set(4, 480)  # set video height

        face_detector = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

        # For each person, enter one numeric face id
        face_id = random.randint(10, 99)

        print("\n [INFO] Initializing face capture. Look the camera and wait ...")
        # Initialize individual sampling face count
        count = 0

        # Directory
        directory = "dataset" + str(face_id)

        # Parent Directory path
        parent_dir = "Data"

        # Path
        path = os.path.join(parent_dir, directory)

        # Create the directory
        # 'GeeksForGeeks' in
        # '/home / User / Documents'
        print(path)
        os.mkdir(path)

        while (True):

            ret, img = cam.read()
            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            faces = face_detector.detectMultiScale(gray, 1.3, 5)

            for (x, y, w, h) in faces:
                cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)
                count += 1

                # Save the captured image into the datasets folder
                cv2.imwrite("Data/dataset" + str(face_id) + "/User." + str(face_id) + '.' + str(count) + ".jpg",
                            gray[y:y + h, x:x + w])
                cv2.imshow('image', img)

            k = cv2.waitKey(10) & 0xff  # Press 'ESC' for exiting video
            if k == 27:
                remove_empty_folders("Data")
                break
            elif count >= 50:  # Take 30 face sample and stop video
                remove_empty_folders("Data")
                break

        # Do a bit of cleanup
        print("\n [INFO] Exiting Program and cleanup stuff")
        cam.release()
        cv2.destroyAllWindows()

        path = 'Data'

        # path = [os.path.join(path,f) for f in os.listdir(path)]
        walk = list(os.walk(path))
        path = [f for (f, _, _) in walk[::-1]]
        path = path[:-1]

        recognizer = cv2.face.LBPHFaceRecognizer_create()
        detector = cv2.CascadeClassifier("haarcascade_frontalface_default.xml");

        # function to get the images and label data
        def getImagesAndLabels(path):

            imagePaths = [os.path.join(path, f) for f in os.listdir(path)]
            faceSamples = []
            ids = []

            for imagePath in imagePaths:

                PIL_img = Image.open(imagePath).convert('L')  # convert it to grayscale
                img_numpy = np.array(PIL_img, 'uint8')

                id = int(os.path.split(imagePath)[-1].split(".")[1])
                faces = detector.detectMultiScale(img_numpy)

                for (x, y, w, h) in faces:
                    faceSamples.append(img_numpy[y:y + h, x:x + w])
                    ids.append(id)

            return faceSamples, ids

        print("\n [INFO] Training faces. It will take a few seconds. Wait ...")
        ids = []
        for i in path:
            faces, id = getImagesAndLabels(i)
            ids.append(id[0])
            recognizer.train(faces, np.array(id))
            # Save the model into trainer/trainer.yml
            recognizer.write('trainer/trainer' + str(i[-2:]) + '.yml')  # recognizer.save() worked on Mac, but not on Pi

        # Print the numer of faces trained and end program
        print("\n [INFO] {0} faces trained. Exiting Program".format(len(ids)))
        messagebox.showerror("Error", "New face ID has been added")
        self.controller.show_frame("Root")
        return

    def remove_face(self):
        messagebox.showerror("Error", "Look at the camera")
        recognizer = cv2.face.LBPHFaceRecognizer_create()
        recognizers = ["trainer/" + f for f in os.listdir('trainer') if not f.startswith(".")]
        cascadePath = "haarcascade_frontalface_default.xml"
        faceCascade = cv2.CascadeClassifier(cascadePath);

        font = cv2.FONT_HERSHEY_SIMPLEX

        # iniciate id counter
        id = 0

        # Initialize and start realtime video capture
        cam = cv2.VideoCapture(0)
        cam.set(3, 640)  # set video widht
        cam.set(4, 480)  # set video height

        # Define min window size to be recognized as a face
        minW = 0.1 * cam.get(3)
        minH = 0.1 * cam.get(4)
        found = False

        correct = 0
        wrong = 0
        interupt = False

        while True:

            ret, img = cam.read()
            # img = cv2.flip(img, -1) # Flip vertically

            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

            faces = faceCascade.detectMultiScale(
                gray,
                scaleFactor=1.2,
                minNeighbors=5,
                minSize=(int(minW), int(minH)),
            )
            for (x, y, w, h) in faces:

                cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)

                for i in range(len(recognizers)):
                    recognizer.read(recognizers[i])
                    id, confidence = recognizer.predict(gray[y:y + h, x:x + w])

                    # Check if confidence is less them 100 ==> "0" is perfect match
                    if (confidence < 45):
                        correct += 1
                        id = "YAAAY"
                        confidence = "  {0}%".format(round(confidence))  # 100 - confidence))
                        found = True
                        directory = "dataset" + str(recognizers[i][-6:-4])
                        # Parent Directory path
                        parent_dir = "Data"
                        # Path
                        path = os.path.join(parent_dir, directory)
                        shutil.rmtree(path)
                        os.remove(recognizers[i])
                    else:
                        wrong += 1
                        id = "unknown"
                        confidence = "  {0}%".format(round(confidence))  # 100 - confidence))
                        if wrong > 10:
                            found = False
                            interupt = True
                    cv2.putText(img, str(id), (x + 5, y - 5), font, 1, (255, 255, 255), 2)
                    cv2.putText(img, str(confidence), (x + 5, y + h - 5), font, 1, (255, 255, 0), 1)

            cv2.imshow('camera', img)

            k = cv2.waitKey(10) & 0xff  # Press 'ESC' for exiting video
            if k == 27 or found == True or interupt == True:
                break

        # Do a bit of cleanup
        print("\n [INFO] Exiting Program and cleanup stuff")

        cam.release()
        cv2.destroyAllWindows()
        if found != True:
            messagebox.showerror("Error", "Unable to delete, face ID was not found")
        else:
            messagebox.showerror("Error", "Face ID has been deleted")
            self.controller.show_frame("Root")

class change_password(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        tk.Label(self, text="Enter old PIN", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=0, column=0, pady=10, padx=5)
        self.old_pin = tk.Entry(self, borderwidth=3, fg="#000000", bg="lightgrey", font='Helvetica 11', show="*")
        tk.Label(self, text="Enter new PIN", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=1, column=0, pady=10, padx=5)
        self.new_pin = tk.Entry(self, borderwidth=3, fg="#000000", bg="lightgrey", font='Helvetica 11', show="*")
        self.old_pin.grid(row=0, column=1, pady=10, padx=10)
        self.new_pin.grid(row=1, column=1, pady=10, padx=10)
        button1 = tk.Button(self, text="Save", fg="#263942", bg="#ffffff",
                            command=self.change_password, anchor="center")
        button2 = tk.Button(self, text="Cancel", fg="#263942", bg="#ffffff",
                            command=lambda: self.controller.show_frame("Root"), anchor="center")
        button1.grid(row=2, column=0, sticky="nsew", ipady=3, ipadx=7)
        button2.grid(row=2, column=1, sticky="nsew", ipady=3, ipadx=2)

    def change_password(self):
        if not self.old_pin.get().isdigit() or not self.new_pin.get().isdigit():
            messagebox.showerror("Error", "Password has to be 4 digits")
            self.clear()
            return
        elif len(self.old_pin.get()) > 4 or len(self.new_pin.get()) > 4:
            messagebox.showerror("Error", "Password is too long, has to be 4 digits")
            self.clear()
            return
        elif len(self.old_pin.get()) < 4 or len(self.new_pin.get()) < 4:
            messagebox.showerror("Error", "Password is too short, has to be 4 digits")
            self.clear()
            return
        if self.new_pin.get() == self.old_pin.get():
            messagebox.showerror("Error", "The PINs cannot be the same")
            self.clear()
            return
        if self.verify_password(self.new_pin.get()) == True:
            messagebox.showerror("Error", "PIN already exists try a different pin")
            self.clear()
            return
        if self.verify_password(self.old_pin.get()) != True:
            messagebox.showerror("Error", "Old PIN is invalid")
            self.clear()
            return
        else:
            self.clear()
            # update password
            messagebox.showerror("Error", "Password has been updates")
            self.controller.show_frame("StartPage")
            return
    def verify_password(self, password):
        if password in ["1234"]:
            return True
        else:
            return False
    def clear(self):
        self.old_pin.delete(0, 'end')
        self.new_pin.delete(0, 'end')

class change_password2(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        tk.Label(self, text="Enter old PIN", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=0, column=0, pady=10, padx=5)
        self.old_pin = tk.Entry(self, borderwidth=3, fg="#000000", bg="lightgrey", font='Helvetica 11', show="*")
        tk.Label(self, text="Enter new PIN", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=1, column=0, pady=10, padx=5)
        self.new_pin = tk.Entry(self, borderwidth=3, fg="#000000", bg="lightgrey", font='Helvetica 11', show="*")
        self.old_pin.grid(row=0, column=1, pady=10, padx=10)
        self.new_pin.grid(row=1, column=1, pady=10, padx=10)
        button1 = tk.Button(self, text="Save", fg="#263942", bg="#ffffff",
                            command=self.change_password, anchor="center")
        button2 = tk.Button(self, text="Cancel", fg="#263942", bg="#ffffff",
                            command=lambda: self.controller.show_frame("general"), anchor="center")
        button1.grid(row=2, column=0, sticky="nsew", ipady=3, ipadx=7)
        button2.grid(row=2, column=1, sticky="nsew", ipady=3, ipadx=2)

    def change_password(self):
        if not self.old_pin.get().isdigit() or not self.new_pin.get().isdigit():
            messagebox.showerror("Error", "Password has to be 4 digits")
            self.clear()
            return
        elif len(self.old_pin.get()) > 4 or len(self.new_pin.get()) > 4:
            messagebox.showerror("Error", "Password is too long, has to be 4 digits")
            self.clear()
            return
        elif len(self.old_pin.get()) < 4 or len(self.new_pin.get()) < 4:
            messagebox.showerror("Error", "Password is too short, has to be 4 digits")
            self.clear()
            return
        if self.new_pin.get() == self.old_pin.get():
            messagebox.showerror("Error", "The PINs cannot be the same")
            self.clear()
            return
        if self.verify_password(self.new_pin.get()) == True:
            messagebox.showerror("Error", "PIN already exists try a different pin")
            self.clear()
            return
        if self.verify_password(self.old_pin.get()) != True:
            messagebox.showerror("Error", "Old PIN is invalid")
            self.clear()
            return
        else:
            self.clear()
            # update password
            messagebox.showerror("Error", "Password has been updates")
            self.controller.show_frame("StartPage")
            return
    def verify_password(self, password):
        if password in ["0000"]:
            return True
        else:
            return False
    def clear(self):
        self.old_pin.delete(0, 'end')
        self.new_pin.delete(0, 'end')

class activate(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        tk.Label(self, text="Select Activation Mode", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=1, column=1, padx=40, sticky="nsew")
        tk.Label(self, text="", anchor="center").grid(row=0, column=0, padx=60, sticky="nsew")
        button1 = tk.Button(self, text="Activate All Sensors", fg="#263942", bg="#ffffff",
                            command=self.activateAll, anchor="center")
        button2 = tk.Button(self, text="Activate Mode 1", fg="#263942", bg="#ffffff",
                            command=self.activate1, anchor="center")
        button3 = tk.Button(self, text="Activate Mode 2", fg="#263942", bg="#ffffff",
                            command=self.activate2, anchor="center")
        button4 = tk.Button(self, text="Cancel", fg="#263942", bg="#ffffff", command=lambda: self.controller.show_frame("Root"), anchor="center")
        button1.grid(row=3, column=1, sticky="nsew", ipady=3, ipadx=7)
        button2.grid(row=4, column=1, sticky="nsew", ipady=3, ipadx=2)
        button3.grid(row=5, column=1, sticky="nsew", ipady=3, ipadx=2)
        button4.grid(row=6, column=1, sticky="nsew", ipady=3, ipadx=32)

    def clear(self):
        self.user_name.delete(0, 'end')

    def activate1(self):
        messagebox.showerror("Error", "Mode 1 has been activated")
        self.controller.show_frame("StartPage")

    def activate2(self):
        messagebox.showerror("Error", "Mode 2 has been activated")
        self.controller.show_frame("StartPage")

    def activateAll(self):
        messagebox.showerror("Error", "All sensors have been activated")
        self.controller.show_frame("StartPage")

class activate2(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        tk.Label(self, text="Select Activation Mode", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=1, column=1, padx=40, sticky="nsew")
        tk.Label(self, text="", anchor="center").grid(row=0, column=0, padx=60, sticky="nsew")
        button1 = tk.Button(self, text="Activate All Sensors", fg="#263942", bg="#ffffff",
                            command=self.activateAll, anchor="center")
        button2 = tk.Button(self, text="Activate Mode 1", fg="#263942", bg="#ffffff",
                            command=self.activate1, anchor="center")
        button3 = tk.Button(self, text="Activate Mode 2", fg="#263942", bg="#ffffff",
                            command=self.activate2, anchor="center")
        button4 = tk.Button(self, text="Cancel", fg="#263942", bg="#ffffff", command=lambda: self.controller.show_frame("general"), anchor="center")
        button1.grid(row=3, column=1, sticky="nsew", ipady=3, ipadx=7)
        button2.grid(row=4, column=1, sticky="nsew", ipady=3, ipadx=2)
        button3.grid(row=5, column=1, sticky="nsew", ipady=3, ipadx=2)
        button4.grid(row=6, column=1, sticky="nsew", ipady=3, ipadx=32)

    def clear(self):
        self.user_name.delete(0, 'end')

    def activate1(self):
        messagebox.showerror("Error", "Mode 1 has been activated")
        self.controller.show_frame("StartPage")

    def activate2(self):
        messagebox.showerror("Error", "Mode 2 has been activated")
        self.controller.show_frame("StartPage")

    def activateAll(self):
        messagebox.showerror("Error", "All sensors have been activated")
        self.controller.show_frame("StartPage")

class general(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        tk.Label(self, text="General User Options", fg="#ffffff", font='Helvetica 12 bold', anchor="center").grid(row=1, column=1, padx=40, sticky="nsew")
        tk.Label(self, text="", anchor="center").grid(row=0, column=0, padx=60, sticky="nsew")
        button1 = tk.Button(self, text="Change Password", fg="#263942", bg="#ffffff",
                            command=lambda: controller.show_frame("change_password2"), anchor="center")
        button2 = tk.Button(self, text="Activate System", fg="#263942", bg="#ffffff",
                            command=lambda: self.controller.show_frame("activate2"), anchor="center")
        button3 = tk.Button(self, text="Deactivate System", fg="#263942", bg="#ffffff",
                            command=self.deactivate, anchor="center")
        button4 = tk.Button(self, text="Log Out", fg="#263942", bg="#ffffff", command=lambda: self.controller.show_frame("StartPage"), anchor="center")
        button1.grid(row=3, column=1, sticky="nsew", ipady=3, ipadx=7)
        button2.grid(row=4, column=1, sticky="nsew", ipady=3, ipadx=2)
        button3.grid(row=5, column=1, sticky="nsew", ipady=3, ipadx=2)
        button4.grid(row=6, column=1, sticky="nsew", ipady=3, ipadx=32)

    def clear(self):
        self.user_name.delete(0, 'end')

    def deactivate(self):
        messagebox.showerror("Error", "The alarm has been deactivated")
        self.controller.show_frame("StartPage")
app = MainUI()
#app.iconphoto(True, tk.PhotoImage(file='icon.ico'))
app.mainloop()