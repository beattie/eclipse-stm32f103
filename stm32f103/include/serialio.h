/*
 * serialio.h
 *
 *  Created on: Sep 10, 2017
 *      Author: beattie
 */

#ifndef _SERIALIO_H_
#define _SERIALIO_H_

#define SERIALSPEED	115200

typedef int32_t ring_size_t;

struct ring {
	uint8_t *data;
	ring_size_t size;
	uint32_t begin;
	uint32_t end;
};

extern struct ring output_ring;

#define RING_SIZE(RING)  ((RING)->size - 1)
#define RING_DATA(RING)  (RING)->data
#define RING_EMPTY(RING) ((RING)->begin == (RING)->end)

void ring_init(struct ring *ring, uint8_t *buf, ring_size_t size);
int32_t ring_write(struct ring *ring, uint8_t *data, ring_size_t size);
int32_t ring_write_ch(struct ring *ring, uint8_t ch);
int32_t ring_write(struct ring *ring, uint8_t *data, ring_size_t size);
int32_t ring_read_ch(struct ring *ring, uint8_t *ch);

#endif /* _SERIALIO_H_ */
