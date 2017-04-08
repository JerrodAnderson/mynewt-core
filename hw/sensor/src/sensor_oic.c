/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "syscfg/syscfg.h"

#if MYNEWT_VAL(SENSOR_BLE_OIC)

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#include "defs/error.h"

#include "os/os.h"

#include "sensor/sensor.h"
#include "sensor/accel.h"
#include "sensor/mag.h"
#include "sensor/light.h"
#include "sensor/quat.h"
#include "sensor/euler.h"
#include "sensor/color.h"
#include "console/console.h"

/* OIC */
#include <oic/oc_rep.h>
#include <oic/oc_ri.h>
#include <oic/oc_api.h>

static int
sensor_oic_encode(struct sensor* sensor, void *arg, void *databuf)
{
    int rc;

    rc = SYS_EINVAL;
    switch(SENSOR_TYPE_ALL & (*(uint32_t *)arg)) {
        /* Gyroscope supported */
        case SENSOR_TYPE_GYROSCOPE:
        /* Accelerometer functionality supported */
        case SENSOR_TYPE_ACCELEROMETER:
        /* Linear Accelerometer (Without Gravity) */
        case SENSOR_TYPE_LINEAR_ACCEL:
        /* Gravity Sensor */
        case SENSOR_TYPE_GRAVITY:

            if (((struct sensor_accel_data *)(databuf))->sad_x_is_valid) {
                oc_rep_set_double(root, x,
                    ((struct sensor_accel_data *)(databuf))->sad_x);
            } else {
                goto err;
            }
            if (((struct sensor_accel_data *)(databuf))->sad_y_is_valid) {
                oc_rep_set_double(root, y,
                    ((struct sensor_accel_data *)(databuf))->sad_y);
            } else {
                goto err;
            }
            if (((struct sensor_accel_data *)(databuf))->sad_z_is_valid) {
                oc_rep_set_double(root, z,
                    ((struct sensor_accel_data *)(databuf))->sad_z);
            } else {
                goto err;
            }
            break;

        /* Magnetic field supported */
        case SENSOR_TYPE_MAGNETIC_FIELD:
            if (((struct sensor_mag_data *)(databuf))->smd_x_is_valid) {
                oc_rep_set_double(root, x,
                    ((struct sensor_mag_data *)(databuf))->smd_x);
            } else {
                goto err;
            }
            if (((struct sensor_mag_data *)(databuf))->smd_y_is_valid) {
                oc_rep_set_double(root, y,
                    ((struct sensor_mag_data *)(databuf))->smd_y);
            } else {
                goto err;
            }
            if (((struct sensor_mag_data *)(databuf))->smd_z_is_valid) {
                oc_rep_set_double(root, z,
                    ((struct sensor_mag_data *)(databuf))->smd_z);
            } else {
                goto err;
            }
            break;
        /* Light supported */
        case SENSOR_TYPE_LIGHT:
            if (((struct sensor_light_data *)(databuf))->sld_ir_is_valid) {
                oc_rep_set_double(root, ir,
                    ((struct sensor_light_data *)(databuf))->sld_ir);
            } else {
                goto err;
            }
            if (((struct sensor_light_data *)(databuf))->sld_full_is_valid) {
                oc_rep_set_double(root, full,
                    ((struct sensor_light_data *)(databuf))->sld_full);
            } else {
                goto err;
            }
            if (((struct sensor_light_data *)(databuf))->sld_lux_is_valid) {
                oc_rep_set_double(root, lux,
                    ((struct sensor_light_data *)(databuf))->sld_lux);
            } else {
                goto err;
            }
            break;

        /* Temperature supported */
        case SENSOR_TYPE_TEMPERATURE:
            oc_rep_set_double(root, temp, *(double *)databuf);
            break;

        /* Ambient temperature supported */
        case SENSOR_TYPE_AMBIENT_TEMPERATURE:
            oc_rep_set_double(root, ambient_temp, *(double *)databuf);
            break;

#if 0
        /* Pressure sensor supported */
        SENSOR_TYPE_PRESSURE:
            oc_rep_set_double(root, pressure, (double *)databuf);

        /* Proximity sensor supported */
        SENSOR_TYPE_PROXIMITY:

        /* Relative humidity supported */
        SENSOR_TYPE_RELATIVE_HUMIDITY:
#endif
        /* Rotation vector (quaternion) supported */
        case SENSOR_TYPE_ROTATION_VECTOR:
            if (((struct sensor_quat_data *)(databuf))->sqd_x_is_valid) {
                oc_rep_set_double(root, x,
                    ((struct sensor_quat_data *)(databuf))->sqd_x);
            } else {
                goto err;
            }
            if (((struct sensor_quat_data *)(databuf))->sqd_y_is_valid) {
                oc_rep_set_double(root, y,
                    ((struct sensor_quat_data *)(databuf))->sqd_y);
            } else {
                goto err;
            }
            if (((struct sensor_quat_data *)(databuf))->sqd_z_is_valid) {
                oc_rep_set_double(root, z,
                    ((struct sensor_quat_data *)(databuf))->sqd_z);
            } else {
                goto err;
            }
            if (((struct sensor_quat_data *)(databuf))->sqd_w_is_valid) {
                oc_rep_set_double(root, w,
                    ((struct sensor_quat_data *)(databuf))->sqd_w);
            } else {
                goto err;
            }
            break;
#if 0
        /* Altitude Supported */
        SENSOR_TYPE_ALTITUDE:

        /* Weight Supported */
        SENSOR_TYPE_WEIGHT:
#endif
        /* Euler Orientation Sensor */
        case SENSOR_TYPE_EULER:
            if (((struct sensor_euler_data *)(databuf))->sed_h_is_valid) {
                oc_rep_set_double(root, h,
                    ((struct sensor_euler_data *)(databuf))->sed_h);
            } else {
                goto err;
            }
            if (((struct sensor_euler_data *)(databuf))->sed_r_is_valid) {
                oc_rep_set_double(root, r,
                    ((struct sensor_euler_data *)(databuf))->sed_r);
            } else {
                goto err;
            }
            if (((struct sensor_euler_data *)(databuf))->sed_p_is_valid) {
                oc_rep_set_double(root, p,
                    ((struct sensor_euler_data *)(databuf))->sed_p);
            } else {
                goto err;
            }
            break;

        /* Color Sensor */
        case SENSOR_TYPE_COLOR:
            if (((struct sensor_color_data *)(databuf))->scd_r_is_valid) {
                oc_rep_set_uint(root, r,
                    ((struct sensor_color_data *)(databuf))->scd_r);
            } else {
                goto err;
            }
            if (((struct sensor_color_data *)(databuf))->scd_g_is_valid) {
                oc_rep_set_uint(root, g,
                    ((struct sensor_color_data *)(databuf))->scd_g);
            } else {
                goto err;
            }
            if (((struct sensor_color_data *)(databuf))->scd_b_is_valid) {
                oc_rep_set_uint(root, b,
                    ((struct sensor_color_data *)(databuf))->scd_b);
            } else {
                goto err;
            }
            if (((struct sensor_color_data *)(databuf))->scd_lux_is_valid) {
                oc_rep_set_uint(root, lux,
                    ((struct sensor_color_data *)(databuf))->scd_lux);
            } else {
                goto err;
            }
            if (((struct sensor_color_data *)(databuf))->scd_colortemp_is_valid) {
                oc_rep_set_uint(root, colortemp,
                    ((struct sensor_color_data *)(databuf))->scd_colortemp);
            } else {
                goto err;
            }
            if (((struct sensor_color_data *)(databuf))->scd_saturation_is_valid) {
                oc_rep_set_uint(root, saturation,
                    ((struct sensor_color_data *)(databuf))->scd_saturation);
            } else {
                goto err;
            }
            if (((struct sensor_color_data *)(databuf))->scd_saturation75_is_valid) {
                oc_rep_set_uint(root, saturation75,
                    ((struct sensor_color_data *)(databuf))->scd_saturation75);
            } else {
                goto err;
            }
            if (((struct sensor_color_data *)(databuf))->scd_is_sat_is_valid) {
                oc_rep_set_uint(root, is_sat,
                    ((struct sensor_color_data *)(databuf))->scd_is_sat);
            } else {
                goto err;
            }
            if (((struct sensor_color_data *)(databuf))->scd_cratio_is_valid) {
                oc_rep_set_double(root, cratio,
                    ((struct sensor_color_data *)(databuf))->scd_cratio);
            } else {
                goto err;
            }
            if (((struct sensor_color_data *)(databuf))->scd_maxlux_is_valid) {
                oc_rep_set_uint(root, maxlux,
                    ((struct sensor_color_data *)(databuf))->scd_maxlux);
            } else {
                goto err;
            }
            if (((struct sensor_color_data *)(databuf))->scd_ir_is_valid) {
                oc_rep_set_uint(root, ir,
                    ((struct sensor_color_data *)(databuf))->scd_ir);
            } else {
                goto err;
            }
            break;

        /* User defined sensor type 1 */
        case SENSOR_TYPE_USER_DEFINED_1:
        /* User defined sensor type 2 */
        case SENSOR_TYPE_USER_DEFINED_2:
            break;
        /* None */
        case SENSOR_TYPE_NONE:
            goto err;
    }

    oc_rep_set_uint(root, ts_secs, (long int)sensor->s_sts.st_ostv.tv_sec);
    oc_rep_set_int(root, ts_usecs, (int)sensor->s_sts.st_ostv.tv_usec);
    oc_rep_set_uint(root, ts_cputime, (unsigned int)sensor->s_sts.st_cputime);

    return 0;
err:
    return rc;
}

static void
sensor_oic_get_data(oc_request_t *request, oc_interface_mask_t interface)
{
    int rc;
    struct sensor *sensor;
    struct sensor_listener listener;

    /* Look up sensor by name */
    sensor = sensor_mgr_find_next_bydevname("color0", NULL);
    if (!sensor) {
        console_printf("Sensor %s not found!\n", "color0");
    }

    oc_rep_start_root_object();

    switch (interface) {
    case OC_IF_BASELINE:
        oc_process_baseline_interface(request->resource);
    case OC_IF_R:
        /* Register a listener and then trigger/read a bunch of samples */
        memset(&listener, 0, sizeof(listener));

        listener.sl_sensor_type = sensor->s_types & SENSOR_TYPE_COLOR;
        listener.sl_func = sensor_oic_encode;
        listener.sl_arg = (void *)&listener.sl_sensor_type;

        rc = sensor_register_listener(sensor, &listener);
        if (rc) {
            goto err;
        }

        /* Sensor read results.  Every time a sensor is read, all of its
         * listeners are called by default.  Specify NULL as a callback,
         * because we just want to run all the listeners.
         */
        rc = sensor_read(sensor, listener.sl_sensor_type, NULL, NULL,
                         OS_TIMEOUT_NEVER);
        if (rc) {
            goto err;
        }

        break;
    default:
        break;
    }

    sensor_unregister_listener(sensor, &listener);
    oc_rep_end_root_object();
    oc_send_response(request, OC_STATUS_OK);
    return;
err:
    sensor_unregister_listener(sensor, &listener);
    oc_send_response(request, OC_STATUS_NOT_FOUND);
}

void
sensor_oic_init(void)
{
    oc_resource_t *res;

    res = oc_new_resource("/color0", 1, 0);
    oc_resource_bind_resource_type(res, "sensors.r.color0");
    oc_resource_bind_resource_interface(res, OC_IF_R);
    oc_resource_set_default_interface(res, OC_IF_R);

    oc_resource_set_discoverable(res);
    oc_resource_set_periodic_observable(res, 1);
    oc_resource_set_request_handler(res, OC_GET, sensor_oic_get_data);
    oc_add_resource(res);
}

#endif
