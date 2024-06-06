#include "bluetooth_le_connection.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(bluetooth_le_connection);

static struct bt_conn *bluetooth_le_connection = NULL;

struct bt_conn* bluetooth_le_connection_get() {
    return bluetooth_le_connection;
}

void bluetooth_le_connection_set(struct bt_conn *bluetooth_le_connection_p) {
    bluetooth_le_connection = bluetooth_le_connection_p;
}

void bluetooth_le_connection_disconnect() {
    int err;

    if (bluetooth_le_connection == NULL) {
        LOG_ERR("Error disconnecting bluetooth connection, bluetooth is not currently connected");
        return;
    }

    err = bt_conn_disconnect(bluetooth_le_connection, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
    if (err) {
        LOG_ERR("Error disconnecting bluetooth connection, error code: %d", err);
    }
    else {
        LOG_INF("Bluetooth connection successfully disconnected");
    }
}
