#ifndef WIFI__WIFI_H
#define WIFI__WIFI_H

void wifi_init();
void wifi_deinit();

void wifi_check(const char *wifi_ssid, const char *wifi_password);

#endif // WIFI__WIFI_H