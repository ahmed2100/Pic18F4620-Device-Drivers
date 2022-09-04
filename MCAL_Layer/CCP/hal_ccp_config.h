/* 
 * File:   hal_ccp_config.h
 * Author: ahmed
 *
 * Created on August 18, 2022, 12:49 PM
 */

#ifndef HAL_CCP_CONFIG_H
#define	HAL_CCP_CONFIG_H

#define CAPTURE_CONFIG_MODE    0x00U
#define COMPARE_CONFIG_MODE    0x01U
#define PWM_CONFIG_MODE        0x02U

#define CCP1_MODE_SELECTED (CAPTURE_CONFIG_MODE)
#define CCP2_MODE_SELECTED (COMPARE_CONFIG_MODE)

#endif	/* HAL_CCP_CONFIG_H */

