#include "bluetooth_le_scanning.h"
#include "bluetooth_le_discovery.h"
#include "bluetooth_le_connection.h"
#include "bluetooth_le_read_write.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/uuid.h>

#define BT_UUID_TESTING_VAL \
        BT_UUID_128_ENCODE(0xbeb5483e, 0x36e1, 0x4688, 0xb7f5, 0xea07361b26a8)

#define BT_UUID_TESTING \
	BT_UUID_DECLARE_128(BT_UUID_TESTING_VAL)

const uint8_t esp32_name[] = "esp32_bt";
const uint8_t test_write_data[] = "12345678901122334455667788";
// const uint8_t test_write_data_2[] = "this is just numbers 1234567891011121314";

//Uses bluetooth as the logging name for logs produced by these functions
LOG_MODULE_REGISTER(main);

int main(void) {	
	bluetooth_le_add_attribute(BT_UUID_TESTING, BLUETOOTH_LE_ATTRIBUTE_INITIAL_HANDLE);

	bluetooth_le_setup();

	while (bt_is_ready() == false) {
		LOG_INF("Waiting for bluetooth...");
		k_msleep(2000);
	}

	bluetooth_le_start_scan(esp32_name);

	while (bluetooth_le_connection_get() == NULL) {
		LOG_INF("Waiting for bluetooth connection to start gatt service discovery...");
		k_msleep(2000);
	}

	bluetooth_le_gatt_print_services();

	k_msleep(4000);

	uint8_t test_arr[25];
	bluetooth_le_read_attribute(BT_UUID_TESTING, test_arr, 25);

	bluetooth_le_write_attribute(BT_UUID_TESTING, test_write_data, sizeof(test_write_data));

	bluetooth_le_read_attribute(BT_UUID_TESTING, test_arr, 25);

	//SOMETHING IS GOING WRONG WITH THE MUTEX THING FOR SOME REASON
	k_msleep(10000);
	for (int i = 0; i < 25; i++) {
		printk("%c", test_arr[i]);
	}

	k_msleep(4000);
	bluetooth_le_connection_disconnect();

	while(1) {
		LOG_PRINTK(".");
		k_msleep(2000);
	}
}
