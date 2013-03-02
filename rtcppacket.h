/* ZeSense
 * --  Very minimal RTP/RTCP implementation
 *
 * Author: Marco Zavatta
 * <marco.zavatta@telecom-bretagne.eu>
 */

#ifndef ZE_RTCP_PACKET_H
#define ZE_RTCP_PACKET_H



/* For both SR and RR. */
/* Memory layout, sizeof(rtcp_hdr_t)=8bytes
   |76543,2,10| is the first byte:
   rc4,rc3,rc2,rc1,rc0,p0,v1,v0
   version |= 2 sets v1
   version |= 0 resets only v0,v1. It does not touch the others
   rc |= 1 << 4 sets rc4
   rc |= 1 << 5 has no effect
   furthermore:
   rtcp_hdr_t a;
   char *c = (char*) &a;
   c points to the byte |rc,p,v|
   c+1 points to ptype
   etc..
 */
typedef struct {
	unsigned int version:2;	/* always two, 10 */
	unsigned int padding:1;	/* padding */
	unsigned int rc:5;			/* reception report count, a value of zero is valid */
	unsigned char ptype;		/* packet type, sr:200, rr:201 */
	unsigned short length;		/* length in 32-bit words minus one including header and padding */
	unsigned int ssrc;			/* ssrc of the packet originator */
} rtcp_hdr_t;

/* Sender report. */
typedef struct {
	long ntpts;					/* NTP formatted timestamp */
	unsigned int rtpts;		/* RTP timestamp against the ntpts */
	int pcount;					/* sender's packet count */
	int ocount;					/* sender's octet count */
} rtcp_sr_t;

/* Receiver report. */
typedef struct {
	unsigned int ssrcn;		/* source identifier */
	unsigned char fraclost;	/* fraction or RTP data packets lost */
	unsigned int cumlost:24;	/* cumulative fraction or RTP data packets lost */
	unsigned int ehsnr;		/* extended highest sequence number received */
	unsigned int interjitter;	/* estimate of the statistical variance of the RTP packs interarrival time */
	unsigned int lsr;			/* last SR timestamp */
	unsigned int dlsr;			/* delay since last SR packet */
} rtcp_rr_t;

typedef struct {
	unsigned int version:2;	/* always two, 10 */
	unsigned int padding:1;	/* padding */
	unsigned int sc:5;			/* reception report count, a value of zero is valid */
	unsigned char ptype;		/* packet type, sr:200, rr:201 */
	unsigned short length;		/* length in 32-bit words minus one including header and padding */
} rtcp_sdes_t;

typedef struct {
	unsigned char code; /* 1 for CNAME item */
	unsigned char length;
} rtcp_itemhead_t;

typedef struct {
	rtcp_itemhead_t head;
	unsigned char *data;
} rtcp_item_t;

typedef struct {
	unsigned int ssrc;			/* ssrc of the packet originator */
	rtcp_item_t *items;			/* list of items for this chunk */
	int citems;					/* item count */
} rtcp_chunk_t;

typedef struct {
	rtcp_hdr_t *hdr;
	int totlength;
	rtcp_sr_t *sr;
	rtcp_sdes_t *sdes;
	rtcp_itemhead_t *ihdr;
} rtcp_pdu_t;

int rtcp_free_pdu(rtcp_pdu_t *pdu);

rtcp_pdu_t* rtcp_pdu_init();

int rtp_add_sr(rtcp_pdu_t *pdu, rtcp_sr_t *sr); //is always present in a sr

int rtp_add_rr(rtcp_pdu_t *pdu, rtcp_rr_t *rr); //increment hdr's rc and length

rtcp_pdu_t *
build_minimal_sr(char cname[], int cnamel, int ssrc);

#endif
