#include "receiver.h"
#include "transmitter.h"
#include "gpio.h"
#include "strings.h"
#include "timer.h"
#include "printf.h"
#include "malloc.h"

#define PAYLOAD_SIZE_BYTES 32

// define macro constants
#define BIT_DELAY_US 416

void main() {
    transmitter_init(2400);
    receiver_init(2400);

    transmission_t trans;
    char* message = "In the earlier epochs of history, we find almost everywhere a complicated arrangement of society into various orders, a manifold gradation of social rank. In ancient Rome we have patricians, knights, plebeians, slaves; in the Middle Ages, feudal lords, vassals, guild-masters, journeymen, apprentices, serfs; in almost all of these classes, again, subordinate gradations. ";
    unsigned int message_length = strlen(message);
    char* buf = malloc(message_length);
    memcpy(buf, message, message_length);

    printf("Starting to format... \n");
    transmitter_format_transmission(buf, message_length, &trans);

    // print packet arr
    unsigned int transmission_packets = trans.packet_arr[0].transmission_packets;
    for (int i = 0; i < transmission_packets; i++) {
        printf("Packet %d:\n", trans.packet_arr[i].packet_number);
        printf("\tNumber of Packets: %d\n", trans.packet_arr[i].transmission_packets);
        printf("\tOriginal Message Size: %d\n", (PAYLOAD_SIZE_BYTES * trans.packet_arr[i].transmission_packets) - trans.packet_arr[i].transmission_padding);
        printf("\tPayload: ");
        for (int j = 0; j < 32; j++) printf("%c", trans.packet_arr[i].payload[j]);
        printf("\n\tChecksum: %d\n\n", trans.packet_arr[i].checksum);
    }

    // receiver_init(2400);
    // char* alphabet = malloc(27);

    // // wait to exit sleep mode
    // receiver_sleep_mode();

    // // start transmission upon exit
    // receiver_start();

    // // read alphabet
    // char SOH = receiver_get_char();
    // unsigned int transmission_size = receiver_get_char();
    // unsigned int packet_numer = receiver_get_char();
    
    // for (int i = 0; i < 26; i++) {
    //     alphabet[i] = receiver_get_char();
    // }

    // // null-terminate alphabet
    // alphabet[26] = 0;
    // printf("Alphabet: %s", alphabet);
}
