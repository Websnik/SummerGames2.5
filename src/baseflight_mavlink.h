#include "v1.0/matrixpilot/mavlink.h"
#include "v1.0/common/common.h"
#include "v1.0/ardupilotmega/mavlink_msg_radio.h"

enum {
	MAV_CMD_NAV_WAYPOINT=16, /* Navigate to MISSION. |Hold time in decimal seconds. (ignored by fixed wing, time to stay at MISSION for rotary wing)| Acceptance radius in meters (if the sphere with this radius is hit, the MISSION counts as reached)| 0 to pass through the WP, if > 0 radius in meters to pass by WP. Positive value for clockwise orbit, negative value for counter-clockwise orbit. Allows trajectory control.| Desired yaw angle at MISSION (rotary wing)| Latitude| Longitude| Altitude|  */
	MAV_CMD_NAV_LOITER_UNLIM=17, /* Loiter around this MISSION an unlimited amount of time |Empty| Empty| Radius around MISSION, in meters. If positive loiter clockwise, else counter-clockwise| Desired yaw angle.| Latitude| Longitude| Altitude|  */
	MAV_CMD_NAV_LOITER_TURNS=18, /* Loiter around this MISSION for X turns |Turns| Empty| Radius around MISSION, in meters. If positive loiter clockwise, else counter-clockwise| Desired yaw angle.| Latitude| Longitude| Altitude|  */
	MAV_CMD_NAV_LOITER_TIME=19, /* Loiter around this MISSION for X seconds |Seconds (decimal)| Empty| Radius around MISSION, in meters. If positive loiter clockwise, else counter-clockwise| Desired yaw angle.| Latitude| Longitude| Altitude|  */
	MAV_CMD_NAV_RETURN_TO_LAUNCH=20, /* Return to launch location |Empty| Empty| Empty| Empty| Empty| Empty| Empty|  */
	MAV_CMD_NAV_LAND=21, /* Land at location |Empty| Empty| Empty| Desired yaw angle.| Latitude| Longitude| Altitude|  */
	MAV_CMD_NAV_TAKEOFF=22, /* Takeoff from ground / hand |Minimum pitch (if airspeed sensor present), desired pitch without sensor| Empty| Empty| Yaw angle (if magnetometer present), ignored without magnetometer| Latitude| Longitude| Altitude|  */
	MAV_CMD_NAV_ROI=80, /* Sets the region of interest (ROI) for a sensor set or the vehicle itself. This can then be used by the vehicles control system to control the vehicle attitude and the attitude of various sensors such as cameras. |Region of intereset mode. (see MAV_ROI enum)| MISSION index/ target ID. (see MAV_ROI enum)| ROI index (allows a vehicle to manage multiple ROI's)| Empty| x the location of the fixed ROI (see MAV_FRAME)| y| z|  */
	MAV_CMD_NAV_PATHPLANNING=81, /* Control autonomous path planning on the MAV. |0: Disable local obstacle avoidance / local path planning (without resetting map), 1: Enable local path planning, 2: Enable and reset local path planning| 0: Disable full path planning (without resetting map), 1: Enable, 2: Enable and reset map/occupancy grid, 3: Enable and reset planned route, but not occupancy grid| Empty| Yaw angle at goal, in compass degrees, [0..360]| Latitude/X of goal| Longitude/Y of goal| Altitude/Z of goal|  */
	MAV_CMD_NAV_LAST=95, /* NOP - This command is only used to mark the upper limit of the NAV/ACTION commands in the enumeration |Empty| Empty| Empty| Empty| Empty| Empty| Empty|  */
	MAV_CMD_CONDITION_DELAY=112, /* Delay mission state machine. |Delay in seconds (decimal)| Empty| Empty| Empty| Empty| Empty| Empty|  */
	MAV_CMD_CONDITION_CHANGE_ALT=113, /* Ascend/descend at rate.  Delay mission state machine until desired altitude reached. |Descent / Ascend rate (m/s)| Empty| Empty| Empty| Empty| Empty| Finish Altitude|  */
	MAV_CMD_CONDITION_DISTANCE=114, /* Delay mission state machine until within desired distance of next NAV point. |Distance (meters)| Empty| Empty| Empty| Empty| Empty| Empty|  */
	MAV_CMD_CONDITION_YAW=115, /* Reach a certain target angle. |target angle: [0-360], 0 is north| speed during yaw change:[deg per second]| direction: negative: counter clockwise, positive: clockwise [-1,1]| relative offset or absolute angle: [ 1,0]| Empty| Empty| Empty|  */
	MAV_CMD_CONDITION_LAST=159, /* NOP - This command is only used to mark the upper limit of the CONDITION commands in the enumeration |Empty| Empty| Empty| Empty| Empty| Empty| Empty|  */
	MAV_CMD_DO_SET_MODE=176, /* Set system mode. |Mode, as defined by ENUM MAV_MODE| Empty| Empty| Empty| Empty| Empty| Empty|  */
	MAV_CMD_DO_JUMP=177, /* Jump to the desired command in the mission list.  Repeat this action only the specified number of times |Sequence number| Repeat count| Empty| Empty| Empty| Empty| Empty|  */
	MAV_CMD_DO_CHANGE_SPEED=178, /* Change speed and/or throttle set points. |Speed type (0=Airspeed, 1=Ground Speed)| Speed  (m/s, -1 indicates no change)| Throttle  ( Percent, -1 indicates no change)| Empty| Empty| Empty| Empty|  */
	MAV_CMD_DO_SET_HOME=179, /* Changes the home location either to the current location or a specified location. |Use current (1=use current location, 0=use specified location)| Empty| Empty| Empty| Latitude| Longitude| Altitude|  */
	MAV_CMD_DO_SET_PARAMETER=180, /* Set a system parameter.  Caution!  Use of this command requires knowledge of the numeric enumeration value of the parameter. |Parameter number| Parameter value| Empty| Empty| Empty| Empty| Empty|  */
	MAV_CMD_DO_SET_RELAY=181, /* Set a relay to a condition. |Relay number| Setting (1=on, 0=off, others possible depending on system hardware)| Empty| Empty| Empty| Empty| Empty|  */
	MAV_CMD_DO_REPEAT_RELAY=182, /* Cycle a relay on and off for a desired number of cyles with a desired period. |Relay number| Cycle count| Cycle time (seconds, decimal)| Empty| Empty| Empty| Empty|  */
	MAV_CMD_DO_SET_SERVO=183, /* Set a servo to a desired PWM value. |Servo number| PWM (microseconds, 1000 to 2000 typical)| Empty| Empty| Empty| Empty| Empty|  */
	MAV_CMD_DO_REPEAT_SERVO=184, /* Cycle a between its nominal setting and a desired PWM for a desired number of cycles with a desired period. |Servo number| PWM (microseconds, 1000 to 2000 typical)| Cycle count| Cycle time (seconds)| Empty| Empty| Empty|  */
	MAV_CMD_DO_CONTROL_VIDEO=200, /* Control onboard camera system. |Camera ID (-1 for all)| Transmission: 0: disabled, 1: enabled compressed, 2: enabled raw| Transmission mode: 0: video stream, >0: single images every n seconds (decimal)| Recording: 0: disabled, 1: enabled compressed, 2: enabled raw| Empty| Empty| Empty|  */
	MAV_CMD_DO_DIGICAM_CONFIGURE=202, /* Mission command to configure an on-board camera controller system. |Modes: P, TV, AV, M, Etc| Shutter speed: Divisor number for one second| Aperture: F stop number| ISO number e.g. 80, 100, 200, Etc| Exposure type enumerator| Command Identity| Main engine cut-off time before camera trigger in seconds/10 (0 means no cut-off)|  */
	MAV_CMD_DO_DIGICAM_CONTROL=203, /* Mission command to control an on-board camera controller system. |Session control e.g. show/hide lens| Zoom's absolute position| Zooming step value to offset zoom from the current position| Focus Locking, Unlocking or Re-locking| Shooting Command| Command Identity| Empty|  */
	MAV_CMD_DO_MOUNT_CONFIGURE=204, /* Mission command to configure a camera or antenna mount |Mount operation mode (see MAV_MOUNT_MODE enum)| stabilize roll? (1 = yes, 0 = no)| stabilize pitch? (1 = yes, 0 = no)| stabilize yaw? (1 = yes, 0 = no)| Empty| Empty| Empty|  */
	MAV_CMD_DO_MOUNT_CONTROL=205, /* Mission command to control a camera or antenna mount |pitch(deg*100) or lat, depending on mount mode.| roll(deg*100) or lon depending on mount mode| yaw(deg*100) or alt (in cm) depending on mount mode| Empty| Empty| Empty| Empty|  */
	MAV_CMD_DO_LAST=240, /* NOP - This command is only used to mark the upper limit of the DO commands in the enumeration |Empty| Empty| Empty| Empty| Empty| Empty| Empty|  */
	MAV_CMD_PREFLIGHT_CALIBRATION=241, /* Trigger calibration. This command will be only accepted if in pre-flight mode. |Gyro calibration: 0: no, 1: yes| Magnetometer calibration: 0: no, 1: yes| Ground pressure: 0: no, 1: yes| Radio calibration: 0: no, 1: yes| Empty| Empty| Empty|  */
	MAV_CMD_PREFLIGHT_SET_SENSOR_OFFSETS=242, /* Set sensor offsets. This command will be only accepted if in pre-flight mode. |Sensor to adjust the offsets for: 0: gyros, 1: accelerometer, 2: magnetometer, 3: barometer, 4: optical flow| X axis offset (or generic dimension 1), in the sensor's raw units| Y axis offset (or generic dimension 2), in the sensor's raw units| Z axis offset (or generic dimension 3), in the sensor's raw units| Generic dimension 4, in the sensor's raw units| Generic dimension 5, in the sensor's raw units| Generic dimension 6, in the sensor's raw units|  */
	MAV_CMD_PREFLIGHT_STORAGE=245, /* Request storage of different parameter values and logs. This command will be only accepted if in pre-flight mode. |Parameter storage: 0: READ FROM FLASH/EEPROM, 1: WRITE CURRENT TO FLASH/EEPROM| Mission storage: 0: READ FROM FLASH/EEPROM, 1: WRITE CURRENT TO FLASH/EEPROM| Reserved| Reserved| Empty| Empty| Empty|  */
	MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN=246, /* Request the reboot or shutdown of system components. |0: Do nothing for autopilot, 1: Reboot autopilot, 2: Shutdown autopilot.| 0: Do nothing for onboard computer, 1: Reboot onboard computer, 2: Shutdown onboard computer.| Reserved| Reserved| Empty| Empty| Empty|  */
	MAV_CMD_OVERRIDE_GOTO=252, /* Hold / continue the current action |MAV_GOTO_DO_HOLD: hold MAV_GOTO_DO_CONTINUE: continue with next item in mission plan| MAV_GOTO_HOLD_AT_CURRENT_POSITION: Hold at current position MAV_GOTO_HOLD_AT_SPECIFIED_POSITION: hold at specified position| MAV_FRAME coordinate frame of hold point| Desired yaw angle in degrees| Latitude / X position| Longitude / Y position| Altitude / Z position|  */
	MAV_CMD_MISSION_START=300, /* start running a mission |first_item: the first mission item to run| last_item:  the last mission item to run (after this item is run, the mission ends)|  */
	MAV_CMD_COMPONENT_ARM_DISARM=400, /* Arms / Disarms a component |1 to arm, 0 to disarm|  */
	MAV_CMD_ENUM_END=401, /*  | */
};

void     reset_mavlink(void);
bool     baseflight_mavlink_receive(char new);
void     baseflight_mavlink_send_message (mavlink_message_t* msg);
void     baseflight_mavlink_handleMessage (mavlink_message_t *msg);
void     baseflight_mavlink_send_updates(void);
bool     baseflight_mavlink_send_paramlist(bool Reset);
void     baseflight_mavlink_send_singleparam(int16_t Nr);
bool     baseflight_mavlink_set_param (mavlink_param_set_t *packet);
bool     baseflight_mavlink_send_1Hzheartbeat(void);

//void stair_mavlink_send_message (mavlink_message_t* msg);
//void stair_mavlink_receive (char new);
//void stair_mavlink_handleMessage (mavlink_message_t* msg);
//void stair_mavlink_send_updates (void);
//void stair_mavlink_set_param (mavlink_param_set_t *packet);
//uint8_t stair_mavlink_send_param (uint8_t num);
//void baseflight_mavlink_set_param (mavlink_param_set_t *packet);
//uint8_t baseflight_mavlink_send_param (uint8_t num);
