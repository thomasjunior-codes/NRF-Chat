# 📡 nRF-Chat (ESP8266 + nRF24L01+PA+LNA Chat System)

A lightweight **offline chat system** built using **ESP8266** and **nRF24L01+PA+LNA** modules.
This project creates a **WiFi-based chat UI** that communicates wirelessly using RF — no internet required.

During natural disasters like earthquakes, floods, or cyclones, traditional communication networks (mobile towers, internet) often fail. NRF-Gram’s ~1KM RF range (with nRF24L01+PA+LNA) enables reliable, direct device-to-device messaging without any infrastructure—so even during a blackout, you can stay connected with nearby people, including your favorite neighbor.

---

## 🚀 Features

* 📶 **Long-range wireless communication (PA+LNA)**
* 📱 **Mobile-friendly web chat UI**
* 🌙 **Dark mode interface (always on)**
* ⌨️ **Typing indicator**
* ✔️ **Message delivery status (✔ / ✔✔)**
* 📊 **Signal strength indicator**
* 🔐 **Basic XOR encryption**
* 🔄 **Auto-scroll chat**
* ⚡ **Low latency communication**

---

## 🧰 Hardware Required

* 2 × ESP8266 (NodeMCU / Wemos D1 Mini)
* 2 × nRF24L01+PA+LNA modules (with external antenna)
* Jumper wires
* Breadboard
* 🔋 Stable 3.3V power supply (**CRITICAL**)
* ⚡ Capacitor (10µF – 100µF recommended)

---

## 🔌 Wiring (ESP8266 ↔ nRF24L01+PA+LNA)

| nRF24L01+PA+LNA | ESP8266     |
| --------------- | ----------- |
| VCC             | 3.3V        |
| GND             | GND         |
| CE              | D2 (GPIO4)  |
| CSN             | D1 (GPIO5)  |
| SCK             | D5 (GPIO14) |
| MOSI            | D7 (GPIO13) |
| MISO            | D6 (GPIO12) |

<img width="1177" height="1109" alt="nrf connection" src="https://github.com/user-attachments/assets/4813df5d-c913-4447-8562-e75eeb8661e0" />

---

## ⚡ Power Stability (IMPORTANT)

The **nRF24L01+PA+LNA module requires stable power** and can draw high current during transmission.
Without proper filtering, you may experience:

* ❌ Message loss
* ❌ Random disconnects
* ❌ Reduced range
* ❌ Module not detected

### ✅ Solution: Add Capacitor

Connect a capacitor directly across VCC and GND:

```
VCC ----||---- GND
        ||
     Capacitor
```

### 🔧 Recommended Values:

* Minimum: **10µF**
* Best: **47µF – 100µF electrolytic capacitor**

### 📌 Tips:

* Place capacitor **as close as possible** to the module pins
* Observe polarity:

  * Long leg → VCC
  * Short leg → GND
* For best performance, use **external 3.3V regulator** (not ESP8266 3.3V pin)

---

## 📦 Libraries Used

Install via Arduino Library Manager:

* `ESP8266WiFi`
* `ESP8266WebServer`
* `RF24` by TMRh20
* `SPI`

---

## ⚙️ How It Works

1. ESP8266 creates a **WiFi Access Point**
2. Open browser → connect to device (`192.168.4.1`)
3. Messages are:

   * Sent via HTTP → ESP8266
   * Encrypted (XOR)
   * Transmitted via nRF24L01
4. Receiver:

   * Decrypts message
   * Displays in chat UI
   * Sends ACK back

---

## 🛠️ Setup Instructions

1. Connect hardware as per wiring table
2. Add capacitor between VCC & GND
3. Upload code to both ESP8266 devices
4. Power both modules
5. Connect your phone to:

   ```
   SSID: NodeA_Chat
   Password: 12345678
   ```
6. Open browser:

   ```
   http://192.168.4.1
   ```

---

## ⚠️ Important Notes

* PA+LNA modules consume **higher current than normal nRF24L01**
* Always use **external power source + capacitor**
* Max payload = **32 bytes**
* Range depends on antenna & environment

---

## 🧠 Known Limitations

* ❌ No message history persistence
* ❌ No group chat
* ❌ Limited payload size (32 bytes)
* ❌ Basic encryption (not secure for production)

---

## 🔮 Future Improvements

* 📅 Message timestamps
* 👥 Group chat
* 💾 Storage (SPIFFS / EEPROM)
* 🔒 AES encryption
* 📡 Mesh networking
* 📲 PWA support

---

## 🤝 Contributing

Pull requests are welcome!

---

## 📜 License

Open-source and free to use.

---

## ⭐ Support

If you like this project, give it a ⭐ on GitHub!
