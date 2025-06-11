import os

def erase_flash():
    os.system("python3 esptool/esptool.py --chip esp32s3 erase_flash")

def write_flash(file_name):
    os.system(f"python3 esptool/esptool.py --chip esp32s3 --baud 961200 write_flash -z 0 {file_name}")

def main():
    print("Please hold the BOOT button and then plug the data cable into the USB port.")
    input("Press Enter after you have done this...")

    print("Please select the firmware to flash:")
    print("1. Xiaozhi")
    print("2. ChatGPT")
    choice = input("Enter your choice (1 or 2): ")

    if choice == "1":
        erase_flash()
        write_flash("Xiaozhi.bin")
        print("Flashing of Xiaozhi firmware is complete.")
    elif choice == "2":
        erase_flash()
        write_flash("ChatGPT.bin")
        print("Flashing of ChatGPT firmware is complete.")
    else:
        print("Invalid choice.")

if __name__ == "__main__":
    main()
