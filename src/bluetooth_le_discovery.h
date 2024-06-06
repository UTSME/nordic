#ifndef BLUETOOTH_LE_DISCOVERY_H
#define BLUETOOTH_LE_DISCOVERY_H

#include "bluetooth_le_connection.h"
#include "bluetooth_le_read_write.h"

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>

//These are nrf header files
#include <bluetooth/gatt_dm.h>

//Size of a uuid in string form with the dashes
#define UUID_PRINT_SIZE 37

//Callback function when bt_gatt_read is called
// static uint8_t bluetooth_le_gatt_attribute_read_callback(struct bt_conn *conn, uint8_t err, struct bt_gatt_read_params *params, const void *data, uint16_t length);

//Callback function that is called when a service is discovered by bt_gatt_discover
// static uint8_t bluetooth_le_discover_callback(struct bt_conn *conn, const struct bt_gatt_attr *attr, struct bt_gatt_discover_params *params);

//Paramaters for reading from an attribute
// static struct bt_gatt_read_params bluetooth_le_gatt_read_paramaters = {.func = &bluetooth_le_gatt_attribute_read_callback,
// 																          .handle_count = 1};

//Paramaters for discovering attribute
// static struct bt_gatt_discover_params bluetooth_le_gatt_discover_paramaters = {.uuid = NULL,
// 																			      .func = &bluetooth_le_discover_callback,
// 																			      .start_handle = BT_ATT_FIRST_ATTRIBUTE_HANDLE,
// 																			      .end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE,
// 																			      .type = BT_GATT_DISCOVER_DESCRIPTOR};

// Starts the discovery of services
void bluetooth_le_gatt_print_services();

#endif