#include "bluetooth_le_scanning.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(bluetooth_le_scanning);

static void bluetooth_le_connected(struct bt_conn *conn, uint8_t conn_err);

// Paramaters taken from this example: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/samples/cellular/lte_ble_gateway/README.html
// https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/zephyr/connectivity/bluetooth/api/gap.html#c.bt_le_scan_param
static struct bt_le_scan_param bluetooth_scan_paramaters = {.type = BT_LE_SCAN_TYPE_ACTIVE,
                                                     		.options = BT_LE_SCAN_OPT_FILTER_DUPLICATE,
                                                     		.interval = 0x0010,
                                                     		.window = 0x0010};

// Paramaters taken from this example: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/samples/cellular/lte_ble_gateway/README.html
// https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/libraries/bluetooth_services/scan.html#c.bt_scan_init_param
static struct bt_scan_init_param bluetooth_initialisation_paramaters = {.connect_if_match = true,
																 		.scan_param = &bluetooth_scan_paramaters,
                                                                 		.conn_param = BT_LE_CONN_PARAM_DEFAULT};

//Sets up the callback function for when a connection is made, bt_connected function will be called on connection
static struct bt_conn_cb bluetooth_connection_cb = {.connected = bluetooth_le_connected};

static void bluetooth_le_connected(struct bt_conn *conn, uint8_t conn_err) {
	//TODO: Print device information what was just connected to
    if (conn_err) {
		LOG_ERR("Error with connecting to bluetooth le device, error code: %d", conn_err);
	}
	else {
		bluetooth_le_connection_set(conn);
		LOG_INF("Getting connection info...");
		const bt_addr_le_t *bluetooth_le_destination_address = bt_conn_get_dst(conn);
		LOG_HEXDUMP_INF(bluetooth_le_destination_address->a.val, BT_ADDR_SIZE, "Connected to: ");
	}
}

static void bluetooth_le_ready(int err) {
	LOG_INF("Bluetooth ready");

    //Registers callback for when their is a connection
	bt_conn_cb_register(&bluetooth_connection_cb);
}

static void bluetooth_le_scan_filter_match(struct bt_scan_device_info *device_info, struct bt_scan_filter_match *filter_match, bool connectable) {
	LOG_INF("Match found");
	bt_scan_stop();
}

void bluetooth_le_start_scan(const uint8_t *bluetooth_le_device_name) {
	int err;

	//Im pretty sure this creates a function called bluetooth_scan_cb and then registers the different callback for each scan.
	//As in either match, no_match, error or connection
	//Ive set it up to only do a callback if it finds a match, the other three are all null
	BT_SCAN_CB_INIT(bluetooth_scan_cb, bluetooth_le_scan_filter_match, NULL, NULL, NULL);
	bt_scan_cb_register(&bluetooth_scan_cb);

	//Sets the paramaters for scanning with bluetooth
	bt_scan_init(&bluetooth_initialisation_paramaters);

	//Enables scanning using the address filter
	err = bt_scan_filter_enable(BT_SCAN_NAME_FILTER, false);
	if (err) {
		LOG_ERR("Error with enabling filter, error code: %d", err);
	}
	else {
		LOG_INF("Filtering enabled successfully");
	}

	//Searches for a match to the address defined above
	err = bt_scan_filter_add(BT_SCAN_FILTER_TYPE_NAME, bluetooth_le_device_name);
	if (err) {
		LOG_ERR("Error with adding filter, error code: %d", err);
	}
	else {
		LOG_INF("Filter added successfully");
	}

	//Starts scanning actively
	err = bt_scan_start(BT_SCAN_TYPE_SCAN_ACTIVE);
	if (err) {
		LOG_ERR("Error with starting scanning, error code: %d", err);
	}
	else {
		LOG_INF("Scanning started successfully");
	}
}

void bluetooth_le_setup() {
    int err;

	err = bt_enable(bluetooth_le_ready);
	if (err) {
		LOG_ERR("Error with starting enabling of bluetooth, error code: %d", err);
	}
    else {
        LOG_INF("Bluetooth enabling started successfully");
    }
}
