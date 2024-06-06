#ifndef BLUETOOTH_LE_SCANNING_H
#define BLUETOOTH_LE_SCANNING_H

#include "bluetooth_le_connection.h"

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>

//These are nrf header files
#include <bluetooth/scan.h>


// Paramaters taken from this example: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/samples/cellular/lte_ble_gateway/README.html
// https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/zephyr/connectivity/bluetooth/api/gap.html#c.bt_le_scan_param
// static struct bt_le_scan_param bluetooth_scan_paramaters = {.type = BT_LE_SCAN_TYPE_ACTIVE,
//                                                      	   .options = BT_LE_SCAN_OPT_FILTER_DUPLICATE,
//                                                      	   .interval = 0x0010,
//                                                      	   .window = 0x0010};

// Paramaters taken from this example: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/samples/cellular/lte_ble_gateway/README.html
// https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/libraries/bluetooth_services/scan.html#c.bt_scan_init_param
// static struct bt_scan_init_param bluetooth_initialisation_paramaters = {.connect_if_match = true,
// 																 		   .scan_param = &bluetooth_scan_paramaters,
//                                                                  	   .conn_param = BT_LE_CONN_PARAM_DEFAULT};

//Sets up the callback function for when a connection is made, bt_connected function will be called on connection
// static struct bt_conn_cb bluetooth_connection_cb = {.connected = bluetooth_le_connected};

//Function called on bluetooth connection
//static void bluetooth_le_connected(struct bt_conn *conn, uint8_t conn_err);

//Function that is called when bluetooth is ready
// static void bluetooth_le_ready(int err);

//Callback for when a bluetooth device matches the filters and then stops the bluetooth scanning
// static void bluetooth_le_scan_filter_match(struct bt_scan_device_info *device_info, struct bt_scan_filter_match *filter_match, bool connectable);

//Sets up callbacks for scan, enables filter, sets filters and starts scanning
void bluetooth_le_start_scan(const uint8_t *bluetooth_le_device_name);

void bluetooth_le_setup();

void bluetooth_le_gatt_print_services();

#endif 