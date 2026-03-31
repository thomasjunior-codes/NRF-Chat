# 📡 NRF-Chat (ESP8266 + nRF24L01 Chat System)

A lightweight **offline chat system** built using **ESP8266** and **nRF24L01** modules.
This project creates a **WiFi-based chat UI** that communicates wirelessly using RF — no internet required.

---

## 🚀 Features

* 📶 **Wireless communication (nRF24L01)**
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
* 2 × nRF24L01 modules
* Jumper wires
* Breadboard
* 3.3V stable power supply (⚠️ IMPORTANT for nRF24L01)

---

## 🔌 Wiring (ESP8266 ↔ nRF24L01)

| nRF24L01 | ESP8266     |
| -------- | ----------- |
| VCC      | 3.3V        |
| GND      | GND         |
| CE       | D2 (GPIO4)  |
| CSN      | D1 (GPIO5)  |
| SCK      | D5 (GPIO14) |
| MOSI     | D7 (GPIO13) |
| MISO     | D6 (GPIO12) |

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
2. Open browser → connect to device IP (`192.168.4.1`)
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
2. Upload code to both ESP8266 devices
3. Power both modules
4. Connect your phone to:

   ```
   SSID: NodeA_Chat
   Password: 12345678
   ```
5. Open browser:

   ```
   http://192.168.4.1
   ```

---

## ⚠️ Important Notes

* nRF24L01 is **very sensitive to power**

  * Use capacitor (10µF–100µF) across VCC & GND
* Max payload = **32 bytes**

  * Longer messages may truncate
* Range depends on environment (walls, interference)

---

## 🧠 Known Limitations

* ❌ No message history persistence
* ❌ No group chat
* ❌ Limited payload size (32 bytes)
* ❌ Basic encryption (not secure for real-world use)

---

## 🔮 Future Improvements

* 📅 Message timestamps
* 👥 Group chat support
* 💾 EEPROM / SPIFFS message storage
* 🔒 Strong encryption (AES)
* 📡 Range optimization
* 📲 Installable PWA (app-like experience)

---

## 📸 Preview

Simple WhatsApp-style dark UI with:

* Chat bubbles
* Typing indicator
* Signal bars

---

## 🤝 Contributing

Pull requests are welcome!
Feel free to improve UI, performance, or add features.

---

## 📜 License

This project is open-source and free to use.

---

## 👨‍💻 Author

Developed by **[Your Name]**

---

## ⭐ Support

If you like this project, give it a ⭐ on GitHub!
