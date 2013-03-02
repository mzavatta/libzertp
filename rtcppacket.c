/* ZeSense
 * --  Very minimal RTP/RTCP implementation
 *
 * Author: Marco Zavatta
 * <marco.zavatta@telecom-bretagne.eu>
 */

#include <stdlib.h>
#include "rtcppacket.h"
#include <arpa/inet.h>
#include "debug.h"

rtcp_pdu_t *
build_minimal_sr(char cname[], int cnamel, int ssrc) {

	int padding = (cnamel+2)%4;

	/* In bytes.. */
	int srlength = sizeof(rtcp_hdr_t)+sizeof(rtcp_sr_t);
	int sdeslength =
			sizeof(rtcp_sdes_t)+
			sizeof(int)+
			sizeof(rtcp_itemhead_t)+
			cnamel+padding;

	rtcp_hdr_t *chunk = malloc(srlength+sdeslength);
	if (chunk == NULL) return NULL;
	memset(chunk, 0, srlength+sdeslength);

	/* Create the pdu and assign the data chunk to it. */
	rtcp_pdu_t *rtcpdu = malloc(sizeof(rtcp_pdu_t));
	if (rtcpdu==NULL) return NULL;
	rtcpdu->hdr = chunk;
	rtcpdu->totlength = srlength+sdeslength;

	/* SR Header. */
	chunk->version = 2;
	chunk->padding = 0;
	chunk->rc = 0;
	chunk->ptype = 200;
	chunk->length = htons(-1 + srlength/4);
	chunk->ssrc = htonl(ssrc);

	rtcpdu->sr = (rtcp_sr_t*)(chunk + sizeof(rtcp_sr_t));

	/* SDES header. */
	rtcp_sdes_t *sdes = (rtcp_sdes_t*)(chunk + srlength);
	rtcpdu->sdes = sdes;
	sdes->version = 2;
	sdes->padding = 0; //not related to packet content
	sdes->sc = 1;
	sdes->length = htons(-1 + sdeslength/4);

	/* SDES chunk header. */
	int *pssrc = (int*)(sdes+sizeof(rtcp_sdes_t));
	*pssrc = htonl(ssrc);

	/* SDES CNAME item header. */
	rtcp_itemhead_t *item = (rtcp_itemhead_t*)(ssrc+sizeof(int));
	rtcpdu->ihdr = item;
	item->code = 1;
	item->length = cnamel+padding;

	/* CNAME content. */
	char *pcname = (char*)(item+sizeof(rtcp_itemhead_t));
	memcpy(pcname, cname, cnamel);

	return rtcpdu;
}
