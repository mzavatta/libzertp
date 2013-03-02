/* ZeSense
 * --  Very minimal RTP/RTCP implementation
 *
 * Credits to libcoap
 * Copyright (C) 2010,2011 Olaf Bergmann <bergmann@tzi.org>
 *
 * Author: Marco Zavatta
 * <marco.zavatta@telecom-bretagne.eu>
 */


#ifndef ZE_RTP_ADDR_H
#define ZE_RTP_ADDR_H

#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <assert.h>

/** multi-purpose address abstraction */
typedef struct __rtp_address_t {
  socklen_t size;		/**< size of addr */
  union {
    struct sockaddr     sa;
    struct sockaddr_storage st;
    struct sockaddr_in  sin;
    struct sockaddr_in6 sin6;
  } addr;
} __rtp_address_t;

#define rtp_address_t __rtp_address_t

static inline int
_rtp_address_equals_impl(const rtp_address_t *a,
			  const rtp_address_t *b) {
  if (a->size != b->size || a->addr.sa.sa_family != b->addr.sa.sa_family)
    return 0;

  /* need to compare only relevant parts of sockaddr_in6 */
 switch (a->addr.sa.sa_family) {
 case AF_INET:
   return
     a->addr.sin.sin_port == b->addr.sin.sin_port &&
     memcmp(&a->addr.sin.sin_addr, &b->addr.sin.sin_addr,
	    sizeof(struct in_addr)) == 0;
 case AF_INET6:
   return a->addr.sin6.sin6_port == b->addr.sin6.sin6_port &&
     memcmp(&a->addr.sin6.sin6_addr, &b->addr.sin6.sin6_addr,
	    sizeof(struct in6_addr)) == 0;
 default: /* fall through and signal error */
   ;
 }
 return 0;
}

static inline int
_rtp_is_mcast_impl(const rtp_address_t *a) {
  if (!a)
    return 0;

 switch (a->addr.sa.sa_family) {
 case AF_INET:
   return IN_MULTICAST(a->addr.sin.sin_addr.s_addr);
case  AF_INET6:
  return IN6_IS_ADDR_MULTICAST(&a->addr.sin6.sin6_addr);
 default:			/* fall through and signal error */
   ;
  }
 return 0;
}

/**
 * Resets the given coap_address_t object @p addr to its default
 * values.  In particular, the member size must be initialized to the
 * available size for storing addresses.
 *
 * @param addr The coap_address_t object to initialize.
 */
static inline void
rtp_address_init(rtp_address_t *addr) {
  assert(addr);
  memset(addr, 0, sizeof(rtp_address_t));
  addr->size = sizeof(addr->addr);
}

/**
 * Compares given address objects @p a and @p b. This function returns
 * @c 1 if addresses are equal, @c 0 otherwise. The parameters @p a
 * and @p b must not be @c NULL;
 */
static inline int
rtp_address_equals(const rtp_address_t *a, const rtp_address_t *b) {
  assert(a); assert(b);
  return _rtp_address_equals_impl(a, b);
}

/**
 * Checks if given address @p a denotes a multicast address.  This
 * function returns @c 1 if @p a is multicast, @c 0 otherwise.
 */
static inline int
rtp_is_mcast(const rtp_address_t *a) {
  return a && _rtp_is_mcast_impl(a);
}

#endif
