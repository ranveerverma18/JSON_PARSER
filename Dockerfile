FROM ubuntu:22.04

# install dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    python3 \
    python3-pip

WORKDIR /app

COPY . .

# compile parser
RUN g++ -std=c++17 src/*.cpp -Iinclude -o json_parser

# install python deps
RUN pip3 install flask

EXPOSE 5000

CMD ["python3", "server.py"]