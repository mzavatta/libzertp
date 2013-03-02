/* ZeSense
 * --  Very minimal RTP/RTCP implementation
 *
 * Author: Marco Zavatta
 * <marco.zavatta@telecom-bretagne.eu>
 */

#include "rtppacket.h"
#include <arpa/inet.h>

rtp_hdr_t *
rtp_hdr_init() {

	rtp_hdr_t *temp;

	temp = malloc(sizeof(rtp_hdr_t));
	if (temp==NULL) return NULL;

	memset(temp, 0, sizeof(rtp_hdr_t));
	return temp;
}

rtp_pdu_t *
rtp_pdu_init(int cccnt, int extlen, int paylen) {
	/*
	 * differently from the RTP spec, in this function call extlen
	 * contains the 4 byte extension header. Indeed we subtract 4
	 * when we assign it to the rtp_hdrext_t.
	 */

	/*
	 * the real packet to be sent with sendto is in
	 * *hdr and of length totlength
	 */

	if (extlen>0 && extlen < 4) return NULL;
	if (cccnt<0 || extlen<0 || paylen<0) return NULL;

	rtp_pdu_t *temp;
	temp = malloc(sizeof(rtp_pdu_t));
	if (temp == NULL) return NULL;

	temp->totlength = sizeof(rtp_hdr_t)+(cccnt*sizeof(int))
			+extlen+paylen; //extlen includes the 32bit extension header
	temp->hdr = malloc(temp->totlength);
	if (temp->hdr == NULL) return NULL;
	memset(temp->hdr, 0, sizeof(rtp_hdr_t));

	temp->csrc = NULL;
	temp->ext = NULL;

	if (cccnt > 0) {
		temp->csrc = temp->hdr + sizeof(rtp_hdr_t);

		if (extlen>0) {
			temp->ext = temp->csrc + cccnt*sizeof(int);
			temp->ext->length = extlen - 4;
			temp->hdr->extension = 1;
		}
	}
	else {
		if (extlen>0) {
			temp->ext = temp->hdr + sizeof(rtp_hdr_t);
			temp->ext->length = extlen - 4;
			temp->hdr->extension = 1;
		}
	}

	temp->hdr->cccnt = htonl(cccnt); //TODO bitwise
	temp->datalength = paylen;

	temp->hdr->version = 2;

	return temp;
}
