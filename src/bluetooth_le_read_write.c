#include "bluetooth_le_read_write.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(bluetooth_le_read_write);

static uint8_t bluetooth_le_gatt_attribute_read_callback(struct bt_conn *conn, uint8_t err, struct bt_gatt_read_params *params, const void *data, uint16_t length);

static void bluetooth_le_gatt_attribute_write_callback(struct bt_conn *conn, uint8_t err, struct bt_gatt_write_params *params);

static struct bt_gatt_read_params bluetooth_le_gatt_read_paramaters = {.func = &bluetooth_le_gatt_attribute_read_callback,
																       .handle_count = 1,
                                                                       .single.offset = 0};

static struct bt_gatt_write_params bluetooth_le_gatt_write_paramaters = {.func = &bluetooth_le_gatt_attribute_write_callback};

static struct bluetooth_le_attributes bluetooth_le_attributes = {.capacity = BLUETOOTH_LE_ATTRIBUTE_CAPACITY,
                                                                 .size = 0}; 

static int r_w_in_progress = false;

static struct bluetooth_le_write_buffer {
    uint8_t *buffer;
    int buffer_length;
} bluetooth_le_write_buffer;

static uint8_t bluetooth_le_gatt_attribute_read_callback(struct bt_conn *conn, uint8_t err, struct bt_gatt_read_params *params, const void *data, uint16_t length) {
    printk("\n");
    if (data == NULL) {
        r_w_in_progress = false;
        bluetooth_le_gatt_read_paramaters.single.offset = 0;
        return BT_GATT_ITER_STOP;
    }

    LOG_INF("Reading from attribute...");

	if (err) {
		LOG_ERR("Error while reading from attribute, error code: %d", err);
	}

	LOG_INF("Data length: %d", length);

	if (length > 0) {
		char string[length + 1];
        memcpy(string, data, length);
        string[length] = '\0';
        LOG_INF("Data: %s", string);
        
        //Can read all bytes in at the correct offset
        if (bluetooth_le_write_buffer.buffer_length >= length + bluetooth_le_gatt_read_paramaters.single.offset) {
            memcpy(bluetooth_le_write_buffer.buffer + bluetooth_le_gatt_read_paramaters.single.offset, data, length);
            LOG_INF("Read %d out of %d bytes in to buffer", length, length);
        }
        /*
        EXAMPLE
        buffer len = 100
        offset = 92
        data size = 15
        */

        //Can only read in data len - data_size bytes
        else if (bluetooth_le_write_buffer.buffer_length > bluetooth_le_gatt_read_paramaters.single.offset) {
            int bytes_to_read = bluetooth_le_write_buffer.buffer_length - bluetooth_le_gatt_read_paramaters.single.offset;
            memcpy(bluetooth_le_write_buffer.buffer + bluetooth_le_gatt_read_paramaters.single.offset, data, bytes_to_read);
            LOG_INF("Read %d out of %d bytes in to buffer", bytes_to_read, length);
        }
        else {
            LOG_INF("Read 0 out of %d bytes into buffer", length);
        }
        
	}

    return BT_GATT_ITER_CONTINUE;
}

static void bluetooth_le_gatt_attribute_write_callback(struct bt_conn *conn, uint8_t err, struct bt_gatt_write_params *params) {
    printk("\n");
    if (err) {
        LOG_ERR("Error while writing to attribute, error code: %d", err);
    }
    else {
        LOG_INF("Wrote data to attribute");
    }
    r_w_in_progress = false;
}

int bluetooth_le_add_attribute(const struct bt_uuid *uuid_p, int handle_p) {
    if (bluetooth_le_attributes.size == bluetooth_le_attributes.capacity) {
        LOG_ERR("Attributes storage is full");
        return 1;
    }

    struct bluetooth_le_attribute temp_attribute = bluetooth_le_get_attribute(uuid_p);
    //Attribute is being added to storage for first time
    if (temp_attribute.handle == BLUETOOTH_LE_ATTRIBUTE_INITIAL_HANDLE) {
        bluetooth_le_attributes.attributes[bluetooth_le_attributes.size].uuid = uuid_p; 
        bluetooth_le_attributes.attributes[bluetooth_le_attributes.size].handle = BLUETOOTH_LE_ATTRIBUTE_ADDED_HANDLE; 
        bluetooth_le_attributes.attributes[bluetooth_le_attributes.size].index = bluetooth_le_attributes.size; 

        bluetooth_le_attributes.size++;
        LOG_INF("Added attribute to storage");
        return 0;
    }
    else {
        //The attribute is already in storage
        if (temp_attribute.handle == BLUETOOTH_LE_ATTRIBUTE_ADDED_HANDLE || temp_attribute.handle > 0) {
            LOG_INF("Attribute is already in storage, updating handle");
            bluetooth_le_attributes.attributes[temp_attribute.index].handle = handle_p;
            return 0;
        }
        //Attribute was not added using the bluetooth_le_add_attribute function initially with the handle_p as BLUETOOTH_LE_ATTRIBUTE_INITIAL_HANDLE
        else {
            LOG_ERR("Attribute was not initially added to storage");
            return 1;
        }
    }
}

struct bluetooth_le_attribute bluetooth_le_get_attribute(const struct bt_uuid *uuid_p) {
    char UUID_buffer[UUID_PRINT_SIZE];
    bt_uuid_to_str(uuid_p, UUID_buffer, UUID_PRINT_SIZE);
    LOG_INF("Looking for attribute with UUID: %s", UUID_buffer);
    for (int i = 0; i < bluetooth_le_attributes.size; i++) {
        if (bt_uuid_cmp(uuid_p, bluetooth_le_attributes.attributes[i].uuid) == 0)  {
            LOG_INF("Matching attribute found");
            return bluetooth_le_attributes.attributes[i];
        }
    }
    LOG_ERR("No matching attribute already in storage found");
    return (struct bluetooth_le_attribute) {.handle = BLUETOOTH_LE_ATTRIBUTE_INITIAL_HANDLE};
}

void bluetooth_le_read_attribute(const struct bt_uuid *uuid, uint8_t *buffer, int buffer_length) {
    printk("\n");

    while (r_w_in_progress == true) {
        LOG_INF("Waiting for mutex to be unlocked to start read...");
        k_msleep(1000);
    }
    r_w_in_progress = true;
    
    int err;

    const struct bluetooth_le_attribute attribute = bluetooth_le_get_attribute(uuid);

    if (attribute.handle <= 0) {
        LOG_ERR("Reading not started as their is no handle for this UUID");
        return;
    }

    if (buffer == NULL || buffer_length <= 0) {
        LOG_ERR("Error with provided buffer or buffer legnth");
        return;
    }
    else {
        bluetooth_le_write_buffer.buffer = buffer;
        bluetooth_le_write_buffer.buffer_length = buffer_length;
    }

    bluetooth_le_gatt_read_paramaters.single.handle = attribute.handle;

    err = bt_gatt_read(bluetooth_le_connection_get(), &bluetooth_le_gatt_read_paramaters);
    if (err) {
        LOG_ERR("Error starting attribute read, error code: %d", err);
    }
    else {
        LOG_INF("Started attribute read");
    }
}

//NEED TO ADD A MUTEX OR SEPHAMORE FOR THE READING AND WRITING SO IT DOESNT READ OR WRITE WHILE A READ OR WRITE IS ALREADY IN PROCESS
void bluetooth_le_write_attribute(const struct bt_uuid *uuid, const uint8_t *buffer, int buffer_length) {
    printk("\n");

    while (r_w_in_progress == true) {
        LOG_INF("Waiting for mutex to be unlocked to start write...");
        k_msleep(1000);
    }
    r_w_in_progress = true;

    int err;

    const struct bluetooth_le_attribute attribute = bluetooth_le_get_attribute(uuid);

    if (attribute.handle <= 0) {
        LOG_ERR("Writing not started as their is no handle for this UUID");
        return;
    }

    bluetooth_le_gatt_write_paramaters.handle = attribute.handle;
    bluetooth_le_gatt_write_paramaters.data = buffer;
    bluetooth_le_gatt_write_paramaters.length = buffer_length;

    err = bt_gatt_write(bluetooth_le_connection_get(), &bluetooth_le_gatt_write_paramaters);
    if (err) {
        LOG_ERR("Error starting attribute write, error code: %d", err);
    }
    else {
        LOG_INF("Started attribute write");
    }
}