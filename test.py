import cv2
import tkinter as tk
from PIL import Image, ImageTk
import threading
import pyttsx3
import time

# Initialize Tkinter GUI
root = tk.Tk()
root.title("Video + Audio Test")

video_label = tk.Label(root)
video_label.pack()

stop_video = False

# Initialize Text-to-Speech Engine
engine = pyttsx3.init()


def play_video(video_path):
    """Function to play a video inside Tkinter GUI."""
    global stop_video
    stop_video = False

    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        print("Error: Could not open video.")
        return

    def update_frame():
        if stop_video:
            cap.release()
            return

        ret, frame = cap.read()
        if not ret:
            cap.set(cv2.CAP_PROP_POS_FRAMES, 0)  # Restart video if it ends
            ret, frame = cap.read()

        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        img = Image.fromarray(frame)
        img = ImageTk.PhotoImage(img)

        video_label.config(image=img)
        video_label.image = img

        root.after(33, update_frame)  # 30 FPS update

    update_frame()


def play_audio_and_remove_video(text):
    """Function to play speech and remove the video from GUI after speech ends."""
    global stop_video
    engine.say(text)
    engine.runAndWait()

    # Remove video from GUI instead of pausing
    video_label.config(image="")
    stop_video = True  # Stop the video


def start_video_and_audio():
    """Start video and audio together twice."""
    video_path = "desktop-app/mars.mp4"  # Ensure this file exists

    for i in range(2):  # Run the test twice
        print(f"Starting test {i + 1}...")
        stop_video = False
        threading.Thread(target=play_video, args=(video_path,), daemon=True).start()
        time.sleep(1)  # Small delay before audio starts
        play_audio_and_remove_video(
            f"Hello! This is test number {i + 1}. The video will be removed after this message."
        )
        time.sleep(2)  # Small gap before repeating


# Run video and audio as soon as GUI starts
threading.Thread(target=start_video_and_audio, daemon=True).start()

root.mainloop()
