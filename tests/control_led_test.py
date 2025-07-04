import hid
import time

# VID и PID устройства
VID = 0x2e3c
PID = 0x5745

# Открываем HID-устройство
dev = hid.Device(VID, PID)
print(f"Устройство открыто: {dev.product}")

# Команда на включение светодиода (Report ID = 0x02, payload = 0x00)
# Убедись, что это корректно для твоей прошивки
report = bytes([0x02, 0x00]) + bytes(62)  # 64 байта
written = dev.write(report)
print(f"Отправлено байт: {written}")

# Подождём немного, чтобы устройство успело обработать команду
time.sleep(0.1)

# Чтение ответа с IN endpoint (Report ID = 0x01, ожидаем до 1 секунды)
response = dev.read(64, timeout=1000)
print("Принятый ответ:", response)

# Закрываем устройство
dev.close()
