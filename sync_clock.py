import time
import hid

# Boardsource Lulu (Blok/RP2040)
# VID and PID from user request
VENDOR_ID = 0x4273
PRODUCT_ID = 0x7685

USAGE_PAGE = 0xFF60
USAGE_ID = 0x61


def get_raw_hid_interface():
    device_interfaces = hid.enumerate(VENDOR_ID, PRODUCT_ID)
    raw_hid_interfaces = [
        i
        for i in device_interfaces
        if (i["usage_page"], i["usage"]) == (USAGE_PAGE, USAGE_ID)
    ]

    if len(raw_hid_interfaces) == 0:
        return None

    target_device = None
    for device in raw_hid_interfaces:
        if (device["vendor_id"], device["product_id"]) == (VENDOR_ID, PRODUCT_ID):
            # In some OS/drivers, we need to check usage_page
            target_device = device["path"]
            break

    if not target_device:
        print("Lulu not found. Ensure Raw HID is enabled and device is connected.")
        return

    interface = hid.Device(path=target_device)
    return interface


def sync_time():
    try:
        interface = get_raw_hid_interface()

        # Get current Unix timestamp
        timestamp = int(time.time())
        offset = time.localtime().tm_gmtoff
        local_timestamp = timestamp + offset

        # Prepare packet: [0] = Report ID (0), [1] = 'T', [2..5] = timestamp
        # QMK raw HID packets are typically 32 bytes (or 64 on some platforms)
        packet = [0] * 33  # First byte is report ID
        packet[1] = ord("T")
        packet[2] = (local_timestamp >> 24) & 0xFF
        packet[3] = (local_timestamp >> 16) & 0xFF
        packet[4] = (local_timestamp >> 8) & 0xFF
        packet[5] = local_timestamp & 0xFF

        interface.write(bytes(packet))
        print(f"Synced time: {time.ctime(local_timestamp)}")
        interface.close()

    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    sync_time()
