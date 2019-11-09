# Usage:
#  - connect an RFQuack dongle
#  - connect to it via the RFQuack-cli
#  - execute the following commands in the RFQuack IPython shell

# Configure the modem to:
#  - use 0xd391d301 as the sync word
#  - use 869.525 as the carrier frequency
#  - modem configuration 18, which means:
#    - 2-GFSK modulation
#    - 250kbps bitrate
#    - 250kHz RX bandwidth
#  - 20dB TX power

q.set_modem_config(
  syncWords=b'\xd3\x91\xd3\x91',
  carrierFreq=869.525,
  modemConfigChoiceIndex=18,
  txPower=20)

# Set the radio in fixed packed mode and no CRC validation (i.e., read packets
# even w/ invalid or no CRC)
q.set_register(0x37, 0b00000000)

# Create payloads (format: 0x95 R G B W ? ? CRC16)
green_and_white = b'\x95\x00\xff\x00\xff\x00\x00\x60\x74'

# Transmit forever
import time
while True:
  time.sleep(0.1)
  q.set_packet(green_and_white, 100)  # burst of 100 packets
