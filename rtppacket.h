/* ZeSense
 * --  Very minimal RTP/RTCP implementation
 *
 * Author: Marco Zavatta
 * <marco.zavatta@telecom-bretagne.eu>
 */

#ifndef ZE_RTP_PACKET_H
#define ZE_RTP_PACKET_H

#include <stdlib.h>
#include "debug.h"

typedef struct {
	unsigned int version:2;	/* always two, 10 */
	unsigned int padding:1;	/* payload padding octects */
	unsigned int extension:1;	/* header extension presence flag */
	unsigned int cccnt:4;	    /* csrc count, 0-15, used to count *csrc's 32.-bit words */
	unsigned int marker:1;		/* marker, profile defined semantics */
	unsigned int ptype:7;		/* payload type, profile specific semantics */
	unsigned short seqn;		/* sequence number */
	unsigned int timestamp;	/* timestamp */
	unsigned int ssrc;			/* synchronization source */
} rtp_hdr_t;

typedef struct {
	unsigned short dbp;
	unsigned short length;
	unsigned char *data;
} rtp_hdrext_t;

typedef struct {
	rtp_hdr_t *hdr;
	int totlength;
	unsigned int *csrc;		/* contributing sources, 32-bit words, list of 0 to 15 elements */
	rtp_hdrext_t *ext;			/* header extension */
	unsigned char *data;		/* payload */
	int datalength;
} rtp_pdu_t;


typedef struct {
	int sensor;
} rtp_payload_hdr_t;

int rtp_free_pdu(rtp_pdu_t *pdu);

/*
 * Have to minimize memory copy (we're streaming, many send operations)
 * Allocate the right amount of memory to hold the entire packet
 * right from the start to the *hdr pointer of the rtp_pdu_t.
 */
rtp_pdu_t* rtp_pdu_init(int cccnt, int extlen, int paylen);

rtp_hdr_t* rtp_hdr_init();

/*
 * Add the field in the proper position in *hdr and update
 * the various length fields inside the structures
 */
int rtp_add_csrc(rtp_pdu_t *pdu, int csrc);

/*
 * Add the field in the proper position in *hdr and update
 * the various length fields inside the structure
 */
int rtp_add_data(rtp_pdu_t *pdu, int datalength, unsigned char *data);

#endif
