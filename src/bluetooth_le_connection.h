#ifndef BLUETOOTH_LE_CONNECTION_H
#define BLUETOOTH_LE_CONNECTION_H

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>

//The pointer to the current bluetooth le connection, will be null if there is no connection established
// static struct bt_conn *bluetooth_le_connection;

//Returns a pointer to the currently established connection, will be null if there is no connection established
struct bt_conn* bluetooth_le_connection_get();

//Sets the pointer for the current connection
void bluetooth_le_connection_set(struct bt_conn *bluetooth_le_connection_p);

//Disconnects curernt bluetooth connection with reason: BT_HCI_ERR_REMOTE_USER_TERM_CONN
//https://docs.zephyrproject.org/latest/connectivity/bluetooth/api/connection_mgmt.html#c.bt_conn_disconnect
void bluetooth_le_connection_disconnect();



#endif