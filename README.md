# 📡 ESP32 Real-Time Gas Monitoring & Telemetry Node

[![Board: ESP32](https://img.shields.io/badge/Board-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Protocol: MQTT](https://img.shields.io/badge/Protocol-MQTT-green.svg)](https://mqtt.org/)
[![Broker: HiveMQ](https://img.shields.io/badge/Broker-HiveMQ-yellow.svg)](https://www.hivemq.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-red.svg)](LICENSE)

Sistem pemantauan kebocoran gas secara *real-time* berbasis IoT menggunakan **ESP32**, **MQ Sensor (12-bit ADC)**, dan **Broker MQTT HiveMQ**. Sistem ini membaca kadar gas, menghitung persen kepekaan gas, memicu alarm bunyi bertingkat (buzzer), serta mengirimkan data telemetri ke server MQTT setiap 3 detik.

---

## 📋 Daftar Isi
- [Fitur Utama](#-fitur-utama)
- [Arsitektur & Alur Kerja](#-arsitektur--alur-kerja)
- [Skema Pin & Rangkaian](#-skema-pin--rangkaian)
- [Ambang Batas & Status Alarm](#-ambang-batas--status-alarm)
- [Topik MQTT & Payload](#-topik-mqtt--payload)
- [Persiapan & Instalasi Software](#-persiapan--instalasi-software)
- [Cara Upload Kode ke ESP32](#-cara-upload-kode-ke-esp32)
- [Cara Monitoring Data via MQTT Client](#-cara-monitoring-data-via-mqtt-client)
- [Troubleshooting](#-troubleshooting)
- [Lisensi](#-lisensi)

---

## 🛠️ Fitur Utama

- **Real-Time Data Sampling:** Sampling sensor analog gas dengan resolusi 12-bit (0–4095) setiap 3 detik.
- **Perhitungan Kepekaan Gas:** Konversi otomatis nilai mentah ADC menjadi persentase kepekaan ($0.00\% - 100.00\%$).
- **Acoustic & Visual Alarm:** 
  - Buzzer alarm dengan pola frekuensi dan irama berbeda berdasarkan tingkat bahaya.
  - Heartbeat LED indikator visual setiap kali transmisi data sukses.
- **Konektivitas MQTT:** Menerbitkan data telemetri secara terpisah (Nilai ADC, Status, dan Persentase).
- **Auto-Reconnect:** Sistem otomatis melakukan koneksi ulang jika koneksi Wi-Fi atau Broker MQTT terputus.

---

## 🏗️ Arsitektur & Alur Kerja

```text
[ MQ Sensor ] ---> (GPIO 34) ---> [ ESP32 Node ] ---> (Wi-Fi) ---> [ HiveMQ Broker ]
                                        │                                 │
                            ┌───────────┴───────────┐                     ├──> [ Dashboard / Mobile App ]
                            ▼                       ▼                     └──> [ MQTTX Client ]
                     [ Active Buzzer ]      [ Heartbeat LED ]
                        (GPIO 14)              (GPIO 2)
