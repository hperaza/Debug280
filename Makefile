.PREFIX:
.PREFIX: .mac .rel

SRCS = debug280.mac \
	dis280.mac

OBJS = $(SRCS:.mac=.rel)

all: debug280.hex

$(OBJS): %.rel: %.mac *.inc
	zxcc zsm4 -"=$</l"

debug280.hex: $(OBJS) mkhex
	zxcc drlink -"debug280=debug280,dis280"
	mkhex -l 0x100 -e 0x100 debug280.com debug280.hex

mkhex: mkhex.c
	cc -o $@ $<

clean:
	rm -f mkhex *.com *.hex *.rel *.sym *.prn core *~ *.\$$\$$\$$
