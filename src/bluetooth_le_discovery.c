#include "bluetooth_le_discovery.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(bluetooth_le_discovery);

static uint8_t bluetooth_le_discover_callback(struct bt_conn *conn, const struct bt_gatt_attr *attr, struct bt_gatt_discover_params *params);

static struct bt_gatt_discover_params bluetooth_le_gatt_discover_paramaters = {.uuid = NULL,
																			   .func = &bluetooth_le_discover_callback,
																			   .start_handle = BT_ATT_FIRST_ATTRIBUTE_HANDLE,
																			   .end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE,
																			   .type = BT_GATT_DISCOVER_DESCRIPTOR};

static uint8_t bluetooth_le_discover_callback(struct bt_conn *conn, const struct bt_gatt_attr *attr, struct bt_gatt_discover_params *params) {
	printk("\n");
	if (attr == NULL) {
		LOG_INF("Discovery process finished");
		return BT_GATT_ITER_STOP;
	}
	else {
		LOG_INF("Found attribute");
		char UUID_buffer[UUID_PRINT_SIZE];
		bt_uuid_to_str(attr->uuid, UUID_buffer, sizeof(UUID_buffer));
		LOG_INF("UUID: %s", UUID_buffer);
		LOG_INF("Handle: %d", attr->handle);
	
		//Add the attribute to storage so it can be used to read and write later only if its been added earlier 
		if (bluetooth_le_get_attribute(attr->uuid).handle != BLUETOOTH_LE_ATTRIBUTE_INITIAL_HANDLE) {
			bluetooth_le_add_attribute(attr->uuid, attr->handle);
		}

		return BT_GATT_ITER_CONTINUE;
	}
}

void bluetooth_le_gatt_print_services() {
	int err;

	err = bt_gatt_discover(bluetooth_le_connection_get(), &bluetooth_le_gatt_discover_paramaters);
	if (err) {
		LOG_ERR("Error with starting discovery of gatt services, error code: %d", err);
	}
    else {
        LOG_INF("Gatt service discovery started");
    }
}
