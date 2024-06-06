#ifndef BLUETOOTH_LE_READ_WRITE_H
#define BLUETOOTH_LE_READ_WRITE_H

#include "bluetooth_le_connection.h"
#include "bluetooth_le_discovery.h"

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

//Intial size for the amount of attributes that can be stored
#define BLUETOOTH_LE_ATTRIBUTE_CAPACITY 10

enum bluetooth_le_attribute_special_values {
    BLUETOOTH_LE_ATTRIBUTE_INITIAL_HANDLE = -1,
    BLUETOOTH_LE_ATTRIBUTE_ADDED_HANDLE = -2
};

//THIS WILL BE WORK AROUND TO USE UUID TO GET HANDLE WHICH THEN CAN BE USED IN READ SO BYTES AT THE END ARE NOT LOST
struct bluetooth_le_attribute {
    struct bt_uuid *uuid;
    int handle;
    int index;
};

struct bluetooth_le_attributes {
    struct bluetooth_le_attribute attributes[BLUETOOTH_LE_ATTRIBUTE_CAPACITY];
    int size;
    int capacity;
};

//Adds attribute to the struct of attributes that can be used to read and write.
//Will return 1 if the attributes struct is full, will return 0 on success
int bluetooth_le_add_attribute(const struct bt_uuid *uuid_p, int handle_p);

//Returns pointer to the attribute that matches the provided uuid
//If there is not such attribute in the bluetooth_le_attributes struct, it will return NULL
struct bluetooth_le_attribute bluetooth_le_get_attribute(const struct bt_uuid *uuid_p);

//Reads data into buffer and returns number of bytes read into it
void bluetooth_le_read_attribute(const struct bt_uuid *uuid, uint8_t *buffer, int buffer_length);

//Writes data from buffer to attribute specified by uuid
void bluetooth_le_write_attribute(const struct bt_uuid *uuid, const uint8_t *buffer, int buffer_length);

#endif