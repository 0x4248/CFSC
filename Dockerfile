FROM alpine:3.18.3

RUN mkdir /app

COPY . /app

WORKDIR /app

RUN apk update && apk add gcc make libc-dev
RUN make

RUN make install

WORKDIR /

ENTRYPOINT ["sh /app/docker/welcome.sh"]