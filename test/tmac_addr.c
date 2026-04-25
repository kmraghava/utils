/* test_mac_addr.c
 *
 * Comprehensive unit tests for mac_addr.c
 * Compile: cd cmblib/utils; gcc -Iinclude src/mac_addr.c/ test/tmac_addr.c -o build/test/tmac_addr
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "kmrUtils/mac_addr.h"

// Prototypes (from your code)
const char* mac_addr_str(mac_addr_t addr, char *buffer_p, mac_addr_fmt_separator_e separator);
int mac_addr_parse(const char *saddr_p, mac_addr_t addr);
int mac_addr_valid(const char *saddr_p);

void test_mac_addr_valid(void) {
    // Valid, no separator
    assert(mac_addr_valid("aabbccddeeff") == 0);
    // Valid, colon
    assert(mac_addr_valid("aa:bb:cc:dd:ee:ff") == 0);
    // Valid, hyphen
    assert(mac_addr_valid("aa-bb-cc-dd-ee-ff") == 0);

    // Invalid length
    assert(mac_addr_valid("aabbcc") == 1);
    // Invalid character
    assert(mac_addr_valid("aa:bb:cc:dd:ee:zz") == 1);
    // Mixed separators
    assert(mac_addr_valid("aa:bb-cc:dd:ee:ff") == 1);
}

void test_mac_addr_parse(void) {
    mac_addr_t mac;

    // Colon format
    assert(mac_addr_parse("aa:bb:cc:dd:ee:ff", mac) == 0);
    assert(mac[0] == 0xaa && mac[5] == 0xff);

    // Hyphen format
    assert(mac_addr_parse("11-22-33-44-55-66", mac) == 0);
    assert(mac[0] == 0x11 && mac[5] == 0x66);

    // No separator
    assert(mac_addr_parse("112233445566", mac) == 0);
    assert(mac[0] == 0x11 && mac[5] == 0x66);

    // Invalid input
    assert(mac_addr_parse("xx:yy:zz:dd:ee:ff", mac) == 1);
}

void test_mac_addr_str(void) {
    mac_addr_t mac = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    char buf[32];

    // Colon
    assert(strcmp(mac_addr_str(mac, buf, MAC_ADDR_FMT_SEPARATOR_COLON), "aa:bb:cc:dd:ee:ff") == 0);

    // Hyphen
    assert(strcmp(mac_addr_str(mac, buf, MAC_ADDR_FMT_SEPARATOR_HYPHEN), "aa-bb-cc-dd-ee-ff") == 0);

    // None
    assert(strcmp(mac_addr_str(mac, buf, MAC_ADDR_FMT_SEPARATOR_NONE), "aabbccddeeff") == 0);

    // Null buffer → expect NULL
    assert(mac_addr_str(mac, NULL, MAC_ADDR_FMT_SEPARATOR_COLON) == NULL);

    // Invalid separator enum → expect NULL
    assert(mac_addr_str(mac, buf, 99) == NULL);
}

void test_mac_addr (void) {
    test_mac_addr_valid();
    test_mac_addr_parse();
    test_mac_addr_str();

    printf("All MAC address tests passed!\n");
}

