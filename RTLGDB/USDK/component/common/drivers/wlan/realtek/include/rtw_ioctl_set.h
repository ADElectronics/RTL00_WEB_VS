/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef __RTW_IOCTL_SET_H_
#define __RTW_IOCTL_SET_H_


#ifdef PLATFORM_OS_XP
typedef struct _NDIS_802_11_PMKID {
	uint32_t	Length;
	uint32_t	BSSIDInfoCount;
	BSSIDInfo BSSIDInfo[1];
} NDIS_802_11_PMKID, *PNDIS_802_11_PMKID;
#endif


#ifdef PLATFORM_WINDOWS
typedef uint8_t NDIS_802_11_PMKID_VALUE[16];

typedef struct _BSSIDInfo {
	NDIS_802_11_MAC_ADDRESS  BSSID;
	NDIS_802_11_PMKID_VALUE  PMKID;
} BSSIDInfo, *PBSSIDInfo;

uint8_t rtw_set_802_11_reload_defaults(_adapter * padapter, NDIS_802_11_RELOAD_DEFAULTS reloadDefaults);
uint8_t rtw_set_802_11_test(_adapter * padapter, NDIS_802_11_TEST * test);
uint8_t rtw_set_802_11_pmkid(_adapter *pdapter, NDIS_802_11_PMKID *pmkid);

uint8_t rtw_pnp_set_power_sleep(_adapter* padapter);
uint8_t rtw_pnp_set_power_wakeup(_adapter* padapter);

void rtw_pnp_resume_wk(void *context);
void rtw_pnp_sleep_wk(void * context);

#endif

uint8_t rtw_set_802_11_add_key(_adapter * padapter, NDIS_802_11_KEY * key);
uint8_t rtw_set_802_11_authentication_mode(_adapter *pdapter, NDIS_802_11_AUTHENTICATION_MODE authmode);
uint8_t rtw_set_802_11_bssid(_adapter* padapter, uint8_t *bssid);
uint8_t rtw_set_802_11_add_wep(_adapter * padapter, NDIS_802_11_WEP * wep);
uint8_t rtw_set_802_11_disassociate(_adapter * padapter);
uint8_t rtw_set_802_11_bssid_list_scan(_adapter* padapter, NDIS_802_11_SSID *pssid, int ssid_max_num);
uint8_t rtw_set_802_11_infrastructure_mode(_adapter * padapter, NDIS_802_11_NETWORK_INFRASTRUCTURE networktype);
uint8_t rtw_set_802_11_remove_wep(_adapter * padapter, uint32_t keyindex);
uint8_t rtw_set_802_11_ssid(_adapter * padapter, NDIS_802_11_SSID * ssid);
uint8_t rtw_set_802_11_connect(_adapter* padapter, uint8_t *bssid, NDIS_802_11_SSID *ssid);
uint8_t rtw_set_802_11_remove_key(_adapter * padapter, NDIS_802_11_REMOVE_KEY * key);

uint8_t rtw_validate_bssid(uint8_t *bssid);
uint8_t rtw_validate_ssid(NDIS_802_11_SSID *ssid);

uint16_t rtw_get_cur_max_rate(_adapter *adapter);
//int rtw_set_scan_mode(_adapter *adapter, RT_SCAN_TYPE scan_mode);
int rtw_set_channel_plan(_adapter *adapter, uint8_t channel_plan);
int rtw_set_country(_adapter *adapter, const char *country_code);
//int rtw_set_band(_adapter *adapter, enum _BAND band);

#endif

