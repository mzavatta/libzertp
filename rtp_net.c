/* ZeSense
 * --  Very minimal RTP implementation
 *
 * Credits to libcoap
 * Copyright (C) 2010,2011 Olaf Bergmann <bergmann@tzi.org>
 *
 * Author: Marco Zavatta
 * <marco.zavatta@telecom-bretagne.eu>
 */

/*
 * Send the pdu. Returns 0 on success.
 */

#include <netinet/in.h>
#include <stdlib.h>
#include "rtp_net.h"

int
rtp_send_impl(rtp_context_t *context,
	       const rtp_address_t *dst,
	       rtp_pdu_t *pdu) {

	LOGI("Entering rtp_send_impl");

	printpacket((unsigned char *)(pdu->hdr), pdu->totlength);

	ssize_t bytes_written;

	if ( !context || !dst || !pdu )
		return -1;

	bytes_written = sendto( context->sockfd, pdu->hdr, pdu->totlength, 0,
			&dst->addr.sa, dst->size);

	if (bytes_written >= 0) return 0;

	return -1;
}

int
rtcp_send_impl(rtp_context_t *context,
	       const rtp_address_t *dst,
	       rtcp_pdu_t *pdu) {

	LOGI("Entering rtp_send_impl");

	printpacket((unsigned char *)(pdu->hdr), pdu->totlength);

	ssize_t bytes_written;

	if ( !context || !dst || !pdu )
		return -1;

	bytes_written = sendto( context->sockfd, pdu->hdr, pdu->totlength, 0,
			&dst->addr.sa, dst->size);

	if (bytes_written >= 0) return 0;

	return -1;
}

rtp_stream_t *
rtp_stream_init(rtp_hdr_t *hdr) {

	rtp_stream_t *stream;

	stream = malloc(sizeof(rtp_stream_t));
	if (stream == NULL) return NULL;
	memset(stream, 0, sizeof(rtp_stream_t));

	stream->invalid = 0;
	stream->octectcount = 0;
	stream->packetcount = 0;
	stream->next = NULL;
	stream->ntptwin = 0;
	stream->rtptwin = 0;
	stream->last_rtcp_octectcount = 0;
	stream->bandwidth = 0;

	return stream;
}

void printpacket(unsigned char *data, int length) {
	int i;
	for(i=0;i<length;i++) {
		printf("%x,", *(data+i));
	}
}

rtp_context_t *
rtp_new_context(const rtp_address_t *listen_addr) {

  rtp_context_t *c = malloc( sizeof( rtp_context_t ) );
  if ( !c ) return NULL;
  memset(c, 0, sizeof( rtp_context_t ) );

  int reuse = 1;

  if (!listen_addr) {
    LOGW("no listen address specified\n");
    return NULL;
  }

  c->streams = NULL;
  c->cnamel = 15;
  c->cname = malloc(c->cnamel);
  if (c->cname == NULL) return NULL;
  memcpy(c->cname, "android@zesense", c->cnamel);

  c->sockfd = socket(listen_addr->addr.sa.sa_family, SOCK_DGRAM, 0);
  if ( c->sockfd < 0 ) {
	LOGW("rtp_new_context: cannot create socket");
    goto onerror;
  }

  if ( setsockopt( c->sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse) ) < 0 ) {
    LOGW("setsockopt SO_REUSEADDR");
  }

  if (bind(c->sockfd, &listen_addr->addr.sa, listen_addr->size) < 0) {
    LOGW("coap_new_context: bind");
    goto onerror;
  }

  return c;

 onerror:
  if ( c->sockfd >= 0 )
    close ( c->sockfd );
  free( c );
  return NULL;

}
