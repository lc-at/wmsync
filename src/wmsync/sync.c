#include <wmsync/api.h>
#include <wmsync/common.h>
#include <wmsync/errors.h>
#include <windows.h>
#include <libhttp/http.h>

int send_login_request(configuration *config_p, char *ip_addr, int *sd);

int start_sync(configuration *config_p) {
    int sd;
    int wsa_init_rv = 0;
    int mac_conv_rv = 0;
    uint8_t mac_addr_buf[6];
    char ip_addr_buf[IP_ADDR_BUF_SZ];

    mac_conv_rv = mac_aton(config_p->client_mac, mac_addr_buf);
    if (mac_conv_rv != 0) {
        log_error("Invalid MAC address. Check your config file.");
        return 1;
    }

    log_info("Initializing synchronizer");
    wsa_init_rv = init_wsa();
    if (wsa_init_rv != 0) {
        log_error("Failed to initialize Winsock (code: %d)", wsa_init_rv);
        return 2;
    }

    log_info("Starting login loop");
    while (true) {
        int send_login_req_retval;
        unsigned long get_ip_retval;
        struct http_message resp;

        memset(&resp, 0, sizeof(resp));

        get_ip_retval =
            get_ip_by_mac(mac_addr_buf, ip_addr_buf, IP_ADDR_BUF_SZ);
        if (get_ip_retval != 0) {
            log_error("Failed to get IP address for the specified MAC address "
                      "(code: %d).",
                      get_ip_retval);
            Sleep(LOGIN_ERROR_TA_INTERVAL);
            continue;
        }

        log_info("Sending a login request for IP address: %s", ip_addr_buf);
        send_login_req_retval = send_login_request(config_p, ip_addr_buf, &sd);
        if (send_login_req_retval == WMSYNC_URL_PARSE_ERROR) {
            log_error("Failed parsing URL, exiting...");
            return 1;
        } else if (send_login_req_retval == WMSYNC_HTTP_CONNECT_ERROR ||
                   send_login_req_retval == WMSYNC_HTTP_SEND_ERROR) {
            log_error("Failed to connect to login URL, trying again...");
            Sleep(LOGIN_ERROR_TA_INTERVAL);
            continue;
        } else {
            while (http_response(sd, &resp) > 0) {
                if (resp.content) {
                    log_info("Received response: %s", resp.content);
                }
            }

            log_info("HTTP response code: %d", resp.header.code);
            if (resp.header.code != 200) {
                log_info("Server didn't respond with 200, trying again...");
                Sleep(LOGIN_ERROR_TA_INTERVAL);
                continue;
            }
        }
        log_info("Sleeping...");
        Sleep(LOGIN_CHECK_INTERVAL);
    }

    return 0;
}

int send_login_request(configuration *config_p, char *ip_addr, int *sd) {
    int retval = 0;
    struct http_url *url;
    char *url_buf;
    char req_buf[1024];
    char *data_buf;
    ssize_t data_buf_sz;
    ssize_t url_buf_sz;

    url_buf_sz = snprintf(
        NULL, 0,
        "%s?ipc=%s&gw_id=%s&mac=%s&redirect=&wlan=%s&landURL=", config_p->url,
        ip_addr, config_p->gw_id, config_p->client_mac, config_p->wlan);
    url_buf = malloc((size_t)url_buf_sz + 1);
    snprintf(
        url_buf, (size_t)url_buf_sz + 1,
        "%s?ipc=%s&gw_id=%s&mac=%s&redirect=&wlan=%s&landURL=", config_p->url,
        ip_addr, config_p->gw_id, config_p->client_mac, config_p->wlan);
    log_trace("%s", url_buf);
    data_buf_sz =
        snprintf(NULL, 0, "autologin_time=86000&username=%s&password=%s",
                 config_p->username, config_p->password);
    data_buf = malloc((size_t)data_buf_sz + 1);
    snprintf(data_buf, (size_t)data_buf_sz + 1,
             "autologin_time=86000&username=%s&password=%s", config_p->username,
             config_p->password);

    if (!(url = http_parse_url(url_buf))) {
        retval = WMSYNC_URL_PARSE_ERROR;
    } else if (!(*sd = http_connect(url))) {
        retval = WMSYNC_HTTP_CONNECT_ERROR;
    } else {
        snprintf(req_buf, sizeof(req_buf),
                 "POST /%s HTTP/1.1\r\n"
                 "User-Agent: Mozilla/5.0 (Linux)\r\n"
                 "Host: %s\r\n"
                 "Accept: */*\r\n"
                 "Referer: https://welcome3.wifi.id/wms/?gw_id=WAG-D7-MTR&client_mac=80:a5:89:50:dc:99&wlan=SBWSBW00016-N/TLK-CI-63700:PEND PARACENDIKIA&sessionid=0A02FFFF78035569-616AFFE0&redirect=http://google.com/\r\n"
                 "X-Requested-With: XMLHTTPRequest\r\n"
                 "Content-Length: %d\r\n"
                 "Connection: close\r\n\r\n"
                 "%s\r\n\r\n",
                 url->query, url->host, (int)strlen(data_buf), data_buf);
        log_trace("%s", req_buf);
        if (http_send(*sd, req_buf)) {
            retval = WMSYNC_HTTP_SEND_ERROR;
        }
    }

    if (url) {
        free(url);
    }
    free(url_buf);
    free(data_buf);

    return retval;
}