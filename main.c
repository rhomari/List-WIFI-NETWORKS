#include <winsock2.h>
#include <Wlanapi.h>
#include <windows.h>
#include <iphlpapi.h>
#include <string.h>

#include <stdio.h>

const char * getState(WLAN_INTERFACE_STATE state){
    

   char * retstr = malloc(20* sizeof(char));
    switch(state){
        case  wlan_interface_state_not_ready :
            retstr = "NOT READY";
        break;
        case wlan_interface_state_connected :
            retstr = "CONNECTED";
        break;
        
        case wlan_interface_state_ad_hoc_network_formed :
            retstr ="ADHOC NETWORK FORMED";
        break;
        case wlan_interface_state_disconnecting :
            retstr = "DISCONNECTING";
        break;
        case wlan_interface_state_disconnected :
            retstr = "DISCONNECTED";
        break;
        case wlan_interface_state_associating :
            retstr = "ASSOCIATING";
        break;
         case wlan_interface_state_discovering  :
            retstr = "DISCOVERING";
        break;
         case wlan_interface_state_authenticating  :
            retstr = "AUTHENTICATING";
        break;
        default:
            retstr = "UNKNOWN STATE";
        break;

    }
    return retstr;
    
}


int main(){
    HANDLE clientHandle;
    DWORD clientVersion = GetVersion();
    DWORD negotiatedVersion = 0;
    WLAN_INTERFACE_INFO * wlaninterfaceinfo;
    PWLAN_INTERFACE_INFO_LIST PIIL;
    WCHAR sguid[39]= {0};
    PWLAN_AVAILABLE_NETWORK_LIST PANL;
    PWLAN_AVAILABLE_NETWORK PAN;
    DWORD result = WlanOpenHandle(2,NULL,&negotiatedVersion, &clientHandle);
    if (result == ERROR_SUCCESS){
        printf("We got the handle!\n");

        result = WlanEnumInterfaces(clientHandle,NULL, &PIIL);
        if (result == ERROR_SUCCESS){
            printf("We can enumerate interfaces now!\n");
            wprintf(L"Number of interfaces found : %lu\n", PIIL->dwNumberOfItems);
            for (int i = 0; i< (int)PIIL->dwNumberOfItems; i++){
                wlaninterfaceinfo = (WLAN_INTERFACE_INFO*)&PIIL->InterfaceInfo[i];
                int sfg= StringFromGUID2(&wlaninterfaceinfo->InterfaceGuid, (LPOLESTR)&sguid, sizeof(sguid)/sizeof(*sguid));
                if (sfg != 0){
                    wprintf(L"[%d] -  %ws - %ws - ",i, sguid, wlaninterfaceinfo->strInterfaceDescription);
                    printf("%s\n", getState(wlaninterfaceinfo->isState));
                    result = WlanGetAvailableNetworkList(clientHandle,&wlaninterfaceinfo->InterfaceGuid,0,NULL,&PANL );
                    if (result == ERROR){
                        wprintf(L"---> Networks found : %lu\n", PANL->dwNumberOfItems);
                        for (int j=0 ; j< PANL->dwNumberOfItems; j++){
                            PAN = (WLAN_AVAILABLE_NETWORK * )&PANL->Network[j];
                            wprintf(L"[%d] -  %ws \n", j, PAN->strProfileName);

                        }
                    }
                }
            }

        }


    }
    else{
            printf("Failed, maybe you don't have any wifi interface...");
            return 1;
    }

    
    if (PANL != NULL) {
        WlanFreeMemory(PANL);
        
    }

    if (PIIL != NULL) {
        WlanFreeMemory(PIIL);
        
    }
    return 0;
}