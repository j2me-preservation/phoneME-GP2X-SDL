/*
 *   
 *
 * Copyright  1990-2007 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

#include "carddevice.h"

/** 
 * Clears error state.
 */
void jsr177_clear_error() {
}

/** 
 * Sets error state and stores message (like printf).
 * @param fmt printf-like format string
 */
void jsr177_set_error(const char *fmt, ...) {
    (void)fmt;
}

/** 
 * Retrives error message into provided buffer and clears state.
 * @param buf Buffer to store message
 * @param buf_size Size of the buffer in bytes
 * @return PCSL_TRUE if error messages were returned, PCSL_FALSE otherwise
 */
jboolean jsr177_get_error(jbyte *buf, jsize buf_size) {
    (void)buf;
    (void)buf_size;
    return PCSL_FALSE;
}

/*
 * JSR177 Card driver API functions
 */

/** 
 * Initializes the driver.
 * @return JSR177_STATUSCODE_OK if all done successfuly, 
 *         JSR177_STATUSCODE_NOT_IMPLEMENTED when the stub was called
 *         JSR177_STATUSCODE_FAIL otherwise
 */
JSR177_STATUSCODE jsr177_init() {
    return JSR177_STATUSCODE_NOT_IMPLEMENTED;
}

/** 
 * De-initializes the driver.
 * @return JSR177_STATUSCODE_OK is if all done successfuly, JSR177_STATUSCODE_FAIL otherwise
 */
JSR177_STATUSCODE jsr177_finalize() {
    return JSR177_STATUSCODE_OK;
}

/** 
 * Sets property value. If the property is used during the initialization
 * process then this method must be called before <code>jsr177_init()</code>
 * @return JSR177_STATUSCODE_OK if all done successfuly, 
 *         JSR177_STATUSCODE_NOT_IMPLEMENTED when this property is not supported
 *         JSR177_STATUSCODE_OUT_OF_MEMORY if there is no enough memory
 *         JSR177_STATUSCODE_FAIL otherwise
 */
JSR177_STATUSCODE jsr177_set_property(const jbyte *prop_name, 
    const jbyte *prop_value) {
    return JSR177_STATUSCODE_OK;
}

/** 
 * Returns number of slots which available for selection.
 * @param slot_cnt Buffer for number of slots.
 * @return JSR177_STATUSCODE_OK if all done successfuly, JSR177_STATUSCODE_FAIL otherwise
 */
JSR177_STATUSCODE jsr177_get_slot_count(jint *slot_cnt) {
	*slot_cnt = 1;
	return JSR177_STATUSCODE_OK;
}

/** 
 * Checks if this slot is SAT slot.
 * @param slot Slot number.
 * @param result <code>PCSL_TRUE</code> if the slot is dedicated for SAT,
 *               <code>PCSL_FALSE</code> otherwise
 * @return JSR177_STATUSCODE_OK if all done successfuly, JSR177_STATUSCODE_FAIL otherwise
 */
JSR177_STATUSCODE jsr177_is_sat(jint slot, jboolean *result) {
    if (slot == 0) {
        *result = PCSL_TRUE;
    } else {
        *result = PCSL_FALSE;
    }
    return JSR177_STATUSCODE_OK;
}

/** 
 * Selects specified slot (if possible).
 * @return JSR177_STATUSCODE_OK is if all done successfuly, JSR177_STATUSCODE_FAIL otherwise
 */
JSR177_STATUSCODE jsr177_select_slot(jint slot_index) {
    (void)slot_index;
    return JSR177_STATUSCODE_OK;
}

/** 
 * Sends 'RESET' command to device and gets ATR into specified buffer.
 * @param atr Buffer to store ATR.
 * @param atr_size Before call: size of provided buffer
 *                 After call: size of received ATR.
 * @param context a context which can keep information between 
 *                <code>jsr177_reset_start</code> and <code>jsr177_reset_finish</code> calls
 * @retval JSR177_STATUSCODE_OK if all done successfuly. 
 * @retval JSR177_STATUSCODE_WOULD_BLOCK if this call has started asynchronous 
 *         operation. In this case <code>jsr177_reset_finish</code> must be used
 *         for finishing this operation.
 * @retval JSR177_STATUSCODE_FAIL otherwise
 */
JSR177_STATUSCODE jsr177_reset_start(jbyte *atr, jsize *atr_size, void **context) {
    (void)atr;
    (void)atr_size;
    (void)context;
    return JSR177_STATUSCODE_OK;
}

/** 
 * Finishes 'RESET' command on device and gets ATR into specified buffer.
 * @param atr Buffer to store ATR.
 * @param atr_size Before call: size of provided buffer
 *                 After call: size of received ATR.
 * @param context a context which can carry information between 
 *                <code>jsr177_reset_start</code> and <code>jsr177_reset_finish</code> calls
 * @retval JSR177_STATUSCODE_OK if all done successfuly. 
 * @retval JSR177_STATUSCODE_WOULD_BLOCK if the asynchronous operation has not 
 *         completed yet. In this case a new call of 
 *         <code>jsr177_reset_finish</code> must be used for finishing 
 *         this operation.
 * @retval JSR177_STATUSCODE_FAIL otherwise
 */
JSR177_STATUSCODE jsr177_reset_finish(jbyte *atr, jsize *atr_size, void *context) {
    (void)atr;
    (void)atr_size;
    (void)context;
    return JSR177_STATUSCODE_OK;
}


/** 
 * Sends 'POWER DOWN' command to device.
 * @return JSR177_STATUSCODE_OK is if all done successfuly, JSR177_STATUSCODE_FAIL otherwise
 */
JSR177_STATUSCODE jsr177_power_down() {
    return JSR177_STATUSCODE_OK;
}

/** 
 * Performs platform lock of the device. This is intended to make
 * sure that no other native application
 * uses the same device during a transaction.
 * @return JSR177_STATUSCODE_OK if all done successfuly, 
           JSR177_STATUSCODE_WOULD_BLOCK if the device is locked by the other
 *         JSR177_STATUSCODE_FAIL if error occured
 */
JSR177_STATUSCODE jsr177_lock() {
    return JSR177_STATUSCODE_OK;
}

/** 
 * Unlocks the device.
 * @return JSR177_STATUSCODE_OK is if all done successfuly, JSR177_STATUSCODE_FAIL otherwise
 */
JSR177_STATUSCODE jsr177_unlock() {
    return JSR177_STATUSCODE_OK;
}

/** 
 * Retrieves current slot's card movement events from driver.
 * Events is retrieved as bit mask. It can include
 * all movements from last reading, but can contain only the last.
 * Enum JSR177_CARD_MOVEMENT should be used to specify type of movement.
 * Clears the slot event state.
 * @param mask Movements retrived.
 * @return JSR177_STATUSCODE_OK if all done successfuly, JSR177_STATUSCODE_FAIL otherwise.
 */
JSR177_STATUSCODE jsr177_card_movement_events(JSR177_CARD_MOVEMENT *mask) {
    *mask = 0;
    return JSR177_STATUSCODE_OK;
}

/** 
 * Transfers APDU data to the device and receives response from the device.
 * @param tx_buffer Buffer with APDU to be sent.
 * @param tx_size Size of APDU.
 * @param rx_buffer Buffer to store the response.
 * @param rx_size Before call: size of <tt>rx_buffer</tt>
 *                 After call: size of received response.
 * @param context a context which can carry information between 
 *                <code>jsr177_xfer_data_start</code> and <code>jsr177_xfer_data_finish</code> calls
 * @retval JSR177_STATUSCODE_OK if all done successfuly. 
 * @retval JSR177_STATUSCODE_WOULD_BLOCK if this call has started asynchronous 
 *         operation. In this case <code>jsr177_xfer_data_finish</code> must be used
 *         for finishing this operation.
 * @retval JSR177_STATUSCODE_FAIL otherwise
 */
JSR177_STATUSCODE jsr177_xfer_data_start(jbyte *tx_buffer, jsize tx_size,
    jbyte *rx_buffer, jsize *rx_size, void **context) {
    (void)tx_buffer;
    (void)tx_size;
    (void)rx_buffer;
    (void)rx_size;
    (void)context;
    return JSR177_STATUSCODE_OK;
}

/** 
 * Finishes APDU data transfer to the device and receiving the response from the device.
 * @param tx_buffer Buffer with APDU to be sent.
 * @param tx_size Size of APDU.
 * @param rx_buffer Buffer to store the response.
 * @param rx_size Before call: size of <tt>rx_buffer</tt>
 *                 After call: size of received response.
 * @param context a context which can keep information between 
 *                <code>jsr177_xfer_data_start</code> and <code>jsr177_xfer_data_finish</code> calls
 * @retval JSR177_STATUSCODE_OK if all done successfuly. 
 * @retval JSR177_STATUSCODE_WOULD_BLOCK if the asynchronous operation has not 
 *         completed yet. In this case a new call of 
 *         <code>jsr177_xfer_data_finish</code> must be used for finishing 
 *         this operation.
 * @retval JSR177_STATUSCODE_FAIL otherwise
 */
JSR177_STATUSCODE jsr177_xfer_data_finish(jbyte *tx_buffer, jsize tx_size,
    jbyte *rx_buffer, jsize *rx_size, void *context) {
    (void)tx_buffer;
    (void)tx_size;
    (void)rx_buffer;
    (void)rx_size;
    (void)context;
    return JSR177_STATUSCODE_OK;
}

