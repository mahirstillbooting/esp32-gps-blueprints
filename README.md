# 🛰️ ESP32 GPS Blueprints

Welcome to the **ESP32 GPS Blueprints** repository! This project provides hardware circuit schematics, wiring diagrams, and Arduino code for interfacing GPS modules with the **ESP32 DevKit V1** microcontroller board.

---

## 📂 Repository Structure

```text
esp32-gps-blueprints/
├── README.md
├── code/
│   ├── esp32_devkit_v1_btns_buzzer_led_full_setup_ver1.ino
│   ├── esp32_devkit_v1_neo6m_buzzer_led.ino
│   ├── esp32_external_single_led_blink.ino
│   └── esp32_onboard_led_blink.ino
├── circuits/
│   ├── breadboard.drawio
│   ├── esp32_dev_kit_v1_btns_buzzer_led_full_setup_ver1.drawio
│   ├── esp32_dev_kit_v1_diagram.drawio
│   ├── esp32_devkit_v1_neo6m_buzzer_led.drawio
│   ├── esp32_devkit_v1_neo6m_buzzer_led_noBreadboard.drawio
│   └── esp32_external_single led.drawio
└── images/
```

---

## 🎨 How to Open Circuit Diagrams (`.drawio` Files)

Follow these steps to view or edit the interactive circuit schematics:

1. **Download the Diagram File**:
   - Navigate to the [`circuits/`](circuits/) directory in this repository.
   - Download the desired `.drawio` file (e.g., `breadboard.drawio`).
2. **Open draw.io**:
   - Open your web browser and navigate to [draw.io](https://app.diagrams.net/).
3. **Load the File**:
   - From the top menu bar, click **File** > **Open from**...
   - Choose **Device** (or local file system) and select your downloaded `.drawio` file.
4. **View & Edit**:
   - The interactive breadboard circuit diagram will load automatically for editing or exporting.

---

## 🛠️ How to Setup & Run the Code on ESP32 DevKit V1

### Step 1: Download & Install Arduino IDE
- Download the latest **Arduino IDE** from the official website: [arduino.cc/en/software](https://www.arduino.cc/en/software).

### Step 2: Install USB to UART Bridge VCP Drivers
To enable USB communication between your PC and the ESP32 board, install the Silicon Labs CP210x driver:
- Download the drivers from the [Silicon Labs USB to UART Bridge VCP Drivers Download Page](https://www.silabs.com/software-and-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads).
- **Windows Users**: For Windows 10/11, it is recommended to download the **Universal Windows Driver**.
- **Installation Procedure**:
  1. Extract the downloaded ZIP file to a local folder.
  2. Locate the `silabser.inf` setup file.
  3. Right-click `silabser.inf` and select **Install** (or install via Windows Device Manager).
  4. Once installed, your computer will recognize the ESP32 when plugged in.

### Step 3: Add ESP32 Board Manager URL
1. Open **Arduino IDE**.
2. Navigate to **File** > **Preferences** (or press `Ctrl + ,`).
3. In the **Additional Boards Manager URLs** field, paste the following Espressif JSON URL:
   ```text
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
   *(Note: If you already have existing URLs in this field, separate them with a comma).*
4. Click **OK** to save settings.

### Step 4: Install ESP32 Board Support
1. On the left sidebar of Arduino IDE, click the **Boards Manager** icon (or go to **Tools** > **Board** > **Boards Manager...**).
2. Type `esp32` in the search box.
3. Find **esp32 by Espressif Systems** and click **Install**.

### Step 5: Connect Board & Select Serial Port
1. Plug your **ESP32 DevKit V1** into your computer using a USB data cable.
2. Select the Serial Port:
   - Go to **Tools** > **Port** > Select the active COM port assigned to your driver (e.g., `COM1`, `COM3`, `COM5`, etc.).
3. Select the ESP32 Board:
   - Go to **Tools** > **Board** > **esp32** > Select **ESP32 Dev Module**.

### Step 6: Download & Upload Sketch
1. Download the `.ino` code file from the [`code/`](code/) directory once uploaded.
2. Open the `.ino` file in Arduino IDE (or copy the code into a new IDE sketch).
3. Click the **Upload** button (right arrow icon in the top toolbar) to compile and flash the sketch to your ESP32.

### Step 7: Open Serial Monitor
1. Open the Serial Monitor via **Tools** > **Serial Monitor** (or press `Ctrl + Shift + M`).
2. Set the baud rate in the bottom-right corner of the Serial Monitor window to **`115200` baud**.
3. You will see real-time location coordinates and GPS data logged to the console!

---

## 📄 License & Credits
Developed by [@mahirstillbooting](https://github.com/mahirstillbooting). Feel free to star ⭐️ the repository and contribute!
