/* ZeSense
 * --  Very minimal RTP/RTCP implementation
 *
 * Author: Marco Zavatta
 * <marco.zavatta@telecom-bretagne.eu>
 */

#ifndef ZE_RTP_NET_H
#define ZE_RTP_NET_H

#include "rtcppacket.h"
#include "rtppacket.h"
#include "addr.h"
#include "debug.h"

typedef struct rtp_stream_t {
	struct rtp_stream_t *next;

	rtp_address_t dest;

	/* Handy to keep the info directly in its
	 * destination struct;
	 * not really actually..
	 */
	//rtp_hdr_t *lasthdr;
	unsigned short seqn;		/* sequence number */
	unsigned int ssrc;			/* synchronization source */

	int ntptwin;
	long rtptwin;

	int packetcount;
	int octectcount;

	/* To control the RTCP sending interval. */
	int last_rtcp_octectcount;
	int bandwidth;

	int invalid;

} rtp_stream_t;

typedef struct {

	  //coap_queue_t *sendqueue, *recvqueue;

	  int sockfd;			/**< send/receive socket */

	  char *cname;
	  int cnamel;

	  /* Network address of whom has demanded the session. */
	  rtp_address_t dst;

	  /* MAybe a global message id, sequence number or similar stuff?
	   * Probably not as most of the status info is kept at the stream
	   * specific session level, therefore inside rtp_stream_t.
	   * By the way there it is also kept the RTCP status info. */

	  /* Stream list */
	  rtp_stream_t *streams;

	  //coap_response_handler_t response_handler;
	  /* Added pointers to support the Streaming
	   * Manager communication, pointers to two
	   * buffers
	   */
	  //ze_coap_request_buf_t *notbuf;
	  //ze_sm_request_buf_t *smreqbuf;

} rtp_context_t;

rtp_stream_t* rtp_stream_init();

int
rtp_send_impl(rtp_context_t *context,
	       const rtp_address_t *dst,
	       rtp_pdu_t *pdu);

#endif
