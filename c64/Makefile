OBJS=decode_basic read1541 read1581
default: ${OBJS}
decode_basic: decode_basic.c
	cc -o $@ $<
read1541: read1541.c
	cc -o $@ $<
read1581: read1541.c
	cc -o $@ $< -DD1581
clean:
	rm -f ${OBJS}
