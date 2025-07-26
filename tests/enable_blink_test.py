import os

fd = os.open('/dev/satellite-1', os.O_RDWR)

# Enable LED (sending byte '1')
os.write(fd, b'\x01')

# Reading response (blocking)
response = os.read(fd, 64)
print(f"Device response: {response.hex()}")

os.close(fd)