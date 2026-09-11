# ESP32 Real-Time Gas Monitoring & Telemetry Node

[![Board: ESP32](https://img.shields.io/badge/Board-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Protocol: MQTT](https://img.shields.io/badge/Protocol-MQTT-green.svg)](https://mqtt.org/)
[![Broker: HiveMQ](https://img.shields.io/badge/Broker-HiveMQ-yellow.svg)](https://www.hivemq.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-red.svg)](LICENSE)

Sistem pemantauan kadar gas secara *real-time* berbasis IoT menggunakan **ESP32**, **Simulasi Potensiometer / MQ Gas Sensor (12-bit ADC)**, dan **Broker MQTT HiveMQ**. Sistem ini membaca kadar analog gas, menghitung persentase kepekaan, memicu alarm bunyi bertingkat via buzzer, serta mengirimkan data telemetri ke server MQTT setiap 3 detik.

---


## Fitur Utama

- **Real-Time Data Sampling:** Sampling masukan analog (0–4095) setiap 3 detik menggunakan ADC 12-bit ESP32.
- **Flexibility Input:** Mendukung pengujian menggunakan **Potensiometer** pada simulator (Wokwi) maupun **MQ Gas Sensor** pada hardware nyata.
- **Perhitungan Kepekaan Gas:** Konversi otomatis nilai mentah ADC menjadi persentase kepekaan ($0.00\% - 100.00\%$).
- **Acoustic & Visual Alarm:** 
  - Active Buzzer dengan irama dan frekuensi nada khusus berdasarkan tingkat bahaya (`WASPADA` vs `BAHAYA GAS`).
  - Heartbeat LED indikator visual setiap kali transmisi data sukses.
- **Konektivitas MQTT:** Menerbitkan data telemetri terpisah (Nilai ADC, Status, dan Persentase).
- **Auto-Reconnect:** Otomatis menghubungkan ulang perangkat jika koneksi Wi-Fi atau Broker MQTT terputus.

---

## Arsitektur & Alur Kerja

```text
[ Potentiometer / MQ Sensor ] ---> (GPIO 34) ---> [ ESP32 Node ] ---> (Wi-Fi) ---> [ HiveMQ Broker ]
                                                        │                                 │
                                            ┌───────────┴───────────┐                     ├──> [ Dashboard / Mobile App ]
                                            ▼                       ▼                     └──> [ MQTTX Client ]
                                     [ Active Buzzer ]      [ Heartbeat LED ]
                                        (GPIO 14)              (GPIO 2)
